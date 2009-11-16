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
#line 105 "parser.yy"


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
#line 100 "parser.yy"
	{ delete (yyvaluep->stringVal); };
#line 217 "parser.cc"
	break;
      case 33: /* "\"STRING\"" */
#line 101 "parser.yy"
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
	  case 7:
#line 131 "parser.yy"
    {printf("DROP TABLE\n");;}
    break;

  case 8:
#line 135 "parser.yy"
    {printf("CREATE TABLE\n");;}
    break;

  case 9:
#line 139 "parser.yy"
    {printf("INSERT INTO\n");;}
    break;

  case 10:
#line 146 "parser.yy"
    {printf("SELECT FROM\n");;}
    break;

  case 11:
#line 150 "parser.yy"
    {printf("DELETE FROM\n");;}
    break;

  case 13:
#line 155 "parser.yy"
    {printf("WHERE\n");;}
    break;

  case 15:
#line 160 "parser.yy"
    {printf("ORDER BY\n");;}
    break;

  case 16:
#line 163 "parser.yy"
    {printf("select_expr_list\n");;}
    break;

  case 17:
#line 164 "parser.yy"
    {printf("*\n");;}
    break;

  case 19:
#line 168 "parser.yy"
    {printf(",\n");;}
    break;

  case 20:
#line 171 "parser.yy"
    {printf("expr\n");;}
    break;

  case 21:
#line 175 "parser.yy"
    {printf("atom\n");;}
    break;

  case 22:
#line 176 "parser.yy"
    {printf("ADD\n");;}
    break;

  case 23:
#line 177 "parser.yy"
    {printf("SUB\n");;}
    break;

  case 24:
#line 178 "parser.yy"
    {printf("MUL\n");;}
    break;

  case 25:
#line 179 "parser.yy"
    {printf("DIV\n");;}
    break;

  case 26:
#line 180 "parser.yy"
    {printf("AND\n");;}
    break;

  case 27:
#line 181 "parser.yy"
    {printf("OR\n");;}
    break;

  case 28:
#line 182 "parser.yy"
    {printf("NOT\n");;}
    break;

  case 29:
#line 183 "parser.yy"
    {std::cout<<*(yysemantic_stack_[(3) - (2)].stringVal);printf("COMP\n");;}
    break;

  case 32:
#line 188 "parser.yy"
    {printf(",\n");;}
    break;

  case 33:
#line 191 "parser.yy"
    {std::cout<<*(yysemantic_stack_[(1) - (1)].stringVal);std::cout<<"\n";;}
    break;

  case 34:
#line 195 "parser.yy"
    {printf("VALUES\n");;}
    break;

  case 35:
#line 196 "parser.yy"
    {printf("SELECT\n");;}
    break;

  case 37:
#line 201 "parser.yy"
    {printf(",");;}
    break;

  case 38:
#line 205 "parser.yy"
    {printf("atom\n");;}
    break;

  case 40:
#line 210 "parser.yy"
    {printf(",\n");;}
    break;

  case 45:
#line 222 "parser.yy"
    {printf(",\n");;}
    break;

  case 47:
#line 227 "parser.yy"
    {printf("DISTINCT");;}
    break;

  case 49:
#line 235 "parser.yy"
    {std::cout<<"literal";std::cout<<"\n";;}
    break;

  case 50:
#line 236 "parser.yy"
    {std::cout<<(yysemantic_stack_[(1) - (1)].integerVal);std::cout<<"\n";;}
    break;

  case 51:
#line 237 "parser.yy"
    {std::cout<<"column_ref";std::cout<<"\n";;}
    break;

  case 53:
#line 245 "parser.yy"
    {std::cout<<*(yysemantic_stack_[(1) - (1)].stringVal);std::cout<<"\n";;}
    break;

  case 54:
#line 246 "parser.yy"
    {std::cout<<*(yysemantic_stack_[(3) - (1)].stringVal)<<"."<<*(yysemantic_stack_[(3) - (3)].stringVal);std::cout<<"\n";;}
    break;

  case 55:
#line 258 "parser.yy"
    {std::cout<<*(yysemantic_stack_[(1) - (1)].stringVal);std::cout<<"\n";;}
    break;

  case 56:
#line 262 "parser.yy"
    {std::cout<<*(yysemantic_stack_[(1) - (1)].stringVal);std::cout<<"\n";;}
    break;

  case 57:
#line 266 "parser.yy"
    {printf("INTEGER");;}
    break;

  case 58:
#line 267 "parser.yy"
    {printf("STR20");;}
    break;


    /* Line 675 of lalr1.cc.  */
#line 611 "parser.cc"
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
  const signed char Parser::yypact_ninf_ = -81;
  const signed char
  Parser::yypact_[] =
  {
         3,   -11,    11,    19,    32,     8,    45,   -81,   -81,   -81,
     -81,   -81,    15,    23,    15,    15,   -81,    43,   -81,   -81,
      39,    52,   -81,    59,    47,   -81,   -81,    57,   -81,    47,
      87,    61,   -81,    36,   -81,   -81,   -81,    64,    47,   -81,
      64,    -6,    56,    66,    22,    67,    47,    47,    47,    47,
      47,    47,    47,    47,    -2,   -81,   -81,     6,    36,    18,
     -81,    68,   -81,   -81,   -81,   -81,   -81,    -5,   -81,   -81,
      10,    56,    60,    30,    30,   -81,   -81,   -81,    64,   -81,
     -81,   -81,   -81,    64,   -10,    67,    88,   -81,   -81,   -81,
      33,   -81,   -81,   -81,    97,   -81,   -81,   -10,    64,   -81,
     -81
  };

  /* YYDEFACT[S] -- default rule to reduce with in state S when YYTABLE
     doesn't specify something else to do.  Zero means the default is an
     error.  */
  const unsigned char
  Parser::yydefact_[] =
  {
         0,     0,     0,     0,     0,    46,     0,     3,     2,     4,
       5,     6,     0,     0,     0,     0,    47,     0,     1,    55,
       0,    12,     7,    42,     0,    17,    50,    53,    56,     0,
       0,    16,    18,    20,    21,    51,    49,     0,     0,    11,
       0,     0,    28,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    41,    48,     0,    13,     0,
      44,     0,    35,     9,    54,    30,    33,    12,    31,    19,
      27,    26,    29,    22,    23,    24,    25,     8,     0,    57,
      58,    52,    43,     0,     0,     0,    14,    40,    45,    39,
       0,    36,    38,    32,     0,    10,    34,     0,     0,    37,
      15
  };

  /* YYPGOTO[NTERM-NUM].  */
  const signed char
  Parser::yypgoto_[] =
  {
       -81,   -81,   -81,   -81,   -81,    62,   -81,    37,   -81,   -81,
     -81,    63,   -23,   -81,    20,   -81,   -81,     9,   -81,   -81,
     -81,   -81,    29,   -80,   -81,   -37,    77,   -81,   -81
  };

  /* YYDEFGOTO[NTERM-NUM].  */
  const signed char
  Parser::yydefgoto_[] =
  {
        -1,     6,     7,     8,     9,    10,    11,    39,    95,    30,
      31,    32,    33,    67,    68,    63,    90,    91,    54,    41,
      59,    17,    55,    34,    56,    35,    20,    36,    81
  };

  /* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule which
     number is the opposite.  If zero, do what YYDEFACT says.  */
  const signed char Parser::yytable_ninf_ = -1;
  const unsigned char
  Parser::yytable_[] =
  {
        57,    42,    89,    60,    92,    12,    44,     1,     2,     5,
       3,    61,     4,    38,    16,    58,    79,    92,     5,    13,
      80,    26,    27,    28,    70,    71,    72,    73,    74,    75,
      76,    85,    48,    77,    78,    14,    49,    50,    51,    52,
      53,    57,    47,    15,    48,    18,    88,    19,    49,    50,
      51,    52,    53,    82,    83,    21,    47,    65,    48,    52,
      53,   100,    49,    50,    51,    52,    53,    24,    96,    97,
      38,    24,    25,    37,    26,    27,    28,    29,    26,    27,
      28,    29,    49,    50,    51,    52,    53,    50,    51,    52,
      53,    22,    23,    40,    43,    45,    27,    46,    64,    66,
      98,    94,    84,    62,    86,    93,    99,    87,     0,    69
  };

  /* YYCHECK.  */
  const signed char
  Parser::yycheck_[] =
  {
        37,    24,    12,    40,    84,    16,    29,     4,     5,    15,
       7,    17,     9,    18,     6,    38,    10,    97,    15,     8,
      14,    31,    32,    33,    47,    48,    49,    50,    51,    52,
      53,    36,    22,    35,    36,    16,    26,    27,    28,    29,
      30,    78,    20,    11,    22,     0,    83,    32,    26,    27,
      28,    29,    30,    35,    36,    32,    20,    35,    22,    29,
      30,    98,    26,    27,    28,    29,    30,    24,    35,    36,
      18,    24,    29,    34,    31,    32,    33,    34,    31,    32,
      33,    34,    26,    27,    28,    29,    30,    27,    28,    29,
      30,    14,    15,    34,    37,     8,    32,    36,    32,    32,
       3,    13,    34,    41,    67,    85,    97,    78,    -1,    46
  };

  /* STOS_[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
  const unsigned char
  Parser::yystos_[] =
  {
         0,     4,     5,     7,     9,    15,    39,    40,    41,    42,
      43,    44,    16,     8,    16,    11,     6,    59,     0,    32,
      64,    32,    64,    64,    24,    29,    31,    32,    33,    34,
      47,    48,    49,    50,    61,    63,    65,    34,    18,    45,
      34,    57,    50,    37,    50,     8,    36,    20,    22,    26,
      27,    28,    29,    30,    56,    60,    62,    63,    50,    58,
      63,    17,    43,    53,    32,    35,    32,    51,    52,    49,
      50,    50,    50,    50,    50,    50,    50,    35,    36,    10,
      14,    66,    35,    36,    34,    36,    45,    60,    63,    12,
      54,    55,    61,    52,    13,    46,    35,    36,     3,    55,
      63
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
      47,   282,   283,   284,    40,    41,    44,    46
  };
#endif

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
  const unsigned char
  Parser::yyr1_[] =
  {
         0,    38,    39,    39,    39,    39,    39,    40,    41,    42,
      43,    44,    45,    45,    46,    46,    47,    47,    48,    48,
      49,    50,    50,    50,    50,    50,    50,    50,    50,    50,
      50,    51,    51,    52,    53,    53,    54,    54,    55,    55,
      56,    56,    57,    57,    58,    58,    59,    59,    60,    61,
      61,    61,    62,    63,    63,    64,    65,    66,    66
  };

  /* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
  const unsigned char
  Parser::yyr2_[] =
  {
         0,     2,     1,     1,     1,     1,     1,     3,     6,     5,
       7,     4,     0,     2,     0,     3,     1,     1,     1,     3,
       1,     1,     3,     3,     3,     3,     3,     3,     2,     3,
       3,     1,     3,     1,     4,     1,     1,     3,     1,     1,
       3,     1,     0,     3,     1,     3,     0,     1,     1,     1,
       1,     1,     2,     1,     3,     1,     1,     1,     1
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
  "'('", "')'", "','", "'.'", "$accept", "statement",
  "drop_table_statement", "create_table_statement", "insert_statement",
  "select_statement", "delete_statement", "opt_where", "opt_orderby",
  "select_expr_di_list", "select_expr_list", "select_expr", "expr",
  "table_references", "table_reference", "values_or_query_spec",
  "insert_atom_commalist", "insert_atom", "base_table_element_commalist",
  "opt_column_commalist", "column_commalist", "opt_distinct",
  "base_table_element", "atom", "column_def", "column_ref", "table",
  "literal", "data_type", 0
  };
#endif

#if YYDEBUG
  /* YYRHS -- A `-1'-separated list of the rules' RHS.  */
  const Parser::rhs_number_type
  Parser::yyrhs_[] =
  {
        39,     0,    -1,    41,    -1,    40,    -1,    42,    -1,    43,
      -1,    44,    -1,     7,    16,    64,    -1,     4,    16,    64,
      34,    56,    35,    -1,     9,    11,    64,    57,    53,    -1,
      15,    59,    47,     8,    51,    45,    46,    -1,     5,     8,
      32,    45,    -1,    -1,    18,    50,    -1,    -1,    13,     3,
      63,    -1,    48,    -1,    29,    -1,    49,    -1,    48,    36,
      49,    -1,    50,    -1,    61,    -1,    50,    27,    50,    -1,
      50,    28,    50,    -1,    50,    29,    50,    -1,    50,    30,
      50,    -1,    50,    22,    50,    -1,    50,    20,    50,    -1,
      24,    50,    -1,    50,    26,    50,    -1,    34,    50,    35,
      -1,    52,    -1,    51,    36,    52,    -1,    32,    -1,    17,
      34,    54,    35,    -1,    43,    -1,    55,    -1,    54,    36,
      55,    -1,    61,    -1,    12,    -1,    56,    36,    60,    -1,
      60,    -1,    -1,    34,    58,    35,    -1,    63,    -1,    58,
      36,    63,    -1,    -1,     6,    -1,    62,    -1,    65,    -1,
      31,    -1,    63,    -1,    63,    66,    -1,    32,    -1,    32,
      37,    32,    -1,    32,    -1,    33,    -1,    10,    -1,    14,
      -1
  };

  /* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
     YYRHS.  */
  const unsigned char
  Parser::yyprhs_[] =
  {
         0,     0,     3,     5,     7,     9,    11,    13,    17,    24,
      30,    38,    43,    44,    47,    48,    52,    54,    56,    58,
      62,    64,    66,    70,    74,    78,    82,    86,    90,    93,
      97,   101,   103,   107,   109,   114,   116,   118,   122,   124,
     126,   130,   132,   133,   137,   139,   143,   144,   146,   148,
     150,   152,   154,   157,   159,   163,   165,   167,   169
  };

  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
  const unsigned short int
  Parser::yyrline_[] =
  {
         0,   123,   123,   124,   125,   126,   127,   131,   135,   139,
     143,   150,   154,   155,   159,   160,   163,   164,   167,   168,
     171,   175,   176,   177,   178,   179,   180,   181,   182,   183,
     184,   187,   188,   191,   195,   196,   200,   201,   205,   206,
     210,   211,   214,   216,   221,   222,   225,   227,   231,   235,
     236,   237,   241,   245,   246,   258,   262,   266,   267
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
      34,    35,    29,    27,    36,    28,    37,    30,     2,     2,
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
      25,    26,    31,    32,    33
    };
    if ((unsigned int) t <= yyuser_token_number_max_)
      return translate_table[t];
    else
      return yyundef_token_;
  }

  const int Parser::yyeof_ = 0;
  const int Parser::yylast_ = 109;
  const int Parser::yynnts_ = 29;
  const int Parser::yyempty_ = -2;
  const int Parser::yyfinal_ = 18;
  const int Parser::yyterror_ = 1;
  const int Parser::yyerrcode_ = 256;
  const int Parser::yyntokens_ = 38;

  const unsigned int Parser::yyuser_token_number_max_ = 284;
  const Parser::token_number_type Parser::yyundef_token_ = 2;

} // namespace example

#line 278 "parser.yy"
 /*** Additional Code ***/

void example::Parser::error(const Parser::location_type& l,
			    const std::string& m)
{
    driver.error(l, m);
}

