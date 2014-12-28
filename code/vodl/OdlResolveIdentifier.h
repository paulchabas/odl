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
class TInterpretContext
{
public:
    TInterpretContext(TOdlDatabasePath& parDatabasePath) :
        FDatabasePath(parDatabasePath)
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
    TNamedDeclarationStack const& NamespaceStack() const { return FNamespaceStack; }

private:
    TNamedDeclarationStack FNamespaceStack;
    TOdlDatabasePath& FDatabasePath;
};
//-------------------------------------------------------------------------------
//*******************************************************************************
//-------------------------------------------------------------------------------
TOdlAstNodeNamedDeclaration* ResolveIdentifier(TInterpretContext& parContext, TOdlAstNodeIdentifier const* identifierNode);
//-------------------------------------------------------------------------------
//*******************************************************************************
//-------------------------------------------------------------------------------
}


