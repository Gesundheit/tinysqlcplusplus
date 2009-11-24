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
	class SQLTree* tree_s;
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
%type <tree_s> table statement drop_table_statement insert_statement create_table_statement select_statement delete_statement
%type <tree_s> opt_where opt_column_commalist opt_orderby opt_distinct  
%type <tree_s> base_table_element_commalist  values_or_query_spec select_expr_di_list table_references select_expr_list
%type <tree_s> expr atom column_ref opt_column_ref base_table_element column_commalist insert_atom_commalist insert_atom

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
%token			EOL		"end of line"
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

start: /* empty */
		| start EOL													{}
		| start statement EOL										{}
		| start statement END										{}


statement:
        create_table_statement											{$$ = $1;}							
        | drop_table_statement											{$$ = $1;}
		| insert_statement												{$$ = $1;}
		| select_statement												{$$ = $1;}
		| delete_statement												{$$ = $1;}
		;

drop_table_statement:
		DROP TABLE table												{printf("DROP TABLE\n");
																		driver.calc.stmt_vector.push_back( driver.calc.aSQLTree->make_stmt((tree *)$3,NULL,NULL,NULL,drop_st) );
																		}
		;

create_table_statement:
        CREATE TABLE table '(' base_table_element_commalist ')'			{printf("CREATE TABLE\n");																	   
																		driver.calc.stmt_vector.push_back( driver.calc.aSQLTree->make_stmt((tree *)$3,(tree *)$5,NULL,NULL,create_st) );
																		}
        ;

insert_statement: 
 		INSERT INTO table opt_column_commalist values_or_query_spec		{printf("INSERT INTO\n");
																		driver.calc.stmt_vector.push_back( driver.calc.aSQLTree->make_stmt((tree *)$3,(tree *)$4,(tree *)$5,NULL,insert_st) );
																		} 
 		; 

select_statement:
		SELECT opt_distinct select_expr_di_list 
		FROM table_references 
		opt_where  
		opt_orderby 												{printf("SELECT FROM\n"); 																		
																	/* driver.calc.aSQLTree->make_stmt((tree *)$3,(tree *)$5,(tree *)$6,(tree *)$7,select_st); */

																	if($2==NULL){
																	driver.calc.stmt_vector.push_back( driver.calc.aSQLTree->make_stmt((tree *)$3,(tree *)$5,(tree *)$6,(tree *)$7,select_st,false) );
																	}else{
																	driver.calc.stmt_vector.push_back( driver.calc.aSQLTree->make_stmt((tree *)$3,(tree *)$5,(tree *)$6,(tree *)$7,select_st,true) );
																	}
		}  
		;

delete_statement:
		DELETE FROM NAME opt_where									{printf("DELETE FROM\n");
																	driver.calc.stmt_vector.push_back( driver.calc.aSQLTree->make_stmt((tree *)$3,(tree *)$4,NULL,NULL,delete_st) );
} 
		;


opt_where: /* empty */												{$$=NULL;}
		| WHERE expr												{printf("WHERE\n");$$=$2;}			
		;


opt_orderby: /* empty */											{$$=NULL;}
		| ORDER BY column_ref										{printf("ORDER BY\n");$$=$3;}			
		;

select_expr_di_list: select_expr_list								{printf("select_expr_list\n");$$=$1;}
		| '*'														{printf("*\n");}
		;

select_expr_list: expr												{ $$=(SQLTree *)driver.calc.aSQLTree->make_list((tree *)$1,select); }
		| select_expr_list ',' expr									{$$=(SQLTree *)driver.calc.aSQLTree->append((tree *)$1,(tree *)$3,select);}										
		;

