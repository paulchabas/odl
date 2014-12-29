#pragma once


#include "vodlParserLexer/OdlAstNode.h"
#include "vodlParserLexer/OdlTokenDatabase.h"

namespace odl
{
//-------------------------------------------------------------------------------
//*******************************************************************************
//-------------------------------------------------------------------------------
typedef std::vector< TOdlAstNodeNamedDeclaration const* > TNamedDeclarationStack;
//-------------------------------------------------------------------------------
//*******************************************************************************
//-------------------------------------------------------------------------------
class TInterpretContext
{
public:
    TInterpretContext(TOdlDatabasePath& parDatabasePath, TNamedDeclarationStack& parDynamicNamespaceStack);

    void EnterNamespace(TOdlAstNodeNamedDeclaration const* parNamedDeclaration);
    void LeaveNamespace(TOdlAstNodeNamedDeclaration const* parNamedDeclaration);

    void EnterTemplateObjectInstanciation(TOdlAstNodeNamedDeclaration const* parTemplateObjectInstanciation);
    void LeaveTemplateObjectInstanciation(TOdlAstNodeNamedDeclaration const* parTemplateObjectInstanciation);

    TOdlDatabasePath& DatabasePath() { return FDatabasePath; }
    TOdlDatabasePath const& DatabasePath() const { return FDatabasePath; }
    
    TNamedDeclarationStack& StaticNamespaceStack() { return FStaticNamespaceStack; }
    TNamedDeclarationStack& DynamicNamespaceStack() { return FDynamicNamespaceStack; }

    TOdlAstNodeExpression const* FindTemplateInstanciationExpressionFromTemplatetDeclarationAndParameterIndexAssumeExists(TOdlAstNodeNamedDeclaration const* parNamedDeclarationOfTemplateDeclaration, size_t parExpressionIndex) const;

private:
    TNamedDeclarationStack FStaticNamespaceStack;         // static declaration stack, used for identifier resolution in scopes.
    TOdlDatabasePath& FDatabasePath;
    TNamedDeclarationStack& FDynamicNamespaceStack; // template instanciation stack to get the final OdlDatabase::Instance().GetObject address.
};
//-------------------------------------------------------------------------------
//*******************************************************************************
//-------------------------------------------------------------------------------
TOdlAstNodeNamedDeclaration* ResolveIdentifier(TInterpretContext& parContext, TOdlAstNodeIdentifier const* identifierNode);
//-------------------------------------------------------------------------------
//*******************************************************************************
//-------------------------------------------------------------------------------
}


