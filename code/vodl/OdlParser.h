#ifndef ODL_PARSER_IMPL_H
# define ODL_PARSER_IMPL_H

#include "vOdlParserLexer/OdlGenLexer.h"
#include "vOdlParserLexer/OdlGenParser.h"
#include "OdlAstNode.h"

#include <sstream>

namespace odl
{
//--------------------------------------------------------
//********************************************************
//--------------------------------------------------------
class TOdlParser : public OdlParserBase
{
private:
	OdlScanner scanner;
public:
	TOdlParser();
    ~TOdlParser();

    TOdlAstNode const* ParseFile(char const* parFilename);
    void InterpretAst();

    void PrettyPrint(std::ostringstream& parOss) const;

protected:
	virtual void yyerror(char * msg);
	virtual int yylex();

private:
    TOdlAstNode* FRootAstNode;
};
//--------------------------------------------------------
//********************************************************
//--------------------------------------------------------
} // namespace odl

#endif