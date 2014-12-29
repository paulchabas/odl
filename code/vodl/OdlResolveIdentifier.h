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
    TInterpretContext(TOdlDatabasePath& parDatabasePath, TTemplateInstanciationStack& parTemplateInstanciationStack) :
        FDatabasePath(parDatabasePath),
        FTemplateInstanciationStack(parTemplateInstanciationStack)
    {
    }


    void EnterNamespace(TOdlAstNodeNamedDeclaration const* parNamedDeclaration)
    {
        assert(parNamedDeclaration->AstNodeType() == TOdlAstNodeType::NAMESPACE ||
               parNamedDeclaration->AstNodeType() == TOdlAstNodeType::NAMED_DECLARATION);

        TOdlAstNodeIdentifier const* identifier = parNamedDeclaration->IdentifierPointer_IFP();
        // root namespace has no name.
        if (identifier == nullptr)
        {
            // intend to detect root namespace...
            assert(FNamespaceStack.empty());
        }
        else
        {
            FDatabasePath.push_back(identifier->Identifier());
        }
        FNamespaceStack.push_back(parNamedDeclaration);
    }

    void LeaveNamespace()
    {
        TOdlAstNodeNamedDeclaration const* namedDeclaration = FNamespaceStack.back();
        TOdlAstNodeIdentifier const* identifier = namedDeclaration->IdentifierPointer_IFP();
        if (identifier != nullptr)
        {
            assert(FDatabasePath.back() == TOdlDatabaseToken(identifier->Identifier()));
            FDatabasePath.pop_back();
        }
        FNamespaceStack.pop_back();
    }


    TOdlDatabasePath& DatabasePath() { return FDatabasePath; }
    TOdlDatabasePath const& DatabasePath() const { return FDatabasePath; }
    
    TNamedDeclarationStack const& StaticNamespaceStack() const { return FNamespaceStack; }
    TNamedDeclarationStack const& InterpretedNamespaceStack() const { return FTemplateInstanciationStack.TemplateInstanciations(); }

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


