/* $Id: parser.yy 48 2009-09-05 08:07:10Z tb $ -*- mode: c++ -*- */
/** \file parser.yy Contains the example Bison parser source */

%{ /*** C/C++ Declarations ***/

#include <stdio.h>
#include <string>
#include <vector>

#include "expression.h"

%}

/*** yacc/bison Declarations ***/

/* Require bison 2.3 or later */
%require "2.3"

/* add debug output code to generated parser. disable this for release
 * versions. */
%debug

/* start symbol is named "start" */
/* %start start */

/* write out a header file containing the token defines */
%defines

/* use newer C++ skeleton file */
%skeleton "lalr1.cc"

/* namespace to enclose parser in */
%name-prefix="example"

/* set the parser's class identifier */
%define "parser_class_name" "Parser"

/* keep track of the current position within the input */
%locations
%initial-action
{
    // initialize the initial location object
    @$.begin.filename = @$.end.filename = &driver.streamname;
};

/* The driver is passed by reference to the parser and to the scanner. This
 * provides a simple but effective pure interface, not relying on global
 * variables. */
%parse-param { class Driver& driver }

/* verbose error messages */
%error-verbose

 /*** BEGIN EXAMPLE - Change the example grammar's tokens below ***/

%union {
    int  			integerVal;
    double 			doubleVal;
    std::string*		stringVal;
    class CalcNode*		calcnode;
}


%token			BY	     "BY"
%token			CREATE   "CREATE"
%token			DELETE   "DELETE"
%token			DISTINCT "DISTINCT"
%token			DROP	 "DROP"
%token			FROM	 "FROM"
%token			INSERT	 "INSERT"
%token			INTEGER	 "INTEGER"
%token			INTO	 "INTO"
%token			NULLX	 "NULLX"
%token			ORDER	 "ORDER"
%token			STR20	 "STR20"
%token			SELECT	 "SELECT"
%token			TABLE	 "TABLE"
%token			VALUES	 "VALUES"
%token			WHERE	 "WHERE"

%left			OR	 "OR"
%left			AND	 "AND"
%left			NOT	 "NOT"
%left <stringVal> COMPARISON	 "COMPARISON"
%left '+' '-'
%left '*' '/'

%token <integerVal> 	INTNUM		"INTNUM"
%token <stringVal> 	NAME		"NAME"
%token <stringVal> 	STRING		"STRING"


/*** BEGINNING OF USELESS BLOCK ***/
%token			END	     0	"end of file"
/*** END OF USELESS BLOCK ***/




%destructor { delete $$; } NAME
%destructor { delete $$; } STRING

 /*** END EXAMPLE - Change the example grammar's tokens above ***/

%{

#include "driver.h"
#include "scanner.h"

/* this "connects" the bison parser in the driver to the flex scanner class
 * object. it defines the yylex() function call to pull the next token from the
 * current lexer object of the driver context. */
#undef yylex
#define yylex driver.lexer->lex

%}

%% /*** Grammar Rules ***/

/*** BEGIN DATABASE GRAMMAR ***/

statement:
        create_table_statement											
        | drop_table_statement											
		| insert_statement											
		| select_statement
		| delete_statement
		;

drop_table_statement:
		DROP TABLE table												{printf("DROP TABLE\n");}
		;

create_table_statement:
        CREATE TABLE table '(' base_table_element_commalist ')'			{printf("CREATE TABLE\n");}
        ;

insert_statement: 
 		INSERT INTO table opt_column_commalist values_or_query_spec		{printf("INSERT INTO\n");} 
 		; 

select_statement:
		SELECT opt_distinct select_expr_di_list 
		FROM table_references 
		opt_where  
		opt_orderby 												{printf("SELECT FROM\n");}  
		;

delete_statement:
		DELETE FROM NAME opt_where									{printf("DELETE FROM\n");} 
		;


opt_where: /* empty */												
		| WHERE expr												{printf("WHERE\n");}			
		;


opt_orderby: /* empty */											
		| ORDER BY column_ref										{printf("ORDER BY\n");}			
		;

select_expr_di_list: select_expr_list								{printf("select_expr_list\n");}
		| '*'														{printf("*\n");}
		;

select_expr_list: select_expr 
		| select_expr_list ',' select_expr							{printf(",\n");}										
		;

select_expr: expr													{printf("expr\n");}
		;

expr: 
		atom														{printf("atom\n");}	 
		| expr '+' expr												{printf("ADD\n");}
		| expr '-' expr												{printf("SUB\n");}										
		| expr '*' expr												{printf("MUL\n");}										 
		| expr '/' expr												{printf("DIV\n");}										 
		| expr AND expr												{printf("AND\n");}										 
		| expr OR expr												{printf("OR\n");}										 
		| NOT expr													{printf("NOT\n");}										 
		| expr COMPARISON expr										{std::cout<<*$2;printf("COMP\n");}	
		| '(' expr ')' 
		;

table_references: table_reference								  
		| table_references ',' table_reference						{printf(",\n");}
		;

table_reference: NAME												{std::cout<<*$1;std::cout<<"\n";}							   
		;

values_or_query_spec:
		VALUES '(' insert_atom_commalist ')'						{printf("VALUES\n");}
		| select_statement											{printf("SELECT\n");}
		;

insert_atom_commalist:
		insert_atom
		| insert_atom_commalist ',' insert_atom						{printf(",");}
		;

insert_atom:
		atom														{printf("atom\n");}
		| NULLX
		;

base_table_element_commalist:
		base_table_element_commalist ',' base_table_element			{printf(",\n");}
		| base_table_element										
		;
		
opt_column_commalist:
		/* empty */
		| '(' column_commalist ')'
		;


column_commalist:
		column_ref												   
		| column_commalist ',' column_ref							{printf(",\n");}
		;

opt_distinct:
		/* empty */
		| DISTINCT													{printf("DISTINCT");}
		;

base_table_element:
		column_def
		;

atom:
	literal										     {std::cout<<"literal";std::cout<<"\n";}							  
	| INTNUM										 {std::cout<<$1;std::cout<<"\n";}							   
	| column_ref									 {std::cout<<"column_ref";std::cout<<"\n";}							  
	;

column_def:
		column_ref data_type
		;

column_ref: 
 		NAME										{std::cout<<*$1;std::cout<<"\n";}							  
		| NAME '.' NAME								{std::cout<<*$1<<"."<<*$3;std::cout<<"\n";}							   
		;


/* column_ref: */
/*		NAME opt_column_ref				{printf("NAME\n");} */
/*		; */
/* opt_column_ref: */
/*		|'.'NAME						{printf(".NAME\n");} */
/*		; */

table:
		NAME										{std::cout<<*$1;std::cout<<"\n";}							  									
		;

literal:
		STRING										{std::cout<<*$1;std::cout<<"\n";}							  
		;

data_type:
		INTEGER										{printf("INTEGER");}
		| STR20										{printf("STR20");}
		;


/*** END DATABASE GRAMMAR ***/

/*** BEGIN EXAMPLE - Change the example grammar rules below ***/


 /*** END EXAMPLE - Change the example grammar rules above ***/

%% /*** Additional Code ***/

void example::Parser::error(const Parser::location_type& l,
			    const std::string& m)
{
    driver.error(l, m);
}
