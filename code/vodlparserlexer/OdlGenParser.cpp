#define YY_OdlParserBase_h_included

/*  A Bison++ parser, made from OdlParser.y  */

 /* with Bison++ version bison++ Version 1.21-8, adapted from GNU bison by coetmeur@icdc.fr
  */


#line 1 "D:\\testcode\\Odl\\extern\\flexppbisonpp\\bison.cc"
/* -*-C-*-  Note some compilers choke on comments on `#line' lines.  */
/* Skeleton output parser for bison,
   Copyright (C) 1984, 1989, 1990 Bob Corbett and Richard Stallman

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 1, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.  */

/* HEADER SECTION */
#if defined( _MSDOS ) || defined(MSDOS) || defined(__MSDOS__) 
#define __MSDOS_AND_ALIKE
#endif
#if defined(_WINDOWS) && defined(_MSC_VER)
#define __HAVE_NO_ALLOCA
#define __MSDOS_AND_ALIKE
#endif

#ifndef alloca
#if defined( __GNUC__)
#define alloca __builtin_alloca

#elif (!defined (__STDC__) && defined (sparc)) || defined (__sparc__) || defined (__sparc)  || defined (__sgi)
#include <alloca.h>

#elif defined (__MSDOS_AND_ALIKE)
#include <malloc.h>
#ifndef __TURBOC__
/* MS C runtime lib */
#define alloca _alloca
#endif

#elif defined(_AIX)
#include <malloc.h>
#pragma alloca

#elif defined(__hpux)
#ifdef __cplusplus
extern "C" {
void *alloca (unsigned int);
};
#else /* not __cplusplus */
void *alloca ();
#endif /* not __cplusplus */

#endif /* not _AIX  not MSDOS, or __TURBOC__ or _AIX, not sparc.  */
#endif /* alloca not defined.  */
#ifdef c_plusplus
#ifndef __cplusplus
#define __cplusplus
#endif
#endif
#ifdef __cplusplus
#ifndef YY_USE_CLASS
#define YY_USE_CLASS
#endif
#else
#ifndef __STDC__
#define const
#endif
#endif
#include <stdio.h>
#define YYBISON 1  

/* #line 73 "D:\\testcode\\Odl\\extern\\flexppbisonpp\\bison.cc" */
#line 85 "OdlGenParser.cpp"
#define YY_OdlParserBase_LSP_NEEDED 
#define YY_OdlParserBase_ERROR_BODY  = 0
#define YY_OdlParserBase_LEX_BODY  = 0
#define YY_OdlParserBase_PARSE_PARAM  odl::TOdlAstNode*& outputAstNode
#define YY_OdlParserBase_MEMBERS  public: int a;
#define YY_OdlParserBase_CONSTRUCTOR_INIT  : a(1)
#line 11 "OdlParser.y"


#ifdef _DEBUG
   #ifndef DBG_NEW
      #define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
      #define new DBG_NEW
   #endif
#endif  // _DEBUG

#include <iostream>
#include <fstream>
#include <string>

#include "vOdl/OdlAstNode.h"


#line 28 "OdlParser.y"
typedef union
{
	odl::TOdlAstNode* FAstNode;
} yy_OdlParserBase_stype;
#define YY_OdlParserBase_STYPE yy_OdlParserBase_stype

#line 73 "D:\\testcode\\Odl\\extern\\flexppbisonpp\\bison.cc"
/* %{ and %header{ and %union, during decl */
#define YY_OdlParserBase_BISON 1
#ifndef YY_OdlParserBase_COMPATIBILITY
#ifndef YY_USE_CLASS
#define  YY_OdlParserBase_COMPATIBILITY 1
#else
#define  YY_OdlParserBase_COMPATIBILITY 0
#endif
#endif

#if YY_OdlParserBase_COMPATIBILITY != 0
/* backward compatibility */
#ifdef YYLTYPE
#ifndef YY_OdlParserBase_LTYPE
#define YY_OdlParserBase_LTYPE YYLTYPE
#endif
#endif
#ifdef YYSTYPE
#ifndef YY_OdlParserBase_STYPE 
#define YY_OdlParserBase_STYPE YYSTYPE
#endif
#endif
#ifdef YYDEBUG
#ifndef YY_OdlParserBase_DEBUG
#define  YY_OdlParserBase_DEBUG YYDEBUG
#endif
#endif
#ifdef YY_OdlParserBase_STYPE
#ifndef yystype
#define yystype YY_OdlParserBase_STYPE
#endif
#endif
/* use goto to be compatible */
#ifndef YY_OdlParserBase_USE_GOTO
#define YY_OdlParserBase_USE_GOTO 1
#endif
#endif

/* use no goto to be clean in C++ */
#ifndef YY_OdlParserBase_USE_GOTO
#define YY_OdlParserBase_USE_GOTO 0
#endif

#ifndef YY_OdlParserBase_PURE

/* #line 117 "D:\\testcode\\Odl\\extern\\flexppbisonpp\\bison.cc" */
#line 163 "OdlGenParser.cpp"

#line 117 "D:\\testcode\\Odl\\extern\\flexppbisonpp\\bison.cc"
/*  YY_OdlParserBase_PURE */
#endif

/* section apres lecture def, avant lecture grammaire S2 */

/* #line 121 "D:\\testcode\\Odl\\extern\\flexppbisonpp\\bison.cc" */
#line 172 "OdlGenParser.cpp"

#line 121 "D:\\testcode\\Odl\\extern\\flexppbisonpp\\bison.cc"
/* prefix */
#ifndef YY_OdlParserBase_DEBUG

/* #line 123 "D:\\testcode\\Odl\\extern\\flexppbisonpp\\bison.cc" */
#line 179 "OdlGenParser.cpp"

#line 123 "D:\\testcode\\Odl\\extern\\flexppbisonpp\\bison.cc"
/* YY_OdlParserBase_DEBUG */
#endif


#ifndef YY_OdlParserBase_LSP_NEEDED

/* #line 128 "D:\\testcode\\Odl\\extern\\flexppbisonpp\\bison.cc" */
#line 189 "OdlGenParser.cpp"

#line 128 "D:\\testcode\\Odl\\extern\\flexppbisonpp\\bison.cc"
 /* YY_OdlParserBase_LSP_NEEDED*/
#endif



/* DEFAULT LTYPE*/
#ifdef YY_OdlParserBase_LSP_NEEDED
#ifndef YY_OdlParserBase_LTYPE
typedef
  struct yyltype
    {
      int timestamp;
      int first_line;
      int first_column;
      int last_line;
      int last_column;
      char *text;
   }
  yyltype;

#define YY_OdlParserBase_LTYPE yyltype
#endif
#endif
/* DEFAULT STYPE*/
      /* We used to use `unsigned long' as YY_OdlParserBase_STYPE on MSDOS,
	 but it seems better to be consistent.
	 Most programs should declare their own type anyway.  */

