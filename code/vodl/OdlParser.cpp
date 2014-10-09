#include "stdafx.h"
#include "OdlParser.h"

#include "OdlInterpreter.h"

namespace odl
{
//--------------------------------------------------------
//********************************************************
//--------------------------------------------------------
TOdlParser::TOdlParser() :
    FRootAstNode(NULL)
{
}
//-------------------------------------------------------------------------------
TOdlParser::~TOdlParser()
{
    delete FRootAstNode;
}
//-------------------------------------------------------- 
TOdlAstNode const* TOdlParser::ParseFile(char const* parFilename)
{
    if (FRootAstNode != nullptr)
    {
        delete FRootAstNode;
        FRootAstNode = nullptr;
    }

	FILE* file = nullptr;
	fopen_s(&file, parFilename, "rt");
	if (file != NULL)
	{
		this->scanner.yyin = file;

        TOdlAstNode* rootNode = nullptr;

		int const parseResult = yyparse(rootNode);
        if (parseResult != 1)
        {
            FRootAstNode = rootNode;
        }
        else
        {
            delete rootNode;
        }

		fclose(file);
	}

    if (FRootAstNode != nullptr)
    {
        // Ast - Post Process
        PostProcessAst(FRootAstNode);
    }

	return FRootAstNode;
}
//-------------------------------------------------------- 
void TOdlParser::InterpretAst()
{
    if (FRootAstNode != nullptr)
    {
        odl::InterpretAst(FRootAstNode);
    }
}
//-------------------------------------------------------- 
void TOdlParser::yyerror(char * msg)
{
	std::cout << yylloc.first_line << ":" << yylloc.first_column <<  ": "
		<< msg << " : <" << yylloc.text << ">" <<  std::endl;
}
//--------------------------------------------------------
int TOdlParser::yylex()
{
	yylloc.first_line = scanner.line;
	yylloc.first_column = scanner.column;
	int token = scanner.yylex(&yylval, &yylloc);
	yylloc.last_line = scanner.line;
	yylloc.last_column = scanner.column;
	yylloc.text = (char*) scanner.yytext;
	return token;
}
//-------------------------------------------------------------------------------
void TOdlParser::PrettyPrint(std::ostringstream& parOss) const
{
    if (FRootAstNode != nullptr)
    {
        FRootAstNode->PrettyPrint(parOss);
    }
}
//--------------------------------------------------------
//********************************************************
//--------------------------------------------------------
} // namespace odl