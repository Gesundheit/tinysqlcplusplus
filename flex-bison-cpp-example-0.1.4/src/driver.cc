// $Id: driver.cc 39 2008-08-03 10:07:15Z tb $
/** \file driver.cc Implementation of the example::Driver class. */

#include <fstream>
#include <sstream>

#include<cstdlib>
#include<string>


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
			switch (calc.stmt_vector[0]->body.stmt.type){
				case create_st:{
					run_create(stmtNo,schemaMgr);
					break;
				}
				case select_st:{
					run_select(stmtNo,schemaMgr);
					break;
				}
			}
		}


		//Observe the tree here by setting break

		return r;
	}

	void Driver::operate_node(tree* node,char* op,vector<string> &fieldNames, vector<string> &fieldTypes){

		if(node!=NULL){
			if(node->body.list.arg1==NULL && node->body.list.arg2==NULL){
				return;
			}
			else if(node->body.list.arg2==NULL){
				printf("insert here\n");
//printf("%s\n",node->body.list.arg1->body.colref.arg1);
				fieldNames.push_back( string((const char*)node->body.list.arg1->body.colref.arg1) );
				if( node->body.list.arg1->body.colref.type==str20 ){
					printf("STR20\n");
					fieldTypes.push_back(string("STR20"));
				}else{
					printf("INT\n");
					fieldTypes.push_back(string("INT"));
				}			   
			}else{

				operate_node(node->body.list.arg1,op,fieldNames,fieldTypes);			
				fieldNames.push_back( string((const char*)node->body.list.arg2->body.colref.arg1) );
				if( node->body.list.arg1->body.colref.type==str20 ){
					printf("STR20\n");
					fieldTypes.push_back(string("STR20"));
				}else{
					printf("INT\n");
					fieldTypes.push_back(string("INT"));
				}			   
			}
		}


		//if(node != NULL && node->nodetype==colref_node){
		//	fieldNames.push_back(string(node->body.colref.arg1));
		//	if(node->body.colref.type==str20){
		//		fieldTypes.push_back(string("STR20"));
		//	}else if(node->body.colref.type==intnum){
		//		fieldTypes.push_back(string("INT"));
		//	}
		//}else{
		//	printf("list in operate_node\n");
		//	operate_node(node->body.list.arg1,op,fieldNames,fieldTypes);
		//}
	}

	bool Driver::run_create(int stmtNo, SchemaManager schemaMgr){
		printf("run_create called\n");

		// Set time of write/read a block to 10ms
		// Without setting, the default time is 10ms.
		setDelay(10);
		// Create a schema
		vector<string> fieldNames;
		vector<string> fieldTypes;
		string relationName(calc.stmt_vector[stmtNo]->body.stmt.arg1->body.variable);

		//if(calc.stmt_vector[stmtNo]->body.stmt.arg2->nodetype==list_node){
		//	printf("list in run_create\n");
		//}else{
		//	printf("colref in run_create\n");
		//}

		operate_node(calc.stmt_vector[stmtNo]->body.stmt.arg2,"c",fieldNames,fieldTypes);

		//printf("fn size %d\n",fieldNames.size());
		//printf("ft size %d\n",fieldTypes.size());

		if(fieldNames.size() == 0){
			printf("Cannot create a relation with zero fields");
		}else{
			Schema schema(fieldNames,fieldTypes);
			// Print the information about the schema
			schema.printSchema();
			cout << "The schema has " << schema.getNumOfFields() << " fields" << endl;
			cout << "The schema has " << schema.getNumOfInt() << " integers" << endl;
			cout << "The schema has " << schema.getNumOfString() << " strings" << endl;
			cout << "The schema allows " << schema.getTuplesPerBlock() << " tuples per block" << endl;
		}


		return true;
	}

	bool Driver::run_select(int stmtNo, SchemaManager schemaMgr){
		vector<string> relations;
		tree *relation_list = (tree*)calc.stmt_vector[stmtNo]->body.stmt.arg2->body.list.arg1;
	    tree *relation      = (tree*)calc.stmt_vector[stmtNo]->body.stmt.arg2->body.list.arg2;
		while(relation!=NULL){
			relations.push_back(relation->body.variable);	
			cout<<"relation: "<<relation->body.variable<<endl;
			if(relation_list->nodetype==variable_node){
				break;
			}
			relation = relation_list->body.list.arg2;
			relation_list = relation_list->body.list.arg1;
			
		}
		relations.push_back(relation_list->body.variable);	
		cout<<"relation: "<<relation_list->body.variable<<endl;
			
		if(calc.stmt_vector[stmtNo]->body.stmt.arg3!=NULL){
			//parse expressions;
		}
		else{
			cout<<"No condition"<<endl;
		}

		vector<column_ref> columns;
		tree *columns_list = (tree*)calc.stmt_vector[stmtNo]->body.stmt.arg1->body.list.arg1;
	    tree *column      = (tree*)calc.stmt_vector[stmtNo]->body.stmt.arg1->body.list.arg2;
		column_ref cr;
		while(column!=NULL){			
			if(column->body.colref.arg2==NULL){
				cr.column_name = column->body.colref.arg1;
				cr.table_name = NULL;
				cout<<"column: "<<cr.column_name<<endl;
			}
			else{
				cr.column_name = column->body.colref.arg2->body.variable;
				cr.table_name = column->body.colref.arg1;
				cout<<"table.column: "<<cr.table_name<<"."<<cr.column_name<<endl;
			}
			columns.push_back(cr);	

			if(columns_list->nodetype==colref_node){
				break;
			}
			column = columns_list->body.list.arg2;
			columns_list = columns_list->body.list.arg1;
			
		}
		if(columns_list->body.colref.arg2==NULL){
			cr.column_name = columns_list->body.colref.arg1;
			cr.table_name = NULL;
			cout<<"column: "<<cr.column_name<<endl;
		}
		else{
			cr.column_name = columns_list->body.colref.arg2->body.variable;
			cr.table_name = columns_list->body.colref.arg1;
			cout<<"table.column: "<<cr.table_name<<"."<<cr.column_name<<endl;
		}
		columns.push_back(cr);	
		return true;
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
