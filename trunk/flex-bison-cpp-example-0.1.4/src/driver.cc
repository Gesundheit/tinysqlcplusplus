// $Id: driver.cc 39 2008-08-03 10:07:15Z tb $
/** \file driver.cc Implementation of the example::Driver class. */

#include <fstream>
#include <sstream>

#include<cstdlib>
#include<string>
#include<string.h>
#include <algorithm>
#include <map>
#include <set>
#include <vector>
#include <deque>
#include <iostream>


using namespace std;

#include "driver.h"
#include "scanner.h"

#include "StorageManager.h"

namespace example {

	Driver::Driver(class CalcContext& _calc)
		: trace_scanning(false),
		trace_parsing(false),
		calc(_calc)
	{
	}

	bool Driver::parse_stream(std::istream& in, const std::string& sname)
	{

		// Initialize the memory and the schema manager
		MainMemory mem;
		cout << "The memory contains " << mem.getMemorySize() << " blocks" << endl << endl;
		SchemaManager schemaMgr(&mem);


		Schema* schema = NULL;

		// Set up relationPtr
		Relation* relationPtr = NULL;

		// Set up relationFieldMap;
		map <string,vector<string>> relationFieldMap;

		bool r = false;
		streamname = sname;

		Scanner scanner(&in);
		scanner.set_debug(trace_scanning);
		this->lexer = &scanner;

		Parser parser(*this);
		parser.set_debug_level(trace_parsing);
		r = (parser.parse() == 0);




		int stmtNo=0;
		for(stmtNo=0;stmtNo<calc.stmt_vector.size();stmtNo++){
			switch (calc.stmt_vector[stmtNo]->body.stmt.type){
				case create_st:{
					run_create(stmtNo,schemaMgr,relationFieldMap);
					break;
				   }
				case insert_st:{
					run_insert(stmtNo,schemaMgr,mem);
					break;
				}
			   	case select_st:{
					run_select(stmtNo,schemaMgr,mem,relationFieldMap);
					break;
				}
				case delete_st:{
					run_delete(stmtNo,schemaMgr,mem,relationFieldMap);
					break;
				   }
				case drop_st:{
					run_drop(stmtNo,schemaMgr,mem);
					break;
				   }

			}
		}

		//relationFieldMap test
		//map<string,vector<string>>::iterator it;
		//it= relationFieldMap.find("school");

		//Observe the tree here by setting break
		return r;
	}

	void Driver::create_by_node(tree* node,vector<string> &fieldNames, vector<string> &fieldTypes){

		if(node!=NULL){
			if(node->body.list.arg1==NULL && node->body.list.arg2==NULL){
				return;
			}
			else if(node->body.list.arg2==NULL){

//printf("%s\n",node->body.list.arg1->body.colref.arg1);

				fieldNames.push_back( string((const char*)node->body.list.arg1->body.colref.arg1) );
				if( node->body.list.arg1->body.colref.type==str20 ){
					fieldTypes.push_back(string("STR20"));
				}else{
					fieldTypes.push_back(string("INT"));
				}			   

			}else{

				create_by_node(node->body.list.arg1,fieldNames,fieldTypes);			

				fieldNames.push_back( string((const char*)node->body.list.arg2->body.colref.arg1) );
				if( node->body.list.arg2->body.colref.type==str20 ){
					fieldTypes.push_back(string("STR20"));
				}else{
					fieldTypes.push_back(string("INT"));
				}			   

			}
		}
	}

	bool Driver::run_drop(int stmtNo, SchemaManager &schemaMgr, MainMemory &mem){
		
		string relationName(calc.stmt_vector[stmtNo]->body.stmt.arg1->body.variable);	
		schemaMgr.deleteRelation(relationName);
		if(schemaMgr.getRelation(relationName) == NULL){
			printf("Relation dropped\n");
		}

		return true;
	}

	bool Driver::run_create(int stmtNo, SchemaManager &schemaMgr,map<string,vector<string>> &relationFieldMap){
		printf("run_create called\n");

		// Create a schema
		vector<string> fieldNames;
		vector<string> fieldTypes;
		string relationName(calc.stmt_vector[stmtNo]->body.stmt.arg1->body.variable);	

		create_by_node(calc.stmt_vector[stmtNo]->body.stmt.arg2,fieldNames,fieldTypes);

		//printf("fn size %d\n",fieldNames.size());
		//printf("ft size %d\n",fieldTypes.size());

		if(fieldNames.size() == 0){
			printf("Cannot create a relation with zero fields\n");
			return false;
		}else{
			// Create Mapping for Select
			relationFieldMap.insert(pair<string,vector<string>>(relationName,fieldNames));

			Schema schema(fieldNames,fieldTypes);

			// Print the information about the schema
			//schema.printSchema();
			//cout << "The schema before createRelation has " << schema.getNumOfFields() << " fields" << endl;
			//cout << "The schema before createRelation has " << schema.getNumOfInt() << " integers" << endl;
			//cout << "The schema before createRelation has " << schema.getNumOfString() << " strings" << endl;
			cout << "The schema before createRelation allows " << schema.getTuplesPerBlock() << " tuples per block" << endl;

			
			schemaMgr.createRelation(relationName,schema);
			// Print the information provided by the schema manager
			cout << "After creating a relation, current schemas and relations: " << endl;
			schemaMgr.printRelationSchemaPairs();
			cout << "From the schema manager, the table " << relationName << " has schema:" << endl;
			schemaMgr.getSchema(relationName)->printSchema();
			cout << "From the schema manager, the table " << relationName << " has name:" <<
			schemaMgr.getRelation(relationName)->getRelationName() << endl << endl;
		}


		//relationPtr = schemaMgr.getRelation( calc.stmt_vector[stmtNo]->body.stmt.arg1->body.variable);
		return true;
	}

