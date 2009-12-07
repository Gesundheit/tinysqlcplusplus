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
        case 33: /* "\"NAME\"" */
#line 106 "parser.yy"
	{ delete (yyvaluep->stringVal); };
#line 217 "parser.cc"
	break;
      case 34: /* "\"STRING\"" */
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
    {(yyval.tree_s) = (yysemantic_stack_[(1) - (1)].tree_s);driver.calc.stmt_vector.push_back((tree *)(yysemantic_stack_[(1) - (1)].tree_s));;}
    break;

  case 7:
#line 136 "parser.yy"
    {(yyval.tree_s) = (yysemantic_stack_[(1) - (1)].tree_s);driver.calc.stmt_vector.push_back((tree *)(yysemantic_stack_[(1) - (1)].tree_s));;}
    break;

  case 8:
#line 137 "parser.yy"
    {(yyval.tree_s) = (yysemantic_stack_[(1) - (1)].tree_s);driver.calc.stmt_vector.push_back((tree *)(yysemantic_stack_[(1) - (1)].tree_s));;}
    break;

  case 9:
#line 138 "parser.yy"
    {(yyval.tree_s) = (yysemantic_stack_[(1) - (1)].tree_s);driver.calc.stmt_vector.push_back((tree *)(yysemantic_stack_[(1) - (1)].tree_s));;}
    break;

  case 10:
#line 139 "parser.yy"
    {(yyval.tree_s) = (yysemantic_stack_[(1) - (1)].tree_s);driver.calc.stmt_vector.push_back((tree *)(yysemantic_stack_[(1) - (1)].tree_s));;}
    break;

  case 11:
#line 143 "parser.yy"
    {printf("DROP TABLE\n");
																		(yyval.tree_s) =(SQLTree *) driver.calc.aSQLTree->make_stmt((tree *)(yysemantic_stack_[(3) - (3)].tree_s),NULL,NULL,NULL,drop_st);
																		;}
    break;

  case 12:
#line 149 "parser.yy"
    {printf("CREATE TABLE\n");																	   
																		(yyval.tree_s) = (SQLTree *)driver.calc.aSQLTree->make_stmt((tree *)(yysemantic_stack_[(6) - (3)].tree_s),(tree *)(yysemantic_stack_[(6) - (5)].tree_s),NULL,NULL,create_st);
																		;}
    break;

  case 13:
#line 155 "parser.yy"
    {printf("INSERT INTO\n");
																		(yyval.tree_s) = (SQLTree *) driver.calc.aSQLTree->make_stmt((tree *)(yysemantic_stack_[(5) - (3)].tree_s),(tree *)(yysemantic_stack_[(5) - (4)].tree_s),(tree *)(yysemantic_stack_[(5) - (5)].tree_s),NULL,insert_st);
																		;}
    break;

  case 14:
#line 164 "parser.yy"
    {printf("SELECT FROM\n"); 																		


																	if((yysemantic_stack_[(7) - (2)].tree_s)==NULL){
																	(yyval.tree_s) = (SQLTree *)driver.calc.aSQLTree->make_stmt((tree *)(yysemantic_stack_[(7) - (3)].tree_s),(tree *)(yysemantic_stack_[(7) - (5)].tree_s),(tree *)(yysemantic_stack_[(7) - (6)].tree_s),(tree *)(yysemantic_stack_[(7) - (7)].tree_s),select_st,false);
																	}else{
																	(yyval.tree_s) = (SQLTree *)driver.calc.aSQLTree->make_stmt((tree *)(yysemantic_stack_[(7) - (3)].tree_s),(tree *)(yysemantic_stack_[(7) - (5)].tree_s),(tree *)(yysemantic_stack_[(7) - (6)].tree_s),(tree *)(yysemantic_stack_[(7) - (7)].tree_s),select_st,true);
																	}
		;}
    break;

  case 15:
#line 176 "parser.yy"
    {printf("DELETE FROM\n");
																	(yyval.tree_s) = (SQLTree *)driver.calc.aSQLTree->make_stmt((tree *)(yysemantic_stack_[(4) - (3)].tree_s),(tree *)(yysemantic_stack_[(4) - (4)].tree_s),NULL,NULL,delete_st);
																	;}
    break;

  case 16:
