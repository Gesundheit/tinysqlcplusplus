// $Id: driver.cc 39 2008-08-03 10:07:15Z tb $
/** \file driver.cc Implementation of the example::Driver class. */

#include <fstream>
#include <sstream>

#include<cstdlib>
#include<string>
#include<string.h>
#include <algorithm>

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
		//schema = schemaMgr.getSchema( calc.stmt_vector[stmtNo]->body.stmt.arg1->body.variable );
		//if(schema == NULL){
		//	return false;
		//}	

		// Set up relationPtr
		Relation* relationPtr = NULL;
		//relationPtr = schemaMgr.getRelation( calc.stmt_vector[stmtNo]->body.stmt.arg1->body.variable);
		//if(relationPtr==NULL){
		//	return false;
		//}

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
					run_create(stmtNo,schemaMgr);
					break;
				   }
				case insert_st:{
					run_insert(stmtNo,schemaMgr,mem);
					break;
				   }
			}
		}


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
				if( node->body.list.arg1->body.colref.type==str20 ){
					fieldTypes.push_back(string("STR20"));
				}else{
					fieldTypes.push_back(string("INT"));
				}			   

			}
		}
	}

	bool Driver::run_create(int stmtNo, SchemaManager &schemaMgr){
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
		get_relations(stmtNo, relations);	

		vector<column_ref> *columns  = new vector<column_ref>;
		get_columns(stmtNo, columns);
		
		if(calc.stmt_vector[stmtNo]->body.stmt.arg3!=NULL){
			//parse expressions

		}
		else{
			cout<<"No condition"<<endl;
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
			printf("Erase\n");
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

		cout << "Append the tuple:" << endl;
		tuple.printTuple();
		cout << "The block is full? " << (block->isFull()==1?"true":"false") << endl;
		cout << "The block currently has " << block->getNumTuples() << " tuples:" << endl;
		block->printBlock();
		cout << endl;

		if(relationPtr->getNumOfBlocks()==0){
			relationPtr->writeBlockFromMemory(0,0);
		}else{
			relationPtr->writeBlockFromMemory(relationPtr->getNumOfBlocks()-1,0);
		}

		// Move the ptr
		if(block->isFull()==1){
			relationPtr->writeBlockFromMemory(relationPtr->getNumOfBlocks(),0);
		}

		relationPtr->printRelation();
		cout << endl;

		return true;
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
	void Driver::get_relations(const int stmtNo, vector<string> *relations){
		tree *relation_list = (tree*)calc.stmt_vector[stmtNo]->body.stmt.arg2->body.list.arg1;
	    tree *relation      = (tree*)calc.stmt_vector[stmtNo]->body.stmt.arg2->body.list.arg2;
		while(relation!=NULL){
			relations->push_back(relation->body.variable);	
			cout<<"relation: "<<relation->body.variable<<endl;
			if(relation_list->nodetype==variable_node){
				break;
			}
			relation = relation_list->body.list.arg2;
			relation_list = relation_list->body.list.arg1;
			
		}
		relations->push_back(relation_list->body.variable);	
		cout<<"relation: "<<relation_list->body.variable<<endl;
		return;
	}

	void Driver::get_columns(const int stmtNo, vector<column_ref> *columns){
		tree *columns_list = (tree*)calc.stmt_vector[stmtNo]->body.stmt.arg1->body.list.arg1;
	    tree *column      = (tree*)calc.stmt_vector[stmtNo]->body.stmt.arg1->body.list.arg2;
		column_ref cr;
		while(column!=NULL){			
			if(column->body.colref.arg2==NULL){
				cr.column_name = column->body.colref.arg1;
				cr.relation_name = NULL;
				cout<<"column: "<<cr.column_name<<endl;
			}
			else{
				cr.column_name = column->body.colref.arg2->body.variable;
				cr.relation_name = column->body.colref.arg1;
				cout<<"table.column: "<<cr.relation_name<<"."<<cr.column_name<<endl;
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
		}
		else{
			cr.column_name = columns_list->body.colref.arg2->body.variable;
			cr.relation_name = columns_list->body.colref.arg1;
			cout<<"table.column: "<<cr.relation_name<<"."<<cr.column_name<<endl;
		}
		columns->push_back(cr);	
		return;

	}

	void Driver::get_attribute_relations(vector<string> *total_relations, column_ref column, SchemaManager &schemaMgr, vector<string> *atr_relations){	
		for (int i = 0; i < total_relations->size(); i++){
			string relation_name = (*total_relations)[i];
			Schema* rel_schema = schemaMgr.getSchema(relation_name);
			int pos = rel_schema->getFieldPos(column.column_name);
			if(pos!=-1){   /* field found in relation */
				atr_relations->push_back(relation_name);
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