	bool Driver::run_select(int stmtNo, SchemaManager &schemaMgr,MainMemory &mem,map <string,vector<string>> relationFieldMap){
		vector<string> *relations  = new vector<string>;
		std::map<string,std::vector<column_ref>*> *attributes_to_project = new std::map<string,std::vector<column_ref>*>;
		std::map<string,std::vector<column_ref>*> attributes_to_print;

		map<string,int> disk_proj;
		map<string,int> disk_proj_size;
		get_relations(stmtNo, relations,schemaMgr, attributes_to_project, disk_proj,disk_proj_size);	
		attributes_to_print = *attributes_to_project;
		if(relations->size()<2){

			// read all tutples from disk to mem, then to a vector, this is to prepare for NOT, OrderBy, and DISTINCT
			Relation* relationPtr = schemaMgr.getRelation( relations->at(0) );
			Schema* schema = schemaMgr.getSchema( relations->at(0) );
			if(relationPtr->getNumOfBlocks()==NULL){
				return false;
			}
			int ct;
			vector<Tuple> totalTuples;
			for(ct=0;ct<relationPtr->getNumOfBlocks();ct++){
				relationPtr->readBlockToMemory(ct,0);
				Block* block=mem.getBlock(0);
				vector<Tuple> tuples=block->getTuples();
				for(int ct2=0;ct2<tuples.size();ct2++){
					totalTuples.push_back(tuples.at(ct2));
				}
			}
			vector<Tuple>*resultTuples = NULL;
			if(calc.stmt_vector[stmtNo]->body.stmt.arg3!=NULL){
				resultTuples = select_by_node_single_relation(calc.stmt_vector[stmtNo]->body.stmt.arg3,totalTuples,schema,relationFieldMap,stmtNo);
				//cout << resultTuples->size() << endl;
			}
			//use totalTuples if no where condition, use resultTuples if there is where condition
			//apply OrderBy
			map<string,vector<string>>::iterator it = relationFieldMap.find(calc.stmt_vector[stmtNo]->body.stmt.arg2->body.list.arg1->body.variable);
			if(calc.stmt_vector[stmtNo]->body.stmt.arg4!=NULL){
				if(resultTuples !=NULL){
					int intKey = 0;
					string stringKey;
					int targetField = 0;
					// find sort by INT field or String field
					targetField = schema->getFieldPos(calc.stmt_vector[stmtNo]->body.stmt.arg4->body.colref.arg1);
					if( schema->getFieldType(calc.stmt_vector[stmtNo]->body.stmt.arg4->body.colref.arg1)
						== "STR20"){
						int j=0;
						for(int ct=1;ct<resultTuples->size();ct++){					   
							stringKey = resultTuples->at(ct).getString(targetField);
							j = ct-1;
							while((j>=0) && (resultTuples->at(j).getString(targetField)>stringKey)){
								swap(resultTuples->at(j+1),resultTuples->at(j));
								j-=1;
							}
						}
						//cout<< resultTuples->size() << endl;
					}else{
						int j=0;
						for(int ct=1;ct<resultTuples->size();ct++){					   
							intKey = resultTuples->at(ct).getInt(targetField);
							j = ct-1;
							while((j>=0) && (resultTuples->at(j).getInt(targetField)>intKey)){
								swap(resultTuples->at(j+1),resultTuples->at(j));
								j-=1;
							}
						}
					}
				}else{
					// sort totalTutples instead
					int intKey = 0;
					string stringKey;
					int targetField = 0;
					// find sort by INT field or String field
					targetField = schema->getFieldPos(calc.stmt_vector[stmtNo]->body.stmt.arg4->body.colref.arg1);
					if( schema->getFieldType(calc.stmt_vector[stmtNo]->body.stmt.arg4->body.colref.arg1)
						== "STR20"){
						int j=0;
						for(int ct=1;ct<totalTuples.size();ct++){					   
							stringKey = totalTuples.at(ct).getString(targetField);
							j = ct-1;
							while((j>=0) && (totalTuples.at(j).getString(targetField)>stringKey)){
								swap(totalTuples.at(j+1),totalTuples.at(j));
								j-=1;
							}
						}
						//cout<< totalTuples.size() << endl;
					}else{
						int j=0;
						for(int ct=1;ct<totalTuples.size();ct++){					   
							intKey = totalTuples.at(ct).getInt(targetField);
							j = ct-1;
							while((j>=0) && (totalTuples.at(j).getInt(targetField)>intKey)){
								swap(totalTuples.at(j+1),totalTuples.at(j));
								j-=1;
							}
						}
					}
				}
			}
			//apply DISTINCT
			vector<Tuple>* distinctRTuples = new vector<Tuple>;
			vector<Tuple> totalTTuples;
			if(calc.stmt_vector[stmtNo]->body.stmt.dtype==dist){
				if(resultTuples!=NULL){

					// find projcted columns
					vector<column_ref> *columns  = new vector<column_ref>;
					get_columns(relations,stmtNo, schemaMgr, columns, attributes_to_project);
					vector<string> columnVect;
					if(calc.stmt_vector[stmtNo]->body.stmt.arg1->nodetype == variable_node){
						for(int ct=0;ct<it->second.size();ct++){
							columnVect.push_back(it->second.at(ct));
						}
					}else{
						for(int ct=0;ct<columns->size();ct++){
							columnVect.push_back(columns->at(ct).column_name);
						}
					}

					vector<Tuple>* pTuples = new vector<Tuple>;
					// project the needed column by forming new vector Tuples
					for(int ct1=0;ct1<resultTuples->size();ct1++){
						for(int ct2=0;ct2<columnVect.size();ct2++){
							Tuple tuple(schema);
							if(schema->getFieldType(columnVect.at(ct2))=="STR20"){
								tuple.setField(schema->getFieldPos(columnVect.at(ct2)) , resultTuples->at(ct1).getString(schema->getFieldPos(columnVect.at(ct2))) );
							}
							if(schema->getFieldType(columnVect.at(ct2))=="INT"){
								tuple.setField(schema->getFieldPos(columnVect.at(ct2)) , resultTuples->at(ct1).getInt(schema->getFieldPos(columnVect.at(ct2))) );
							}
							pTuples->push_back(tuple);
						}
					}
					distinctRTuples->push_back(pTuples->at(0));
					int ct2;
					for(ct2=1;ct2<pTuples->size();ct2++){
						int ct3;
						bool found=false;
						int ct4;
						// take resultTuples compare with dinstinct
						for(ct3=0;ct3<distinctRTuples->size();ct3++){
							int sfound=it->second.size();
							for(ct4=0;ct4<it->second.size();ct4++){
								if(pTuples->at(ct2).getInt(schema->getFieldPos(it->second.at(ct4)))==0){
									// Not a number field, getString
									if( pTuples->at(ct2).getString(schema->getFieldPos(it->second.at(ct4))) ==
										distinctRTuples->at(ct3).getString(schema->getFieldPos(it->second.at(ct4))) ){
											sfound--;
									}
								}else{
									if( pTuples->at(ct2).getInt(schema->getFieldPos(it->second.at(ct4))) ==
										distinctRTuples->at(ct3).getInt(schema->getFieldPos(it->second.at(ct4)))){
											sfound--;
									}
								}
							}
							if(sfound==0){found=true;}							
						}
					}

					//distinctRTuples->push_back(resultTuples->at(0));
					//int ct2;
					//for(ct2=1;ct2<resultTuples->size();ct2++){
					//	int ct3;
					//	bool found=false;
					//	int ct4;
					//	// take resultTuples compare with dinstinct
					//	for(ct3=0;ct3<distinctRTuples->size();ct3++){
					//		int sfound=it->second.size();
					//		for(ct4=0;ct4<it->second.size();ct4++){
					//			if(resultTuples->at(ct2).getInt(schema->getFieldPos(it->second.at(ct4)))==0){
					//				// Not a number field, getString
					//				if( resultTuples->at(ct2).getString(schema->getFieldPos(it->second.at(ct4))) ==
					//					distinctRTuples->at(ct3).getString(schema->getFieldPos(it->second.at(ct4))) ){
					//						sfound--;
					//				}
					//			}else{
					//				if( resultTuples->at(ct2).getInt(schema->getFieldPos(it->second.at(ct4))) ==
					//					distinctRTuples->at(ct3).getInt(schema->getFieldPos(it->second.at(ct4)))){
					//						sfound--;
					//				}
					//			}
					//		}
					//		if(sfound==0){found=true;}							
					//	}
					//		// match found
					//		if(found==false){distinctRTuples->push_back(resultTuples->at(ct2));}						
					//}

					resultTuples=distinctRTuples;
					cout<<resultTuples->size()<<endl;
				}else{
					totalTTuples.push_back(totalTuples.at(0));
					int ct2;
					for(ct2=1;ct2<totalTuples.size();ct2++){
						int ct3;
						bool found=false;
						int ct4;
						// take resultTuples compare with dinstinct
						for(ct3=0;ct3<totalTTuples.size();ct3++){
							int sfound=it->second.size();
							for(ct4=0;ct4<it->second.size();ct4++){
								if(totalTuples.at(ct2).getInt(schema->getFieldPos(it->second.at(ct4)))==0){
									// Not a number field, getString
									if( totalTuples.at(ct2).getString(schema->getFieldPos(it->second.at(ct4))) ==
										totalTTuples.at(ct3).getString(schema->getFieldPos(it->second.at(ct4))) ){
											sfound--;
									}
								}else{
									if( totalTuples.at(ct2).getInt(schema->getFieldPos(it->second.at(ct4))) ==
										totalTTuples.at(ct3).getInt(schema->getFieldPos(it->second.at(ct4)))){
											sfound--;
									}
								}
							}
							if(sfound==0){found=true;}							
						}
							// match found
							if(found==false){totalTTuples.push_back(totalTuples.at(ct2));}						
					}
					totalTuples=totalTTuples;
					cout << totalTuples.size() <<endl;
				}
			}
			//print function, based on the projected column(s), 


			vector<column_ref> *columns  = new vector<column_ref>;
			get_columns(relations,stmtNo, schemaMgr, columns, attributes_to_project);
			vector<string> columnVect;
			if(calc.stmt_vector[stmtNo]->body.stmt.arg1->nodetype == variable_node){
				for(int ct=0;ct<it->second.size();ct++){
					columnVect.push_back(it->second.at(ct));
				}
			}else{
				for(int ct=0;ct<columns->size();ct++){
					columnVect.push_back(columns->at(ct).column_name);
				}
			}
			cout << "Print query on Relation: " << relations->at(0) << endl;
			print_select_single(resultTuples,totalTuples,columnVect,relationFieldMap,schema);

			//below is for multi-relations

		}else{
			vector<column_ref> *columns  = new vector<column_ref>;
			get_columns(relations,stmtNo, schemaMgr, columns, attributes_to_project);
			if(calc.stmt_vector[stmtNo]->body.stmt.arg3!=NULL){
				process_condition(relations, stmtNo,schemaMgr, calc.stmt_vector[stmtNo]->body.stmt.arg3, attributes_to_project);				
				execute_condition(relations, stmtNo,schemaMgr, calc.stmt_vector[stmtNo]->body.stmt.arg3, attributes_to_project, 
					relationFieldMap,mem, disk_proj,disk_proj_size,0,attributes_to_print);
				if (columns->size()==0){


				}
				else {

				}
			}
			else{
				//apply cross join
				cross_join(relations, schemaMgr, mem, attributes_to_project, relationFieldMap);
				cout<<"No condition"<<endl;
			}
		}

		// single or multi relation, call appropriate, return resulting bags to memory, if appropriate	   
		// apply distinct or order by, if appropriate
		
		return true;

	}
	void Driver::cross_join(vector<string> *relations, SchemaManager &schemaMgr, MainMemory &mem, 
		std::map<string,std::vector<column_ref>*> *attributes_to_project,map<string,vector<string>> relationFieldMap) {

		for(int i=0; i<relations->size();i++) {
			string relation = (*relations)[i]; 
			if(schemaMgr.getSchema(relation) == NULL){
				printf("Table not found\n");
				return;
			}
			if(schemaMgr.getRelation(relation) == NULL){
				printf("Table not found\n");
				return;
			}
		}
		deque<string> relations_to_print ;
		relations_to_print.insert(relations_to_print.begin(),relations->begin(),relations->end());
		int last_rel_index = relations_to_print.size()-1;
		bool last_in_mem=false;
		Relation* relationPtr = schemaMgr.getRelation(relations_to_print[last_rel_index]);
		int last_rel_size = relationPtr->getNumOfBlocks();
		if (mem.getMemorySize()-last_rel_index>=relationPtr->getNumOfBlocks()){
			Driver::get_blocks_to_mem(last_rel_index,mem,last_rel_size,relationPtr);
			last_in_mem = true;
		}	
		string s("");
		print_cross_join(relations_to_print,schemaMgr,mem,attributes_to_project,s,0,last_in_mem,relationFieldMap);
		return;
	}

	void Driver::print_cross_join(deque<string> relations_to_print, SchemaManager &schemaMgr, MainMemory &mem,
									std::map<string,std::vector<column_ref>*> *attributes_to_project, string &res_tuple, 
									int memindex, bool last_in_mem, map<string,vector<string>> relationFieldMap){
		string print_relation(relations_to_print.front());
		relations_to_print.pop_front();
		Relation* relationPtr = schemaMgr.getRelation(print_relation);
		Schema* schema = schemaMgr.getSchema(print_relation);
		vector<column_ref> *attributes = (*attributes_to_project)[relationPtr->getRelationName()];
		if (relations_to_print.size()==0) {
			if(relationPtr->getNumOfBlocks()==NULL){
				print_result_tuple(res_tuple);
			}
			if(last_in_mem) {
				vector<Tuple> tuples = mem.getTuples(memindex,relationPtr->getNumOfBlocks());
				if(attributes->size()!=0) {
					for(int i=0;i<tuples.size();i++) {
						Tuple t = tuples[i];
						Driver::process_tuple(*attributes,t,schema, res_tuple,relationFieldMap[print_relation]);
						Driver::print_result_tuple(res_tuple);
					}				
				}
				else {
					for(int i=0;i<tuples.size();i++) {
						Tuple t = tuples[i];
						cout<<res_tuple<<"\t";
						Driver::print_result_tuple(t);
					}	
				}
			}
			else{
				for(int i=0; i<relationPtr->getNumOfBlocks();i++){
						relationPtr->readBlockToMemory(i,memindex);
						Block *b = mem.getBlock(memindex);
						vector<Tuple> tuples = b->getTuples();
						if(attributes->size()!=0) {
							for(int j=0; j<tuples.size();j++){
								Tuple t = tuples[j];
								string temp = res_tuple;
								Driver::process_tuple(*attributes,t,schema, temp,relationFieldMap[print_relation]);
								Driver::print_result_tuple(temp);
							}
						}
						else{
							for(int i=0;i<tuples.size();i++) {
								Tuple t = tuples[i];
								cout<<res_tuple<<"\t";
								Driver::print_result_tuple(t);
							}
						}
				}
			}					
		}
		else {
			for(int i=0; i<relationPtr->getNumOfBlocks();i++){
				relationPtr->readBlockToMemory(i,memindex);
				Block *b = mem.getBlock(memindex);
				vector<Tuple> tuples = b->getTuples();

				if(attributes->size()!=0) {
					for(int j=0; j<tuples.size();j++){
						Tuple t = tuples[j];
						string temp = res_tuple;
						Driver::process_tuple(*attributes,t,schema, temp, relationFieldMap[print_relation]);
						Driver::print_cross_join(relations_to_print,schemaMgr, mem,attributes_to_project, temp, memindex+1, last_in_mem, relationFieldMap);										
					}
				}
				else{
					for(int i=0;i<tuples.size();i++) {
						Tuple t = tuples[i];
						string temp = res_tuple;
						Driver::process_tuple(*attributes,t,schema, temp, relationFieldMap[print_relation]);
						Driver::print_cross_join(relations_to_print,schemaMgr, mem,attributes_to_project, temp, memindex+1, last_in_mem, relationFieldMap);												
					}
				}
			}
		}		
	}