#line 181 "parser.yy"
    {(yyval.tree_s)=NULL;;}
    break;

  case 17:
#line 182 "parser.yy"
    {printf("WHERE\n");(yyval.tree_s)=(yysemantic_stack_[(2) - (2)].tree_s);;}
    break;

  case 18:
#line 186 "parser.yy"
    {(yyval.tree_s)=NULL;;}
    break;

  case 19:
#line 187 "parser.yy"
    {printf("ORDER BY\n");(yyval.tree_s)=(yysemantic_stack_[(3) - (3)].tree_s);;}
    break;

  case 20:
#line 190 "parser.yy"
    {printf("select_expr_list\n");(yyval.tree_s)=(yysemantic_stack_[(1) - (1)].tree_s);;}
    break;

  case 21:
#line 191 "parser.yy"
    {printf("*\n");(yyval.tree_s) = (SQLTree *)driver.calc.aSQLTree->make_variable("*");;}
    break;

  case 22:
#line 194 "parser.yy"
    { (yyval.tree_s)=(SQLTree *)driver.calc.aSQLTree->make_list((tree *)(yysemantic_stack_[(1) - (1)].tree_s),select); ;}
    break;

  case 23:
#line 195 "parser.yy"
    {(yyval.tree_s)=(SQLTree *)driver.calc.aSQLTree->append((tree *)(yysemantic_stack_[(3) - (1)].tree_s),(tree *)(yysemantic_stack_[(3) - (3)].tree_s),select);;}
    break;

  case 24:
#line 198 "parser.yy"
    {(yyval.tree_s)=(yysemantic_stack_[(1) - (1)].tree_s);;}
    break;

  case 25:
#line 199 "parser.yy"
    {printf("OR\n"); (yyval.tree_s)=(SQLTree *)driver.calc.aSQLTree->make_expr((tree *)(yysemantic_stack_[(3) - (1)].tree_s),(tree *)(yysemantic_stack_[(3) - (3)].tree_s),"OR",binary);;}
    break;

  case 27:
#line 202 "parser.yy"
    {printf("AND\n"); (yyval.tree_s)=(SQLTree *)driver.calc.aSQLTree->make_expr((tree *)(yysemantic_stack_[(3) - (1)].tree_s),(tree *)(yysemantic_stack_[(3) - (3)].tree_s),"AND",binary);;}
    break;

  case 28:
#line 205 "parser.yy"
    {(yyval.tree_s)=(yysemantic_stack_[(1) - (1)].tree_s);;}
    break;

  case 29:
#line 206 "parser.yy"
    {printf("NOT\n"); (yyval.tree_s)=(SQLTree *)driver.calc.aSQLTree->make_expr((tree *)(yysemantic_stack_[(2) - (2)].tree_s),NULL,NULL,not);;}
    break;

  case 30:
#line 209 "parser.yy"
    {(yyval.tree_s)=(yysemantic_stack_[(1) - (1)].tree_s);;}
    break;

  case 31:
#line 210 "parser.yy"
    {(yyval.tree_s)=(yysemantic_stack_[(3) - (2)].tree_s);;}
    break;

  case 32:
#line 213 "parser.yy"
    {printf("COMP\n"); (yyval.tree_s)=(SQLTree *)driver.calc.aSQLTree->make_expr((tree *)(yysemantic_stack_[(3) - (1)].tree_s),(tree *)(yysemantic_stack_[(3) - (3)].tree_s),">",binary);;}
    break;

  case 33:
#line 214 "parser.yy"
    {printf("COMP\n"); (yyval.tree_s)=(SQLTree *)driver.calc.aSQLTree->make_expr((tree *)(yysemantic_stack_[(3) - (1)].tree_s),(tree *)(yysemantic_stack_[(3) - (3)].tree_s),"<",binary);;}
    break;

  case 34:
