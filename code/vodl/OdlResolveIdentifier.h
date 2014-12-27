#pragma once


#include "vodlParserLexer/OdlAstNode.h"
#include "vodlParserLexer/OdlTokenDatabase.h"

namespace odl
{
//-------------------------------------------------------------------------------
//*******************************************************************************
//-------------------------------------------------------------------------------
class TInterpretContext
{
public:
    TInterpretContext(TOdlDatabasePath& parDatabasePath) :
        FDatabasePath(parDatabasePath)
    {
    }

    void EnterNamespace(std::string const& parNamespaceName)
    {
        FDatabasePath.push_back(parNamespaceName);
    }

    void LeaveNamespace()
    {
        FDatabasePath.pop_back();
    }

    TOdlDatabasePath& DatabasePath() { return FDatabasePath; }
    TOdlDatabasePath const& DatabasePath() const { return FDatabasePath; }

private:
    TOdlDatabasePath& FDatabasePath;
};
//-------------------------------------------------------------------------------
typedef void (*TAstOperationCallback)(TOdlAstNode* parAstNode, TInterpretContext& parContext);
//-------------------------------------------------------------------------------
//*******************************************************************************
//-------------------------------------------------------------------------------
class TResolveValueIdentifierInterpretContext : public TInterpretContext
{
public:
    TResolveValueIdentifierInterpretContext(TOdlDatabasePath& parOdlDatabasePath) :
        TInterpretContext(parOdlDatabasePath)
    {

    }

    void PushParentNode(TOdlAstNode* parParent)
    {
        FParents.push_back(parParent);
    }

    void PopParentNode()
    {
        FParents.pop_back();
    }

    std::vector< TOdlAstNode* > const& Parents() const { return FParents; }
	
private:
    std::vector< TOdlAstNode* > FParents;
};
//-------------------------------------------------------------------------------
//*******************************************************************************
//-------------------------------------------------------------------------------
TOdlAstNode const* ResolveIdentifier(TInterpretContext& parContext, TOdlAstNodeIdentifier* identifierNode);
//-------------------------------------------------------------------------------
//*******************************************************************************
//-------------------------------------------------------------------------------
}