	vector<pair<Tuple,Tuple>> Driver::natural_join(string relation1,string relation2, SchemaManager &schemaMgr, MainMemory &mem,
									std::map<string,std::vector<column_ref>*> *attributes_to_project,
									int memindex, map<string,vector<string>> relationFieldMap, char* op, string column,
									map<string,int> &disk_proj, map<string,int> &disk_proj_size)
	{
		Schema *schema1 = schemaMgr.getSchema(relation1);
		Schema *schema2 = schemaMgr.getSchema(relation2);
		Relation *relationPtr1 = schemaMgr.getRelation(relation1);
		Relation *relationPtr2 = schemaMgr.getRelation(relation2);
		int size1 = disk_proj_size[relation1];
		int size2 = disk_proj_size[relation1];
		Driver::clearMemory(mem);
		vector<pair<Tuple,Tuple>> result;
		if(size1<=mem.getMemorySize()-1 ||size2<=mem.getMemorySize()-1) { // one-pass
			if(size1<=mem.getMemorySize()-1) {
				Driver::get_blocks_to_mem(0,mem, size1,relationPtr1, disk_proj[relation1]);
				for(int i=0;i<disk_proj_size[relation2];i++) {
					relationPtr2->readBlockToMemory(disk_proj[relation2]+i,mem.getMemorySize()-1);
					Block *b2 = mem.getBlock(mem.getMemorySize()-1);
					vector<Tuple> tuples2 = b2->getTuples();
					for(int j=0; j<tuples2.size(); j++){
						Tuple t2  = tuples2[j];
						if(schema2->getFieldType(column)=="STR20") {
							string str2 = t2.getString(schema2->getFieldPos(column));
							if(str2=="") continue;
							vector<Tuple> tuples =mem.getTuples(0,relationPtr1->getNumOfBlocks());
							for(int k =0; k<tuples.size();k++ ) {
								Tuple t1 = tuples[k];
								string str1 = t1.getString(schema1->getFieldPos(column));
								if(str1=="") continue;
								if(strcmp("=",op)==0 && str1==str2 ||
									strcmp(">",op)==0 && str1>str2 ||
									strcmp("<",op)==0 && str1<str2) {
									pair<Tuple,Tuple> r(t1,t2);
									result.push_back(r);
								}
							}
						}
						else {
							int int2 = t2.getInt(schema2->getFieldPos(column));
							if(int2==-1) continue;
							vector<Tuple> tuples =mem.getTuples(0,relationPtr1->getNumOfBlocks());
							for(int k =0; k<tuples.size();k++ ) {
								Tuple t1 = tuples[k];
								int int1 = t1.getInt(schema1->getFieldPos(column));
								if(int1==-1) continue;
								if (strcmp("=",op)==0 && int1==int2 ||
									strcmp(">",op)==0 && int1>int2 ||
									strcmp("<",op)==0 && int1<int2) {
									pair<Tuple,Tuple> r(t1,t2);
									result.push_back(r);
								}
							}

						}
						
					}
				}
			}
			else {
				Driver::get_blocks_to_mem(0,mem,disk_proj_size[relation2],relationPtr2,disk_proj_size[relation2]);
//				mem.dumpMemory();
				for(int i=0;i<relationPtr1->getNumOfBlocks();i++) {
					relationPtr1->readBlockToMemory(disk_proj[relation1]+i,mem.getMemorySize()-1);
//					mem.dumpMemory();
					Block *b1 = mem.getBlock(mem.getMemorySize()-1);
					vector<Tuple> tuples1 = b1->getTuples();
					for(int j=0; j<tuples1.size(); j++){
						Tuple t1  = tuples1[j];
						if(schema1->getFieldType(column)=="STR20") {
							string str1 = t1.getString(schema1->getFieldPos(column));
							if(str1=="") continue;
							vector<Tuple> tuples =mem.getTuples(0,relationPtr1->getNumOfBlocks());
							for(int k =0; k<tuples.size();k++ ) {
								Tuple t2 = tuples[k];
								string str2 = t2.getString(schema2->getFieldPos(column));
								if(str2=="") continue;
								if(strcmp("=",op)==0 && str1==str2 ||
									strcmp(">",op)==0 && str1>str2 ||
									strcmp("<",op)==0 && str1<str2) {
									pair<Tuple,Tuple> r(t1,t2);
									result.push_back(r);
								}
							}
						}
						else {
							int int1 = t1.getInt(schema2->getFieldPos(column));
							if(int1==-1) continue;
							vector<Tuple> tuples =mem.getTuples(0,relationPtr1->getNumOfBlocks());
							for(int k =0; k<tuples.size();k++ ) {
								Tuple t2 = tuples[k];
								int int2 = t2.getInt(schema1->getFieldPos(column));
								if(int2==-1) continue;
								if (strcmp("=",op)==0 && int1==int2 ||
									strcmp(">",op)==0 && int1>int2 ||
									strcmp("<",op)==0 && int1<int2) {
									pair<Tuple,Tuple> r(t1,t2);
									result.push_back(r);
								}
							}

						}						
					}
				}
			}
		}
		else { // two-pass
			//sort first
			int sorted_index1 = Driver::sort_relation(relation1,schemaMgr,mem,0,column,disk_proj,disk_proj_size);
			Driver::clearMemory(mem);
			//sort second
			int sorted_index2 = Driver::sort_relation(relation2,schemaMgr,mem,0,column,disk_proj,disk_proj_size);
			Driver::clearMemory(mem);
			int remaining_blocks = disk_proj_size[relation1];
			int chunks_blocks1 = ceil((float)disk_proj[relation1]/(mem.getMemorySize()-1));

			for(int i=0; i<chunks_blocks1;i++) {

				Driver::get_blocks_to_mem(0,mem,mem.getMemorySize()-1,relationPtr1,sorted_index1);
				sorted_index1 += mem.getMemorySize()-1;
				remaining_blocks -= min(remaining_blocks,mem.getMemorySize()-1);
				vector<Tuple> tuples1 = mem.getTuples(0,min(remaining_blocks,mem.getMemorySize()-1));
				
				for (int j=0; j<disk_proj_size[relation2]; j++) {
					
					if(schema1->getFieldType(column)=="STR20") {							
						for(int i=0;i<tuples1.size();) {
							Tuple t1 = tuples1[i];
							string str1 = t1.getString(schema1->getFieldPos(column));
							if(str1=="") continue;
							relationPtr2->readBlockToMemory(sorted_index2+j,mem.getMemorySize()-1);
							Block *b2 = mem.getBlock(mem.getMemorySize()-1);
							vector<Tuple> tuples2 = b2->getTuples();
							for(int j=0; j<tuples2.size();) {
								Tuple t2 = tuples2[j];
								string str2 = t2.getString(schema2->getFieldPos(column));
								if(str2=="") continue;
								if(strcmp("=",op)==0) {
									if(str1==str2) {
										pair<Tuple,Tuple> r(t1,t2);
										result.push_back(r);
										i++;j++;
									}
									else if(str1<str2) {
										i++;
									}
									else {
										j++;
									}
								}
								if(strcmp(">",op)==0) {
									if(str1>str2) {
										pair<Tuple,Tuple> r(t1,t2);
										result.push_back(r);
										j++;
									}
									else if(str1>str2) {
										return result;
									}
									else {
										j++;
									}
								}
								if(strcmp("<",op)==0) {
									if(str1<str2) {
										pair<Tuple,Tuple> r(t1,t2);
										result.push_back(r);
										j++;
									}
									else if(str1<str2) {
										i++;
									}
									else {
										i++;
									}
								}
								if(i>tuples1.size() ||j>tuples2.size()) {
									break;
								}
							}								
							if(i>tuples1.size()){
								break;
							}
						}
						
					}
					if(schema1->getFieldType(column)=="INT") {							
						for(int i=0;i<tuples1.size();) {
							Tuple t1 = tuples1[i];
							int int1 = t1.getInt(schema1->getFieldPos(column));
							if(int1==-1) continue;
							relationPtr2->readBlockToMemory(sorted_index2+j,mem.getMemorySize()-1);
							Block *b2 = mem.getBlock(mem.getMemorySize()-1);
							vector<Tuple> tuples2 = b2->getTuples();
							for(int j=0; j<tuples2.size();) {
								Tuple t2 = tuples2[j];
								int int2 = t2.getInt(schema2->getFieldPos(column));
								if(int2==-1) continue;
								if(strcmp("=",op)==0) {
									if(int1==int2) {
										pair<Tuple,Tuple> r(t1,t2);
										result.push_back(r);
										i++;j++;
									}
									else if(int1<int2) {
										i++;
									}
									else {
										j++;
									}
								}
								if(strcmp(">",op)==0) {
									if(int1>int2) {
										pair<Tuple,Tuple> r(t1,t2);
										result.push_back(r);
										j++;
									}
									else if(int1>int2) {
										return result;
									}
									else {
										j++;
									}
								}
								if(strcmp("<",op)==0) {
									if(int1<int2) {
										pair<Tuple,Tuple> r(t1,t2);
										result.push_back(r);
										j++;
									}
									else if(int1>int2) {
										i++;
									}
									else {
										i++;
									}
								}
								if(i>tuples1.size() ||j>tuples2.size()) {
									break;
								}
							}								
							if(i>tuples1.size()){
								break;
							}
						}
						
					}			
				
				}
					
			}
						

		}
		return result;

	}

	int Driver::sort_relation(string relation, SchemaManager &schemaMgr, MainMemory &mem, int memindex, 
									 string column, map<string,int> &disk_proj, map<string,int> &disk_proj_size)
	{
		Relation *relationPtr = schemaMgr.getRelation(relation);
		Schema *schema = schemaMgr.getSchema(relation);
		Driver::get_blocks_to_mem(0,mem,disk_proj_size[relation],relationPtr,disk_proj[relation]);
		vector<Tuple> to_sort;
		for(int i=0; i<disk_proj_size[relation]; i++ ) {
			Block *b = mem.getBlock(i);
			vector<Tuple> tuples = b->getTuples();
			for(int j=0; j<tuples.size(); j++) {
				to_sort.push_back(tuples[j]);
			}			
		}

		if(schema->getFieldType(column)=="STR20") {
			sort( to_sort.begin(),to_sort.end(),comparator(schema->getFieldPos(column)));
		}
		if(schema->getFieldType(column)=="INT") {
			sort( to_sort.begin(),to_sort.end(),comparator(schema->getFieldPos(column)));
		}

		int to_sort_index = 0;
		int result_rel_ind = 500;
		for(int i=0; i<disk_proj_size[relation]; i++ ) {
			Block *b = mem.getBlock(i);
			vector<Tuple> tuples = b->getTuples();
			for(int j=0; j<tuples.size() && to_sort_index<to_sort.size(); j++) {
				b->setTuple(j,to_sort[to_sort_index]);
			}
			relationPtr->writeBlockFromMemory(result_rel_ind+i,i);
		}

		return result_rel_ind;	
	}


	void Driver::clearMemory(MainMemory &mem){
		for(int i=0;i<mem.getMemorySize(); i++){
			Block *b = mem.getBlock(i);
			b->clear();
		}
	}


