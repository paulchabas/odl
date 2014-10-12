#ifndef YY_OdlParserBase_h_included
#define YY_OdlParserBase_h_included

#line 1 "D:\\GitHub\\odl\\extern\\flexppbisonpp\\bison.h"
/* before anything */
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
#endif
#include <stdio.h>

/* #line 14 "D:\\GitHub\\odl\\extern\\flexppbisonpp\\bison.h" */
#line 21 "OdlGenParser.h"
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

#line 14 "D:\\GitHub\\odl\\extern\\flexppbisonpp\\bison.h"
 /* %{ and %header{ and %union, during decl */
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
/* WARNING obsolete !!! user defined YYLTYPE not reported into generated header */
/* use %define LTYPE */
#endif
#endif
#ifdef YYSTYPE
#ifndef YY_OdlParserBase_STYPE 
#define YY_OdlParserBase_STYPE YYSTYPE
/* WARNING obsolete !!! user defined YYSTYPE not reported into generated header */
/* use %define STYPE */
#endif
#endif
#ifdef YYDEBUG
#ifndef YY_OdlParserBase_DEBUG
#define  YY_OdlParserBase_DEBUG YYDEBUG
/* WARNING obsolete !!! user defined YYDEBUG not reported into generated header */
/* use %define DEBUG */
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

/* #line 63 "D:\\GitHub\\odl\\extern\\flexppbisonpp\\bison.h" */
#line 104 "OdlGenParser.h"

#line 63 "D:\\GitHub\\odl\\extern\\flexppbisonpp\\bison.h"
/* YY_OdlParserBase_PURE */
#endif

/* #line 65 "D:\\GitHub\\odl\\extern\\flexppbisonpp\\bison.h" */
#line 111 "OdlGenParser.h"

#line 65 "D:\\GitHub\\odl\\extern\\flexppbisonpp\\bison.h"
/* prefix */
#ifndef YY_OdlParserBase_DEBUG

/* #line 67 "D:\\GitHub\\odl\\extern\\flexppbisonpp\\bison.h" */
#line 118 "OdlGenParser.h"

#line 67 "D:\\GitHub\\odl\\extern\\flexppbisonpp\\bison.h"
/* YY_OdlParserBase_DEBUG */
#endif
#ifndef YY_OdlParserBase_LSP_NEEDED

/* #line 70 "D:\\GitHub\\odl\\extern\\flexppbisonpp\\bison.h" */
#line 126 "OdlGenParser.h"

#line 70 "D:\\GitHub\\odl\\extern\\flexppbisonpp\\bison.h"
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

/* TOKEN C */
#ifndef YY_USE_CLASS

#ifndef YY_OdlParserBase_PURE
extern YY_OdlParserBase_STYPE YY_OdlParserBase_LVAL;
#endif


/* #line 143 "D:\\GitHub\\odl\\extern\\flexppbisonpp\\bison.h" */
#line 204 "OdlGenParser.h"
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
#define	TOKEN_NULLPTR	270
#define	OPERATOR_PLUS	271
#define	OPERATOR_MINUS	272
#define	OPERATOR_MULTIPLY	273
#define	OPERATOR_DIVIDE	274
#define	OPERATOR_MODULO	275
#define	VALUE_INTEGER	276
#define	VALUE_FLOAT	277
#define	VALUE_STRING	278
#define	IDENTIFIER	279


#line 143 "D:\\GitHub\\odl\\extern\\flexppbisonpp\\bison.h"
 /* #defines token */
/* after #define tokens, before const tokens S5*/
#else
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

/* #line 182 "D:\\GitHub\\odl\\extern\\flexppbisonpp\\bison.h" */
#line 271 "OdlGenParser.h"
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
static const int TOKEN_NULLPTR;
static const int OPERATOR_PLUS;
static const int OPERATOR_MINUS;
static const int OPERATOR_MULTIPLY;
static const int OPERATOR_DIVIDE;
static const int OPERATOR_MODULO;
static const int VALUE_INTEGER;
static const int VALUE_FLOAT;
static const int VALUE_STRING;
static const int IDENTIFIER;


#line 182 "D:\\GitHub\\odl\\extern\\flexppbisonpp\\bison.h"
 /* decl const */
#else
enum YY_OdlParserBase_ENUM_TOKEN { YY_OdlParserBase_NULL_TOKEN=0

/* #line 185 "D:\\GitHub\\odl\\extern\\flexppbisonpp\\bison.h" */
#line 302 "OdlGenParser.h"
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
	,TOKEN_NULLPTR=270
	,OPERATOR_PLUS=271
	,OPERATOR_MINUS=272
	,OPERATOR_MULTIPLY=273
	,OPERATOR_DIVIDE=274
	,OPERATOR_MODULO=275
	,VALUE_INTEGER=276
	,VALUE_FLOAT=277
	,VALUE_STRING=278
	,IDENTIFIER=279


#line 185 "D:\\GitHub\\odl\\extern\\flexppbisonpp\\bison.h"
 /* enum token */
     }; /* end of enum declaration */
#endif
public:
 int YY_OdlParserBase_PARSE(YY_OdlParserBase_PARSE_PARAM);
 virtual void YY_OdlParserBase_ERROR(char *msg) YY_OdlParserBase_ERROR_BODY;
#ifdef YY_OdlParserBase_PURE
#ifdef YY_OdlParserBase_LSP_NEEDED
 virtual int  YY_OdlParserBase_LEX(YY_OdlParserBase_STYPE *YY_OdlParserBase_LVAL,YY_OdlParserBase_LTYPE *YY_OdlParserBase_LLOC) YY_OdlParserBase_LEX_BODY;
#else
 virtual int  YY_OdlParserBase_LEX(YY_OdlParserBase_STYPE *YY_OdlParserBase_LVAL) YY_OdlParserBase_LEX_BODY;
#endif
#else
 virtual int YY_OdlParserBase_LEX() YY_OdlParserBase_LEX_BODY;
 YY_OdlParserBase_STYPE YY_OdlParserBase_LVAL;
#ifdef YY_OdlParserBase_LSP_NEEDED
 YY_OdlParserBase_LTYPE YY_OdlParserBase_LLOC;
#endif
 int YY_OdlParserBase_NERRS;
 int YY_OdlParserBase_CHAR;
#endif
#if YY_OdlParserBase_DEBUG != 0
public:
 int YY_OdlParserBase_DEBUG_FLAG;	/*  nonzero means print parse trace	*/
#endif
public:
 YY_OdlParserBase_CLASS(YY_OdlParserBase_CONSTRUCTOR_PARAM);
public:
 YY_OdlParserBase_MEMBERS 
};
/* other declare folow */
#endif


#if YY_OdlParserBase_COMPATIBILITY != 0
/* backward compatibility */
#ifndef YYSTYPE
#define YYSTYPE YY_OdlParserBase_STYPE
#endif

#ifndef YYLTYPE
#define YYLTYPE YY_OdlParserBase_LTYPE
#endif
#ifndef YYDEBUG
#ifdef YY_OdlParserBase_DEBUG 
#define YYDEBUG YY_OdlParserBase_DEBUG
#endif
#endif

#endif
/* END */

/* #line 236 "D:\\GitHub\\odl\\extern\\flexppbisonpp\\bison.h" */
#line 381 "OdlGenParser.h"
#endif
