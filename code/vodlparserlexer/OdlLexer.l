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

SLASH "/"
PURE_IDENTIFIER [a-zA-Z_][a-zA-Z0-9_]*
LINK_NAMESPACE {PURE_IDENTIFIER}{SLASH}
IDENTIFIER {LINK_NAMESPACE}*{PURE_IDENTIFIER}

VALUE_STRING ["][^"]*["]
VALUE_PURE_INTEGER {DIGIT1}{DIGIT}*|[0]
VALUE_DECIMAL [.]{DIGIT}*
VALUE_FLOAT_OR_INTEGER {VALUE_PURE_INTEGER}{VALUE_DECIMAL}?
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
"nullptr"			{
						column += 7;
						return OdlParserBase::TOKEN_NULLPTR;
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
{VALUE_FLOAT_OR_INTEGER}	{
								int const characterCount = strlen(yytext);
								column += characterCount;

								bool isFloat = false;
								for (int i = 0; i < characterCount; ++i)
								{
									if (yytext[i] == '.')
									{
										isFloat = true;
										break ;
									}
								}
								if (isFloat)
								{
									float const floatValue = (float) atof(yytext);
									val->FAstNode = new odl::TOdlAstNode();
									val->FAstNode->SetAsFloatValue(floatValue);
									return OdlParserBase::VALUE_FLOAT;
								}
								else
								{
									int integerValue = atoi(yytext);
									val->FAstNode = new odl::TOdlAstNode();
									val->FAstNode->SetAsIntegerValue(integerValue);
									return OdlParserBase::VALUE_INTEGER;
								}
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