	void Driver::print_result_tuple(Tuple t){
		t.printTuple();
		std::cout<<std::endl;
		return;
		
	}
	void Driver::print_result_tuple(string res){
		std::cout<<res<<std::endl;
		return;
	}

	void Driver::process_tuple(std::vector<column_ref> attributes,Tuple t, Schema *schema, string &res_tuple, vector<string> relationFieldMap){
		if (attributes.size()==0) {
			for(int j=0; j<relationFieldMap.size();j++) {
				int pos = schema->getFieldPos(relationFieldMap[j]);
				if (schema->getFieldType(relationFieldMap[j])=="INT") {
					std::stringstream out;
					out << t.getInt(pos);
					res_tuple.append(out.str());
					res_tuple.append("\t");
				} else if (schema->getFieldType(relationFieldMap[j])=="STR20") {
					res_tuple.append(t.getString(pos));
					res_tuple.append("\t");							
				}
			}
			return;
		}
		for(int j=0; j<attributes.size();j++) {
			int pos = schema->getFieldPos(attributes[j].column_name);
			if (schema->getFieldType(attributes[j].column_name)=="INT") {
				std::stringstream out;
				out << t.getInt(pos);
				res_tuple.append(out.str());
				res_tuple.append("\t");
			} else if (schema->getFieldType(attributes[j].column_name)=="STR20") {
				res_tuple.append(t.getString(pos));
				res_tuple.append("\t");							
			}
		}
		return;
	}

	void Driver::get_blocks_to_mem(int mem_start_ind, MainMemory &mem, int rel_size,Relation* relationPtr,int rel_start_ind){
		for(int i=0;i<rel_size;i++){
			relationPtr->readBlockToMemory(i+rel_start_ind,i+mem_start_ind);
		}		
	}

	bool Driver::run_delete(int stmtNo, SchemaManager &schemaMgr, MainMemory &mem,map <string,vector<string>> relationFieldMap){
		printf("run_delete\n");

		if(calc.stmt_vector[stmtNo]->body.stmt.arg2 == NULL){
			Schema* schema = schemaMgr.getSchema( calc.stmt_vector[stmtNo]->body.stmt.arg1->body.variable );
			if(schema == NULL){
				printf("Table not found\n");
				return false;
			}
			Relation* relationPtr = schemaMgr.getRelation( calc.stmt_vector[stmtNo]->body.stmt.arg1->body.variable);
			if(relationPtr == NULL){
				printf("Table not found\n");
				return false;
			}
			int ct;
			for (ct=0;ct<relationPtr->getNumOfBlocks();ct++) {
				relationPtr->deleteBlock(ct);
			}
			relationPtr->printRelation();
		}else{
			printf("has argue\n");
			// Obtain all tuples of this relation
			Relation* relationPtr = schemaMgr.getRelation( calc.stmt_vector[stmtNo]->body.stmt.arg1->body.variable);
			Schema* schema = schemaMgr.getSchema( calc.stmt_vector[stmtNo]->body.stmt.arg1->body.variable );
			if(relationPtr->getNumOfBlocks()==NULL){
				return false;
			}
			int ct;
			vector<Tuple> totalTuples;
			for(ct=0;ct<relationPtr->getNumOfBlocks();ct++){
				relationPtr->readBlockToMemory(ct,0);
				Block* block=mem.getBlock(0);
				vector<Tuple> tuples=block->getTuples();
				for(int ct2=0;ct2<tuples.size();ct2++){
					totalTuples.push_back(tuples.at(ct2));
				}
			}
			vector<Tuple>* targetTuples = delete_by_node(calc.stmt_vector[stmtNo]->body.stmt.arg2,totalTuples,schema);
			map<string,vector<string>>::iterator it = relationFieldMap.find(calc.stmt_vector[stmtNo]->body.stmt.arg1->body.variable);

			for (ct=0;ct<relationPtr->getNumOfBlocks();ct++) {
			//pull each block to memory, examine, if not matching target, store.
			//remove the block, wrtie the store back
				vector<Tuple> newTuples;
				Block* block=mem.getBlock(0);
				block->clear();
				relationPtr->readBlockToMemory(ct,0);
				block=mem.getBlock(0);
				vector<Tuple> tuplesOfBlock=block->getTuples();
				int ct2;
				for(ct2=0;ct2<tuplesOfBlock.size();ct2++){
					int ct3;
					int found=0;
					int ct4;
					// compare with targetTuples
					for(ct3=0;ct3<targetTuples->size();ct3++){
						for(ct4=0;ct4<it->second.size();ct4++){
							if(tuplesOfBlock.at(ct2).getInt(schema->getFieldPos(it->second.at(ct4)))==0){
								// Not a number field, getString
								if( tuplesOfBlock.at(ct2).getString(schema->getFieldPos(it->second.at(ct4))) ==
									targetTuples->at(ct3).getString(schema->getFieldPos(it->second.at(ct4))) ){
										found++;
								}
							}else{
								if( tuplesOfBlock.at(ct2).getInt(schema->getFieldPos(it->second.at(ct4))) ==
									targetTuples->at(ct3).getInt(schema->getFieldPos(it->second.at(ct4)))){
										found++;
								}
							}
						}
						if(found!=0){break;}
					}
					if(found==0){newTuples.push_back(tuplesOfBlock.at(ct2));}
				}

				block->clear();
				for(int ct5=0;ct5<newTuples.size();ct5++){
					block->appendTuple(((Tuple)newTuples.at(ct5)));
				}
				relationPtr->writeBlockFromMemory(ct,0);
				relationPtr->printRelation();
			}

		}	   
		return true;
	}
	
	void Driver::print_select_single(vector<Tuple>* resultTuples,vector<Tuple> totalTuples,
		vector<string>fields,map <string,vector<string>> relationFieldMap,Schema* schema){

			if(resultTuples==NULL){
				for(int ct=0;ct<fields.size();ct++){
					cout<<fields.at(ct)<<"\t";
				}
				cout<<endl;
				for(int ct1=0;ct1<totalTuples.size();ct1++){
					for(int ct2=0;ct2<fields.size();ct2++){
						if(schema->getFieldType(fields.at(ct2))=="STR20"){
							cout<<totalTuples.at(ct1).getString(schema->getFieldPos(fields.at(ct2)))<<"\t";						
						}
						if(schema->getFieldType(fields.at(ct2))=="INT"){
							cout<<totalTuples.at(ct1).getInt(schema->getFieldPos(fields.at(ct2)))<<"\t";						
						}
					}
					cout<<endl;
				}
			}else{
				for(int ct=0;ct<fields.size();ct++){
					cout<<fields.at(ct)<<"\t";
				}
				cout<<endl;
				for(int ct1=0;ct1<resultTuples->size();ct1++){
					for(int ct2=0;ct2<fields.size();ct2++){
						if(schema->getFieldType(fields.at(ct2))=="STR20"){
							cout<<resultTuples->at(ct1).getString(schema->getFieldPos(fields.at(ct2)))<<"\t";						
						}
						if(schema->getFieldType(fields.at(ct2))=="INT"){
							cout<<resultTuples->at(ct1).getInt(schema->getFieldPos(fields.at(ct2)))<<"\t";						
						}
					}
					cout<<endl;
				}
			}

		return;
	}

	bool Driver::run_insert(int stmtNo, SchemaManager &schemaMgr, MainMemory &mem){

		// Always use memory_index 0 to insert, clear whatever is there
		printf("run_insert\n");

		// Load data from parse tree
		vector<string> fieldNames;
		vector<atom_ref> fieldValues;

		insert_by_node(calc.stmt_vector[stmtNo]->body.stmt.arg2,fieldNames,fieldValues,"n");
		insert_by_node(calc.stmt_vector[stmtNo]->body.stmt.arg3,fieldNames,fieldValues,"v");

		// Correctness checking
		if(fieldNames.size()!=fieldValues.size()){
			printf("number of field and type does not match\n");
			return false;
		}

		Schema* schema = schemaMgr.getSchema( calc.stmt_vector[stmtNo]->body.stmt.arg1->body.variable );
		if(schema == NULL){
			printf("Table not found\n");
			return false;
		}
		Relation* relationPtr = schemaMgr.getRelation( calc.stmt_vector[stmtNo]->body.stmt.arg1->body.variable);
		if(relationPtr == NULL){
			printf("Table not found\n");
			return false;
		}
		relationPtr->printRelation();

		// Read last block from relation into memory_index 0
		printf("relationPtr NumOfBlocks\n");
		cout << relationPtr->getNumOfBlocks() << endl;
		if(relationPtr->getNumOfBlocks()>0){
			relationPtr->readBlockToMemory(relationPtr->getNumOfBlocks()-1,0);
		}

		// Set up a block in the memory
	    Tuple tuple(schema);
		Block* block=mem.getBlock(0);	 
		if(block->isFull()==1){
			block->clear();
		} 
		// Clean up residual
		if(relationPtr->getNumOfBlocks()==0){
			block->clear();
		}


		int ct = 0; 
		for(ct=0; ct<fieldValues.size();ct++){
			if(fieldValues[ct].atom_type == literal_node){
				tuple.setField(schema->getFieldPos(fieldNames[ct]) ,string((const char*) fieldValues[ct].string_value));
			}else{
				tuple.setField(schema->getFieldPos(fieldNames[ct]) ,fieldValues[ct].int_value);
			}
		}
		block->appendTuple(tuple);

		//cout << "Append the tuple:" << endl;
		//tuple.printTuple();
		//cout << "The block is full? " << (block->isFull()==1?"true":"false") << endl;
		//cout << "The block currently has " << block->getNumTuples() << " tuples:" << endl;
		//block->printBlock();
		//cout << endl;

		if(relationPtr->getNumOfBlocks()==0){
			relationPtr->writeBlockFromMemory(0,0);
		}else{
			relationPtr->writeBlockFromMemory(relationPtr->getNumOfBlocks()-1,0);
		}

		// Move the ptr
		if(block->isFull()==1){
			relationPtr->writeBlockFromMemory(relationPtr->getNumOfBlocks(),0);
		}

		return true;
	}

