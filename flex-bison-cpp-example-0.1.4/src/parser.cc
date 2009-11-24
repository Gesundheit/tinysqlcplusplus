/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton implementation for Bison LALR(1) parsers in C++

   Copyright (C) 2002, 2003, 2004, 2005, 2006 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

// Take the name prefix into account.
#define yylex   examplelex

#include "parser.hh"

/* User implementation prologue.  */
#line 111 "parser.yy"


#include "driver.h"
#include "scanner.h"

/* this "connects" the bison parser in the driver to the flex scanner class
 * object. it defines the yylex() function call to pull the next token from the
 * current lexer object of the driver context. */
#undef yylex
#define yylex driver.lexer->lex



/* Line 317 of lalr1.cc.  */
#line 57 "parser.cc"

#ifndef YY_
# if YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* FIXME: INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#define YYUSE(e) ((void) (e))

/* A pseudo ostream that takes yydebug_ into account.  */
# define YYCDEBUG							\
  for (bool yydebugcond_ = yydebug_; yydebugcond_; yydebugcond_ = false)	\
    (*yycdebug_)

/* Enable debugging if requested.  */
#if YYDEBUG

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)	\
do {							\
  if (yydebug_)						\
    {							\
      *yycdebug_ << Title << ' ';			\
      yy_symbol_print_ ((Type), (Value), (Location));	\
      *yycdebug_ << std::endl;				\
    }							\
} while (false)

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug_)				\
    yy_reduce_print_ (Rule);		\
} while (false)

# define YY_STACK_PRINT()		\
do {					\
  if (yydebug_)				\
    yystack_print_ ();			\
} while (false)

#else /* !YYDEBUG */

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_REDUCE_PRINT(Rule)
# define YY_STACK_PRINT()

#endif /* !YYDEBUG */

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab

namespace example
{
#if YYERROR_VERBOSE

  /* Return YYSTR after stripping away unnecessary quotes and
     backslashes, so that it's suitable for yyerror.  The heuristic is
     that double-quoting is unnecessary unless the string contains an
     apostrophe, a comma, or backslash (other than backslash-backslash).
     YYSTR is taken from yytname.  */
  std::string
  Parser::yytnamerr_ (const char *yystr)
  {
    if (*yystr == '"')
      {
        std::string yyr = "";
        char const *yyp = yystr;

        for (;;)
          switch (*++yyp)
            {
            case '\'':
            case ',':
              goto do_not_strip_quotes;

            case '\\':
              if (*++yyp != '\\')
                goto do_not_strip_quotes;
              /* Fall through.  */
            default:
              yyr += *yyp;
              break;

            case '"':
              return yyr;
            }
      do_not_strip_quotes: ;
      }

    return yystr;
  }

#endif

  /// Build a parser object.
  Parser::Parser (class Driver& driver_yyarg)
    : yydebug_ (false),
      yycdebug_ (&std::cerr),
      driver (driver_yyarg)
  {
  }

  Parser::~Parser ()
  {
  }

#if YYDEBUG
  /*--------------------------------.
  | Print this symbol on YYOUTPUT.  |
  `--------------------------------*/

  inline void
  Parser::yy_symbol_value_print_ (int yytype,
			   const semantic_type* yyvaluep, const location_type* yylocationp)
  {
    YYUSE (yylocationp);
    YYUSE (yyvaluep);
    switch (yytype)
      {
         default:
	  break;
      }
  }


  void
  Parser::yy_symbol_print_ (int yytype,
			   const semantic_type* yyvaluep, const location_type* yylocationp)
  {
    *yycdebug_ << (yytype < yyntokens_ ? "token" : "nterm")
	       << ' ' << yytname_[yytype] << " ("
	       << *yylocationp << ": ";
    yy_symbol_value_print_ (yytype, yyvaluep, yylocationp);
    *yycdebug_ << ')';
  }
#endif /* ! YYDEBUG */

  void
  Parser::yydestruct_ (const char* yymsg,
			   int yytype, semantic_type* yyvaluep, location_type* yylocationp)
  {
    YYUSE (yylocationp);
    YYUSE (yymsg);
    YYUSE (yyvaluep);

    YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

    switch (yytype)
      {
        case 32: /* "\"NAME\"" */
#line 106 "parser.yy"
	{ delete (yyvaluep->stringVal); };
#line 217 "parser.cc"
	break;
      case 33: /* "\"STRING\"" */
#line 107 "parser.yy"
	{ delete (yyvaluep->stringVal); };
#line 222 "parser.cc"
	break;

	default:
	  break;
      }
  }

  void
  Parser::yypop_ (unsigned int n)
  {
    yystate_stack_.pop (n);
    yysemantic_stack_.pop (n);
    yylocation_stack_.pop (n);
  }

  std::ostream&
  Parser::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  Parser::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  Parser::debug_level_type
  Parser::debug_level () const
  {
    return yydebug_;
  }