#line 215 "parser.yy"
    {printf("COMP\n"); (yyval.tree_s)=(SQLTree *)driver.calc.aSQLTree->make_expr((tree *)(yysemantic_stack_[(3) - (1)].tree_s),(tree *)(yysemantic_stack_[(3) - (3)].tree_s),"=",binary);;}
    break;

  case 35:
#line 218 "parser.yy"
    {printf("atom\n");;}
    break;

  case 36:
#line 219 "parser.yy"
    {printf("ADD\n"); (yyval.tree_s)=(SQLTree *)driver.calc.aSQLTree->make_expr((tree *)(yysemantic_stack_[(3) - (1)].tree_s),(tree *)(yysemantic_stack_[(3) - (3)].tree_s),"+",binary);;}
    break;

  case 37:
#line 220 "parser.yy"
    {printf("SUB\n"); (yyval.tree_s)=(SQLTree *)driver.calc.aSQLTree->make_expr((tree *)(yysemantic_stack_[(3) - (1)].tree_s),(tree *)(yysemantic_stack_[(3) - (3)].tree_s),"-",binary);;}
    break;

  case 39:
#line 225 "parser.yy"
    {printf("MUL\n"); (yyval.tree_s)=(SQLTree *)driver.calc.aSQLTree->make_expr((tree *)(yysemantic_stack_[(3) - (1)].tree_s),(tree *)(yysemantic_stack_[(3) - (3)].tree_s),"*",binary);;}
    break;

  case 40:
#line 226 "parser.yy"
    {printf("DIV\n"); (yyval.tree_s)=(SQLTree *)driver.calc.aSQLTree->make_expr((tree *)(yysemantic_stack_[(3) - (1)].tree_s),(tree *)(yysemantic_stack_[(3) - (3)].tree_s),"/",binary);;}
    break;

  case 41:
#line 228 "parser.yy"
    { (yyval.tree_s)=(SQLTree *)driver.calc.aSQLTree->make_list((tree *)(yysemantic_stack_[(1) - (1)].tree_s),table_ref); ;}
    break;

  case 42:
#line 229 "parser.yy"
    { (yyval.tree_s)=(SQLTree *)driver.calc.aSQLTree->append((tree *)(yysemantic_stack_[(3) - (1)].tree_s),(tree *)(yysemantic_stack_[(3) - (3)].tree_s),table_ref); ;}
    break;

  case 43:
#line 233 "parser.yy"
    {printf("VALUES\n");(yyval.tree_s)=(yysemantic_stack_[(4) - (3)].tree_s);;}
    break;

  case 44:
#line 234 "parser.yy"
    {printf("SELECT\n");(yyval.tree_s)=(yysemantic_stack_[(1) - (1)].tree_s);;}
    break;

  case 45:
#line 238 "parser.yy"
    {(yyval.tree_s)=(SQLTree *)driver.calc.aSQLTree->make_list((tree *)(yysemantic_stack_[(1) - (1)].tree_s),column);;}
    break;

  case 46:
#line 239 "parser.yy"
    {(yyval.tree_s)=(SQLTree *)driver.calc.aSQLTree->append((tree *)(yysemantic_stack_[(3) - (1)].tree_s),(tree *)(yysemantic_stack_[(3) - (3)].tree_s),column);;}
    break;

  case 47:
#line 243 "parser.yy"
    {printf("atom in insert_atom\n");(yyval.tree_s)=(yysemantic_stack_[(1) - (1)].tree_s);;}
    break;

  case 48:
#line 244 "parser.yy"
    {(yyval.tree_s)=NULL;;}
    break;

  case 49:
#line 248 "parser.yy"
    {(yyval.tree_s)=(SQLTree *)driver.calc.aSQLTree->append((tree *)(yysemantic_stack_[(3) - (1)].tree_s),(tree *)(yysemantic_stack_[(3) - (3)].tree_s),column);;}
    break;

  case 50:
#line 249 "parser.yy"
    {(yyval.tree_s)=(SQLTree *)driver.calc.aSQLTree->make_list((tree *)(yysemantic_stack_[(1) - (1)].tree_s),column);;}
    break;

  case 51:
#line 253 "parser.yy"
    {(yyval.tree_s)=NULL;;}
    break;

  case 52:
#line 254 "parser.yy"
    {(yyval.tree_s)=(yysemantic_stack_[(3) - (2)].tree_s);;}
    break;

  case 53:
#line 259 "parser.yy"
    {(yyval.tree_s)=(SQLTree *)driver.calc.aSQLTree->make_list((tree *)(yysemantic_stack_[(1) - (1)].tree_s),column);;}
    break;

  case 54:
#line 260 "parser.yy"
    {printf(",\n");(yyval.tree_s)=(SQLTree *)driver.calc.aSQLTree->append((tree *)(yysemantic_stack_[(3) - (1)].tree_s),(tree *)(yysemantic_stack_[(3) - (3)].tree_s),column);;}
    break;

  case 55:
#line 264 "parser.yy"
    {(yyval.tree_s)=NULL;;}
    break;

  case 56:
#line 265 "parser.yy"
    {(yyval.tree_s)=(SQLTree *)driver.calc.aSQLTree->make_stmt(NULL,NULL,NULL,NULL,select_st);;}
    break;

  case 57:
#line 269 "parser.yy"
    {(yyval.tree_s)=(SQLTree *)driver.calc.aSQLTree->set_type((tree *)(yysemantic_stack_[(2) - (1)].tree_s),"i");;}
    break;

  case 58:
#line 270 "parser.yy"
    {(yyval.tree_s)=(SQLTree *)driver.calc.aSQLTree->set_type((tree *)(yysemantic_stack_[(2) - (1)].tree_s),"s");;}
    break;

  case 59:
#line 274 "parser.yy"
    {(yyval.tree_s)=(SQLTree *)driver.calc.aSQLTree->make_literal(*(yysemantic_stack_[(1) - (1)].stringVal));;}
    break;

  case 60:
#line 275 "parser.yy"
    {(yyval.tree_s)=(SQLTree *)driver.calc.aSQLTree->make_number((yysemantic_stack_[(1) - (1)].integerVal));;}
    break;

  case 61:
#line 276 "parser.yy"
    {(yyval.tree_s)=(yysemantic_stack_[(1) - (1)].tree_s);;}
    break;

  case 62:
#line 277 "parser.yy"
    {(yyval.tree_s)=(SQLTree *)driver.calc.aSQLTree->make_expr((tree *)(yysemantic_stack_[(3) - (2)].tree_s),NULL,NULL,paren);;}
    break;

  case 63:
#line 281 "parser.yy"
    {(yyval.tree_s)=(SQLTree *)driver.calc.aSQLTree->make_colref(*(yysemantic_stack_[(2) - (1)].stringVal),(tree *)(yysemantic_stack_[(2) - (2)].tree_s));;}
    break;

  case 64:
#line 286 "parser.yy"
    {(yyval.tree_s)=NULL;;}
    break;

  case 65:
#line 287 "parser.yy"
    {(yyval.tree_s)=(SQLTree *)driver.calc.aSQLTree->make_variable(*(yysemantic_stack_[(2) - (2)].stringVal));;}
    break;

  case 66:
#line 291 "parser.yy"
    {(yyval.tree_s) = (SQLTree *)driver.calc.aSQLTree->make_variable(*(yysemantic_stack_[(1) - (1)].stringVal));;}
    break;


    /* Line 675 of lalr1.cc.  */