	vector<Tuple>* Driver::delete_by_node(tree* node,vector<Tuple> origSet,Schema* schema){
		if(node!=NULL){			if(node->body.list.arg1==NULL && node->body.list.arg2==NULL){
				return NULL;
			} else if(node->body.expr.arg1->nodetype==colref_node && 
				node->body.expr.arg2->nodetype==literal_node){
					string s;
					s = string((const char*)node->body.expr.arg2->body.variable);
					s.erase(
						remove(s.begin(),s.end(),'\"'),s.end()
						);
					//cout << s <<endl;
					vector<Tuple> *r = new vector<Tuple>;
					int ct;

					// find tuple(s) that would match condition
					for(ct=0;ct<origSet.size();ct++){
						//cout << ((Tuple)origSet.at(ct)).getString(
						//schema->getFieldPos(node->body.expr.arg1->body.colref.arg1) )
						//<< endl;
						if(s == ((Tuple)origSet.at(ct)).getString(
						schema->getFieldPos(node->body.expr.arg1->body.colref.arg1)) ){
							r->push_back(origSet.at(ct));
						}
					}
					return r;
			} else if(node->body.expr.arg1->nodetype==colref_node &&
				node->body.expr.arg2->nodetype==number_node){
					int i;
					i = node->body.expr.arg2->body.number;
					printf("condition number %d\n",i);
					vector<Tuple> *r = new vector<Tuple>;
					int ct;

					if(node->body.expr.op[0] == '='){
							// find tuple(s) that would match condition
							for(ct=0;ct<origSet.size();ct++){
								if(i == ((Tuple)origSet.at(ct)).getInt(
									schema->getFieldPos(node->body.expr.arg1->body.colref.arg1)) ){
										r->push_back(origSet.at(ct));
								}
							}
					}else if(node->body.expr.op[0] == '<'){
							// find tuple(s) that would match condition
							for(ct=0;ct<origSet.size();ct++){
								if(i <= ((Tuple)origSet.at(ct)).getInt(
									schema->getFieldPos(node->body.expr.arg1->body.colref.arg1)) ){
										r->push_back(origSet.at(ct));
								}
							}
					}else if(node->body.expr.op[0] == '>'){
							// find tuple(s) that would match condition
							for(ct=0;ct<origSet.size();ct++){
								if(i >= ((Tuple)origSet.at(ct)).getInt(
									schema->getFieldPos(node->body.expr.arg1->body.colref.arg1)) ){
										r->push_back(origSet.at(ct));
								}
							}
					}
					return r;
			} else if(node->body.expr.arg1->nodetype==number_node &&
				node->body.expr.arg2->nodetype==number_node){
					// not handling for now
			} else{
				delete_by_node(node->body.expr.arg1,origSet,schema);
				delete_by_node(node->body.expr.arg2,origSet,schema);
				// not handling for now
			}
		}
		return NULL;
	}

	vector<Tuple>* Driver::select_by_node_single_relation(tree* node,vector<Tuple> origSet,Schema* schema,	map <string,vector<string>> relationFieldMap,int stmtNo){
		if(node!=NULL){			if(node->body.list.arg1==NULL && node->body.list.arg2==NULL){
				return NULL;
			} else if(node->body.expr.arg1->nodetype==colref_node && 
				node->body.expr.arg2->nodetype==literal_node){
					string s;
					s = string((const char*)node->body.expr.arg2->body.variable);
					s.erase(
						remove(s.begin(),s.end(),'\"'),s.end()
						);
					//cout << s <<endl;
					vector<Tuple> *r = new vector<Tuple>;
					int ct;

					// find tuple(s) that would match condition
					for(ct=0;ct<origSet.size();ct++){
						//cout << ((Tuple)origSet.at(ct)).getString(
						//schema->getFieldPos(node->body.expr.arg1->body.colref.arg1) )
						//<< endl;
						if(s == ((Tuple)origSet.at(ct)).getString(
						schema->getFieldPos(node->body.expr.arg1->body.colref.arg1)) ){
							r->push_back(origSet.at(ct));
						}
					}
					return r;
			} else if(node->body.expr.arg1->nodetype==colref_node &&
				node->body.expr.arg2->nodetype==number_node){
					int i;
					i = node->body.expr.arg2->body.number;
					vector<Tuple> *r = new vector<Tuple>;
					int ct;

					if(node->body.expr.op[0] == '='){
							// find tuple(s) that would match condition
							for(ct=0;ct<origSet.size();ct++){
								if(i == ((Tuple)origSet.at(ct)).getInt(
									schema->getFieldPos(node->body.expr.arg1->body.colref.arg1)) ){
										r->push_back(origSet.at(ct));
								}
							}
					}else if(node->body.expr.op[0] == '<'){
							// find tuple(s) that would match condition
							for(ct=0;ct<origSet.size();ct++){
								if(i > ((Tuple)origSet.at(ct)).getInt(
									schema->getFieldPos(node->body.expr.arg1->body.colref.arg1)) ){
										r->push_back(origSet.at(ct));
								}
							}
					}else if(node->body.expr.op[0] == '>'){
							// find tuple(s) that would match condition
							for(ct=0;ct<origSet.size();ct++){
								if(i < ((Tuple)origSet.at(ct)).getInt(
									schema->getFieldPos(node->body.expr.arg1->body.colref.arg1)) ){
										r->push_back(origSet.at(ct));
								}
							}
					}
					return r;
			} else if(node->body.expr.arg1->nodetype==number_node &&
				node->body.expr.arg2->nodetype==number_node){
					// not handling for now
			} else if(node->body.expr.arg1->nodetype==colref_node &&
				node->body.expr.arg2->nodetype==colref_node){
					vector<Tuple> *r = new vector<Tuple>;
					char*add = "+";
					char*sub = "-";
					char*mul = "*";
					char*div = "/";

					if(strcmp(node->body.expr.op,add)==0){
						if(schema->getFieldType(node->body.expr.arg1->body.colref.arg1)=="INT" &&
							schema->getFieldType(node->body.expr.arg2->body.colref.arg1)=="INT"){
								for(int ct=0;ct<origSet.size();ct++){
									origSet.at(ct).setField(schema->getFieldPos(node->body.expr.arg1->body.colref.arg1), origSet.at(ct).getInt(schema->getFieldPos(node->body.expr.arg1->body.colref.arg1))+  
										origSet.at(ct).getInt(schema->getFieldPos(node->body.expr.arg2->body.colref.arg1))
										);
									r->push_back(origSet.at(ct));
								}
						}

					}else if(node->body.expr.op == "-"){
					}else if(node->body.expr.op=="*"){
					}else if(node->body.expr.op=="/"){
					}
					cout<<"here\n"<<endl;
					return r;
			} else if(node->body.expr.arg1->nodetype==expr_node && node->body.expr.arg2==NULL){
				vector<Tuple>* leftNode = select_by_node_single_relation(node->body.expr.arg1,origSet,schema,relationFieldMap,stmtNo);
				vector<Tuple>* rightNode  = select_by_node_single_relation(node->body.expr.arg2,origSet,schema,relationFieldMap,stmtNo);

				vector<Tuple> *r = new vector<Tuple>;
				map<string,vector<string>>::iterator it = relationFieldMap.find(calc.stmt_vector[stmtNo]->body.stmt.arg2->body.list.arg1->body.variable);
				if(node->body.expr.type==not){
					int ct2;
					for(ct2=0;ct2<origSet.size();ct2++){
						int ct3;
						bool found=false;
						int ct4;
						// take orig. set, compare with left node
						for(ct3=0;ct3< leftNode->size();ct3++){
							for(ct4=0;ct4<it->second.size();ct4++){
								if( ((Tuple)origSet.at(ct2)).getInt(schema->getFieldPos(it->second.at(ct4)))==0){
									// Not a number field, getString
									if(((Tuple)origSet.at(ct2)).getString(schema->getFieldPos(it->second.at(ct4))) ==
										leftNode->at(ct3).getString(schema->getFieldPos(it->second.at(ct4))) ){
											found=true;
									}
								}else{
									if( ((Tuple)origSet.at(ct2)).getInt(schema->getFieldPos(it->second.at(ct4))) ==
										leftNode->at(ct3).getInt(schema->getFieldPos(it->second.at(ct4)))){
											found=true;
									}
								}
							}
						}
						if(found == false){r->push_back(origSet.at(ct2));}		
					}
					return r;
				}

			}else if(node->body.expr.arg1->nodetype==expr_node && node->body.expr.arg2->nodetype==number_node){

				vector<Tuple>* leftNode = select_by_node_single_relation(node->body.expr.arg1,origSet,schema,relationFieldMap,stmtNo);
				printf("expr & number\n");

				//for(int ct=0; ct< leftNode->size();ct++){
					//cout<< leftNode->at(ct).getInt(schema->getFieldPos("exam"))<<endl;
				//}
				int i;
				i = node->body.expr.arg2->body.number;
				vector<Tuple> *r = new vector<Tuple>;
				int ct;

				if(node->body.expr.op[0] == '='){
							// find tuple(s) that would match condition
							for(ct=0;ct<leftNode->size();ct++){
								if(i == ((Tuple)leftNode->at(ct)).getInt(
									schema->getFieldPos(node->body.expr.arg1->body.expr.arg1->body.colref.arg1)) ){
										r->push_back(leftNode->at(ct));
								}
							}
				}else if(node->body.expr.op[0] == '<'){
							// find tuple(s) that would match condition
							for(ct=0;ct<leftNode->size();ct++){
								if(i > ((Tuple)leftNode->at(ct)).getInt(
									schema->getFieldPos(node->body.expr.arg1->body.expr.arg1->body.colref.arg1)) ){
										r->push_back(leftNode->at(ct));
								}
							}
				}else if(node->body.expr.op[0] == '>'){
							// find tuple(s) that would match condition
							for(ct=0;ct<leftNode->size();ct++){
								if(i < ((Tuple)leftNode->at(ct)).getInt(
									schema->getFieldPos(node->body.expr.arg1->body.expr.arg1->body.colref.arg1)) ){
										r->push_back(leftNode->at(ct));
								}
							}
					}
				cout<< r->size()<<endl;
					return r;
			}else{
				vector<Tuple>* leftNode = select_by_node_single_relation(node->body.expr.arg1,origSet,schema,relationFieldMap,stmtNo);
				vector<Tuple>* rightNode  = select_by_node_single_relation(node->body.expr.arg2,origSet,schema,relationFieldMap,stmtNo);

				vector<Tuple> *r = new vector<Tuple>;
				map<string,vector<string>>::iterator it = relationFieldMap.find(calc.stmt_vector[stmtNo]->body.stmt.arg2->body.list.arg1->body.variable);
				if(node->body.expr.type==binary && node->body.expr.op[0] == 'A'){
					int ct2;
					for(ct2=0;ct2<leftNode->size();ct2++){
						int ct3;
						int found;
						int ct4;
						// take leftNode, compare with rightNode
						for(ct3=0;ct3<rightNode->size();ct3++){
							found=it->second.size();
							for(ct4=0;ct4<it->second.size();ct4++){
								if(leftNode->at(ct2).getInt(schema->getFieldPos(it->second.at(ct4)))==0){
									// Not a number field, getString
									if( leftNode->at(ct2).getString(schema->getFieldPos(it->second.at(ct4))) ==
										rightNode->at(ct3).getString(schema->getFieldPos(it->second.at(ct4))) ){
											found--;
									}
								}else{
									if( leftNode->at(ct2).getInt(schema->getFieldPos(it->second.at(ct4))) ==
										rightNode->at(ct3).getInt(schema->getFieldPos(it->second.at(ct4)))){
											found--;
									}
								}
							}
							// match found
							if(found==0){r->push_back(leftNode->at(ct2));}						
						}
					}
					return r;
				}else if(node->body.expr.type==binary && node->body.expr.op[0] == 'O'){
					for(int ct=0;ct<rightNode->size();ct++){
						r->push_back(rightNode->at(ct));
					}
					int ct2;
					for(ct2=0;ct2<leftNode->size();ct2++){
						int ct3;
						int found;
						int ct4;
						// take leftNode, compare with rightNode
						for(ct3=0;ct3<rightNode->size();ct3++){
							found=it->second.size();
							for(ct4=0;ct4<it->second.size();ct4++){
								if(leftNode->at(ct2).getInt(schema->getFieldPos(it->second.at(ct4)))==0){
									// Not a number field, getString
									if( leftNode->at(ct2).getString(schema->getFieldPos(it->second.at(ct4))) ==
										rightNode->at(ct3).getString(schema->getFieldPos(it->second.at(ct4))) ){
											found--;
									}
								}else{
									if( leftNode->at(ct2).getInt(schema->getFieldPos(it->second.at(ct4))) ==
										rightNode->at(ct3).getInt(schema->getFieldPos(it->second.at(ct4)))){
											found--;
									}
								}
							}
							// match found
							if(found!=0){
								r->push_back(leftNode->at(ct3));
							}						
						}
					}
					return r;
				}else if(node->body.expr.type==not){
					int ct2;
					for(ct2=0;ct2<origSet.size();ct2++){
						int ct3;
						bool found=false;
						int ct4;
						// take orig. set, compare with left node
						for(ct3=0;ct3< leftNode->size();ct3++){
							for(ct4=0;ct4<it->second.size();ct4++){
								if( ((Tuple)origSet.at(ct2)).getInt(schema->getFieldPos(it->second.at(ct4)))==0){
									// Not a number field, getString
									if(((Tuple)origSet.at(ct2)).getString(schema->getFieldPos(it->second.at(ct4))) ==
										leftNode->at(ct3).getString(schema->getFieldPos(it->second.at(ct4))) ){
											found=true;
									}
								}else{
									if( ((Tuple)origSet.at(ct2)).getInt(schema->getFieldPos(it->second.at(ct4))) ==
										leftNode->at(ct3).getInt(schema->getFieldPos(it->second.at(ct4)))){
											found=true;
									}
								}
							}
						}
						if(found == false){r->push_back(origSet.at(ct2));}		
					}				
					return r;
				}
			}
		}
		return NULL;
	}