  void
  Parser::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }


  int
  Parser::parse ()
  {
    /// Look-ahead and look-ahead in internal form.
    int yychar = yyempty_;
    int yytoken = 0;

    /* State.  */
    int yyn;
    int yylen = 0;
    int yystate = 0;

    /* Error handling.  */
    int yynerrs_ = 0;
    int yyerrstatus_ = 0;

    /// Semantic value of the look-ahead.
    semantic_type yylval;
    /// Location of the look-ahead.
    location_type yylloc;
    /// The locations where the error started and ended.
    location yyerror_range[2];

    /// $$.
    semantic_type yyval;
    /// @$.
    location_type yyloc;

    int yyresult;

    YYCDEBUG << "Starting parse" << std::endl;


    /* User initialization code.  */
    #line 41 "parser.yy"
{
    // initialize the initial location object
    yylloc.begin.filename = yylloc.end.filename = &driver.streamname;
}
  /* Line 547 of yacc.c.  */
#line 304 "parser.cc"
    /* Initialize the stacks.  The initial state will be pushed in
       yynewstate, since the latter expects the semantical and the
       location values to have been already stored, initialize these
       stacks with a primary value.  */
    yystate_stack_ = state_stack_type (0);
    yysemantic_stack_ = semantic_stack_type (0);
    yylocation_stack_ = location_stack_type (0);
    yysemantic_stack_.push (yylval);
    yylocation_stack_.push (yylloc);

    /* New state.  */
  yynewstate:
    yystate_stack_.push (yystate);
    YYCDEBUG << "Entering state " << yystate << std::endl;
    goto yybackup;

    /* Backup.  */
  yybackup:

    /* Try to take a decision without look-ahead.  */
    yyn = yypact_[yystate];
    if (yyn == yypact_ninf_)
      goto yydefault;

    /* Read a look-ahead token.  */
    if (yychar == yyempty_)
      {
	YYCDEBUG << "Reading a token: ";
	yychar = yylex (&yylval, &yylloc);
      }


    /* Convert token to internal form.  */
    if (yychar <= yyeof_)
      {
	yychar = yytoken = yyeof_;
	YYCDEBUG << "Now at end of input." << std::endl;
      }
    else
      {
	yytoken = yytranslate_ (yychar);
	YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
      }

    /* If the proper action on seeing token YYTOKEN is to reduce or to
       detect an error, take that action.  */
    yyn += yytoken;
    if (yyn < 0 || yylast_ < yyn || yycheck_[yyn] != yytoken)
      goto yydefault;

    /* Reduce or error.  */
    yyn = yytable_[yyn];
    if (yyn <= 0)
      {
	if (yyn == 0 || yyn == yytable_ninf_)
	goto yyerrlab;
	yyn = -yyn;
	goto yyreduce;
      }

    /* Accept?  */
    if (yyn == yyfinal_)
      goto yyacceptlab;

    /* Shift the look-ahead token.  */
    YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

    /* Discard the token being shifted unless it is eof.  */
    if (yychar != yyeof_)
      yychar = yyempty_;

    yysemantic_stack_.push (yylval);
    yylocation_stack_.push (yylloc);

    /* Count tokens shifted since error; after three, turn off error
       status.  */
    if (yyerrstatus_)
      --yyerrstatus_;

    yystate = yyn;
    goto yynewstate;

  /*-----------------------------------------------------------.
  | yydefault -- do the default action for the current state.  |
  `-----------------------------------------------------------*/
  yydefault:
    yyn = yydefact_[yystate];
    if (yyn == 0)
      goto yyerrlab;
    goto yyreduce;

  /*-----------------------------.
  | yyreduce -- Do a reduction.  |
  `-----------------------------*/
  yyreduce:
    yylen = yyr2_[yyn];
    /* If YYLEN is nonzero, implement the default value of the action:
       `$$ = $1'.  Otherwise, use the top of the stack.

       Otherwise, the following line sets YYVAL to garbage.
       This behavior is undocumented and Bison
       users should not rely upon it.  */
    if (yylen)
      yyval = yysemantic_stack_[yylen - 1];
    else
      yyval = yysemantic_stack_[0];

    {
      slice<location_type, location_stack_type> slice (yylocation_stack_, yylen);
      YYLLOC_DEFAULT (yyloc, slice, yylen);
    }
    YY_REDUCE_PRINT (yyn);
    switch (yyn)
      {
	  case 3:
#line 129 "parser.yy"
    {;}
    break;

  case 4:
#line 130 "parser.yy"
    {;}
    break;

  case 5:
#line 131 "parser.yy"
    {;}
    break;

  case 6:
#line 135 "parser.yy"
    {(yyval.tree_s) = (yysemantic_stack_[(1) - (1)].tree_s);;}
    break;

  case 7:
#line 136 "parser.yy"
    {(yyval.tree_s) = (yysemantic_stack_[(1) - (1)].tree_s);;}
    break;

  case 8:
#line 137 "parser.yy"
    {(yyval.tree_s) = (yysemantic_stack_[(1) - (1)].tree_s);;}
    break;

  case 9:
#line 138 "parser.yy"
    {(yyval.tree_s) = (yysemantic_stack_[(1) - (1)].tree_s);;}
    break;

  case 10:
#line 139 "parser.yy"
    {(yyval.tree_s) = (yysemantic_stack_[(1) - (1)].tree_s);;}
    break;

  case 11:
#line 143 "parser.yy"
    {printf("DROP TABLE\n");
																		driver.calc.stmt_vector.push_back( driver.calc.aSQLTree->make_stmt((tree *)(yysemantic_stack_[(3) - (3)].tree_s),NULL,NULL,NULL,drop_st) );
																		;}
    break;

  case 12:
#line 149 "parser.yy"
    {printf("CREATE TABLE\n");																	   
																		driver.calc.stmt_vector.push_back( driver.calc.aSQLTree->make_stmt((tree *)(yysemantic_stack_[(6) - (3)].tree_s),(tree *)(yysemantic_stack_[(6) - (5)].tree_s),NULL,NULL,create_st) );
																		;}
    break;

  case 13:
#line 155 "parser.yy"
    {printf("INSERT INTO\n");
																		driver.calc.stmt_vector.push_back( driver.calc.aSQLTree->make_stmt((tree *)(yysemantic_stack_[(5) - (3)].tree_s),(tree *)(yysemantic_stack_[(5) - (4)].tree_s),(tree *)(yysemantic_stack_[(5) - (5)].tree_s),NULL,insert_st) );
																		;}
    break;

  case 14:
#line 164 "parser.yy"
    {printf("SELECT FROM\n"); 																		
																	/* driver.calc.aSQLTree->make_stmt((tree *)$3,(tree *)$5,(tree *)$6,(tree *)$7,select_st); */

																	if((yysemantic_stack_[(7) - (2)].tree_s)==NULL){
																	driver.calc.stmt_vector.push_back( driver.calc.aSQLTree->make_stmt((tree *)(yysemantic_stack_[(7) - (3)].tree_s),(tree *)(yysemantic_stack_[(7) - (5)].tree_s),(tree *)(yysemantic_stack_[(7) - (6)].tree_s),(tree *)(yysemantic_stack_[(7) - (7)].tree_s),select_st,false) );
																	}else{
																	driver.calc.stmt_vector.push_back( driver.calc.aSQLTree->make_stmt((tree *)(yysemantic_stack_[(7) - (3)].tree_s),(tree *)(yysemantic_stack_[(7) - (5)].tree_s),(tree *)(yysemantic_stack_[(7) - (6)].tree_s),(tree *)(yysemantic_stack_[(7) - (7)].tree_s),select_st,true) );
																	}
		;}
    break;

  case 15:
#line 176 "parser.yy"
    {printf("DELETE FROM\n");
																	driver.calc.stmt_vector.push_back( driver.calc.aSQLTree->make_stmt((tree *)(yysemantic_stack_[(4) - (3)].stringVal),(tree *)(yysemantic_stack_[(4) - (4)].tree_s),NULL,NULL,delete_st) );
;}
    break;

  case 16:
#line 182 "parser.yy"
    {(yyval.tree_s)=NULL;;}
    break;

  case 17:
#line 183 "parser.yy"
    {printf("WHERE\n");(yyval.tree_s)=(yysemantic_stack_[(2) - (2)].tree_s);;}
    break;

  case 18:
#line 187 "parser.yy"
    {(yyval.tree_s)=NULL;;}
    break;

  case 19:
#line 188 "parser.yy"
    {printf("ORDER BY\n");(yyval.tree_s)=(yysemantic_stack_[(3) - (3)].tree_s);;}
    break;

  case 20:
#line 191 "parser.yy"
    {printf("select_expr_list\n");(yyval.tree_s)=(yysemantic_stack_[(1) - (1)].tree_s);;}
    break;

  case 21:
#line 192 "parser.yy"
    {printf("*\n");(yyval.tree_s) = (SQLTree *)driver.calc.aSQLTree->make_variable("*");;}
    break;

  case 22:
#line 195 "parser.yy"
    { (yyval.tree_s)=(SQLTree *)driver.calc.aSQLTree->make_list((tree *)(yysemantic_stack_[(1) - (1)].tree_s),select); ;}
    break;

  case 23:
#line 196 "parser.yy"
    {(yyval.tree_s)=(SQLTree *)driver.calc.aSQLTree->append((tree *)(yysemantic_stack_[(3) - (1)].tree_s),(tree *)(yysemantic_stack_[(3) - (3)].tree_s),select);;}
    break;

  case 24:
#line 200 "parser.yy"
    {printf("atom\n");;}
    break;

  case 25:
#line 201 "parser.yy"
    {printf("ADD\n"); (yyval.tree_s)=(SQLTree *)driver.calc.aSQLTree->make_expr((tree *)(yysemantic_stack_[(3) - (1)].tree_s),(tree *)(yysemantic_stack_[(3) - (3)].tree_s),"+",binary);;}
    break;

  case 26:
#line 202 "parser.yy"
    {printf("SUB\n"); (yyval.tree_s)=(SQLTree *)driver.calc.aSQLTree->make_expr((tree *)(yysemantic_stack_[(3) - (1)].tree_s),(tree *)(yysemantic_stack_[(3) - (3)].tree_s),"-",binary);;}
    break;

  case 27:
#line 203 "parser.yy"
    {printf("MUL\n"); (yyval.tree_s)=(SQLTree *)driver.calc.aSQLTree->make_expr((tree *)(yysemantic_stack_[(3) - (1)].tree_s),(tree *)(yysemantic_stack_[(3) - (3)].tree_s),"*",binary);;}
    break;

  case 28:
#line 204 "parser.yy"
    {printf("DIV\n"); (yyval.tree_s)=(SQLTree *)driver.calc.aSQLTree->make_expr((tree *)(yysemantic_stack_[(3) - (1)].tree_s),(tree *)(yysemantic_stack_[(3) - (3)].tree_s),"/",binary);;}
    break;

  case 29:
#line 205 "parser.yy"
    {printf("AND\n"); (yyval.tree_s)=(SQLTree *)driver.calc.aSQLTree->make_expr((tree *)(yysemantic_stack_[(3) - (1)].tree_s),(tree *)(yysemantic_stack_[(3) - (3)].tree_s),"AND",binary);;}
    break;

  case 30:
#line 206 "parser.yy"
    {printf("OR\n"); (yyval.tree_s)=(SQLTree *)driver.calc.aSQLTree->make_expr((tree *)(yysemantic_stack_[(3) - (1)].tree_s),(tree *)(yysemantic_stack_[(3) - (3)].tree_s),"OR",binary);;}
    break;

  case 31:
#line 207 "parser.yy"
    {printf("NOT\n"); (yyval.tree_s)=(SQLTree *)driver.calc.aSQLTree->make_expr((tree *)(yysemantic_stack_[(2) - (2)].tree_s),NULL,NULL,not);;}
    break;

  case 32:
#line 208 "parser.yy"
    {printf("COMP\n"); (yyval.tree_s)=(SQLTree *)driver.calc.aSQLTree->make_expr((tree *)(yysemantic_stack_[(3) - (1)].tree_s),(tree *)(yysemantic_stack_[(3) - (3)].tree_s),*(yysemantic_stack_[(3) - (2)].stringVal),binary);;}
    break;

  case 33:
#line 209 "parser.yy"
    {(yyval.tree_s)=(SQLTree *)driver.calc.aSQLTree->make_expr((tree *)(yysemantic_stack_[(3) - (2)].tree_s),NULL,NULL,paren);;}
    break;

  case 34:
#line 212 "parser.yy"
    { (yyval.tree_s)=(SQLTree *)driver.calc.aSQLTree->make_list((tree *)(yysemantic_stack_[(1) - (1)].tree_s),table_ref); ;}
    break;

  case 35:
#line 213 "parser.yy"
    { (yyval.tree_s)=(SQLTree *)driver.calc.aSQLTree->append((tree *)(yysemantic_stack_[(3) - (1)].tree_s),(tree *)(yysemantic_stack_[(3) - (3)].tree_s),table_ref); ;}
    break;

  case 36:
#line 217 "parser.yy"
    {printf("VALUES\n");(yyval.tree_s)=(yysemantic_stack_[(4) - (3)].tree_s);;}
    break;

  case 37:
#line 218 "parser.yy"
    {printf("SELECT\n");(yyval.tree_s)=(yysemantic_stack_[(1) - (1)].tree_s);;}
    break;

  case 38:
#line 222 "parser.yy"
    {(yyval.tree_s)=(SQLTree *)driver.calc.aSQLTree->make_list((tree *)(yysemantic_stack_[(1) - (1)].tree_s),column);;}
    break;

  case 39:
#line 223 "parser.yy"
    {(yyval.tree_s)=(SQLTree *)driver.calc.aSQLTree->append((tree *)(yysemantic_stack_[(3) - (1)].tree_s),(tree *)(yysemantic_stack_[(3) - (3)].tree_s),column);;}
    break;

  case 40:
#line 227 "parser.yy"
    {printf("atom in insert_atom\n");(yyval.tree_s)=(yysemantic_stack_[(1) - (1)].tree_s);;}
    break;

  case 41:
#line 228 "parser.yy"
    {(yyval.tree_s)=NULL;;}
    break;

  case 42:
#line 232 "parser.yy"
    {(yyval.tree_s)=(SQLTree *)driver.calc.aSQLTree->append((tree *)(yysemantic_stack_[(3) - (1)].tree_s),(tree *)(yysemantic_stack_[(3) - (3)].tree_s),column);;}
    break;

  case 43:
#line 233 "parser.yy"
    {(yyval.tree_s)=(SQLTree *)driver.calc.aSQLTree->make_list((tree *)(yysemantic_stack_[(1) - (1)].tree_s),column);;}
    break;

  case 44:
#line 237 "parser.yy"
    {(yyval.tree_s)=NULL;;}
    break;

  case 45:
#line 238 "parser.yy"
    {(yyval.tree_s)=(yysemantic_stack_[(3) - (2)].tree_s);;}
    break;

  case 46:
#line 243 "parser.yy"
    {(yyval.tree_s)=(SQLTree *)driver.calc.aSQLTree->make_list((tree *)(yysemantic_stack_[(1) - (1)].tree_s),column);;}
    break;

  case 47:
#line 244 "parser.yy"
    {printf(",\n");(yyval.tree_s)=(SQLTree *)driver.calc.aSQLTree->append((tree *)(yysemantic_stack_[(3) - (1)].tree_s),(tree *)(yysemantic_stack_[(3) - (3)].tree_s),column);;}
    break;

  case 48:
#line 248 "parser.yy"
    {(yyval.tree_s)=NULL;;}
    break;

  case 49:
#line 249 "parser.yy"
    {(yyval.tree_s)=(SQLTree *)driver.calc.aSQLTree->make_stmt(NULL,NULL,NULL,NULL,select_st);;}
    break;

  case 52:
#line 258 "parser.yy"
    {(yyval.tree_s)=(SQLTree *)driver.calc.aSQLTree->make_literal(*(yysemantic_stack_[(1) - (1)].stringVal));;}
    break;

  case 53:
#line 259 "parser.yy"
    {(yyval.tree_s)=(SQLTree *)driver.calc.aSQLTree->make_number((yysemantic_stack_[(1) - (1)].integerVal));;}
    break;

  case 54:
#line 260 "parser.yy"
    {(yyval.tree_s)=(yysemantic_stack_[(1) - (1)].tree_s);;}
    break;

  case 55:
#line 264 "parser.yy"
    {printf("NAME\n");(yyval.tree_s)=(SQLTree *)driver.calc.aSQLTree->make_colref(*(yysemantic_stack_[(2) - (1)].stringVal),(tree *)(yysemantic_stack_[(2) - (2)].tree_s));;}
    break;

  case 56:
#line 269 "parser.yy"
    {(yyval.tree_s)=NULL;;}
    break;

  case 57:
#line 270 "parser.yy"
    {printf(".NAME\n");(yyval.tree_s)=(SQLTree *)driver.calc.aSQLTree->make_variable(*(yysemantic_stack_[(2) - (2)].stringVal));;}
    break;

  case 58:
#line 274 "parser.yy"
    {std::cout<<*(yysemantic_stack_[(1) - (1)].stringVal);std::cout<<"\n";(yyval.tree_s) = (SQLTree *)driver.calc.aSQLTree->make_variable(*(yysemantic_stack_[(1) - (1)].stringVal));;}
    break;


    /* Line 675 of lalr1.cc.  */
#line 707 "parser.cc"
	default: break;
      }
    YY_SYMBOL_PRINT ("-> $$ =", yyr1_[yyn], &yyval, &yyloc);

    yypop_ (yylen);
    yylen = 0;
    YY_STACK_PRINT ();

    yysemantic_stack_.push (yyval);
    yylocation_stack_.push (yyloc);

    /* Shift the result of the reduction.  */
    yyn = yyr1_[yyn];
    yystate = yypgoto_[yyn - yyntokens_] + yystate_stack_[0];
    if (0 <= yystate && yystate <= yylast_
	&& yycheck_[yystate] == yystate_stack_[0])
      yystate = yytable_[yystate];
    else
      yystate = yydefgoto_[yyn - yyntokens_];
    goto yynewstate;

  /*------------------------------------.
  | yyerrlab -- here on detecting error |
  `------------------------------------*/
  yyerrlab:
    /* If not already recovering from an error, report this error.  */
    if (!yyerrstatus_)
      {
	++yynerrs_;
	error (yylloc, yysyntax_error_ (yystate, yytoken));
      }

    yyerror_range[0] = yylloc;
    if (yyerrstatus_ == 3)
      {
	/* If just tried and failed to reuse look-ahead token after an
	 error, discard it.  */

	if (yychar <= yyeof_)
	  {
	  /* Return failure if at end of input.  */
	  if (yychar == yyeof_)
	    YYABORT;
	  }
	else
	  {
	    yydestruct_ ("Error: discarding", yytoken, &yylval, &yylloc);
	    yychar = yyempty_;
	  }
      }

    /* Else will try to reuse look-ahead token after shifting the error
       token.  */
    goto yyerrlab1;


  /*---------------------------------------------------.
  | yyerrorlab -- error raised explicitly by YYERROR.  |
  `---------------------------------------------------*/
  yyerrorlab:

    /* Pacify compilers like GCC when the user code never invokes
       YYERROR and the label yyerrorlab therefore never appears in user
       code.  */
    if (false)
      goto yyerrorlab;

    yyerror_range[0] = yylocation_stack_[yylen - 1];
    /* Do not reclaim the symbols of the rule which action triggered
       this YYERROR.  */
    yypop_ (yylen);
    yylen = 0;
    yystate = yystate_stack_[0];
    goto yyerrlab1;

  /*-------------------------------------------------------------.
  | yyerrlab1 -- common code for both syntax error and YYERROR.  |
  `-------------------------------------------------------------*/
  yyerrlab1:
    yyerrstatus_ = 3;	/* Each real token shifted decrements this.  */

    for (;;)
      {
	yyn = yypact_[yystate];
	if (yyn != yypact_ninf_)
	{
	  yyn += yyterror_;
	  if (0 <= yyn && yyn <= yylast_ && yycheck_[yyn] == yyterror_)
	    {
	      yyn = yytable_[yyn];
	      if (0 < yyn)
		break;
	    }
	}

	/* Pop the current state because it cannot handle the error token.  */
	if (yystate_stack_.height () == 1)
	YYABORT;

	yyerror_range[0] = yylocation_stack_[0];
	yydestruct_ ("Error: popping",
		     yystos_[yystate],
		     &yysemantic_stack_[0], &yylocation_stack_[0]);
	yypop_ ();
	yystate = yystate_stack_[0];
	YY_STACK_PRINT ();
      }

    if (yyn == yyfinal_)
      goto yyacceptlab;

    yyerror_range[1] = yylloc;
    // Using YYLLOC is tempting, but would change the location of
    // the look-ahead.  YYLOC is available though.
    YYLLOC_DEFAULT (yyloc, (yyerror_range - 1), 2);
    yysemantic_stack_.push (yylval);
    yylocation_stack_.push (yyloc);

    /* Shift the error token.  */
    YY_SYMBOL_PRINT ("Shifting", yystos_[yyn],
		   &yysemantic_stack_[0], &yylocation_stack_[0]);

    yystate = yyn;
    goto yynewstate;

    /* Accept.  */
  yyacceptlab:
    yyresult = 0;
    goto yyreturn;

    /* Abort.  */
  yyabortlab:
    yyresult = 1;
    goto yyreturn;

  yyreturn:
    if (yychar != yyeof_ && yychar != yyempty_)
      yydestruct_ ("Cleanup: discarding lookahead", yytoken, &yylval, &yylloc);

    /* Do not reclaim the symbols of the rule which action triggered
       this YYABORT or YYACCEPT.  */
    yypop_ (yylen);
    while (yystate_stack_.height () != 1)
      {
	yydestruct_ ("Cleanup: popping",
		   yystos_[yystate_stack_[0]],
		   &yysemantic_stack_[0],
		   &yylocation_stack_[0]);
	yypop_ ();
      }

    return yyresult;
  }

  // Generate an error message.
  std::string
  Parser::yysyntax_error_ (int yystate, int tok)
  {
    std::string res;
    YYUSE (yystate);
#if YYERROR_VERBOSE
    int yyn = yypact_[yystate];
    if (yypact_ninf_ < yyn && yyn <= yylast_)
      {
	/* Start YYX at -YYN if negative to avoid negative indexes in
	   YYCHECK.  */
	int yyxbegin = yyn < 0 ? -yyn : 0;

	/* Stay within bounds of both yycheck and yytname.  */
	int yychecklim = yylast_ - yyn + 1;
	int yyxend = yychecklim < yyntokens_ ? yychecklim : yyntokens_;
	int count = 0;
	for (int x = yyxbegin; x < yyxend; ++x)
	  if (yycheck_[x + yyn] == x && x != yyterror_)
	    ++count;

	// FIXME: This method of building the message is not compatible
	// with internationalization.  It should work like yacc.c does it.
	// That is, first build a string that looks like this:
	// "syntax error, unexpected %s or %s or %s"
	// Then, invoke YY_ on this string.
	// Finally, use the string as a format to output
	// yytname_[tok], etc.
	// Until this gets fixed, this message appears in English only.
	res = "syntax error, unexpected ";
	res += yytnamerr_ (yytname_[tok]);
	if (count < 5)
	  {
	    count = 0;
	    for (int x = yyxbegin; x < yyxend; ++x)
	      if (yycheck_[x + yyn] == x && x != yyterror_)
		{
		  res += (!count++) ? ", expecting " : " or ";
		  res += yytnamerr_ (yytname_[x]);
		}
	  }
      }
    else
#endif
      res = YY_("syntax error");
    return res;
  }


  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
  const signed char Parser::yypact_ninf_ = -74;
  const signed char
  Parser::yypact_[] =
  {
       -74,     3,   -74,    -2,    26,    10,    29,    51,   -74,     4,
     -74,   -74,   -74,   -74,   -74,    47,    48,    47,    47,   -74,
      12,   -74,   -74,   -74,    65,    77,   -74,    66,    43,   -74,
     -74,    58,   -74,    43,    91,    67,    -7,   -74,   -74,    70,
      43,   -74,    70,    18,    60,    71,   -74,    36,    47,    43,
      43,    43,    43,    43,    43,    43,    43,   -20,   -74,    59,
      -7,    23,   -74,    72,   -74,   -74,   -74,   -74,   -12,   -74,
      -7,    55,    60,    64,    41,    41,   -74,   -74,   -74,    70,
     -74,   -74,   -74,    70,    -3,    47,    92,   -74,   -74,   -74,
      61,   -74,   -74,   -74,   103,   -74,   -74,    -3,    70,   -74,
     -74
  };

  /* YYDEFACT[S] -- default rule to reduce with in state S when YYTABLE
     doesn't specify something else to do.  Zero means the default is an
     error.  */
  const unsigned char
  Parser::yydefact_[] =
  {
         2,     0,     1,     0,     0,     0,     0,    48,     3,     0,
       7,     6,     8,     9,    10,     0,     0,     0,     0,    49,
       0,     5,     4,    58,     0,    16,    11,    44,     0,    21,
      53,    56,    52,     0,     0,    20,    22,    24,    54,     0,
       0,    15,     0,     0,    31,     0,    55,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    43,     0,
      17,     0,    46,     0,    37,    13,    57,    33,    16,    34,
      23,    30,    29,    32,    25,    26,    27,    28,    12,     0,
      50,    51,    45,     0,     0,     0,    18,    42,    47,    41,
       0,    38,    40,    35,     0,    14,    36,     0,     0,    39,
      19
  };

  /* YYPGOTO[NTERM-NUM].  */
  const signed char
  Parser::yypgoto_[] =
  {
       -74,   -74,   -74,   -74,   -74,   -74,    68,   -74,    40,   -74,
     -74,   -74,    -1,   -74,   -74,   -74,    13,   -74,   -74,   -74,
     -74,    30,   -73,   -37,   -74,   -17
  };

  /* YYDEFGOTO[NTERM-NUM].  */
  const signed char
  Parser::yydefgoto_[] =
  {
        -1,     1,     9,    10,    11,    12,    13,    14,    41,    95,
      34,    35,    36,    68,    65,    90,    91,    57,    43,    61,
      20,    58,    37,    38,    46,    24
  };

  /* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule which
     number is the opposite.  If zero, do what YYDEFACT says.  */
  const signed char Parser::yytable_ninf_ = -1;
  const unsigned char
  Parser::yytable_[] =
  {
        26,    27,    59,     2,    21,    62,    40,     3,     4,    89,
       5,    92,     6,    50,    15,    51,    78,    79,     7,    52,
      53,    54,    55,    56,    92,    85,    17,    44,    30,    31,
      32,    69,    47,     7,    16,    63,    28,     8,    22,    60,
      18,    29,    59,    30,    31,    32,    88,    33,    70,    71,
      72,    73,    74,    75,    76,    77,    50,    19,    51,    82,
      83,   100,    52,    53,    54,    55,    56,    28,    93,    80,
      55,    56,    67,    81,    30,    31,    32,    51,    33,    23,
      25,    52,    53,    54,    55,    56,    52,    53,    54,    55,
      56,    53,    54,    55,    56,    40,    45,    96,    97,    48,
      39,    42,    31,    66,    49,    94,    98,    84,    86,    87,
      99,    64
  };

  /* YYCHECK.  */
  const unsigned char
  Parser::yycheck_[] =
  {
        17,    18,    39,     0,     0,    42,    18,     4,     5,    12,
       7,    84,     9,    20,    16,    22,    36,    37,    15,    26,
      27,    28,    29,    30,    97,    37,    16,    28,    31,    32,
      33,    48,    33,    15,     8,    17,    24,    34,    34,    40,
      11,    29,    79,    31,    32,    33,    83,    35,    49,    50,
      51,    52,    53,    54,    55,    56,    20,     6,    22,    36,
      37,    98,    26,    27,    28,    29,    30,    24,    85,    10,
      29,    30,    36,    14,    31,    32,    33,    22,    35,    32,
      32,    26,    27,    28,    29,    30,    26,    27,    28,    29,
      30,    27,    28,    29,    30,    18,    38,    36,    37,     8,
      35,    35,    32,    32,    37,    13,     3,    35,    68,    79,
      97,    43
  };

  /* STOS_[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
  const unsigned char
  Parser::yystos_[] =
  {
         0,    40,     0,     4,     5,     7,     9,    15,    34,    41,
      42,    43,    44,    45,    46,    16,     8,    16,    11,     6,
      59,     0,    34,    32,    64,    32,    64,    64,    24,    29,
      31,    32,    33,    35,    49,    50,    51,    61,    62,    35,
      18,    47,    35,    57,    51,    38,    63,    51,     8,    37,
      20,    22,    26,    27,    28,    29,    30,    56,    60,    62,
      51,    58,    62,    17,    45,    53,    32,    36,    52,    64,
      51,    51,    51,    51,    51,    51,    51,    51,    36,    37,
      10,    14,    36,    37,    35,    37,    47,    60,    62,    12,
      54,    55,    61,    64,    13,    48,    36,    37,     3,    55,
      62
  };

#if YYDEBUG
  /* TOKEN_NUMBER_[YYLEX-NUM] -- Internal symbol number corresponding
     to YYLEX-NUM.  */
  const unsigned short int
  Parser::yytoken_number_[] =
  {
         0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,    43,    45,    42,
      47,   282,   283,   284,   285,    40,    41,    44,    46
  };
