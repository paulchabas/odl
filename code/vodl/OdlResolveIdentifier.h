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
class TTemplateInstanciationStack
{
public:
    TTemplateInstanciationStack();

    void EnterTemplateObjectInstanciation(TOdlAstNodeNamedDeclaration const* parTemplateObjectInstanciation);
    void LeaveTemplateObjectInstanciation(TOdlAstNodeNamedDeclaration const* parTemplateObjectInstanciation);

    TOdlAstNodeExpression const* FindTemplateInstanciationExpressionFromTemplatetDeclarationAndParameterIndexAssumeExists(TOdlAstNodeNamedDeclaration const* parNamedDeclarationOfTemplateDeclaration, size_t parExpressionIndex) const;

    bool Empty() const { return FTemplateInstanciations.empty(); }

    // Paul(2014/12/28)  rename/concept -> 
    // 1) path statique -> recherche de parametres ou d'autre objets
    // 2) path interprete.
    TNamedDeclarationStack const& TemplateInstanciations() const { return FTemplateInstanciations; }

private:
    TNamedDeclarationStack FTemplateInstanciations;
};
//-------------------------------------------------------------------------------
//*******************************************************************************
//-------------------------------------------------------------------------------
class TInterpretContext
{
public:
    TInterpretContext(TOdlDatabasePath& parDatabasePath, TTemplateInstanciationStack& parTemplateInstanciationStack);

    void EnterNamespace(TOdlAstNodeNamedDeclaration const* parNamedDeclaration);
    void LeaveNamespace(TOdlAstNodeNamedDeclaration const* parNamedDeclaration);

    void EnterTemplateObjectInstanciation(TOdlAstNodeNamedDeclaration const* parTemplateObjectInstanciation);
    void LeaveTemplateObjectInstanciation(TOdlAstNodeNamedDeclaration const* parTemplateObjectInstanciation);

    TOdlDatabasePath& DatabasePath() { return FDatabasePath; }
    TOdlDatabasePath const& DatabasePath() const { return FDatabasePath; }
    
    TNamedDeclarationStack const& StaticNamespaceStack() const { return FNamespaceStack; }
    TNamedDeclarationStack const& DynamicNamespaceStack() const { return FTemplateInstanciationStack.TemplateInstanciations(); }

    TTemplateInstanciationStack& TemplateInstanciationNodeStack() { return FTemplateInstanciationStack; }
    
private:
    TNamedDeclarationStack FNamespaceStack;
    TOdlDatabasePath& FDatabasePath;
    TTemplateInstanciationStack& FTemplateInstanciationStack;
};
//-------------------------------------------------------------------------------
//*******************************************************************************
//-------------------------------------------------------------------------------
TOdlAstNodeNamedDeclaration* ResolveIdentifier(TInterpretContext& parContext, TOdlAstNodeIdentifier const* identifierNode);
//-------------------------------------------------------------------------------
//*******************************************************************************
//-------------------------------------------------------------------------------
}