#line 747 "parser.cc"
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
  const signed char Parser::yypact_ninf_ = -69;
  const signed char
  Parser::yypact_[] =
  {
       -69,     5,   -69,    -8,     7,     3,    19,    39,   -69,     6,
     -69,   -69,   -69,   -69,   -69,    30,    30,    30,    30,   -69,
       9,   -69,   -69,   -69,    22,    62,   -69,    60,   -69,    26,
      76,    51,   -69,    64,    23,   -69,    64,    29,    65,   -69,
      30,    64,    41,   -69,    67,   -10,   -69,   -69,    23,   -69,
      75,    77,   -69,   -69,    47,   -69,    52,   -69,    48,   -69,
      66,   -69,   -69,   -69,     0,   -69,   -69,   -69,    64,   -69,
     -69,   -69,    63,    25,    23,    23,    34,    34,    34,    34,
      34,    34,    34,   -69,    64,    -5,    30,    88,   -69,   -69,
     -69,   -69,   -69,    34,    59,    59,    59,   -69,   -69,   -69,
     -69,   -69,   -69,    53,   -69,   -69,   -69,   100,   -69,    32,
     -69,    -5,    64,   -69,   -69
  };

  /* YYDEFACT[S] -- default rule to reduce with in state S when YYTABLE
     doesn't specify something else to do.  Zero means the default is an
     error.  */
  const unsigned char
  Parser::yydefact_[] =
  {
         2,     0,     1,     0,     0,     0,     0,    55,     3,     0,
       7,     6,     8,     9,    10,     0,     0,     0,     0,    56,
       0,     5,     4,    66,     0,    16,    11,    51,    21,    64,
       0,    20,    22,     0,     0,    15,     0,     0,     0,    63,
       0,     0,     0,    50,     0,     0,    60,    59,     0,    17,
      24,    26,    28,    30,     0,    35,    38,    61,     0,    53,
       0,    44,    13,    65,    16,    41,    23,    12,     0,    57,
      58,    29,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    52,     0,     0,     0,    18,    49,    31,
      62,    25,    27,     0,    33,    32,    34,    36,    37,    39,
      40,    54,    48,     0,    45,    47,    42,     0,    14,     0,
      43,     0,     0,    46,    19
  };

  /* YYPGOTO[NTERM-NUM].  */
  const signed char
  Parser::yypgoto_[] =
  {
       -69,   -69,   -69,   -69,   -69,   -69,    68,   -69,    40,   -69,
     -69,   -69,   -37,    31,   -69,    69,   -69,   -44,    12,   -69,
     -69,   -69,    -4,   -69,   -69,   -69,   -69,    42,   -68,   -20,
     -69,   -15
  };

  /* YYDEFGOTO[NTERM-NUM].  */
  const signed char
  Parser::yydefgoto_[] =
  {
        -1,     1,     9,    10,    11,    12,    13,    14,    35,   108,
      30,    31,    49,    50,    51,    52,    53,    54,    55,    64,
      62,   103,   104,    42,    37,    58,    20,    43,    56,    57,
      39,    24
  };

  /* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule which
     number is the opposite.  If zero, do what YYDEFACT says.  */
  const signed char Parser::yytable_ninf_ = -1;
  const unsigned char
  Parser::yytable_[] =
  {
        32,    25,    26,    27,    73,     2,    21,   102,    15,     3,
       4,    72,     5,    44,     6,    16,    59,   105,    34,    17,
       7,    66,    46,    29,    47,    65,    48,    46,    29,    47,
      18,    93,    94,    95,    96,    97,    98,    91,    86,    28,
       8,    22,    29,   105,     7,    19,    60,    45,    44,   109,
      76,    77,    78,    79,    80,    46,    29,    47,    33,    48,
      79,    80,    90,    23,   101,    38,    46,    29,    47,    90,
      93,   106,    76,    77,    78,    79,    80,    69,    67,    68,
      34,    70,    81,    82,    40,    83,    84,    79,    80,    41,
     110,   111,   114,    99,   100,    74,    36,    29,    63,    75,
      89,   107,    85,   112,    87,    61,    92,   113,     0,     0,
      88,     0,     0,     0,    71
  };

  /* YYCHECK.  */
  const signed char
  Parser::yycheck_[] =
  {
        20,    16,    17,    18,    48,     0,     0,    12,    16,     4,
       5,    48,     7,    33,     9,     8,    36,    85,    18,    16,
      15,    41,    32,    33,    34,    40,    36,    32,    33,    34,
      11,    36,    76,    77,    78,    79,    80,    74,    38,    30,
      35,    35,    33,   111,    15,     6,    17,    24,    68,    93,
      25,    26,    27,    28,    29,    32,    33,    34,    36,    36,
      28,    29,    37,    33,    84,    39,    32,    33,    34,    37,
      36,    86,    25,    26,    27,    28,    29,    10,    37,    38,
      18,    14,    30,    31,     8,    37,    38,    28,    29,    38,
      37,    38,   112,    81,    82,    20,    36,    33,    33,    22,
      37,    13,    36,     3,    64,    37,    75,   111,    -1,    -1,
      68,    -1,    -1,    -1,    45
  };

  /* STOS_[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
  const unsigned char
  Parser::yystos_[] =
  {
         0,    41,     0,     4,     5,     7,     9,    15,    35,    42,
      43,    44,    45,    46,    47,    16,     8,    16,    11,     6,
      66,     0,    35,    33,    71,    71,    71,    71,    30,    33,
      50,    51,    69,    36,    18,    48,    36,    64,    39,    70,
       8,    38,    63,    67,    69,    24,    32,    34,    36,    52,
      53,    54,    55,    56,    57,    58,    68,    69,    65,    69,
      17,    46,    60,    33,    59,    71,    69,    37,    38,    10,
      14,    55,    52,    57,    20,    22,    25,    26,    27,    28,
      29,    30,    31,    37,    38,    36,    38,    48,    67,    37,
      37,    52,    53,    36,    57,    57,    57,    57,    57,    58,
      58,    69,    12,    61,    62,    68,    71,    13,    49,    57,
      37,    38,     3,    62,    69
  };

#if YYDEBUG
  /* TOKEN_NUMBER_[YYLEX-NUM] -- Internal symbol number corresponding
     to YYLEX-NUM.  */
  const unsigned short int
  Parser::yytoken_number_[] =
  {
         0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,    60,    62,    61,    43,    45,
      42,    47,   280,   281,   282,   283,    40,    41,    44,    46
  };