	void Driver::insert_by_node(tree* node,vector<string> &fieldNames,vector<atom_ref> &fieldValues,char* op){
		if(node!=NULL){
			if(node->body.list.arg1==NULL && node->body.list.arg2==NULL){
				return;
			}
			else if(node->body.list.arg2==NULL){

				if(op == "n"){
					fieldNames.push_back( string((const char*)node->body.list.arg1->body.colref.arg1) );
				}else{
					if(node->body.list.arg1->nodetype == literal_node){
						atom_ref ar;
						ar.atom_type=literal_node;
						string s;
						s = string((const char*)node->body.list.arg1->body.variable);
						s.erase(
							remove(s.begin(),s.end(),'\"'),s.end()
							);
						char *cstr;
						cstr=new char[s.size()+1];
						strcpy(cstr,s.c_str());
						ar.string_value= cstr;
						fieldValues.push_back(ar);
					}else{
						atom_ref ar;
						ar.atom_type=number_node;
						ar.int_value=node->body.list.arg1->body.number;
						fieldValues.push_back(ar);
					}

				}


			}else{

				if(op == "n"){
					insert_by_node(node->body.list.arg1,fieldNames,fieldValues,op);			
					fieldNames.push_back( string((const char*)node->body.list.arg2->body.colref.arg1) );
				}else{
					insert_by_node(node->body.list.arg1,fieldNames,fieldValues,op);		
	
					if(node->body.list.arg2->nodetype == literal_node){
						atom_ref ar;
						ar.atom_type=literal_node;
						string s;
						s = string((const char*)node->body.list.arg2->body.variable);
						s.erase(
							remove(s.begin(),s.end(),'\"'),s.end()
							);
						char *cstr;
						cstr=new char[s.size()+1];
						strcpy(cstr,s.c_str());
						ar.string_value= cstr;
						fieldValues.push_back(ar);
					}else{
						atom_ref ar;
						ar.atom_type=number_node;
						ar.int_value=node->body.list.arg2->body.number;
						fieldValues.push_back(ar);
					}

				}

			}
		}

	}

/*	map<string,vector<tuples>> Driver::natutal_join(const vector<string> *total_relations, SchemaManager &schemaMgr, tree* condition, 
										std::map<string,std::vector<column_ref>*> *attributes_to_project,
										map <string,vector<string>> relationFieldMap,
										vector<tuples>> arg1, map<string,vector<tuples>> arg2)
	{
				

		
	}
*/

	int Driver::project_relation(vector<column_ref> *attributes, vector<string> relationFields, 
											SchemaManager &schemaMgr, MainMemory &mem, string rel_name) 
	{
		if(attributes->size()==0){
			return 0;
		}
		Driver::clearMemory(mem);
		Relation* relationPtr = schemaMgr.getRelation(rel_name);
		int last_rel_size = relationPtr->getNumOfBlocks();
		Schema schema = *(schemaMgr.getSchema(rel_name));
		vector<string> cols(attributes->size());
		for (int i=0; i<attributes->size();i++) {
			cols[i] = (*attributes)[i].column_name;
		}
		int size =relationPtr->getNumOfBlocks();
		if (mem.getMemorySize()>relationPtr->getNumOfBlocks()){
			Driver::get_blocks_to_mem(0,mem,size,relationPtr);
			for(int i=0; i<size; i++){
//				mem.dumpMemory();
				Driver::project_rel_block(cols,relationFields, *mem.getBlock(i), schema);
//				mem.dumpMemory();
				relationPtr->writeBlockFromMemory(size+i,i);
			}
		}	
		else {
			for(int i=0; i<size; i++) {
				relationPtr->readBlockToMemory(i,0);
				Driver::project_rel_block(cols,relationFields, *mem.getBlock(i), schema);				
				relationPtr->writeBlockFromMemory(size+i,0);
			}
		}
		return size;
	}

	void Driver::project_rel_block(std::vector<string> attributes,  vector<string> relationFields, 
											Block &b , Schema &schema) 
	{

		for(int i=0; i<b.getNumTuples();i++) {
			Tuple temp = b.getTuple(i);
			temp.printTuple();
			Tuple t(&schema);
			b.setTuple(i,t);
			t.printTuple();
			for (int j=0; j<relationFields.size();j++) {
				std::vector<string>::iterator  it= std::find(attributes.begin(),attributes.end(),relationFields[j]);
				if(it != attributes.end() ){
					if(schema.getFieldType(relationFields[j]) == "STR20") {
						t.setField(schema.getFieldPos(relationFields[j]),temp.getString(schema.getFieldPos(relationFields[j])));
					}
					if(schema.getFieldType(relationFields[j]) == "INT") {
						t.setField(schema.getFieldPos(relationFields[j]),temp.getInt(schema.getFieldPos(relationFields[j])));
					}					
				}
			}
			t.printTuple();
			b.setTuple(i,t);
		}
		return;
	}