#ifndef YY_OdlParserBase_STYPE
#define YY_OdlParserBase_STYPE int
#endif
/* DEFAULT MISCELANEOUS */
#ifndef YY_OdlParserBase_PARSE
#define YY_OdlParserBase_PARSE yyparse
#endif
#ifndef YY_OdlParserBase_LEX
#define YY_OdlParserBase_LEX yylex
#endif
#ifndef YY_OdlParserBase_LVAL
#define YY_OdlParserBase_LVAL yylval
#endif
#ifndef YY_OdlParserBase_LLOC
#define YY_OdlParserBase_LLOC yylloc
#endif
#ifndef YY_OdlParserBase_CHAR
#define YY_OdlParserBase_CHAR yychar
#endif
#ifndef YY_OdlParserBase_NERRS
#define YY_OdlParserBase_NERRS yynerrs
#endif
#ifndef YY_OdlParserBase_DEBUG_FLAG
#define YY_OdlParserBase_DEBUG_FLAG yydebug
#endif
#ifndef YY_OdlParserBase_ERROR
#define YY_OdlParserBase_ERROR yyerror
#endif
#ifndef YY_OdlParserBase_PARSE_PARAM
#ifndef __STDC__
#ifndef __cplusplus
#ifndef YY_USE_CLASS
#define YY_OdlParserBase_PARSE_PARAM
#ifndef YY_OdlParserBase_PARSE_PARAM_DEF
#define YY_OdlParserBase_PARSE_PARAM_DEF
#endif
#endif
#endif
#endif
#ifndef YY_OdlParserBase_PARSE_PARAM
#define YY_OdlParserBase_PARSE_PARAM void
#endif
#endif
#if YY_OdlParserBase_COMPATIBILITY != 0
/* backward compatibility */
#ifdef YY_OdlParserBase_LTYPE
#ifndef YYLTYPE
#define YYLTYPE YY_OdlParserBase_LTYPE
#else
/* WARNING obsolete !!! user defined YYLTYPE not reported into generated header */
#endif
#endif
#ifndef YYSTYPE
#define YYSTYPE YY_OdlParserBase_STYPE
#else
/* WARNING obsolete !!! user defined YYSTYPE not reported into generated header */
#endif
#ifdef YY_OdlParserBase_PURE
#ifndef YYPURE
#define YYPURE YY_OdlParserBase_PURE
#endif
#endif
#ifdef YY_OdlParserBase_DEBUG
#ifndef YYDEBUG
#define YYDEBUG YY_OdlParserBase_DEBUG 
#endif
#endif
#ifndef YY_OdlParserBase_ERROR_VERBOSE
#ifdef YYERROR_VERBOSE
#define YY_OdlParserBase_ERROR_VERBOSE YYERROR_VERBOSE
#endif
#endif
#ifndef YY_OdlParserBase_LSP_NEEDED
#ifdef YYLSP_NEEDED
#define YY_OdlParserBase_LSP_NEEDED YYLSP_NEEDED
#endif
#endif
#endif
#ifndef YY_USE_CLASS
/* TOKEN C */

/* #line 236 "D:\\testcode\\Odl\\extern\\flexppbisonpp\\bison.cc" */
#line 302 "OdlGenParser.cpp"
#define	UNKNOWN	258
#define	TOKEN_IS	259
#define	TOKEN_OPEN_BRACE	260
#define	TOKEN_CLOSE_BRACE	261
#define	TOKEN_OPEN_BRACKET	262
#define	TOKEN_CLOSE_BRACKET	263
#define	TOKEN_OPEN_PARENTHESIS	264
#define	TOKEN_CLOSE_PARENTHESIS	265
#define	TOKEN_EQUALS	266
#define	TOKEN_COMMA	267
#define	TOKEN_NAMESPACE	268
#define	TOKEN_TEMPLATE	269
#define	TOKEN_COLON	270
#define	OPERATOR_PLUS	271
#define	OPERATOR_MINUS	272
#define	OPERATOR_MULTIPLY	273
#define	OPERATOR_DIVIDE	274
#define	OPERATOR_MODULO	275
#define	VALUE_INTEGER	276
#define	VALUE_FLOAT	277
#define	VALUE_STRING	278
#define	IDENTIFIER	279


#line 236 "D:\\testcode\\Odl\\extern\\flexppbisonpp\\bison.cc"
 /* #defines tokens */
#else
/* CLASS */
#ifndef YY_OdlParserBase_CLASS
#define YY_OdlParserBase_CLASS OdlParserBase
#endif
#ifndef YY_OdlParserBase_INHERIT
#define YY_OdlParserBase_INHERIT
#endif
#ifndef YY_OdlParserBase_MEMBERS
#define YY_OdlParserBase_MEMBERS 
#endif
#ifndef YY_OdlParserBase_LEX_BODY
#define YY_OdlParserBase_LEX_BODY  
#endif
#ifndef YY_OdlParserBase_ERROR_BODY
#define YY_OdlParserBase_ERROR_BODY  
#endif
#ifndef YY_OdlParserBase_CONSTRUCTOR_PARAM
#define YY_OdlParserBase_CONSTRUCTOR_PARAM
#endif
#ifndef YY_OdlParserBase_CONSTRUCTOR_CODE
#define YY_OdlParserBase_CONSTRUCTOR_CODE
#endif
#ifndef YY_OdlParserBase_CONSTRUCTOR_INIT
#define YY_OdlParserBase_CONSTRUCTOR_INIT
#endif
/* choose between enum and const */
#ifndef YY_OdlParserBase_USE_CONST_TOKEN
#define YY_OdlParserBase_USE_CONST_TOKEN 0
/* yes enum is more compatible with flex,  */
/* so by default we use it */ 
#endif
#if YY_OdlParserBase_USE_CONST_TOKEN != 0
#ifndef YY_OdlParserBase_ENUM_TOKEN
#define YY_OdlParserBase_ENUM_TOKEN yy_OdlParserBase_enum_token
#endif
#endif

class YY_OdlParserBase_CLASS YY_OdlParserBase_INHERIT
{
public: 
#if YY_OdlParserBase_USE_CONST_TOKEN != 0
/* static const int token ... */

/* #line 280 "D:\\testcode\\Odl\\extern\\flexppbisonpp\\bison.cc" */
#line 374 "OdlGenParser.cpp"
static const int UNKNOWN;
static const int TOKEN_IS;
static const int TOKEN_OPEN_BRACE;
static const int TOKEN_CLOSE_BRACE;
static const int TOKEN_OPEN_BRACKET;
static const int TOKEN_CLOSE_BRACKET;
static const int TOKEN_OPEN_PARENTHESIS;
static const int TOKEN_CLOSE_PARENTHESIS;
static const int TOKEN_EQUALS;
static const int TOKEN_COMMA;
static const int TOKEN_NAMESPACE;
static const int TOKEN_TEMPLATE;
static const int TOKEN_COLON;
static const int OPERATOR_PLUS;
static const int OPERATOR_MINUS;
static const int OPERATOR_MULTIPLY;
static const int OPERATOR_DIVIDE;
static const int OPERATOR_MODULO;
static const int VALUE_INTEGER;
static const int VALUE_FLOAT;
static const int VALUE_STRING;
static const int IDENTIFIER;


#line 280 "D:\\testcode\\Odl\\extern\\flexppbisonpp\\bison.cc"
 /* decl const */
#else
enum YY_OdlParserBase_ENUM_TOKEN { YY_OdlParserBase_NULL_TOKEN=0

/* #line 283 "D:\\testcode\\Odl\\extern\\flexppbisonpp\\bison.cc" */
#line 405 "OdlGenParser.cpp"
	,UNKNOWN=258
	,TOKEN_IS=259
	,TOKEN_OPEN_BRACE=260
	,TOKEN_CLOSE_BRACE=261
	,TOKEN_OPEN_BRACKET=262
	,TOKEN_CLOSE_BRACKET=263
	,TOKEN_OPEN_PARENTHESIS=264
	,TOKEN_CLOSE_PARENTHESIS=265
	,TOKEN_EQUALS=266
	,TOKEN_COMMA=267
	,TOKEN_NAMESPACE=268
	,TOKEN_TEMPLATE=269
	,TOKEN_COLON=270
	,OPERATOR_PLUS=271
	,OPERATOR_MINUS=272
	,OPERATOR_MULTIPLY=273
	,OPERATOR_DIVIDE=274
	,OPERATOR_MODULO=275
	,VALUE_INTEGER=276
	,VALUE_FLOAT=277
	,VALUE_STRING=278
	,IDENTIFIER=279


#line 283 "D:\\testcode\\Odl\\extern\\flexppbisonpp\\bison.cc"
 /* enum token */
     }; /* end of enum declaration */
#endif
public:
 int YY_OdlParserBase_PARSE (YY_OdlParserBase_PARSE_PARAM);
 virtual void YY_OdlParserBase_ERROR(char *msg) YY_OdlParserBase_ERROR_BODY;
#ifdef YY_OdlParserBase_PURE
#ifdef YY_OdlParserBase_LSP_NEEDED
 virtual int  YY_OdlParserBase_LEX (YY_OdlParserBase_STYPE *YY_OdlParserBase_LVAL,YY_OdlParserBase_LTYPE *YY_OdlParserBase_LLOC) YY_OdlParserBase_LEX_BODY;
#else
 virtual int  YY_OdlParserBase_LEX (YY_OdlParserBase_STYPE *YY_OdlParserBase_LVAL) YY_OdlParserBase_LEX_BODY;
#endif
#else
 virtual int YY_OdlParserBase_LEX() YY_OdlParserBase_LEX_BODY;
 YY_OdlParserBase_STYPE YY_OdlParserBase_LVAL;
#ifdef YY_OdlParserBase_LSP_NEEDED
 YY_OdlParserBase_LTYPE YY_OdlParserBase_LLOC;
#endif
 int   YY_OdlParserBase_NERRS;
 int    YY_OdlParserBase_CHAR;
#endif
#if YY_OdlParserBase_DEBUG != 0
 int YY_OdlParserBase_DEBUG_FLAG;   /*  nonzero means print parse trace     */
#endif
public:
 YY_OdlParserBase_CLASS(YY_OdlParserBase_CONSTRUCTOR_PARAM);
public:
 YY_OdlParserBase_MEMBERS 
};
/* other declare folow */
#if YY_OdlParserBase_USE_CONST_TOKEN != 0