#endif

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
  const unsigned char
  Parser::yyr1_[] =
  {
         0,    39,    40,    40,    40,    40,    41,    41,    41,    41,
      41,    42,    43,    44,    45,    46,    47,    47,    48,    48,
      49,    49,    50,    50,    51,    51,    51,    51,    51,    51,
      51,    51,    51,    51,    52,    52,    53,    53,    54,    54,
      55,    55,    56,    56,    57,    57,    58,    58,    59,    59,
      60,    60,    61,    61,    61,    62,    63,    63,    64
  };

  /* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
  const unsigned char
  Parser::yyr2_[] =
  {
         0,     2,     0,     2,     3,     3,     1,     1,     1,     1,
       1,     3,     6,     5,     7,     4,     0,     2,     0,     3,
       1,     1,     1,     3,     1,     3,     3,     3,     3,     3,
       3,     2,     3,     3,     1,     3,     4,     1,     1,     3,
       1,     1,     3,     1,     0,     3,     1,     3,     0,     1,
       2,     2,     1,     1,     1,     2,     0,     2,     1
  };

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
  /* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
     First, the terminals, then, starting at \a yyntokens_, nonterminals.  */
  const char*
  const Parser::yytname_[] =
  {
    "\"end of file\"", "error", "$undefined", "\"BY\"", "\"CREATE\"",
  "\"DELETE\"", "\"DISTINCT\"", "\"DROP\"", "\"FROM\"", "\"INSERT\"",
  "\"INTEGER\"", "\"INTO\"", "\"NULLX\"", "\"ORDER\"", "\"STR20\"",
  "\"SELECT\"", "\"TABLE\"", "\"VALUES\"", "\"WHERE\"", "\"OR\"", "OR",
  "\"AND\"", "AND", "\"NOT\"", "NOT", "\"COMPARISON\"", "COMPARISON",
  "'+'", "'-'", "'*'", "'/'", "\"INTNUM\"", "\"NAME\"", "\"STRING\"",
  "\"end of line\"", "'('", "')'", "','", "'.'", "$accept", "start",
  "statement", "drop_table_statement", "create_table_statement",
  "insert_statement", "select_statement", "delete_statement", "opt_where",
  "opt_orderby", "select_expr_di_list", "select_expr_list", "expr",
  "table_references", "values_or_query_spec", "insert_atom_commalist",
  "insert_atom", "base_table_element_commalist", "opt_column_commalist",
  "column_commalist", "opt_distinct", "base_table_element", "atom",
  "column_ref", "opt_column_ref", "table", 0
  };