	vector<pair<Tuple,Tuple>> Driver::execute_condition(const vector<string> *total_relations,const int stmtNo,
											SchemaManager &schemaMgr, tree* condition, 
											std::map<string,std::vector<column_ref>*> *attributes_to_project,
											map <string,vector<string>> relationFieldMap, MainMemory &mem,
											map<string,int> &disk_proj, map<string,int> &disk_proj_size, int nest_level,
											std::map<string,std::vector<column_ref>*> attributes_to_print, int rel_offset)
	{
		vector<pair<Tuple,Tuple>> condition_relations1;
		vector<pair<Tuple,Tuple>> condition_relations2;
		if(condition->body.expr.type==binary) {
			tree *arg1 = condition->body.expr.arg1;
			tree *arg2 = condition->body.expr.arg2;
			string relation_name1,relation_name2,column_name1,column_name2;
			if(arg1->nodetype==colref_node) {
				relation_name1 = arg1->body.colref.arg1;
				column_name1 = arg1->body.colref.arg2->body.variable;
				disk_proj[relation_name1] = Driver::project_relation((*attributes_to_project)[relation_name1],relationFieldMap[relation_name1],schemaMgr,mem,relation_name1);				
			}
			if(arg2->nodetype==colref_node) {
				relation_name2 = arg2->body.colref.arg1;
				column_name2 = arg2->body.colref.arg2->body.variable;
				disk_proj[relation_name2] = Driver::project_relation((*attributes_to_project)[relation_name2],relationFieldMap[relation_name2],schemaMgr,mem,relation_name2);				
			}

			if(arg2->nodetype==number_node || arg2->nodetype==literal_node){
				condition->body.expr.res_size = disk_proj[relation_name1];
				Relation *relationPtr = schemaMgr.getRelation(relation_name1);
				int start_disk_block = start_disk_block=disk_proj[relation_name1];
				Schema *schema = schemaMgr.getSchema(relation_name1);
				bool str = ((schema->getFieldType(column_name1))=="STR20")?true:false;
				vector<Tuple> result;
				if (mem.getMemorySize()>=relationPtr->getNumOfBlocks()){
					Driver::get_blocks_to_mem(0,mem,disk_proj_size[relation_name1],relationPtr,start_disk_block);
					for(int i=0; i<relationPtr->getNumOfBlocks(); i++){
						vector<Tuple> tuples= mem.getBlock(i)->getTuples();
						for(int j=0; j<tuples.size(); j++){
							Tuple t =tuples[j]; 
							if(strcmp(condition->body.expr.op,">")==0) {
								if(str) {
									if(t.getString(schema->getFieldPos(column_name1))>arg2->body.variable){									
										result.push_back(t);
									}
									else {
										Tuple t(schema);
										tuples[j] = t;
									}
								}
								else {
									if(t.getInt(schema->getFieldPos(column_name1))>arg2->body.number){									
										result.push_back(t);
									}
									else {
										Tuple t(schema);
										tuples[j] = t;
									}
								}
							}
							else if(strcmp(condition->body.expr.op,"<")==0) {
								if(str) {
									if(t.getString(schema->getFieldPos(column_name1))<arg2->body.variable){									
										result.push_back(t);
									}
									else {
										Tuple t(schema);
										tuples[j] = t;
									}
								}
								else {
									
									if(t.getInt(schema->getFieldPos(column_name1))<arg2->body.number){									
										result.push_back(t);
									}
									else {
										Tuple t(schema);
										tuples[j] = t;
									}
								}
							}
							else if(strcmp(condition->body.expr.op,"=")==0) {
								if(str) {
									if(t.getString(schema->getFieldPos(column_name1))==arg2->body.variable){									
										result.push_back(t);
									}
									else {
										Tuple t(schema);
										tuples[j] = t;
									}
								}
								else {
									if(t.getInt(schema->getFieldPos(column_name1))==arg2->body.number){									
										result.push_back(t);
									}
									else {
										Tuple t(schema);
										tuples[j] = t;
									}
								}
							}								
							
						}
						relationPtr->writeBlockFromMemory(disk_proj[relation_name1],i);

					}
				}	
				else {
					for(int i=0; i<relationPtr->getNumOfBlocks(); i++) {
						relationPtr->readBlockToMemory(start_disk_block,0);
						vector<Tuple> tuples= mem.getBlock(0)->getTuples();
						for(int j=0; j<tuples.size(); j++){
							Tuple t =tuples[j]; 
							if(strcmp(condition->body.expr.op,">")==0) {
								if(str) {
									if(t.getString(schema->getFieldPos(column_name1))>arg2->body.variable){									
										result.push_back(t);
									}
									else {
										Tuple t(schema);
										tuples[j] = t;
									}
								}
								else {
									if(t.getInt(schema->getFieldPos(column_name1))>arg2->body.number){									
										result.push_back(t);
									}
									else {
										Tuple t(schema);
										tuples[j] = t;
									}
								}
							}
							else if(strcmp(condition->body.expr.op,"<")==0) {
								if(str) {
									if(t.getString(schema->getFieldPos(column_name1))<arg2->body.variable){									
										result.push_back(t);
									}
									else {
										Tuple t(schema);
										tuples[j] = t;
									}
								}
								else {
									if(t.getInt(schema->getFieldPos(column_name1))<arg2->body.number){									
										result.push_back(t);
									}
									else {
										Tuple t(schema);
										tuples[j] = t;
									}
								}
							}
							else if(strcmp(condition->body.expr.op,"=")==0) {
								if(str) {
									if(t.getString(schema->getFieldPos(column_name1))==arg2->body.variable){									
										result.push_back(t);
									}
									else {
										Tuple t(schema);
										tuples[j] = t;
									}
								}
								else {
									if(t.getInt(schema->getFieldPos(column_name1))==arg2->body.number){									
										result.push_back(t);
									}
									else {
										Tuple t(schema);
										tuples[j] = t;
									}
								}
							}								
							
						}
						relationPtr->writeBlockFromMemory(disk_proj[relation_name1],0);		
					}
				}
				
				if(nest_level==0) {
					Driver::print_result(result,*(attributes_to_print[relation_name1]),relationFieldMap[relation_name1],relation_name1,schemaMgr);
				}
				else {
					
				}
			}

	
			vector<pair<Tuple,Tuple>> result;
			if (arg1->nodetype==colref_node && arg2->nodetype==colref_node) {
				result = Driver::natural_join(relation_name1,relation_name2,schemaMgr,mem,attributes_to_project,0,relationFieldMap,condition->body.expr.op,column_name1,disk_proj,disk_proj_size);
				if(nest_level==0) {
					Driver::print_result(result,attributes_to_print,relationFieldMap,relation_name1,relation_name2,schemaMgr);

				}
				return result;
			}
			int proj_offset1 = 0, proj_offset2 = 0;

			if(strcmp(condition->body.expr.op,"OR")==0) {
				if(condition->body.expr.jtype==nojoin && arg1->body.expr.jtype==nojoin && arg2->body.expr.jtype==nojoin) {
					vector<Tuple> result;
					string relation_name1 = arg1->body.expr.arg1->body.colref.arg1;
					proj_offset1 = disk_proj[relation_name1];
					execute_condition(total_relations, stmtNo, schemaMgr, condition->body.expr.arg1,attributes_to_project,relationFieldMap,mem,disk_proj,disk_proj_size,nest_level+1,attributes_to_print, proj_offset1);					
					proj_offset2 = disk_proj[relation_name1]+arg1->body.expr.res_size;
					execute_condition(total_relations, stmtNo, schemaMgr, condition->body.expr.arg2,attributes_to_project,relationFieldMap,mem,disk_proj,disk_proj_size,nest_level+1,attributes_to_print, proj_offset2);	
					Relation *relationPtr1 = schemaMgr.getRelation(relation_name1);

					if(nest_level==0){
						for(int i=0; i<arg1->body.expr.res_size; i++){
							relationPtr1->readBlockToMemory(proj_offset1+i-1,0);
							vector<Tuple> result = (mem.getBlock(0))->getTuples();
							Driver::print_result(result,*attributes_to_print[relation_name1],relationFieldMap[relation_name1],relation_name1,schemaMgr);					
						}
						for(int i=0; i< arg2->body.expr.res_size; i++){
							relationPtr1->readBlockToMemory(proj_offset2+i-1,0);
							vector<Tuple> result = (mem.getBlock(0))->getTuples();
							Driver::print_result(result,*attributes_to_print[relation_name1],relationFieldMap[relation_name1],relation_name1,schemaMgr);					
						}
					}
					int i=0;
					for(; i<arg2->body.expr.res_size;i++) {
						relationPtr1->readBlockToMemory(proj_offset2+i,0);
						relationPtr1->writeBlockFromMemory(proj_offset2+condition->body.expr.res_size+i,0);		
						
					}
				}

				if(condition->body.expr.jtype==join && arg1->body.expr.jtype==nojoin && arg2->body.expr.jtype==nojoin) {
					vector<Tuple> result;
					string relation_name1 = arg1->body.expr.arg1->body.colref.arg1;
					string relation_name2 = arg2->body.expr.arg1->body.colref.arg1;
					proj_offset1 = disk_proj[relation_name1]+disk_proj_size[relation_name1];
					execute_condition(total_relations, stmtNo, schemaMgr, condition->body.expr.arg1,attributes_to_project,relationFieldMap,mem,disk_proj,disk_proj_size,nest_level+1,attributes_to_print, proj_offset1);					
					proj_offset2 = disk_proj[relation_name2]+disk_proj_size[relation_name2];
					execute_condition(total_relations, stmtNo, schemaMgr, condition->body.expr.arg2,attributes_to_project,relationFieldMap,mem,disk_proj,disk_proj_size,nest_level+1,attributes_to_print, proj_offset2);	
					Relation *relationPtr1 = schemaMgr.getRelation(relation_name1);
					Relation *relationPtr2 = schemaMgr.getRelation(relation_name2);
					if(nest_level==0){
						for(int i=0; i<arg1->body.expr.res_size; i++){
							relationPtr1->readBlockToMemory(proj_offset1+i-1,0);
							vector<Tuple> result = (mem.getBlock(0))->getTuples();
							Driver::print_result(result,*attributes_to_print[relation_name1],relationFieldMap[relation_name1],relation_name1,schemaMgr);					
						}
						for(int i=0; i< arg2->body.expr.res_size; i++){
							relationPtr1->readBlockToMemory(proj_offset2+i-1,0);
							vector<Tuple> result = (mem.getBlock(0))->getTuples();
							Driver::print_result(result,*attributes_to_print[relation_name2],relationFieldMap[relation_name2],relation_name2,schemaMgr);					
						}
					}
					int i=0;
					for(; i<arg2->body.expr.res_size;i++) {
						relationPtr1->readBlockToMemory(proj_offset2+i,0);
						relationPtr1->writeBlockFromMemory(proj_offset2+condition->body.expr.res_size+i,0);		
						
					}
				}

				
			}
			
			if(strcmp(condition->body.expr.op,"AND")==0) {
				if(condition->body.expr.jtype==join && arg1->body.expr.jtype==join && arg2->body.expr.jtype==nojoin) {
					vector<Tuple> result;
					string relation_name = (*condition->body.expr.j->arg2_relations)[0];
					Driver::execute_condition(total_relations,stmtNo,schemaMgr,condition->body.expr.arg2,attributes_to_project,relationFieldMap,mem,disk_proj,disk_proj_size,nest_level+1,attributes_to_print);
					string relation_name1 = arg1->body.expr.arg1->body.colref.arg1;
					string relation_name2 = arg1->body.expr.arg2->body.colref.arg1;
					 vector<pair<Tuple,Tuple>> res = Driver::execute_condition(total_relations, stmtNo, schemaMgr, condition->body.expr.arg1,attributes_to_project,relationFieldMap,mem,disk_proj,disk_proj_size,nest_level+1,attributes_to_print);					

					if(nest_level==0) {

						Driver::print_result(res,attributes_to_print,relationFieldMap,relation_name1,relation_name2,schemaMgr);
					}					
				}

			}
	

			if(condition->body.expr.jtype==join) {
				if(proj_offset1!=0 && proj_offset2!=0) {
					map<string,int> disk_p;
					disk_p[relation_name1] = proj_offset1;
					disk_p[relation_name2] = proj_offset2;
				}

			}

			if(condition_relations1.size()==0) {
				return condition_relations2;				
			}
			else if(condition_relations2.size()==0) {
				return condition_relations1;
			}
			else{
				condition->body.expr.jtype = join; 
				condition->body.expr.j = new(join_n);
			}
		
		}
		else if(condition->nodetype==paren) {
		}
		else if(condition->nodetype==not) {
		}
		condition_relations1.insert(condition_relations1.end(),condition_relations2.begin(),condition_relations2.end());
		return condition_relations1;
	}


	void Driver::print_result(vector<Tuple> res, vector<column_ref> attributes_to_print,
								vector<string> attributes,string relation, SchemaManager &schemaMgr) 
	{

		for(int i=0; i<res.size(); i++) {
			Tuple tuple = res[i];

			Schema *schema = schemaMgr.getSchema(relation);

			if(attributes_to_print.size()!=0) {
				cout<< attributes_to_print[0].relation_name<<":: \t";
				for(int j=0; j<attributes_to_print.size();j++){
					int pos = schema->getFieldPos(attributes_to_print[j].column_name);
					if(schema->getFieldType(attributes_to_print[j].column_name)=="STR20") {
						if(tuple.getString(pos)=="") break;
						cout<< attributes_to_print[j].column_name<<":"<<(tuple.getString(pos))<<" \t";
					}
					else {
						if(tuple.getInt(pos)==-1) break;
						cout<< attributes_to_print[j].column_name<<":"<<(tuple.getInt(pos))<<" \t";
					}	
				}
			}
			else{
				cout<< attributes[0]<<":: \t";
				for(int j=0; j<attributes.size();j++){
					int pos = schema->getFieldPos(attributes[j]);
					if(schema->getFieldType(attributes[j])=="STR20") {
						if(tuple.getString(pos)=="") break;
						cout<< attributes[j]<<":"<<(tuple.getString(pos))<<" \t";
					}
					else {
						if(tuple.getInt(pos)==-1) break;
						cout<< attributes[j]<<":"<<(tuple.getInt(pos))<<" \t";
					}	
				}
			}

			cout<<endl;
		}
	}
	void Driver::print_result(vector<pair<Tuple,Tuple>> res, std::map<string,std::vector<column_ref>*> attributes_to_print,
								map <string,vector<string>> relationFieldMap,string relation1, string relation2, SchemaManager &schemaMgr) 
	{

		for(int i=0; i<res.size(); i++) {
			pair<Tuple, Tuple> pt = res[i];
			Tuple tuple1 = pt.first;
			Tuple tuple2 = pt.second;
			Schema *schema1 = schemaMgr.getSchema(relation1);
			Schema *schema2 = schemaMgr.getSchema(relation2);

			if(attributes_to_print.size()!=0) {
				vector<column_ref> attributes1 = *(attributes_to_print[relation1]);
				vector<column_ref> attributes2 = *(attributes_to_print[relation2]);
				cout<< attributes1[0].relation_name<<":: \t";
				for(int j=0; j<attributes1.size();j++){
					int pos = schema1->getFieldPos(attributes1[j].column_name);
					if(schema1->getFieldType(attributes1[j].column_name)=="STR20") {
						cout<< attributes1[j].column_name<<":"<<(tuple1.getString(pos))<<" \t";
					}
					else {
						cout<< attributes1[j].column_name<<":"<<(tuple1.getInt(pos))<<" \t";
					}	
				}
				cout<< attributes2[0].relation_name<<":: \t";
				for(int j=0; j<attributes2.size();j++){
					int pos = schema2->getFieldPos(attributes2[j].column_name);
					if(schema2->getFieldType(attributes2[j].column_name)=="STR20") {
						cout<< attributes2[j].column_name<<":"<<(tuple2.getString(pos))<<" \t";
					}
					else {
						cout<< attributes2[j].column_name<<":"<<(tuple2.getInt(pos))<<" \t";
					}	
				}

			}

			else{
				vector<string> attributes1 = relationFieldMap[relation1];
				vector<string> attributes2 = relationFieldMap[relation2];
				cout<< attributes1[0]<<":: \t";
				for(int j=0; j<attributes1.size();j++){
					int pos = schema1->getFieldPos(attributes1[j]);
					if(schema1->getFieldType(attributes1[j])=="STR20") {
						cout<< attributes1[j]<<":"<<(tuple1.getString(pos))<<" \t";
					}
					else {
						cout<< attributes1[j]<<":"<<(tuple1.getInt(pos))<<" \t";
					}	
				}
				cout<< attributes2[0]<<":: \t";
				for(int j=0; j<attributes2.size();j++){
					int pos = schema2->getFieldPos(attributes2[j]);
					if(schema2->getFieldType(attributes2[j])=="STR20") {
						cout<< attributes1[j]<<":"<<(tuple2.getString(pos))<<" \t";
					}
					else {
						cout<< attributes1[j]<<":"<<(tuple2.getInt(pos))<<" \t";
					}	
				}
			}

			cout<<endl;
		}
	}