/* #line 314 "D:\\testcode\\Odl\\extern\\flexppbisonpp\\bison.cc" */
#line 464 "OdlGenParser.cpp"
const int YY_OdlParserBase_CLASS::UNKNOWN=258;
const int YY_OdlParserBase_CLASS::TOKEN_IS=259;
const int YY_OdlParserBase_CLASS::TOKEN_OPEN_BRACE=260;
const int YY_OdlParserBase_CLASS::TOKEN_CLOSE_BRACE=261;
const int YY_OdlParserBase_CLASS::TOKEN_OPEN_BRACKET=262;
const int YY_OdlParserBase_CLASS::TOKEN_CLOSE_BRACKET=263;
const int YY_OdlParserBase_CLASS::TOKEN_OPEN_PARENTHESIS=264;
const int YY_OdlParserBase_CLASS::TOKEN_CLOSE_PARENTHESIS=265;
const int YY_OdlParserBase_CLASS::TOKEN_EQUALS=266;
const int YY_OdlParserBase_CLASS::TOKEN_COMMA=267;
const int YY_OdlParserBase_CLASS::TOKEN_NAMESPACE=268;
const int YY_OdlParserBase_CLASS::TOKEN_TEMPLATE=269;
const int YY_OdlParserBase_CLASS::TOKEN_COLON=270;
const int YY_OdlParserBase_CLASS::OPERATOR_PLUS=271;
const int YY_OdlParserBase_CLASS::OPERATOR_MINUS=272;
const int YY_OdlParserBase_CLASS::OPERATOR_MULTIPLY=273;
const int YY_OdlParserBase_CLASS::OPERATOR_DIVIDE=274;
const int YY_OdlParserBase_CLASS::OPERATOR_MODULO=275;
const int YY_OdlParserBase_CLASS::VALUE_INTEGER=276;
const int YY_OdlParserBase_CLASS::VALUE_FLOAT=277;
const int YY_OdlParserBase_CLASS::VALUE_STRING=278;
const int YY_OdlParserBase_CLASS::IDENTIFIER=279;


#line 314 "D:\\testcode\\Odl\\extern\\flexppbisonpp\\bison.cc"
 /* const YY_OdlParserBase_CLASS::token */
#endif
/*apres const  */
YY_OdlParserBase_CLASS::YY_OdlParserBase_CLASS(YY_OdlParserBase_CONSTRUCTOR_PARAM) YY_OdlParserBase_CONSTRUCTOR_INIT
{
#if YY_OdlParserBase_DEBUG != 0
YY_OdlParserBase_DEBUG_FLAG=0;
#endif
YY_OdlParserBase_CONSTRUCTOR_CODE;
};
#endif

/* #line 325 "D:\\testcode\\Odl\\extern\\flexppbisonpp\\bison.cc" */
#line 503 "OdlGenParser.cpp"


#define	YYFINAL		71
#define	YYFLAG		32768
#define	YYNTBASE	25

#define YYTRANSLATE(x) ((unsigned)(x) <= 279 ? yytranslate[x] : 40)

static const char yytranslate[] = {     0,
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
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     1,     2,     3,     4,     5,
     6,     7,     8,     9,    10,    11,    12,    13,    14,    15,
    16,    17,    18,    19,    20,    21,    22,    23,    24
};

#if YY_OdlParserBase_DEBUG != 0
static const short yyprhs[] = {     0,
     0,     2,     5,     6,    10,    16,    27,    32,    37,    39,
    43,    45,    46,    48,    49,    53,    55,    58,    61,    62,
    66,    70,    74,    76,    80,    84,    88,    90,    94,    97,
    99,   101,   103,   105,   107,   111,   115,   117
};

static const short yyrhs[] = {    26,
     0,    26,    27,     0,     0,    24,     4,    35,     0,    13,
    24,     5,    26,     6,     0,    14,    24,     4,    24,     9,
    30,    10,     5,    33,     6,     0,    24,     5,    33,     6,
     0,    24,     9,    29,    10,     0,    24,     0,    35,    12,
    29,     0,    35,     0,     0,    31,     0,     0,    31,    12,
    32,     0,    32,     0,    24,    24,     0,    33,    34,     0,
     0,    24,    11,    35,     0,    35,    16,    36,     0,    35,
    17,    36,     0,    36,     0,    36,    18,    37,     0,    36,
    19,    37,     0,    36,    20,    37,     0,    37,     0,     9,
    35,    10,     0,    17,    37,     0,    21,     0,    22,     0,
    23,     0,    38,     0,    28,     0,     7,    39,     8,     0,
    35,    12,    39,     0,    35,     0,     0
};

#endif

#if YY_OdlParserBase_DEBUG != 0
static const short yyrline[] = { 0,
    67,    74,    80,    89,    95,   101,   119,   125,   136,   146,
   153,   159,   167,   171,   178,   185,   194,   203,   209,   218,
   227,   233,   239,   246,   252,   258,   264,   271,   275,   281,
   285,   289,   293,   297,   304,   312,   319,   326
};

