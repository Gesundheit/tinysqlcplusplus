// $Id: driver.h 17 2007-08-19 18:51:39Z tb $ 	
/** \file driver.h Declaration of the example::Driver class. */

#ifndef EXAMPLE_DRIVER_H
#define EXAMPLE_DRIVER_H

#include <string>
#include <vector>
#include <deque>
#include<list>
#include <utility>
#include <string.h>

#include "StorageManager.h"
#include "expression.h"

#define DISK_OFFSET 300
// forward declaration
class CalcContext;

/** The example namespace is used to encapsulate the three parser classes
 * example::Parser, example::Scanner and example::Driver */

struct comparator { int _i; comparator(int i) : _i( i ) { }; 
bool operator()(const Tuple& t1, const Tuple& t2) const { return t1.getString(_i)>t2.getString(_i); } };

struct comparator2 { int _i; comparator2(int i) : _i( i ) { } 
bool operator()(const Tuple& t1, const Tuple& t2) const { return t1.getInt(_i)>t2.getInt(_i); } };


namespace example {

/** The Driver class brings together all components. It creates an instance of
 * the Parser and Scanner classes and connects them. Then the input stream is
 * fed into the scanner object and the parser gets it's token
 * sequence. Furthermore the driver object is available in the grammar rules as
 * a parameter. Therefore the driver class contains a reference to the
 * structure into which the parsed data is saved. */

class Driver
{
public:
    /// construct a new parser driver context
    Driver(class CalcContext& calc);

    /// enable debug output in the flex scanner
    bool trace_scanning;

    /// enable debug output in the bison parser
    bool trace_parsing;

    /// stream name (file or input stream) used for error messages.
    std::string streamname;

    /** DB Project: run create statement, put schema into virtual.
     * @param node
     * @param 
     * @return		
     */
	void operate_node(tree* node,char* op,vector<string> &fieldNames,vector<string> &fieldValues);
	void create_by_node(tree* node,vector<string> &fieldNames,vector<string> &fieldValues);
	void insert_by_node(tree* node,vector<string>& fieldNames,vector<atom_ref>& fieldValues,char* op);
	vector<Tuple>* delete_by_node(tree* node,vector<Tuple>origSet,Schema* schema);
	void print_select_single(vector<Tuple>* resultTuples,vector<Tuple> totalTuples,vector<string>fields,
                map <string,vector<string>> relationFieldMap,Schema* schema);
	vector<Tuple>* Driver::select_by_node_single_relation(tree* node,vector<Tuple> origSet,Schema* schema,
		map <string,vector<string>> relationFieldMap,int stmt);
	void Driver::process_tuple(std::vector<column_ref> attributes,Tuple t,Schema *schema, string &res_tuple, vector<string> relationFieldMap);
	void Driver::print_cross_join(deque<string> relations_to_print, SchemaManager &schemaMgr, MainMemory &mem,
									std::map<string,std::vector<column_ref>*> *attributes_to_project, string &res_tuple, 
									int memindex, bool last_in_mem, map<string,vector<string>> relationFieldMap);
	void Driver::cross_join(vector<string> *relations, SchemaManager &schemaMgr, MainMemory &mem, 
		std::map<string,std::vector<column_ref>*> *attributes_to_project,map<string,vector<string>> relationFieldMap);

	void Driver::print_result_tuple(string res);
	void Driver::print_result_tuple(Tuple t);
	void Driver::get_blocks_to_mem(int mem_start_ind, MainMemory &mem, int size, Relation* relationPtr, int rel_start_ind=0);
	int Driver::sort_relation(string relation, SchemaManager &schemaMgr, MainMemory &mem, int memindex, 
									 string column, map<string,int> &disk_proj, map<string,int> &disk_proj_size);
	/** DB Project: run create statement, put schema into virtual.
     * @param stmtNo	
     * @param schemaMgr
     * @return		true if successfully put
     */
	bool run_create(int stmtNo, SchemaManager &schemaMgr,map<string,vector<string>> &relationFieldMap);

    /** DB Project: run insert statement, add data into virtual disk.
     * @param stmtNo	
     * @param schemaMgr
	 * @param mem
     * @return		true if successfully put
     */
	bool run_insert(int stmtNo, SchemaManager &schemaMgr, MainMemory &mem);

    /** DB Project: run delete statement, modify virtual disk.
     * @param stmtNo	
     * @param schemaMgr
	 * @param mem
     * @return		true if successfully put
     */
	bool run_delete(int stmtNo, SchemaManager &schemaMgr, MainMemory &mem,map<string,vector<string>> relationFieldMap);

    /** DB Project: run drop statement, modify virtual disk.
     * @param stmtNo	
     * @param schemaMgr
	 * @param mem
     * @return		true if successfully put
     */
	bool run_drop(int stmtNo, SchemaManager &schemaMgr,MainMemory &mem);

