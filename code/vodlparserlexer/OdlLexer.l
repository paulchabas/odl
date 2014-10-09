%name OdlScanner
%define LEX_PARAM YY_OdlParserBase_STYPE *val, YY_OdlParserBase_LTYPE *loc
%define MEMBERS public: int line, column;
%define CONSTRUCTOR_INIT : line(1), column(1)



%header{

#ifdef _DEBUG
   #ifndef DBG_NEW
      #define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
      #define new DBG_NEW
   #endif
#endif  // _DEBUG

#include <iostream>
#include<sstream>
#include <cassert>
#include "OdlGenParser.h"
%}
 
DIGIT   [0-9]
DIGIT1  [1-9]

IDENTIFIER [a-zA-Z][a-zA-Z0-9]*
VALUE_STRING ["][^"]*["]
VALUE_INTEGER {DIGIT1}{DIGIT}*|[0]
LINE_COMMENT "//".*\n
%%
 
" "               {
                    ++column;
                  }
"\r\n"			  {
					line +=1;
					column = 1;
				  }
"\n"			  {
					line +=1;
					column = 1;
				  }
"\t"              {
                    column += 4;
                  }
{LINE_COMMENT}    {
					 line +=1;
					 column = 1;					
				  }
":"				  {
					column += 1;
					return OdlParserBase::TOKEN_COLON;
				  }
"+"               {
                    ++column;
					val->FAstNode = new odl::TOdlAstNode();
					val->FAstNode->SetAsOperator(odl::TOdlAstNodeOperatorType::OPERATOR_PLUS);
                    return OdlParserBase::OPERATOR_PLUS;
                  }
"-"               {
                    ++column;
					val->FAstNode = new odl::TOdlAstNode();
					val->FAstNode->SetAsOperator(odl::TOdlAstNodeOperatorType::OPERATOR_MINUS);
                    return OdlParserBase::OPERATOR_MINUS;
                  }
"*"               {
                    ++column;
					val->FAstNode = new odl::TOdlAstNode();
					val->FAstNode->SetAsOperator(odl::TOdlAstNodeOperatorType::OPERATOR_MULTIPLY);
                    return OdlParserBase::OPERATOR_MULTIPLY;
                  }
"/"               {
                    ++column;
					val->FAstNode = new odl::TOdlAstNode();
					val->FAstNode->SetAsOperator(odl::TOdlAstNodeOperatorType::OPERATOR_DIVIDE);
                    return OdlParserBase::OPERATOR_DIVIDE;
                  }
"%"				  {
                    ++column;
					val->FAstNode = new odl::TOdlAstNode();
					val->FAstNode->SetAsOperator(odl::TOdlAstNodeOperatorType::OPERATOR_MODULO);
                    return OdlParserBase::OPERATOR_MODULO;						
				  }

"="               {
                    ++column;
                    return OdlParserBase::TOKEN_EQUALS;
                  }
"is"			  {
					column += 2;
					return OdlParserBase::TOKEN_IS;
				  }
"namespace"			{
						column += 9;
						return OdlParserBase::TOKEN_NAMESPACE;						
					}
"template"			{
						column += 8;
						return OdlParserBase::TOKEN_TEMPLATE;
					}

"{"					{
						++column;
						return OdlParserBase::TOKEN_OPEN_BRACE;
					}
"}"					{
						++column;
						return OdlParserBase::TOKEN_CLOSE_BRACE;
					}
"["					{
						++column;
						return OdlParserBase::TOKEN_OPEN_BRACKET;
					}
"]"					{
						++column;
						return OdlParserBase::TOKEN_CLOSE_BRACKET;
					}
"("					{
						++column;
						return OdlParserBase::TOKEN_OPEN_PARENTHESIS;
					}
")"					{
						++column;
						return OdlParserBase::TOKEN_CLOSE_PARENTHESIS;
					}
","					{
						++column;
						return OdlParserBase::TOKEN_COMMA;
					}

{VALUE_STRING}		{
						int const textLength = strlen(yytext);
						column += textLength;

						// remove quotes.
						assert(textLength >= 2);
						val->FAstNode = new odl::TOdlAstNode();
						val->FAstNode->SetAsStringValue(std::string(yytext + 1, yytext + textLength - 1));
						return OdlParserBase::VALUE_STRING;
					}
{VALUE_INTEGER}   {
                    column += strlen(yytext);
					int integerValue = atoi(yytext);
                    val->FAstNode = new odl::TOdlAstNode();
					val->FAstNode->SetAsIntegerValue(integerValue);

                    return OdlParserBase::VALUE_INTEGER;
                  }
{IDENTIFIER}	  {
						int const textLength = strlen(yytext);
						column += textLength;
						
						val->FAstNode = new odl::TOdlAstNode();
						val->FAstNode->SetAsIdentifier(std::string(yytext));
						return OdlParserBase::IDENTIFIER;
				  }
<<EOF>>           {
                    yyterminate();
                  }

.				  {
					 int const textLength = strlen(yytext);
					 ++column;
					 yyterminate();
				  }
 
%%