static const char * const yytname[] = {   "$","error","$illegal.","UNKNOWN",
"TOKEN_IS","TOKEN_OPEN_BRACE","TOKEN_CLOSE_BRACE","TOKEN_OPEN_BRACKET","TOKEN_CLOSE_BRACKET",
"TOKEN_OPEN_PARENTHESIS","TOKEN_CLOSE_PARENTHESIS","TOKEN_EQUALS","TOKEN_COMMA",
"TOKEN_NAMESPACE","TOKEN_TEMPLATE","TOKEN_COLON","OPERATOR_PLUS","OPERATOR_MINUS",
"OPERATOR_MULTIPLY","OPERATOR_DIVIDE","OPERATOR_MODULO","VALUE_INTEGER","VALUE_FLOAT",
"VALUE_STRING","IDENTIFIER","odl_ast","named_declaration_list","named_declaration",
"anomymous_object_declaration_or_reference","identifier_list","typed_identifier_list",
"typed_identifier_list_rec","typed_identifier","property_declaration_list","property_declaration",
"expression","term","factor","vector_value","vector_value_list",""
};
#endif

static const short yyr1[] = {     0,
    25,    26,    26,    27,    27,    27,    28,    28,    28,    29,
    29,    29,    30,    30,    31,    31,    32,    33,    33,    34,
    35,    35,    35,    36,    36,    36,    36,    37,    37,    37,
    37,    37,    37,    37,    38,    39,    39,    39
};

static const short yyr2[] = {     0,
     1,     2,     0,     3,     5,    10,     4,     4,     1,     3,
     1,     0,     1,     0,     3,     1,     2,     2,     0,     3,
     3,     3,     1,     3,     3,     3,     1,     3,     2,     1,
     1,     1,     1,     1,     3,     3,     1,     0
};

static const short yydefact[] = {     3,
     1,     0,     0,     0,     2,     0,     0,     0,     3,     0,
    38,     0,     0,    30,    31,    32,     9,    34,     4,    23,
    27,    33,     0,     0,    37,     0,     0,    29,    19,    12,
     0,     0,     0,     0,     0,     5,    14,    38,    35,    28,
     0,     0,    11,    21,    22,    24,    25,    26,     0,     0,
    13,    16,    36,     7,     0,    18,     8,    12,    17,     0,
     0,     0,    10,    19,    15,    20,     0,     6,     0,     0,
     0
};

static const short yydefgoto[] = {    69,
     1,     5,    18,    42,    50,    51,    52,    41,    56,    25,
    20,    21,    22,    26
};

static const short yypact[] = {-32768,
    -7,   -19,    -3,    23,-32768,    35,    43,     9,-32768,    24,
     9,     9,     9,-32768,-32768,-32768,     6,-32768,    27,    -6,
-32768,-32768,    -5,    40,    22,    44,    19,-32768,-32768,     9,
     9,     9,     9,     9,     9,-32768,    29,     9,-32768,-32768,
    -4,    41,    25,    -6,    -6,-32768,-32768,-32768,    31,    46,
    45,-32768,-32768,-32768,    47,-32768,-32768,     9,-32768,    54,
    29,     9,-32768,-32768,-32768,    27,     4,-32768,    60,    61,
-32768
};

static const short yypgoto[] = {-32768,
    53,-32768,-32768,     5,-32768,-32768,     3,     1,-32768,    -8,
    14,   -10,-32768,    28
};


#define	YYLAST		66


static const short yytable[] = {    19,
    36,    54,    28,    27,     6,     2,     3,     2,     3,    68,
    29,    33,    34,    35,    30,    11,     4,    12,     4,    55,
     7,    43,    46,    47,    48,    13,     8,    55,    40,    14,
    15,    16,    17,    38,    31,    32,    58,    31,    32,     9,
    31,    32,    31,    32,    44,    45,    10,    24,    37,    43,
    57,    39,    49,    66,    59,    60,    61,    62,    64,    70,
    71,    23,    63,    65,    67,    53
};

static const short yycheck[] = {     8,
     6,     6,    13,    12,    24,    13,    14,    13,    14,     6,
     5,    18,    19,    20,     9,     7,    24,     9,    24,    24,
    24,    30,    33,    34,    35,    17,     4,    24,    10,    21,
    22,    23,    24,    12,    16,    17,    12,    16,    17,     5,
    16,    17,    16,    17,    31,    32,     4,    24,     9,    58,
    10,     8,    24,    62,    24,    10,    12,    11,     5,     0,
     0,     9,    58,    61,    64,    38
};

#line 325 "D:\\testcode\\Odl\\extern\\flexppbisonpp\\bison.cc"
 /* fattrs + tables */

/* parser code folow  */


/* This is the parser code that is written into each bison parser
  when the %semantic_parser declaration is not specified in the grammar.
  It was written by Richard Stallman by simplifying the hairy parser
  used when %semantic_parser is specified.  */

/* Note: dollar marks section change
   the next  is replaced by the list of actions, each action
   as one case of the switch.  */ 