	/** DB Project: run select statement, put schema into virtual.
     * @param stmtNo	
     * @param schemaMgr
     * @return		true if successfully executed
     */
	bool run_select(int stmtNo, SchemaManager &schemaMgr,MainMemory &mem,map <string,vector<string>> relationFieldMap);

    /** Invoke the scanner and parser for a stream.
     * @param in	input stream
     * @param sname	stream name for error messages
     * @return		true if successfully parsed
     */
    bool parse_stream(std::istream& in,
		      const std::string& sname = "stream input");

    /** Invoke the scanner and parser on an input string.
     * @param input	input string
     * @param sname	stream name for error messages
     * @return		true if successfully parsed
     */
    bool parse_string(const std::string& input,
		      const std::string& sname = "string stream");

    /** Invoke the scanner and parser on a file. Use parse_stream with a
     * std::ifstream if detection of file reading errors is required.
     * @param filename	input file name
     * @return		true if successfully parsed
     */
    bool parse_file(const std::string& filename);

    // To demonstrate pure handling of parse errors, instead of
    // simply dumping them on the standard error output, we will pass
    // them to the driver using the following two member functions.

    /** Error handling with associated line number. This can be modified to
     * output the error e.g. to a dialog box. */
    void error(const class location& l, const std::string& m);

    /** General error handling. This can be modified to output the error
     * e.g. to a dialog box. */
    void error(const std::string& m);
	
	/** Rerurns a vector with the relations from the FROM body of a SELECT
     * statement. */
	void Driver::get_relations(const int stmtNo, vector<string> *relations, SchemaManager &schemaMgr,
		std::map<string,std::vector<column_ref>*> *attributes_to_project,map<string,int> &disk_proj, 
		map<string,int> &disk_proj_size);	
	/** Rerurns a vector with the relations from the SELECT body of a SELECT
     * statement. */
	void Driver::get_columns(const vector<string> *total_relations, const int stmtNo, SchemaManager &schemaMgr, 
							vector<column_ref> *columns, std::map<string,std::vector<column_ref>*> *attributes_to_project);
	/** Returns a vector with the relations having a specific column */	
	void Driver::get_attribute_relations( vector<string> *total_relations,const column_ref column, SchemaManager &schemaMgr,
								vector<string> *atr_relations);

	/**Process the WHERE body of a select statement */	
	 vector<string> Driver::process_condition(const vector<string> *total_relations,const int stmtNo,SchemaManager &schemaMgr,tree* condition, 
					std::map<string,std::vector<column_ref>*> *attributes_to_project);
	 
	vector<pair<Tuple,Tuple>> Driver::execute_condition(const vector<string> *total_relations,const int stmtNo,
											SchemaManager &schemaMgr, tree* condition, 
											std::map<string,std::vector<column_ref>*> *attributes_to_project,
											map <string,vector<string>> relationFieldMap, MainMemory &mem,
											map<string,int> &disk_proj, map<string,int> &disk_proj_size,int nest_level,
											std::map<string,std::vector<column_ref>*> attributes_to_print, int disk_offset=0);
	vector<pair<Tuple,Tuple>> Driver::natural_join(string relation1,string relation2, SchemaManager &schemaMgr, MainMemory &mem,
									std::map<string,std::vector<column_ref>*> *attributes_to_project,
									int memindex, map<string,vector<string>> relationFieldMap, char* op, string column,
									map<string,int> &disk_proj, map<string,int> &disk_proj_size);

	void Driver::clearMemory(MainMemory &mem);
		
	void Driver::print_result(vector<Tuple> res, vector<column_ref> attributes_to_print,
								vector<string> relationFieldMap,string relation, SchemaManager &schemaMgr);

	void Driver::print_result(vector<pair<Tuple,Tuple>> res, std::map<string,std::vector<column_ref>*> attributes_to_project,
								map <string,vector<string>> relationFieldMap,string relation1, string relation2, SchemaManager &schemaMgr); 


	/**return the total size of the given relations */	
	unsigned long Driver::relations_get_size(const vector<string> &relations, SchemaManager &schemaMgr);
	void Driver::project_rel_block(std::vector<string> attributes,  vector<string> relationFields, 
											Block &b , Schema &schema);
	
	int Driver::project_relation(vector<column_ref> *attributes, vector<string> relationFields, 
											SchemaManager &schemaMgr, MainMemory &mem, string rel_name);

    /** Pointer to the current lexer instance, this is used to connect the
     * parser to the scanner. It is used in the yylex macro. */
    class Scanner* lexer;

    /** Reference to the calculator context filled during parsing of the
     * expressions. */
    class CalcContext& calc;


	/**
	* DB project used from TA's library
	* 
	*/

};

} // namespace example

#endif // EXAMPLE_DRIVER_H
