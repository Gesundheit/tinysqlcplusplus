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
					run_select(stmtNo,schemaMgr);
					break;
				}
				case delete_st:{
					run_delete(stmtNo,schemaMgr,mem,relationFieldMap);
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
			schema.printSchema();
			cout << "The schema before createRelation has " << schema.getNumOfFields() << " fields" << endl;
			cout << "The schema before createRelation has " << schema.getNumOfInt() << " integers" << endl;
			cout << "The schema before createRelation has " << schema.getNumOfString() << " strings" << endl;
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

	bool Driver::run_select(int stmtNo, SchemaManager &schemaMgr){
		vector<string> *relations  = new vector<string>;
		std::map<string,std::vector<column_ref>*> *attributes_to_project = new std::map<string,std::vector<column_ref>*>;
		get_relations(stmtNo, relations,attributes_to_project);	

		vector<column_ref> *columns  = new vector<column_ref>;
		get_columns(relations,stmtNo, schemaMgr, columns, attributes_to_project);
		
		if(calc.stmt_vector[stmtNo]->body.stmt.arg3!=NULL){
			process_condition(relations, stmtNo, schemaMgr, calc.stmt_vector[stmtNo]->body.stmt.arg3, attributes_to_project);
		}
		else{
			cout<<"No condition"<<endl;
		}
		return true;
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
					printf("%d\n",i);
					vector<Tuple> *r = new vector<Tuple>;
					int ct;
					// find tuple(s) that would match condition
					for(ct=0;ct<origSet.size();ct++){
						if(i == ((Tuple)origSet.at(ct)).getInt(
						schema->getFieldPos(node->body.expr.arg1->body.colref.arg1)) ){
							r->push_back(origSet.at(ct));
						}
					}
					return r;
			} else if(node->body.expr.arg1->nodetype==number_node &&
				node->body.expr.arg2->nodetype==number_node){
					// not handling for now
			} else{
				delete_by_node(node->body.expr.arg1,origSet,schema);
				delete_by_node(node->body.expr.arg2,origSet,schema);
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
					(*attributes_to_project)[total_relations->front()]->push_back(cr);
				}
				else {
					cr.column_name = arg1->body.colref.arg2->body.variable;
					cr.relation_name = arg1->body.colref.arg1;
					(*attributes_to_project)[cr.relation_name]->push_back(cr);
				}
				condition_relations1.push_back(cr.relation_name);
			}
			if(arg2->nodetype==colref_node) {
				column_ref cr;
				if(arg2->body.colref.arg2==NULL){						
					cr.column_name = arg2->body.colref.arg1;	
					(*attributes_to_project)[total_relations->front()]->push_back(cr);
				}
				else {
					cr.column_name = arg2->body.colref.arg2->body.variable;
					cr.relation_name = arg2->body.colref.arg1;
					(*attributes_to_project)[cr.relation_name]->push_back(cr);
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

	void Driver::get_relations(const int stmtNo, vector<string> *relations, std::map<string,std::vector<column_ref>*> *attributes_to_project){
		tree *relation_list = (tree*)calc.stmt_vector[stmtNo]->body.stmt.arg2->body.list.arg1;
	    tree *relation      = (tree*)calc.stmt_vector[stmtNo]->body.stmt.arg2->body.list.arg2;
		while(relation!=NULL){
			relations->push_back(relation->body.variable);	
			(*attributes_to_project)[relation->body.variable]=new vector<column_ref>;
			cout<<"relation: "<<relation->body.variable<<endl;
			if(relation_list->nodetype==variable_node){
				break;
			}
			relation = relation_list->body.list.arg2;
			relation_list = relation_list->body.list.arg1;
			
		}
		relations->push_back(relation_list->body.variable);	
		(*attributes_to_project)[relation_list->body.variable]=new vector<column_ref>;
		cout<<"relation: "<<relation_list->body.variable<<endl;
		return;
	}

	void Driver::get_columns(const vector<string> *total_relations, const int stmtNo, SchemaManager &schemaMgr, 
							vector<column_ref> *columns, std::map<string,std::vector<column_ref>*> *attributes_to_project){
		tree *columns_list = (tree*)calc.stmt_vector[stmtNo]->body.stmt.arg1->body.list.arg1;
	    tree *column      = (tree*)calc.stmt_vector[stmtNo]->body.stmt.arg1->body.list.arg2;
		column_ref cr;
		while(column!=NULL){			
			if(column->body.colref.arg2==NULL){
				cr.column_name = column->body.colref.arg1;
				cr.relation_name = NULL;
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
			cr.relation_name = NULL;
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