#if YY_OdlParserBase_USE_GOTO != 0
/* 
 SUPRESSION OF GOTO : on some C++ compiler (sun c++)
  the goto is strictly forbidden if any constructor/destructor
  is used in the whole function (very stupid isn't it ?)
 so goto are to be replaced with a 'while/switch/case construct'
 here are the macro to keep some apparent compatibility
*/
#define YYGOTO(lb) {yy_gotostate=lb;continue;}
#define YYBEGINGOTO  enum yy_labels yy_gotostate=yygotostart; \
                     for(;;) switch(yy_gotostate) { case yygotostart: {
#define YYLABEL(lb) } case lb: {
#define YYENDGOTO } } 
#define YYBEGINDECLARELABEL enum yy_labels {yygotostart
#define YYDECLARELABEL(lb) ,lb
#define YYENDDECLARELABEL  };
#else
/* macro to keep goto */
#define YYGOTO(lb) goto lb
#define YYBEGINGOTO 
#define YYLABEL(lb) lb:
#define YYENDGOTO
#define YYBEGINDECLARELABEL 
#define YYDECLARELABEL(lb)
#define YYENDDECLARELABEL 
#endif
/* LABEL DECLARATION */
YYBEGINDECLARELABEL
  YYDECLARELABEL(yynewstate)
  YYDECLARELABEL(yybackup)
/* YYDECLARELABEL(yyresume) */
  YYDECLARELABEL(yydefault)
  YYDECLARELABEL(yyreduce)
  YYDECLARELABEL(yyerrlab)   /* here on detecting error */
  YYDECLARELABEL(yyerrlab1)   /* here on error raised explicitly by an action */
  YYDECLARELABEL(yyerrdefault)  /* current state does not do anything special for the error token. */
  YYDECLARELABEL(yyerrpop)   /* pop the current state because it cannot handle the error token */
  YYDECLARELABEL(yyerrhandle)  
YYENDDECLARELABEL
/* ALLOCA SIMULATION */
/* __HAVE_NO_ALLOCA */
#ifdef __HAVE_NO_ALLOCA
int __alloca_free_ptr(char *ptr,char *ref)
{if(ptr!=ref) free(ptr);
 return 0;}

#define __ALLOCA_alloca(size) malloc(size)
#define __ALLOCA_free(ptr,ref) __alloca_free_ptr((char *)ptr,(char *)ref)

#ifdef YY_OdlParserBase_LSP_NEEDED
#define __ALLOCA_return(num) \
            return( __ALLOCA_free(yyss,yyssa)+\
		    __ALLOCA_free(yyvs,yyvsa)+\
		    __ALLOCA_free(yyls,yylsa)+\
		   (num))
#else
#define __ALLOCA_return(num) \
            return( __ALLOCA_free(yyss,yyssa)+\
		    __ALLOCA_free(yyvs,yyvsa)+\
		   (num))
#endif
#else
#define __ALLOCA_return(num) return(num)
#define __ALLOCA_alloca(size) alloca(size)
#define __ALLOCA_free(ptr,ref) 
#endif

/* ENDALLOCA SIMULATION */

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (YY_OdlParserBase_CHAR = YYEMPTY)
#define YYEMPTY         -2
#define YYEOF           0
#define YYACCEPT        __ALLOCA_return(0)
#define YYABORT         __ALLOCA_return(1)
#define YYERROR         YYGOTO(yyerrlab1)
/* Like YYERROR except do call yyerror.
   This remains here temporarily to ease the
   transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */
#define YYFAIL          YYGOTO(yyerrlab)
#define YYRECOVERING()  (!!yyerrstatus)
#define YYBACKUP(token, value) \
do                                                              \
  if (YY_OdlParserBase_CHAR == YYEMPTY && yylen == 1)                               \
    { YY_OdlParserBase_CHAR = (token), YY_OdlParserBase_LVAL = (value);                 \
      yychar1 = YYTRANSLATE (YY_OdlParserBase_CHAR);                                \
      YYPOPSTACK;                                               \
      YYGOTO(yybackup);                                            \
    }                                                           \
  else                                                          \
    { YY_OdlParserBase_ERROR ("syntax error: cannot back up"); YYERROR; }   \
while (0)

#define YYTERROR        1
#define YYERRCODE       256

#ifndef YY_OdlParserBase_PURE
/* UNPURE */
#define YYLEX           YY_OdlParserBase_LEX()
#ifndef YY_USE_CLASS
/* If nonreentrant, and not class , generate the variables here */
int     YY_OdlParserBase_CHAR;                      /*  the lookahead symbol        */
YY_OdlParserBase_STYPE      YY_OdlParserBase_LVAL;              /*  the semantic value of the */
				/*  lookahead symbol    */
int YY_OdlParserBase_NERRS;                 /*  number of parse errors so far */
#ifdef YY_OdlParserBase_LSP_NEEDED
YY_OdlParserBase_LTYPE YY_OdlParserBase_LLOC;   /*  location data for the lookahead     */
			/*  symbol                              */
#endif
#endif


#else
/* PURE */
#ifdef YY_OdlParserBase_LSP_NEEDED
#define YYLEX           YY_OdlParserBase_LEX(&YY_OdlParserBase_LVAL, &YY_OdlParserBase_LLOC)
#else
#define YYLEX           YY_OdlParserBase_LEX(&YY_OdlParserBase_LVAL)
#endif
#endif
#ifndef YY_USE_CLASS
#if YY_OdlParserBase_DEBUG != 0
int YY_OdlParserBase_DEBUG_FLAG;                    /*  nonzero means print parse trace     */
/* Since this is uninitialized, it does not stop multiple parsers
   from coexisting.  */
#endif
#endif



/*  YYINITDEPTH indicates the initial size of the parser's stacks       */

#ifndef YYINITDEPTH
#define YYINITDEPTH 200
#endif

/*  YYMAXDEPTH is the maximum size the stacks can grow to
    (effective only if the built-in stack extension method is used).  */

#if YYMAXDEPTH == 0
#undef YYMAXDEPTH
#endif

#ifndef YYMAXDEPTH
#define YYMAXDEPTH 10000
#endif


#if __GNUC__ > 1                /* GNU C and GNU C++ define this.  */
#define __yy_bcopy(FROM,TO,COUNT)       __builtin_memcpy(TO,FROM,COUNT)
#else                           /* not GNU C or C++ */

/* This is the most reliable way to avoid incompatibilities
   in available built-in functions on various systems.  */

#ifdef __cplusplus
static void __yy_bcopy (char *from, char *to, int count)
#else
#ifdef __STDC__
static void __yy_bcopy (char *from, char *to, int count)
#else
static void __yy_bcopy (from, to, count)
     char *from;
     char *to;
     int count;
#endif
#endif
{
  register char *f = from;
  register char *t = to;
  register int i = count;

  while (i-- > 0)
    *t++ = *f++;
}
#endif

int
#ifdef YY_USE_CLASS
 YY_OdlParserBase_CLASS::
#endif
     YY_OdlParserBase_PARSE(YY_OdlParserBase_PARSE_PARAM)
#ifndef __STDC__
#ifndef __cplusplus
#ifndef YY_USE_CLASS
/* parameter definition without protypes */
YY_OdlParserBase_PARSE_PARAM_DEF
#endif
#endif
#endif
{
  register int yystate;
  register int yyn;
  register short *yyssp;
  register YY_OdlParserBase_STYPE *yyvsp;
  int yyerrstatus;      /*  number of tokens to shift before error messages enabled */
  int yychar1=0;          /*  lookahead token as an internal (translated) token number */

  short yyssa[YYINITDEPTH];     /*  the state stack                     */
  YY_OdlParserBase_STYPE yyvsa[YYINITDEPTH];        /*  the semantic value stack            */

  short *yyss = yyssa;          /*  refer to the stacks thru separate pointers */
  YY_OdlParserBase_STYPE *yyvs = yyvsa;     /*  to allow yyoverflow to reallocate them elsewhere */

#ifdef YY_OdlParserBase_LSP_NEEDED
  YY_OdlParserBase_LTYPE yylsa[YYINITDEPTH];        /*  the location stack                  */
  YY_OdlParserBase_LTYPE *yyls = yylsa;
  YY_OdlParserBase_LTYPE *yylsp;

#define YYPOPSTACK   (yyvsp--, yyssp--, yylsp--)
#else
#define YYPOPSTACK   (yyvsp--, yyssp--)
#endif

  int yystacksize = YYINITDEPTH;

#ifdef YY_OdlParserBase_PURE
  int YY_OdlParserBase_CHAR;
  YY_OdlParserBase_STYPE YY_OdlParserBase_LVAL;
  int YY_OdlParserBase_NERRS;
#ifdef YY_OdlParserBase_LSP_NEEDED
  YY_OdlParserBase_LTYPE YY_OdlParserBase_LLOC;
#endif
#endif

  YY_OdlParserBase_STYPE yyval;             /*  the variable used to return         */
				/*  semantic values from the action     */
				/*  routines                            */

  int yylen;
/* start loop, in which YYGOTO may be used. */
YYBEGINGOTO

#if YY_OdlParserBase_DEBUG != 0
  if (YY_OdlParserBase_DEBUG_FLAG)
    fprintf(stderr, "Starting parse\n");
#endif
  yystate = 0;
  yyerrstatus = 0;
  YY_OdlParserBase_NERRS = 0;
  YY_OdlParserBase_CHAR = YYEMPTY;          /* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss - 1;
  yyvsp = yyvs;
#ifdef YY_OdlParserBase_LSP_NEEDED
  yylsp = yyls;
#endif

/* Push a new state, which is found in  yystate  .  */
/* In all cases, when you get here, the value and location stacks
   have just been pushed. so pushing a state here evens the stacks.  */
YYLABEL(yynewstate)

  *++yyssp = yystate;

  if (yyssp >= yyss + yystacksize - 1)
    {
      /* Give user a chance to reallocate the stack */
      /* Use copies of these so that the &'s don't force the real ones into memory. */
      YY_OdlParserBase_STYPE *yyvs1 = yyvs;
      short *yyss1 = yyss;
#ifdef YY_OdlParserBase_LSP_NEEDED
      YY_OdlParserBase_LTYPE *yyls1 = yyls;
#endif

      /* Get the current used size of the three stacks, in elements.  */
      int size = yyssp - yyss + 1;

#ifdef yyoverflow
      /* Each stack pointer address is followed by the size of
	 the data in use in that stack, in bytes.  */
#ifdef YY_OdlParserBase_LSP_NEEDED
      /* This used to be a conditional around just the two extra args,
	 but that might be undefined if yyoverflow is a macro.  */
      yyoverflow("parser stack overflow",
		 &yyss1, size * sizeof (*yyssp),
		 &yyvs1, size * sizeof (*yyvsp),
		 &yyls1, size * sizeof (*yylsp),
		 &yystacksize);
#else
      yyoverflow("parser stack overflow",
		 &yyss1, size * sizeof (*yyssp),
		 &yyvs1, size * sizeof (*yyvsp),
		 &yystacksize);
#endif

      yyss = yyss1; yyvs = yyvs1;
#ifdef YY_OdlParserBase_LSP_NEEDED
      yyls = yyls1;
#endif
#else /* no yyoverflow */
      /* Extend the stack our own way.  */
      if (yystacksize >= YYMAXDEPTH)
	{
	  YY_OdlParserBase_ERROR("parser stack overflow");
	  __ALLOCA_return(2);
	}
      yystacksize *= 2;
      if (yystacksize > YYMAXDEPTH)
	yystacksize = YYMAXDEPTH;
      yyss = (short *) __ALLOCA_alloca (yystacksize * sizeof (*yyssp));
      __yy_bcopy ((char *)yyss1, (char *)yyss, size * sizeof (*yyssp));
      __ALLOCA_free(yyss1,yyssa);
      yyvs = (YY_OdlParserBase_STYPE *) __ALLOCA_alloca (yystacksize * sizeof (*yyvsp));
      __yy_bcopy ((char *)yyvs1, (char *)yyvs, size * sizeof (*yyvsp));
      __ALLOCA_free(yyvs1,yyvsa);
#ifdef YY_OdlParserBase_LSP_NEEDED
      yyls = (YY_OdlParserBase_LTYPE *) __ALLOCA_alloca (yystacksize * sizeof (*yylsp));
      __yy_bcopy ((char *)yyls1, (char *)yyls, size * sizeof (*yylsp));
      __ALLOCA_free(yyls1,yylsa);
#endif
#endif /* no yyoverflow */

      yyssp = yyss + size - 1;
      yyvsp = yyvs + size - 1;
#ifdef YY_OdlParserBase_LSP_NEEDED
      yylsp = yyls + size - 1;
#endif

#if YY_OdlParserBase_DEBUG != 0
      if (YY_OdlParserBase_DEBUG_FLAG)
	fprintf(stderr, "Stack size increased to %d\n", yystacksize);
#endif

      if (yyssp >= yyss + yystacksize - 1)
	YYABORT;
    }

#if YY_OdlParserBase_DEBUG != 0
  if (YY_OdlParserBase_DEBUG_FLAG)
    fprintf(stderr, "Entering state %d\n", yystate);
#endif

  YYGOTO(yybackup);
YYLABEL(yybackup)

/* Do appropriate processing given the current state.  */
/* Read a lookahead token if we need one and don't already have one.  */
/* YYLABEL(yyresume) */

  /* First try to decide what to do without reference to lookahead token.  */

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    YYGOTO(yydefault);

  /* Not known => get a lookahead token if don't already have one.  */

  /* yychar is either YYEMPTY or YYEOF
     or a valid token in external form.  */

  if (YY_OdlParserBase_CHAR == YYEMPTY)
    {
#if YY_OdlParserBase_DEBUG != 0
      if (YY_OdlParserBase_DEBUG_FLAG)
	fprintf(stderr, "Reading a token: ");
#endif
      YY_OdlParserBase_CHAR = YYLEX;
    }

  /* Convert token to internal form (in yychar1) for indexing tables with */

  if (YY_OdlParserBase_CHAR <= 0)           /* This means end of input. */
    {
      yychar1 = 0;
      YY_OdlParserBase_CHAR = YYEOF;                /* Don't call YYLEX any more */

#if YY_OdlParserBase_DEBUG != 0
      if (YY_OdlParserBase_DEBUG_FLAG)
	fprintf(stderr, "Now at end of input.\n");
#endif
    }
  else
    {
      yychar1 = YYTRANSLATE(YY_OdlParserBase_CHAR);

#if YY_OdlParserBase_DEBUG != 0
      if (YY_OdlParserBase_DEBUG_FLAG)
	{
	  fprintf (stderr, "Next token is %d (%s", YY_OdlParserBase_CHAR, yytname[yychar1]);
	  /* Give the individual parser a way to print the precise meaning
	     of a token, for further debugging info.  */
#ifdef YYPRINT
	  YYPRINT (stderr, YY_OdlParserBase_CHAR, YY_OdlParserBase_LVAL);
#endif
	  fprintf (stderr, ")\n");
	}
#endif
    }

  yyn += yychar1;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != yychar1)
    YYGOTO(yydefault);

  yyn = yytable[yyn];

  /* yyn is what to do for this token type in this state.
     Negative => reduce, -yyn is rule number.
     Positive => shift, yyn is new state.
       New state is final state => don't bother to shift,
       just return success.
     0, or most negative number => error.  */

  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	YYGOTO(yyerrlab);
      yyn = -yyn;
      YYGOTO(yyreduce);
    }
  else if (yyn == 0)
    YYGOTO(yyerrlab);

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Shift the lookahead token.  */

#if YY_OdlParserBase_DEBUG != 0
  if (YY_OdlParserBase_DEBUG_FLAG)
    fprintf(stderr, "Shifting token %d (%s), ", YY_OdlParserBase_CHAR, yytname[yychar1]);
#endif

  /* Discard the token being shifted unless it is eof.  */
  if (YY_OdlParserBase_CHAR != YYEOF)
    YY_OdlParserBase_CHAR = YYEMPTY;

  *++yyvsp = YY_OdlParserBase_LVAL;
#ifdef YY_OdlParserBase_LSP_NEEDED
  *++yylsp = YY_OdlParserBase_LLOC;
#endif

  /* count tokens shifted since error; after three, turn off error status.  */
  if (yyerrstatus) yyerrstatus--;

  yystate = yyn;
  YYGOTO(yynewstate);

/* Do the default action for the current state.  */
YYLABEL(yydefault)

  yyn = yydefact[yystate];
  if (yyn == 0)
    YYGOTO(yyerrlab);

/* Do a reduction.  yyn is the number of a rule to reduce with.  */
YYLABEL(yyreduce)
  yylen = yyr2[yyn];
  if (yylen > 0)
    yyval = yyvsp[1-yylen]; /* implement default value of the action */

#if YY_OdlParserBase_DEBUG != 0
  if (YY_OdlParserBase_DEBUG_FLAG)
    {
      int i;

      fprintf (stderr, "Reducing via rule %d (line %d), ",
	       yyn, yyrline[yyn]);

      /* Print the symbols being reduced, and their result.  */
      for (i = yyprhs[yyn]; yyrhs[i] > 0; i++)
	fprintf (stderr, "%s ", yytname[yyrhs[i]]);
      fprintf (stderr, " -> %s\n", yytname[yyr1[yyn]]);
    }
#endif


/* #line 811 "D:\\testcode\\Odl\\extern\\flexppbisonpp\\bison.cc" */
#line 1147 "OdlGenParser.cpp"

  switch (yyn) {

case 1:
#line 68 "OdlParser.y"
{
	outputAstNode = yyvsp[0].FAstNode;
;
    break;}
case 2:
#line 75 "OdlParser.y"
{
	odl::TOdlAstNode* theNamespace = yyvsp[-1].FAstNode;
	theNamespace->Namespace_AppendNamedDeclaration(yyvsp[0].FAstNode);
	yyval.FAstNode = theNamespace;
;
    break;}
case 3:
#line 81 "OdlParser.y"
{
	odl::TOdlAstNode* theNamespace = new odl::TOdlAstNode();
	theNamespace->SetAsNamespace();
	yyval.FAstNode = theNamespace;	
;
    break;}
case 4:
#line 90 "OdlParser.y"
{
	odl::TOdlAstNode* namedDeclaration = yyvsp[0].FAstNode;
	namedDeclaration->SetIdentifierPointer(yyvsp[-2].FAstNode);
	yyval.FAstNode = namedDeclaration;
;
    break;}
case 5:
#line 96 "OdlParser.y"
{
	odl::TOdlAstNode* namedDeclaration = yyvsp[-1].FAstNode;
	namedDeclaration->SetIdentifierPointer(yyvsp[-3].FAstNode);
	yyval.FAstNode = namedDeclaration;
;
    break;}
case 6:
#line 102 "OdlParser.y"
{
	// {TODO}
	// not a good idea to put template declaration here.
	// it avoid the shift-reduce conflict but need a semantical analysis after.
//	delete $1;
//	delete $3;
//	delete $6;

	odl::TOdlAstNode* templateDeclaration = new odl::TOdlAstNode();
	templateDeclaration->SetAsTemplateDeclaration();
	templateDeclaration->SetIdentifierPointer(yyvsp[-8].FAstNode);

	yyval.FAstNode = templateDeclaration;
;
    break;}
case 7:
#line 120 "OdlParser.y"
{
	odl::TOdlAstNode* objectDeclaration = new odl::TOdlAstNode();
	objectDeclaration->SetAsObjectDeclaration(yyvsp[-3].FAstNode, yyvsp[-1].FAstNode);
	yyval.FAstNode = objectDeclaration;
;
    break;}
case 8:
#line 126 "OdlParser.y"
{
	// {TODO}
	delete yyvsp[-3].FAstNode;
	delete yyvsp[-1].FAstNode;

	// template instanciation.
	odl::TOdlAstNode* templateDeclaration = new odl::TOdlAstNode();
	templateDeclaration->SetAsTemplateInstanciation();
	yyval.FAstNode = templateDeclaration;
;
    break;}
case 9:
#line 137 "OdlParser.y"
{
	odl::TOdlAstNode* reference = yyvsp[0].FAstNode;
	reference->SetAsReferenceToResolve();
	yyval.FAstNode = reference;
;
    break;}
case 10:
#line 147 "OdlParser.y"
{
	// {TODO}
	delete yyvsp[-2].FAstNode;
	delete yyvsp[0].FAstNode;
	yyval.FAstNode = nullptr;
;
    break;}
case 11:
#line 154 "OdlParser.y"
{
	// {TODO}
	delete yyvsp[0].FAstNode;
	yyval.FAstNode = nullptr;
;
    break;}
case 12:
#line 160 "OdlParser.y"
{
	yyval.FAstNode = nullptr;
;
    break;}
case 13:
#line 168 "OdlParser.y"
{
	yyval.FAstNode = nullptr;
;
    break;}
case 14:
#line 172 "OdlParser.y"
{
	yyval.FAstNode = nullptr;
;
    break;}
case 15:
#line 179 "OdlParser.y"
{
	// {TODO}
	delete yyvsp[-2].FAstNode;
	delete yyvsp[0].FAstNode;
	yyval.FAstNode = nullptr;
;
    break;}
case 16:
#line 186 "OdlParser.y"
{
	// {TODO}
	delete yyvsp[0].FAstNode;
	yyval.FAstNode = nullptr;
;
    break;}
case 17:
#line 195 "OdlParser.y"
{
	delete yyvsp[-1].FAstNode;
	delete yyvsp[0].FAstNode;
	yyval.FAstNode = nullptr;
;
    break;}
case 18:
#line 204 "OdlParser.y"
{
	odl::TOdlAstNode* propertyDeclList = yyvsp[-1].FAstNode;
	propertyDeclList->PropertyDeclarationList_AppendPropertyDeclaration(yyvsp[0].FAstNode);
	yyval.FAstNode = propertyDeclList;
;
    break;}
case 19:
#line 210 "OdlParser.y"
{
	odl::TOdlAstNode* propertyListNode = new odl::TOdlAstNode();
	propertyListNode->SetAsPropertyDeclarationList();
	yyval.FAstNode = propertyListNode;
;
    break;}
case 20:
#line 219 "OdlParser.y"
{
	odl::TOdlAstNode* node = new odl::TOdlAstNode();
	node->SetAsPropertyDeclaration(yyvsp[-2].FAstNode, yyvsp[0].FAstNode);
	yyval.FAstNode = node;
;
    break;}
case 21:
#line 228 "OdlParser.y"
{
	odl::TOdlAstNode* node = new odl::TOdlAstNode();
	node->SetAsExpression(yyvsp[-2].FAstNode, yyvsp[-1].FAstNode, yyvsp[0].FAstNode);
	yyval.FAstNode = node;
;
    break;}
case 22:
#line 234 "OdlParser.y"
{
	odl::TOdlAstNode* node = new odl::TOdlAstNode();
	node->SetAsExpression(yyvsp[-2].FAstNode, yyvsp[-1].FAstNode, yyvsp[0].FAstNode);
	yyval.FAstNode = node;
;
    break;}
case 23:
#line 240 "OdlParser.y"
{
	yyval.FAstNode = yyvsp[0].FAstNode;
;
    break;}
case 24:
#line 247 "OdlParser.y"
{
	odl::TOdlAstNode* node = new odl::TOdlAstNode();
	node->SetAsExpression(yyvsp[-2].FAstNode, yyvsp[-1].FAstNode, yyvsp[0].FAstNode);
	yyval.FAstNode = node;
;
    break;}
case 25:
#line 253 "OdlParser.y"
{
	odl::TOdlAstNode* node = new odl::TOdlAstNode();
	node->SetAsExpression(yyvsp[-2].FAstNode, yyvsp[-1].FAstNode, yyvsp[0].FAstNode);
	yyval.FAstNode = node;
;
    break;}
case 26:
#line 259 "OdlParser.y"
{
	odl::TOdlAstNode* node = new odl::TOdlAstNode();
	node->SetAsExpression(yyvsp[-2].FAstNode, yyvsp[-1].FAstNode, yyvsp[0].FAstNode);
	yyval.FAstNode = node;
;
    break;}
case 27:
#line 265 "OdlParser.y"
{
	yyval.FAstNode = yyvsp[0].FAstNode;
;
    break;}
case 28:
#line 272 "OdlParser.y"
{
	yyval.FAstNode = yyvsp[-1].FAstNode;
;
    break;}
case 29:
#line 276 "OdlParser.y"
{
	odl::TOdlAstNode* node = new odl::TOdlAstNode();
	node->SetAsExpression(nullptr, yyvsp[-1].FAstNode, yyvsp[0].FAstNode);
	yyval.FAstNode = node;
;
    break;}
case 30:
#line 282 "OdlParser.y"
{
	yyval.FAstNode = yyvsp[0].FAstNode;
;
    break;}
case 31:
#line 286 "OdlParser.y"
{
	yyval.FAstNode = yyvsp[0].FAstNode;
;
    break;}
case 32:
#line 290 "OdlParser.y"
{
	yyval.FAstNode = yyvsp[0].FAstNode;
;
    break;}
case 33:
#line 294 "OdlParser.y"
{
	yyval.FAstNode = yyvsp[0].FAstNode;
;
    break;}
case 34:
#line 298 "OdlParser.y"
{
	yyval.FAstNode = yyvsp[0].FAstNode;
;
    break;}
case 35:
#line 305 "OdlParser.y"
{
	odl::TOdlAstNode* vector = yyvsp[-1].FAstNode;
	yyval.FAstNode = vector;
;
    break;}
case 36:
#line 313 "OdlParser.y"
{
	odl::TOdlAstNode* vector = yyvsp[0].FAstNode;
	odl::TOdlAstNode* expression = yyvsp[-2].FAstNode;
	vector->Vector_AppendItem(expression);
	yyval.FAstNode = vector;
;
    break;}
case 37:
#line 320 "OdlParser.y"
{
	odl::TOdlAstNode* node = new odl::TOdlAstNode();
	node->SetAsVector();
	node->Vector_AppendItem(yyvsp[0].FAstNode);
	yyval.FAstNode = node;
;
    break;}
case 38:
#line 327 "OdlParser.y"
{
	odl::TOdlAstNode* node = new odl::TOdlAstNode();
	node->SetAsVector();
	yyval.FAstNode = node;
;
    break;}
}

#line 811 "D:\\testcode\\Odl\\extern\\flexppbisonpp\\bison.cc"
   /* the action file gets copied in in place of this dollarsign  */
  yyvsp -= yylen;
  yyssp -= yylen;
#ifdef YY_OdlParserBase_LSP_NEEDED
  yylsp -= yylen;
#endif

#if YY_OdlParserBase_DEBUG != 0
  if (YY_OdlParserBase_DEBUG_FLAG)
    {
      short *ssp1 = yyss - 1;
      fprintf (stderr, "state stack now");
      while (ssp1 != yyssp)
	fprintf (stderr, " %d", *++ssp1);
      fprintf (stderr, "\n");
    }
#endif

  *++yyvsp = yyval;

#ifdef YY_OdlParserBase_LSP_NEEDED
  yylsp++;
  if (yylen == 0)
    {
      yylsp->first_line = YY_OdlParserBase_LLOC.first_line;
      yylsp->first_column = YY_OdlParserBase_LLOC.first_column;
      yylsp->last_line = (yylsp-1)->last_line;
      yylsp->last_column = (yylsp-1)->last_column;
      yylsp->text = 0;
    }
  else
    {
      yylsp->last_line = (yylsp+yylen-1)->last_line;
      yylsp->last_column = (yylsp+yylen-1)->last_column;
    }
#endif

  /* Now "shift" the result of the reduction.
     Determine what state that goes to,
     based on the state we popped back to
     and the rule number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTBASE] + *yyssp;
  if (yystate >= 0 && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTBASE];

  YYGOTO(yynewstate);

YYLABEL(yyerrlab)   /* here on detecting error */

  if (! yyerrstatus)
    /* If not already recovering from an error, report this error.  */
    {
      ++YY_OdlParserBase_NERRS;

#ifdef YY_OdlParserBase_ERROR_VERBOSE
      yyn = yypact[yystate];

      if (yyn > YYFLAG && yyn < YYLAST)
	{
	  int size = 0;
	  char *msg;
	  int x, count;

	  count = 0;
	  /* Start X at -yyn if nec to avoid negative indexes in yycheck.  */
	  for (x = (yyn < 0 ? -yyn : 0);
	       x < (sizeof(yytname) / sizeof(char *)); x++)
	    if (yycheck[x + yyn] == x)
	      size += strlen(yytname[x]) + 15, count++;
	  msg = (char *) malloc(size + 15);
	  if (msg != 0)
	    {
	      strcpy(msg, "parse error");

	      if (count < 5)
		{
		  count = 0;
		  for (x = (yyn < 0 ? -yyn : 0);
		       x < (sizeof(yytname) / sizeof(char *)); x++)
		    if (yycheck[x + yyn] == x)
		      {
			strcat(msg, count == 0 ? ", expecting `" : " or `");
			strcat(msg, yytname[x]);
			strcat(msg, "'");
			count++;
		      }
		}
	      YY_OdlParserBase_ERROR(msg);
	      free(msg);
	    }
	  else
	    YY_OdlParserBase_ERROR ("parse error; also virtual memory exceeded");
	}
      else
#endif /* YY_OdlParserBase_ERROR_VERBOSE */
	YY_OdlParserBase_ERROR("parse error");
    }

  YYGOTO(yyerrlab1);
YYLABEL(yyerrlab1)   /* here on error raised explicitly by an action */

  if (yyerrstatus == 3)
    {
      /* if just tried and failed to reuse lookahead token after an error, discard it.  */

      /* return failure if at end of input */
      if (YY_OdlParserBase_CHAR == YYEOF)
	YYABORT;

#if YY_OdlParserBase_DEBUG != 0
      if (YY_OdlParserBase_DEBUG_FLAG)
	fprintf(stderr, "Discarding token %d (%s).\n", YY_OdlParserBase_CHAR, yytname[yychar1]);
#endif

      YY_OdlParserBase_CHAR = YYEMPTY;
    }

  /* Else will try to reuse lookahead token
     after shifting the error token.  */

  yyerrstatus = 3;              /* Each real token shifted decrements this */

  YYGOTO(yyerrhandle);

YYLABEL(yyerrdefault)  /* current state does not do anything special for the error token. */

#if 0
  /* This is wrong; only states that explicitly want error tokens
     should shift them.  */
  yyn = yydefact[yystate];  /* If its default is to accept any token, ok.  Otherwise pop it.*/
  if (yyn) YYGOTO(yydefault);
#endif

YYLABEL(yyerrpop)   /* pop the current state because it cannot handle the error token */

  if (yyssp == yyss) YYABORT;
  yyvsp--;
  yystate = *--yyssp;
#ifdef YY_OdlParserBase_LSP_NEEDED
  yylsp--;
#endif

#if YY_OdlParserBase_DEBUG != 0
  if (YY_OdlParserBase_DEBUG_FLAG)
    {
      short *ssp1 = yyss - 1;
      fprintf (stderr, "Error: state stack now");
      while (ssp1 != yyssp)
	fprintf (stderr, " %d", *++ssp1);
      fprintf (stderr, "\n");
    }
#endif

YYLABEL(yyerrhandle)

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    YYGOTO(yyerrdefault);

  yyn += YYTERROR;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != YYTERROR)
    YYGOTO(yyerrdefault);

  yyn = yytable[yyn];
  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	YYGOTO(yyerrpop);
      yyn = -yyn;
      YYGOTO(yyreduce);
    }
  else if (yyn == 0)
    YYGOTO(yyerrpop);

  if (yyn == YYFINAL)
    YYACCEPT;

#if YY_OdlParserBase_DEBUG != 0
  if (YY_OdlParserBase_DEBUG_FLAG)
    fprintf(stderr, "Shifting error token, ");
#endif

  *++yyvsp = YY_OdlParserBase_LVAL;
#ifdef YY_OdlParserBase_LSP_NEEDED
  *++yylsp = YY_OdlParserBase_LLOC;
#endif

  yystate = yyn;
  YYGOTO(yynewstate);
/* end loop, in which YYGOTO may be used. */
  YYENDGOTO
}

/* END */

/* #line 1010 "D:\\testcode\\Odl\\extern\\flexppbisonpp\\bison.cc" */
#line 1652 "OdlGenParser.cpp"
#line 335 "OdlParser.y"