expr: 
		atom														{printf("atom\n");}	 
		| expr '+' expr												{printf("ADD\n"); $$=(SQLTree *)driver.calc.aSQLTree->make_expr((tree *)$1,(tree *)$3,"+",binary);} 
		| expr '-' expr												{printf("SUB\n"); $$=(SQLTree *)driver.calc.aSQLTree->make_expr((tree *)$1,(tree *)$3,"-",binary);} 
		| expr '*' expr												{printf("MUL\n"); $$=(SQLTree *)driver.calc.aSQLTree->make_expr((tree *)$1,(tree *)$3,"*",binary);} 
		| expr '/' expr												{printf("DIV\n"); $$=(SQLTree *)driver.calc.aSQLTree->make_expr((tree *)$1,(tree *)$3,"/",binary);} 
		| expr AND expr												{printf("AND\n"); $$=(SQLTree *)driver.calc.aSQLTree->make_expr((tree *)$1,(tree *)$3,"AND",binary);} 
		| expr OR expr												{printf("OR\n"); $$=(SQLTree *)driver.calc.aSQLTree->make_expr((tree *)$1,(tree *)$3,"OR",binary);} 
		| NOT expr													{printf("NOT\n"); $$=(SQLTree *)driver.calc.aSQLTree->make_expr((tree *)$2,NULL,NULL,not);} 
		| expr COMPARISON expr										{printf("COMP\n"); $$=(SQLTree *)driver.calc.aSQLTree->make_expr((tree *)$1,(tree *)$3,*$2,binary);} 
		| '(' expr ')'												{$$=(SQLTree *)driver.calc.aSQLTree->make_expr((tree *)$2,NULL,NULL,paren);} 
		;

table_references: table												{ $$=(SQLTree *)driver.calc.aSQLTree->make_list((tree *)$1,table_ref); }
		| table_references ',' table								{ $$=(SQLTree *)driver.calc.aSQLTree->append((tree *)$1,(tree *)$3,table_ref); }
		;

values_or_query_spec:
		VALUES '(' insert_atom_commalist ')'						{printf("VALUES\n");$$=$3;}
		| select_statement											{printf("SELECT\n");$$=$1;}
		;

insert_atom_commalist:
		insert_atom													{$$=(SQLTree *)driver.calc.aSQLTree->make_list((tree *)$1,column);}
		| insert_atom_commalist ',' insert_atom						{$$=(SQLTree *)driver.calc.aSQLTree->append((tree *)$1,(tree *)$3,column);}
		;

insert_atom:
		atom														{printf("atom in insert_atom\n");$$=$1;}
		| NULLX														{$$=NULL;}
		;

base_table_element_commalist:
		base_table_element_commalist ',' base_table_element			{$$=(SQLTree *)driver.calc.aSQLTree->append((tree *)$1,(tree *)$3,column);}
		| base_table_element										{$$=(SQLTree *)driver.calc.aSQLTree->make_list((tree *)$1,column);}
		;
		
opt_column_commalist:
		/* empty */													{$$=NULL;}
		| '(' column_commalist ')'									{$$=$2;}
		;


column_commalist:
		column_ref												    {$$=(SQLTree *)driver.calc.aSQLTree->make_list((tree *)$1,column);}
		| column_commalist ',' column_ref							{printf(",\n");$$=(SQLTree *)driver.calc.aSQLTree->append((tree *)$1,(tree *)$3,column);}
		;

opt_distinct:
		/* empty */													{$$=NULL;}
		| DISTINCT													{$$=(SQLTree *)driver.calc.aSQLTree->make_stmt(NULL,NULL,NULL,NULL,select_st);}
		;

base_table_element:
		column_ref INTEGER
		| column_ref STR20
		;

atom: 
	STRING															{$$=(SQLTree *)driver.calc.aSQLTree->make_literal(*$1);}							  
	| INTNUM														{$$=(SQLTree *)driver.calc.aSQLTree->make_number($1);}							   
	| column_ref													{$$=$1;}							  
	;

column_ref: 
		NAME opt_column_ref											{printf("NAME\n");$$=(SQLTree *)driver.calc.aSQLTree->make_colref(*$1,(tree *)$2);}
		;



opt_column_ref: /* empty*/											{$$=NULL;}
		|'.'NAME													{printf(".NAME\n");$$=(SQLTree *)driver.calc.aSQLTree->make_variable(*$2);}
		; 

table:
		NAME														{std::cout<<*$1;std::cout<<"\n";$$ = (SQLTree *)driver.calc.aSQLTree->make_variable(*$1);}							  									
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
