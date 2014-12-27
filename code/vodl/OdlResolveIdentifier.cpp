#include "stdafx.h"
#include "OdlResolveIdentifier.h"



namespace odl
{
//-------------------------------------------------------------------------------
//*******************************************************************************
//-------------------------------------------------------------------------------
TOdlAstNode const* ResolveIdentifier(TInterpretContext& parContext, TOdlAstNodeIdentifier* identifierNode)
{
    TOdlAstNode const* foundReference = nullptr;

    TResolveValueIdentifierInterpretContext const& context = static_cast<TResolveValueIdentifierInterpretContext const&>(parContext);
    std::vector< TOdlAstNode* > const& parentNamespaces = context.Parents();
    size_t const parentNamespaceCount = parentNamespaces.size();

    std::string const& fullIdentifierToResolve = identifierNode->Identifier();

    TOdlDatabasePath searchedNameSpaceDatabasePath(fullIdentifierToResolve.c_str());
    if (searchedNameSpaceDatabasePath.size() <= 1)
    {
        // research name resolution in parent namespaces recursively
        std::string const& identifierToResolve = searchedNameSpaceDatabasePath.back().ToString();

        for (size_t i = 0; i < parentNamespaceCount && (foundReference == nullptr); ++i)
        {
            size_t const invI = parentNamespaceCount - i - 1;

            TOdlAstNode* namespaceCandidate = parentNamespaces[invI];

            // PAUL(14/10/2014) property filled in case of namespace.
            if (namespaceCandidate->AstNodeType() == TOdlAstNodeType::NAMESPACE)
            {
                TOdlAstNodeNamespaceDeclaration const* typedNamespaceCandidate = namespaceCandidate->CastNode<TOdlAstNodeNamespaceDeclaration>();
                std::vector< TOdlAstNodeNamedDeclaration* > const& namespaceContent = typedNamespaceCandidate->NamespaceContent();
                for (size_t j = 0; j < namespaceContent.size(); ++j)
                {
                    TOdlAstNodeNamedDeclaration* candidateNode = namespaceContent[j];
                    if (candidateNode->AstNodeType() == TOdlAstNodeType::NAMED_DECLARATION ||
                        candidateNode->AstNodeType() == TOdlAstNodeType::TEMPLATE_OBJECT_DECLARATION)
                    {
                        std::string const& declarationIdentifier = candidateNode->IdentifierPointer()->Identifier();
                        if (declarationIdentifier == identifierToResolve)
                        {
#if ODL_ENABLE_VERBOSE_DEBUG
                            std::string pathDebug = context.DatabasePath().ToString();
#endif
                            identifierNode->ResolveReference(candidateNode);
                            foundReference = candidateNode;
                            break;
                        }
                    }
                }
            }
            else if (namespaceCandidate->AstNodeType() == TOdlAstNodeType::TEMPLATE_OBJECT_DECLARATION)
            {
                TOdlAstNodeTemplateObjectDeclaration* templateObjectDeclarationNode = namespaceCandidate->CastNode<TOdlAstNodeTemplateObjectDeclaration>();

                std::vector< TOdlAstNodeTemplateParameter* > const& templateParameters = templateObjectDeclarationNode->TemplateParameterListPointer()->TemplateParameterList();
                for (size_t i = 0; i < templateParameters.size(); ++i)
                {
                    TOdlAstNodeTemplateParameter const* templateParameter = templateParameters[i];
                    std::string const& parameterName = templateParameter->IdentifierPointer()->Identifier();
                    if (parameterName == identifierToResolve)
                    {
#if ODL_ENABLE_VERBOSE_DEBUG
                        std::string pathDebug = context.DatabasePath().ToString();
#endif
                        identifierNode->ResolveReference(templateParameter);
                        foundReference = templateParameter;
                        break;
                    }
                }
            }
        }
    }
    else
    {
        // research in one namespace
        // 1) search root namespace
        std::string const rootNamespaceToFind = searchedNameSpaceDatabasePath[0].ToString();
        TOdlAstNode const* rootNamespace = nullptr;
        {
            for (size_t i = 0; i < parentNamespaceCount; ++i)
            {
                size_t const invI = parentNamespaceCount - i - 1;
                TOdlAstNode const* rootNamespaceCandidate = parentNamespaces[invI];

                if (rootNamespaceCandidate->AstNodeType() == TOdlAstNodeType::NAMESPACE)
                {
                    TOdlAstNodeNamespaceDeclaration const* typedRootNamespaceCandidate = rootNamespaceCandidate->CastNode<TOdlAstNodeNamespaceDeclaration>();
                    std::vector< TOdlAstNodeNamedDeclaration* > const& rootNamespaceContent = typedRootNamespaceCandidate->NamespaceContent();
                    for (size_t j = 0; j < rootNamespaceContent.size(); ++j)
                    {
                        TOdlAstNodeNamedDeclaration* namedDeclarationNode = rootNamespaceContent[j];
                        std::string const& namedDeclarationName = namedDeclarationNode->IdentifierPointer()->Identifier();

                        if (namedDeclarationName == rootNamespaceToFind)
                        {
                            rootNamespace = rootNamespaceCandidate;
                            goto rootNamespaceDone;
                        }
                    }
                }
                //else
                //{
                //    // {TODO} Paul(2014/12/21) est-ce que c'est une erreur ou pas ? gerer correctement les named values.
                //    assert(false);
                //}
            }
        rootNamespaceDone:
            int a = 0;
        }

        if (rootNamespace != nullptr)
        {
            // 2) search childs namespace of root namespace.
            TOdlAstNode const* childNamespace = rootNamespace;
            {
                for (int i = 0; i < (int)searchedNameSpaceDatabasePath.size() - 1; ++i)
                {
                    std::string const& searchedChildNamespace = searchedNameSpaceDatabasePath[i].ToString();

                    bool childNamespaceFound = false;

                    if (childNamespace->AstNodeType() == TOdlAstNodeType::NAMESPACE)
                    {
                        TOdlAstNodeNamespaceDeclaration const* childNamespaceDeclaration = childNamespace->CastNode<TOdlAstNodeNamespaceDeclaration>();
                        std::vector< TOdlAstNodeNamedDeclaration* > const& childNamespaceContent = childNamespaceDeclaration->NamespaceContent();
                        for (size_t j = 0; j < childNamespaceContent.size(); ++j)
                        {
                            TOdlAstNodeNamedDeclaration* candidateNamespace = childNamespaceContent[j];
                            if (candidateNamespace->AstNodeType() == TOdlAstNodeType::NAMESPACE ||
                                candidateNamespace->AstNodeType() == TOdlAstNodeType::NAMED_DECLARATION)
                            {
                                std::string const& namedDeclarationIdentifier = candidateNamespace->IdentifierPointer()->Identifier();
                                if (namedDeclarationIdentifier == searchedChildNamespace)
                                {
                                    childNamespace = candidateNamespace;
                                    childNamespaceFound = true;
                                    break;
                                }
                            }
                        }
                    }
                    else
                    {
                        // {TODO} Paul(2014/12/21)  checker si c'est une erreur... gerer correctement les named values et namespace.
                        assert(false);
                    }

                    if (!childNamespaceFound)
                        assert(false); // cannot find tutu in namespace tata for example: toto/tata/tutu/object
                }
            }

            if (childNamespace != nullptr)
            {
                // 3) search for the final named value and resolve.
                TOdlAstNode const* finalNamespace = childNamespace;
                std::string const& identifierToResolve = searchedNameSpaceDatabasePath.back().ToString();

                if (finalNamespace->AstNodeType() == TOdlAstNodeType::NAMESPACE)
                {
                    TOdlAstNodeNamespaceDeclaration const* typedFinalNamespace = finalNamespace->CastNode<TOdlAstNodeNamespaceDeclaration>();
                    std::vector< TOdlAstNodeNamedDeclaration* > const& namespaceContent = typedFinalNamespace->NamespaceContent();
                    for (size_t j = 0; j < namespaceContent.size(); ++j)
                    {
                        TOdlAstNodeNamedDeclaration* candidateNode = namespaceContent[j];
                        std::string const& declarationIdentifier = candidateNode->IdentifierPointer()->Identifier();
                        if (candidateNode->AstNodeType() == TOdlAstNodeType::NAMED_DECLARATION ||
                            candidateNode->AstNodeType() == TOdlAstNodeType::TEMPLATE_OBJECT_DECLARATION)
                        {
                            if (identifierToResolve == declarationIdentifier)
                            {
#if ODL_ENABLE_VERBOSE_DEBUG
                                std::string pathDebug = context.DatabasePath().ToString();
#endif
                                identifierNode->ResolveReference(candidateNode);
                                foundReference = candidateNode;
                                break;
                            }
                        }
                    }
                }
                else
                {
                    // {TODO} Paul(2014/12/21)  check named value et namespace... est-ce que c'est une erreur ou pas ?
                    assert(false);
                }

            }
            else
            {
                assert(false); // namespace not found.
            }
        }
        else
        {
            // unrecognized namespace
            assert(false);
        }
    }
    return foundReference;
}
//-------------------------------------------------------------------------------
//*******************************************************************************
//-------------------------------------------------------------------------------
} // namespace odl

