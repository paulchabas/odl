#pragma once


#include "vodlParserLexer/OdlAstNode.h"
#include "vodlParserLexer/OdlTokenDatabase.h"

namespace odl
{
//-------------------------------------------------------------------------------
//*******************************************************************************
//-------------------------------------------------------------------------------
class TNamedDeclarationStack
{
public:
    TNamedDeclarationStack()
    {

    }

    void Push(TOdlAstNodeNamedDeclaration const* parNamedDeclaration)
    {
        FStack.push_back(parNamedDeclaration);
    }

    void Pop(TOdlAstNodeNamedDeclaration const* parNamedDeclaration)
    {
        assert(!FStack.empty());
        assert(FStack.back() == parNamedDeclaration);
        FStack.pop_back();
    }

    bool Empty() const { return FStack.empty(); }

    size_t Size() const { return FStack.size(); }

    TOdlAstNodeNamedDeclaration const* operator [] (int parIndex) const
    {
        return FStack[parIndex];
    }

private:
    std::vector< TOdlAstNodeNamedDeclaration const* > FStack;
};
//-------------------------------------------------------------------------------
//*******************************************************************************
//-------------------------------------------------------------------------------
class TInterpretContext
{
public:
    TInterpretContext(TOdlDatabasePath& parDatabasePath,
                      TNamedDeclarationStack& parStaticNamespaceStack,
                      TNamedDeclarationStack& parDynamicNamespaceStack);

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
    TOdlDatabasePath& FDatabasePath;
    TNamedDeclarationStack& FStaticNamespaceStack;  // static declaration stack, used for identifier resolution in scopes.
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