#endif

#if YYDEBUG
  /* YYRHS -- A `-1'-separated list of the rules' RHS.  */
  const Parser::rhs_number_type
  Parser::yyrhs_[] =
  {
        40,     0,    -1,    -1,    40,    34,    -1,    40,    41,    34,
      -1,    40,    41,     0,    -1,    43,    -1,    42,    -1,    44,
      -1,    45,    -1,    46,    -1,     7,    16,    64,    -1,     4,
      16,    64,    35,    56,    36,    -1,     9,    11,    64,    57,
      53,    -1,    15,    59,    49,     8,    52,    47,    48,    -1,
       5,     8,    32,    47,    -1,    -1,    18,    51,    -1,    -1,
      13,     3,    62,    -1,    50,    -1,    29,    -1,    51,    -1,
      50,    37,    51,    -1,    61,    -1,    51,    27,    51,    -1,
      51,    28,    51,    -1,    51,    29,    51,    -1,    51,    30,
      51,    -1,    51,    22,    51,    -1,    51,    20,    51,    -1,
      24,    51,    -1,    51,    26,    51,    -1,    35,    51,    36,
      -1,    64,    -1,    52,    37,    64,    -1,    17,    35,    54,
      36,    -1,    45,    -1,    55,    -1,    54,    37,    55,    -1,
      61,    -1,    12,    -1,    56,    37,    60,    -1,    60,    -1,
      -1,    35,    58,    36,    -1,    62,    -1,    58,    37,    62,
      -1,    -1,     6,    -1,    62,    10,    -1,    62,    14,    -1,
      33,    -1,    31,    -1,    62,    -1,    32,    63,    -1,    -1,
      38,    32,    -1,    32,    -1
  };

  /* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
     YYRHS.  */
  const unsigned char
  Parser::yyprhs_[] =
  {
         0,     0,     3,     4,     7,    11,    15,    17,    19,    21,
      23,    25,    29,    36,    42,    50,    55,    56,    59,    60,
      64,    66,    68,    70,    74,    76,    80,    84,    88,    92,
      96,   100,   103,   107,   111,   113,   117,   122,   124,   126,
     130,   132,   134,   138,   140,   141,   145,   147,   151,   152,
     154,   157,   160,   162,   164,   166,   169,   170,   173
  };

  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
  const unsigned short int
  Parser::yyrline_[] =
  {
         0,   128,   128,   129,   130,   131,   135,   136,   137,   138,
     139,   143,   149,   155,   161,   176,   182,   183,   187,   188,
     191,   192,   195,   196,   200,   201,   202,   203,   204,   205,
     206,   207,   208,   209,   212,   213,   217,   218,   222,   223,
     227,   228,   232,   233,   237,   238,   243,   244,   248,   249,
     253,   254,   258,   259,   260,   264,   269,   270,   274
  };

  // Print the state stack on the debug stream.
  void
  Parser::yystack_print_ ()
  {
    *yycdebug_ << "Stack now";
    for (state_stack_type::const_iterator i = yystate_stack_.begin ();
	 i != yystate_stack_.end (); ++i)
      *yycdebug_ << ' ' << *i;
    *yycdebug_ << std::endl;
  }

  // Report on the debug stream that the rule \a yyrule is going to be reduced.
  void
  Parser::yy_reduce_print_ (int yyrule)
  {
    unsigned int yylno = yyrline_[yyrule];
    int yynrhs = yyr2_[yyrule];
    /* Print the symbols being reduced, and their result.  */
    *yycdebug_ << "Reducing stack by rule " << yyrule - 1
	       << " (line " << yylno << "), ";
    /* The symbols being reduced.  */
    for (int yyi = 0; yyi < yynrhs; yyi++)
      YY_SYMBOL_PRINT ("   $" << yyi + 1 << " =",
		       yyrhs_[yyprhs_[yyrule] + yyi],
		       &(yysemantic_stack_[(yynrhs) - (yyi + 1)]),
		       &(yylocation_stack_[(yynrhs) - (yyi + 1)]));
  }
#endif // YYDEBUG

  /* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
  Parser::token_number_type
  Parser::yytranslate_ (int t)
  {
    static
    const token_number_type
    translate_table[] =
    {
           0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      35,    36,    29,    27,    37,    28,    38,    30,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    31,    32,    33,    34
    };
    if ((unsigned int) t <= yyuser_token_number_max_)
      return translate_table[t];
    else
      return yyundef_token_;
  }

  const int Parser::yyeof_ = 0;
  const int Parser::yylast_ = 111;
  const int Parser::yynnts_ = 26;
  const int Parser::yyempty_ = -2;
  const int Parser::yyfinal_ = 2;
  const int Parser::yyterror_ = 1;
  const int Parser::yyerrcode_ = 256;
  const int Parser::yyntokens_ = 39;

  const unsigned int Parser::yyuser_token_number_max_ = 285;
  const Parser::token_number_type Parser::yyundef_token_ = 2;

} // namespace example

#line 287 "parser.yy"
 /*** Additional Code ***/

void example::Parser::error(const Parser::location_type& l,
			    const std::string& m)
{
    driver.error(l, m);
}