	vector<string> Driver::process_condition(const vector<string> *total_relations,const int stmtNo,
											SchemaManager &schemaMgr, tree* condition, 
											std::map<string,std::vector<column_ref>*> *attributes_to_project){
		vector<string> condition_relations1;
		vector<string> condition_relations2;
		if(condition->body.expr.type==binary) {
			tree *arg1 = condition->body.expr.arg1;
			tree *arg2 = condition->body.expr.arg2;
			if(arg1->nodetype==colref_node) {
				column_ref cr;
				if(arg1->body.colref.arg2==NULL){						
					cr.column_name = arg1->body.colref.arg1;
					cr.relation_name = new char();
					std::strcpy(cr.relation_name,total_relations->front().c_str());
				}
				else {
					cr.column_name = arg1->body.colref.arg2->body.variable;
					cr.relation_name = arg1->body.colref.arg1;
				}
				if ( (*attributes_to_project)[cr.relation_name]->size()!=0) { 
					vector<column_ref> *cols= (*attributes_to_project)[cr.relation_name];
					vector<column_ref>::iterator pos = std::find(cols->begin(),cols->end(),cr);
					if(pos==cols->end()){
						(*attributes_to_project)[cr.relation_name]->push_back(cr);
					}
				}
				condition_relations1.push_back(cr.relation_name);
			}
			if(arg2->nodetype==colref_node) {
				column_ref cr;
				if(arg2->body.colref.arg2==NULL){						
					cr.column_name = arg2->body.colref.arg1;
					cr.relation_name = new char();
					std::strcpy(cr.relation_name,total_relations->front().c_str());
				}
				else {
					cr.column_name = arg2->body.colref.arg2->body.variable;
					cr.relation_name = arg2->body.colref.arg1;
				}
				if ((*attributes_to_project)[cr.relation_name]->size()) { 
					vector<column_ref> *cols= (*attributes_to_project)[cr.relation_name];
					vector<column_ref>::iterator pos = std::find(cols->begin(),cols->end(),cr);
					if(pos==cols->end()){
						(*attributes_to_project)[cr.relation_name]->push_back(cr);
					}
				}
				condition_relations2.push_back(cr.relation_name);
			}
			
			if(arg1->nodetype==number_node || arg2->nodetype==number_node || arg2->nodetype==literal_node || arg1->nodetype==literal_node) {
			} 
			if(arg1->nodetype==expr_node) {
				condition_relations1 = process_condition(total_relations, stmtNo, schemaMgr, condition->body.expr.arg1,attributes_to_project);
			}
			if(arg2->nodetype==expr_node) {
				condition_relations2 = process_condition(total_relations, stmtNo, schemaMgr, condition->body.expr.arg2,attributes_to_project);
			}	

			if(condition_relations1.size()==0) {
				return condition_relations2;				
			}
			else if(condition_relations2.size()==0) {
				return condition_relations1;
			}
			else if(condition_relations1.size()==1 && condition_relations2.size()==1 && condition_relations1[0]==condition_relations2[0]) {

			}
			else{
				condition->body.expr.jtype = join; 
				condition->body.expr.j = new(join_n);
				condition->body.expr.j->arg1_relations  = new vector<string>(condition_relations1);
				condition->body.expr.j->arg2_relations  = new vector<string>(condition_relations2);
			}

			if(Driver::relations_get_size(condition_relations1, schemaMgr)> relations_get_size(condition_relations1,schemaMgr)){
				//execute the right argument first.
			}
		
		}
		else if(condition->nodetype==paren) {
		}
		else if(condition->nodetype==not) {
		}
		condition_relations1.insert(condition_relations1.end(),condition_relations2.begin(),condition_relations2.end());
		return condition_relations1;
	}

	unsigned long Driver::relations_get_size(const vector<string> &relations, SchemaManager &schemaMgr) {
		long size=0;
		for (int i = 0; i < relations.size(); i++){
			size += schemaMgr.getRelation(relations[i])->getNumOfTuples();
		}
		return size;
	}

	void Driver::get_relations(const int stmtNo, vector<string> *relations, SchemaManager &schemaMgr,
		std::map<string,std::vector<column_ref>*> *attributes_to_project,map<string,int> &disk_proj, 
		map<string,int> &disk_proj_size){
		tree *relation_list = (tree*)calc.stmt_vector[stmtNo]->body.stmt.arg2->body.list.arg1;
	    tree *relation      = (tree*)calc.stmt_vector[stmtNo]->body.stmt.arg2->body.list.arg2;
		while(relation!=NULL){
			relations->push_back(relation->body.variable);	
			(*attributes_to_project)[relation->body.variable]=new vector<column_ref>;
			cout<<"relation: "<<relation->body.variable<<endl;
			disk_proj[relation->body.variable]=0;
			Relation *relationptr = schemaMgr.getRelation(relation->body.variable);
			disk_proj_size[relation->body.variable]= relationptr->getNumOfBlocks();
			if(relation_list->nodetype==variable_node){
				break;
			}
			relation = relation_list->body.list.arg2;
			relation_list = relation_list->body.list.arg1;
			
		}
		relations->push_back(relation_list->body.variable);	
		(*attributes_to_project)[relation_list->body.variable]=new vector<column_ref>;
		cout<<"relation: "<<relation_list->body.variable<<endl;
		disk_proj[relation_list->body.variable]=0;
		Relation *relationptr = schemaMgr.getRelation(relation_list->body.variable);
		disk_proj_size[relation_list->body.variable]= relationptr->getNumOfBlocks();

		return;
	}

	void Driver::get_columns(const vector<string> *total_relations, const int stmtNo, SchemaManager &schemaMgr, 
							vector<column_ref> *columns, std::map<string,std::vector<column_ref>*> *attributes_to_project){
		if(calc.stmt_vector[stmtNo]->body.stmt.arg1->nodetype==variable_node && 
				std::strcmp(calc.stmt_vector[stmtNo]->body.stmt.arg1->body.variable,"*")==0){
			return;
		}		
		tree *columns_list = (tree*)calc.stmt_vector[stmtNo]->body.stmt.arg1->body.list.arg1;
	    tree *column      = (tree*)calc.stmt_vector[stmtNo]->body.stmt.arg1->body.list.arg2;
		column_ref cr;

		while(column!=NULL){			
			if(column->body.colref.arg2==NULL){
				cr.column_name = column->body.colref.arg1;
				cr.relation_name = new char();
				std::strcpy(cr.relation_name,total_relations->front().c_str());
				cout<<"column: "<<cr.column_name<<endl;
				(*attributes_to_project)[total_relations->front()]->push_back(cr);
			}
			else{
				cr.column_name = column->body.colref.arg2->body.variable;
				cr.relation_name = column->body.colref.arg1;
				cout<<"table.column: "<<cr.relation_name<<"."<<cr.column_name<<endl;
				(*attributes_to_project)[cr.relation_name]->push_back(cr);
			}
			columns->push_back(cr);	

			if(columns_list->nodetype==colref_node){
				break;
			}
			column = columns_list->body.list.arg2;
			columns_list = columns_list->body.list.arg1;
			
		}
		if(columns_list->body.colref.arg2==NULL){
			cr.column_name = columns_list->body.colref.arg1;
			cr.relation_name = new char();
			std::strcpy(cr.relation_name,total_relations->front().c_str());
			cout<<"column: "<<cr.column_name<<endl;
			(*attributes_to_project)[total_relations->front()]->push_back(cr);

		}
		else{
			cr.column_name = columns_list->body.colref.arg2->body.variable;
			cr.relation_name = columns_list->body.colref.arg1;
			cout<<"table.column: "<<cr.relation_name<<"."<<cr.column_name<<endl;
			(*attributes_to_project)[cr.relation_name]->push_back(cr);
		}
		columns->push_back(cr);	
		return;

	}
	void Driver::get_attribute_relations(vector<string> *total_relations,const column_ref column, 
										SchemaManager &schemaMgr, vector<string> *atr_relations){
		for (int i = 0; i < total_relations->size(); i++){
			string relation_name = (*total_relations)[i];
			Schema* rel_schema = schemaMgr.getSchema(relation_name);
			int pos = rel_schema->getFieldPos(column.column_name);
			if(pos!=-1){   /* field found in relation */
				vector<string>::iterator p = find(total_relations->begin(), total_relations->end(), relation_name);    
				if (p != total_relations->end()) {
					atr_relations->push_back(relation_name);
				}
			}
		}	
	}

	bool Driver::parse_file(const std::string &filename)
	{
		std::ifstream in(filename.c_str());
		if (!in.good()) return false;
		return parse_stream(in, filename);
	}

	bool Driver::parse_string(const std::string &input, const std::string& sname)
	{
		std::istringstream iss(input);
		return parse_stream(iss, sname);
	}

	void Driver::error(const class location& l,
		const std::string& m)
	{
		std::cerr << l << ": " << m << std::endl;
	}

	void Driver::error(const std::string& m)
	{
		std::cerr << m << std::endl;
	}

} // namespace example
