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
    TInterpretContext(TOdlDatabasePath& parDatabasePath,
                      TOdlNamedDeclarationStack& parStaticNamespaceStack,
                      TOdlNamedDeclarationStack& parDynamicNamespaceStack);

    void EnterNamespace(TOdlAstNodeNamedDeclaration const* parNamedDeclaration);
    void LeaveNamespace(TOdlAstNodeNamedDeclaration const* parNamedDeclaration);

    void EnterTemplateObjectInstanciation(TOdlAstNodeNamedDeclaration const* parTemplateObjectInstanciation);
    void LeaveTemplateObjectInstanciation(TOdlAstNodeNamedDeclaration const* parTemplateObjectInstanciation);

    TOdlDatabasePath& DatabasePath() { return FDatabasePath; }
    TOdlDatabasePath const& DatabasePath() const { return FDatabasePath; }
    
    TOdlNamedDeclarationStack& StaticNamespaceStack() { return FStaticNamespaceStack; }
    TOdlNamedDeclarationStack& DynamicNamespaceStack() { return FDynamicNamespaceStack; }

    TOdlAstNodeExpression const* FindTemplateInstanciationExpressionFromTemplateDeclarationAndParameterIndexAssumeExists(TOdlAstNodeNamedDeclaration const* parNamedDeclarationOfTemplateDeclaration, size_t parExpressionIndex) const;
    
    TOdlAstNodeNamedDeclaration const* FindTemplateDeclarationNamedDeclarationFromTemplateInstanceTargetIdentifier(TOdlAstNodeIdentifier const* parSearchedTemplateDeclarationIdentifier) const;

private:
    TOdlDatabasePath& FDatabasePath;
    TOdlNamedDeclarationStack& FStaticNamespaceStack;  // static declaration stack, used for identifier resolution in scopes.
    TOdlNamedDeclarationStack& FDynamicNamespaceStack; // template instanciation stack to get the final OdlDatabase::Instance().GetObject address.
};
//-------------------------------------------------------------------------------
//*******************************************************************************
//-------------------------------------------------------------------------------
TOdlAstNodeNamedDeclaration* ResolveIdentifier(TInterpretContext& parContext, TOdlAstNodeIdentifier const* identifierNode);
//-------------------------------------------------------------------------------
//*******************************************************************************
//-------------------------------------------------------------------------------
}