#endif

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
  const unsigned char
  Parser::yyr1_[] =
  {
         0,    40,    41,    41,    41,    41,    42,    42,    42,    42,
      42,    43,    44,    45,    46,    47,    48,    48,    49,    49,
      50,    50,    51,    51,    52,    52,    53,    53,    54,    54,
      55,    55,    56,    56,    56,    57,    57,    57,    58,    58,
      58,    59,    59,    60,    60,    61,    61,    62,    62,    63,
      63,    64,    64,    65,    65,    66,    66,    67,    67,    68,
      68,    68,    68,    69,    70,    70,    71
  };

  /* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
  const unsigned char
  Parser::yyr2_[] =
  {
         0,     2,     0,     2,     3,     3,     1,     1,     1,     1,
       1,     3,     6,     5,     7,     4,     0,     2,     0,     3,
       1,     1,     1,     3,     1,     3,     1,     3,     1,     2,
       1,     3,     3,     3,     3,     1,     3,     3,     1,     3,
       3,     1,     3,     4,     1,     1,     3,     1,     1,     3,
       1,     0,     3,     1,     3,     0,     1,     2,     2,     1,
       1,     1,     3,     2,     0,     2,     1
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
  "\"AND\"", "AND", "\"NOT\"", "NOT", "'<'", "'>'", "'='", "'+'", "'-'",
  "'*'", "'/'", "\"INTNUM\"", "\"NAME\"", "\"STRING\"", "\"end of line\"",
  "'('", "')'", "','", "'.'", "$accept", "start", "statement",
  "drop_table_statement", "create_table_statement", "insert_statement",
  "select_statement", "delete_statement", "opt_where", "opt_orderby",
  "select_expr_di_list", "select_expr_list", "search_condition",
  "boolean_term", "boolean_factor", "boolean_primary",
  "comparison_predicate", "expr", "term", "table_references",
  "values_or_query_spec", "insert_atom_commalist", "insert_atom",
  "base_table_element_commalist", "opt_column_commalist",
  "column_commalist", "opt_distinct", "base_table_element", "atom",
  "column_ref", "opt_column_ref", "table", 0
  };
#endif

#if YYDEBUG
  /* YYRHS -- A `-1'-separated list of the rules' RHS.  */
  const Parser::rhs_number_type
  Parser::yyrhs_[] =
  {
        41,     0,    -1,    -1,    41,    35,    -1,    41,    42,    35,
      -1,    41,    42,     0,    -1,    44,    -1,    43,    -1,    45,
      -1,    46,    -1,    47,    -1,     7,    16,    71,    -1,     4,
      16,    71,    36,    63,    37,    -1,     9,    11,    71,    64,
      60,    -1,    15,    66,    50,     8,    59,    48,    49,    -1,
       5,     8,    71,    48,    -1,    -1,    18,    52,    -1,    -1,
      13,     3,    69,    -1,    51,    -1,    30,    -1,    69,    -1,
      51,    38,    69,    -1,    53,    -1,    53,    20,    52,    -1,
      54,    -1,    54,    22,    53,    -1,    55,    -1,    24,    55,
      -1,    56,    -1,    36,    52,    37,    -1,    57,    26,    57,
      -1,    57,    25,    57,    -1,    57,    27,    57,    -1,    58,
      -1,    57,    28,    57,    -1,    57,    29,    57,    -1,    68,
      -1,    68,    30,    58,    -1,    68,    31,    58,    -1,    71,
      -1,    59,    38,    71,    -1,    17,    36,    61,    37,    -1,
      46,    -1,    62,    -1,    61,    38,    62,    -1,    68,    -1,
      12,    -1,    63,    38,    67,    -1,    67,    -1,    -1,    36,
      65,    37,    -1,    69,    -1,    65,    38,    69,    -1,    -1,
       6,    -1,    69,    10,    -1,    69,    14,    -1,    34,    -1,
      32,    -1,    69,    -1,    36,    57,    37,    -1,    33,    70,
      -1,    -1,    39,    33,    -1,    33,    -1
  };

  /* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
     YYRHS.  */
  const unsigned char
  Parser::yyprhs_[] =
  {
         0,     0,     3,     4,     7,    11,    15,    17,    19,    21,
      23,    25,    29,    36,    42,    50,    55,    56,    59,    60,
      64,    66,    68,    70,    74,    76,    80,    82,    86,    88,
      91,    93,    97,   101,   105,   109,   111,   115,   119,   121,
     125,   129,   131,   135,   140,   142,   144,   148,   150,   152,
     156,   158,   159,   163,   165,   169,   170,   172,   175,   178,
     180,   182,   184,   188,   191,   192,   195
  };

  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
  const unsigned short int
  Parser::yyrline_[] =
  {
         0,   128,   128,   129,   130,   131,   135,   136,   137,   138,
     139,   143,   149,   155,   161,   176,   181,   182,   186,   187,
     190,   191,   194,   195,   198,   199,   201,   202,   205,   206,
     209,   210,   213,   214,   215,   218,   219,   220,   224,   225,
     226,   228,   229,   233,   234,   238,   239,   243,   244,   248,
     249,   253,   254,   259,   260,   264,   265,   269,   270,   274,
     275,   276,   277,   281,   286,   287,   291
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
      36,    37,    30,    28,    38,    29,    39,    31,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      25,    27,    26,     2,     2,     2,     2,     2,     2,     2,
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
      32,    33,    34,    35
    };
    if ((unsigned int) t <= yyuser_token_number_max_)
      return translate_table[t];
    else
      return yyundef_token_;
  }

  const int Parser::yyeof_ = 0;
  const int Parser::yylast_ = 114;
  const int Parser::yynnts_ = 32;
  const int Parser::yyempty_ = -2;
  const int Parser::yyfinal_ = 2;
  const int Parser::yyterror_ = 1;
  const int Parser::yyerrcode_ = 256;
  const int Parser::yyntokens_ = 40;

  const unsigned int Parser::yyuser_token_number_max_ = 283;
  const Parser::token_number_type Parser::yyundef_token_ = 2;

} // namespace example

#line 304 "parser.yy"
 /*** Additional Code ***/

void example::Parser::error(const Parser::location_type& l,
			    const std::string& m)
{
    driver.error(l, m);
}

