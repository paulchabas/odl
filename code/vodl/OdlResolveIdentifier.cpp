#include "stdafx.h"
#include "OdlResolveIdentifier.h"



namespace odl
{
//-------------------------------------------------------------------------------
//*******************************************************************************
//-------------------------------------------------------------------------------
static TOdlAstNodeNamedDeclaration* FindTemplateParameter(TOdlAstNodeTemplateParameterList const* templateParameterListPointer, 
                                                          std::string const& identifierToResolve)
{
    assert(templateParameterListPointer != nullptr);
    TOdlAstNodeNamedDeclaration* foundReference = nullptr;

    std::vector< TOdlAstNodeTemplateParameter* > const& templateParameters = templateParameterListPointer->TemplateParameterList();
    for (size_t i = 0; i < templateParameters.size(); ++i)
    {
        TOdlAstNodeTemplateParameter* templateParameter = templateParameters[i];
        std::string const& parameterName = templateParameter->IdentifierPointer()->Identifier();
        if (parameterName == identifierToResolve)
        {
            foundReference = templateParameter;
            break;
        }
    }	

    return foundReference;
}
//-------------------------------------------------------------------------------
//*******************************************************************************
//-------------------------------------------------------------------------------
static TOdlAstNodeNamedDeclaration* FindIdentifierInNamespace(TInterpretContext const& parContext,
                                                              TOdlAstNodeNamedDeclaration const* parNamespaceCandidate,
                                                              std::string const& parIdentifierToResolve,
                                                              bool parSearchInTemplateParameters)
{
    TOdlAstNodeNamedDeclaration* foundReference = nullptr;

    if (parNamespaceCandidate->AstNodeType() == TOdlAstNodeType::NAMED_DECLARATION)
    {
        if (parSearchInTemplateParameters)
        {
            TOdlAstNodeExpression const* expression = parNamespaceCandidate->ExpressionPointer();
            if (expression->AstNodeType() == TOdlAstNodeType::TEMPLATE_OBJECT_DECLARATION)
            {
                #if ODL_ENABLE_VERBOSE_DEBUG
                TOdlAstNodeTemplateObjectDeclaration const* templateObjectDeclarationNode = expression->CastNode<TOdlAstNodeTemplateObjectDeclaration>();
                std::string const& pathDebug = parContext.DatabasePath().ToString();
                #endif

                TOdlAstNodeTemplateParameterList const* templateParameterListPointer = parNamespaceCandidate->TemplateParameterListPointer();
                foundReference = FindTemplateParameter(templateParameterListPointer, parIdentifierToResolve);
            }
            else if (expression->AstNodeType())
            {
                int a = 0;
            }
        }
    }
    else if (parNamespaceCandidate->AstNodeType() == TOdlAstNodeType::NAMESPACE)
    {
        TOdlAstNodeNamespaceDeclaration const* typedNamespaceCandidate = parNamespaceCandidate->CastNode<TOdlAstNodeNamespaceDeclaration>();
        #if ODL_ENABLE_VERBOSE_DEBUG
        std::string pathDebug = parContext.DatabasePath().ToString();
        #endif

        // search in namespace content first.
        std::vector< TOdlAstNodeNamedDeclaration* > const& namespaceContent = typedNamespaceCandidate->NamespaceContent();
        for (size_t j = 0; j < namespaceContent.size(); ++j)
        {
            TOdlAstNodeNamedDeclaration* candidateNode = namespaceContent[j];

            std::string const& declarationIdentifier = candidateNode->IdentifierPointer()->Identifier();
            if (declarationIdentifier == parIdentifierToResolve)
            {
                foundReference = candidateNode;
                break;
            }
        }

        // search in template namespace parameter if possible.
        if (foundReference == nullptr)
        {
            if (parSearchInTemplateParameters)
            {
                TOdlAstNodeTemplateParameterList const* templateParameterListPointer = typedNamespaceCandidate->TemplateParameterListPointer();
                if (templateParameterListPointer != nullptr)
                {
                    TOdlAstNodeNamedDeclaration* result = FindTemplateParameter(templateParameterListPointer, parIdentifierToResolve);
                    if (result != nullptr)
                    {
                        foundReference = result;
                    }
                }
                else
                {
                    assert(typedNamespaceCandidate->AstNodeType() == TOdlAstNodeType::NAMESPACE);
                    assert(!typedNamespaceCandidate->IsTemplate());
                }
            }
        }
    }
    
    return foundReference;
}
//-------------------------------------------------------------------------------
//*******************************************************************************
//-------------------------------------------------------------------------------
TOdlAstNodeNamedDeclaration const* FindNamespace(TNamedDeclarationStack const& parParentNamespaces, std::string const& parRootNamespaceToFind)
{
    TOdlAstNodeNamedDeclaration const* rootNamespace = nullptr;

    size_t const parentNamespaceCount = parParentNamespaces.size();
    for (size_t i = 0; i < parentNamespaceCount; ++i)
    {
        size_t const invI = parentNamespaceCount - i - 1;
        TOdlAstNodeNamedDeclaration const* rootNamespaceCandidate = parParentNamespaces[invI];

        if (rootNamespaceCandidate->AstNodeType() == TOdlAstNodeType::NAMESPACE)
        {
            TOdlAstNodeNamespaceDeclaration const* typedRootNamespaceCandidate = rootNamespaceCandidate->CastNode<TOdlAstNodeNamespaceDeclaration>();
            
            std::vector< TOdlAstNodeNamedDeclaration* > const& rootNamespaceContent = typedRootNamespaceCandidate->NamespaceContent();
            for (size_t j = 0; j < rootNamespaceContent.size(); ++j)
            {
                TOdlAstNodeNamedDeclaration* namedDeclarationNode = rootNamespaceContent[j];
                std::string const& namedDeclarationName = namedDeclarationNode->IdentifierPointer()->Identifier();

                if (namedDeclarationName == parRootNamespaceToFind)
                {
                    rootNamespace = rootNamespaceCandidate;
                    goto rootNamespaceDone;
                }
            }
        }
    }

rootNamespaceDone:
    return rootNamespace;
}
//-------------------------------------------------------------------------------
//*******************************************************************************
//-------------------------------------------------------------------------------
TOdlAstNodeNamedDeclaration* ResolveIdentifier(TInterpretContext& parContext, TOdlAstNodeIdentifier const* identifierNode)
{
    TOdlAstNodeNamedDeclaration* foundReference = nullptr;

    TNamedDeclarationStack const& parentNamespaces = parContext.StaticNamespaceStack();

    size_t const parentNamespaceCount = parentNamespaces.size();
    std::string const& fullIdentifierToResolve = identifierNode->Identifier();

    #if ODL_ENABLE_VERBOSE_DEBUG
    std::string debug0 = parContext.DatabasePath().ToString();
    #endif

    TOdlDatabasePath searchedNameSpaceDatabasePath(fullIdentifierToResolve.c_str());
    if (searchedNameSpaceDatabasePath.size() <= 1)
    {
        // research name resolution in parent namespaces recursively
        std::string const& identifierToResolve = searchedNameSpaceDatabasePath.back().ToString();

        for (size_t i = 0; i < parentNamespaceCount; ++i)
        {
            size_t const invI = parentNamespaceCount - i - 1;
            TOdlAstNodeNamedDeclaration const* namespaceCandidate = parentNamespaces[invI];
            foundReference = FindIdentifierInNamespace(parContext, namespaceCandidate, identifierToResolve, true);
            if (foundReference != nullptr)
                break ;
        }	
    }
    else
    {
        // research in one namespace
        // 1) search root namespace
        std::string const& rootNamespaceToFind = searchedNameSpaceDatabasePath[0].ToString();
        TOdlAstNodeNamedDeclaration const* rootNamespace = FindNamespace(parentNamespaces, rootNamespaceToFind);
        assert(rootNamespace != nullptr); // path not completed.
        if (rootNamespace != nullptr)
        {
            // 2) search childs namespace of root namespace.
            TOdlAstNodeNamedDeclaration const* childNamespace = rootNamespace;
            {
                for (int i = 0; i < (int)searchedNameSpaceDatabasePath.size() - 1; ++i)
                {
                    std::string const& searchedChildNamespace = searchedNameSpaceDatabasePath[i].ToString();

                    TOdlAstNodeNamedDeclaration const* nextChildNamespace = FindIdentifierInNamespace(parContext, childNamespace, searchedChildNamespace, false);
                    assert(nextChildNamespace != nullptr);

                    // check for template namespace 
                    TOdlAstNodeExpression const* namedDeclarationExpression = nextChildNamespace->ExpressionPointer();
                    if (namedDeclarationExpression != nullptr && namedDeclarationExpression->AstNodeType() == TOdlAstNodeType::TEMPLATE_NAMESPACE_INSTANCIATION)
                    {
                        TOdlAstNodeTemplateNamespaceInstanciation const* templateNamespaceInstanciation = namedDeclarationExpression->CastNode<TOdlAstNodeTemplateNamespaceInstanciation>();

                        TOdlAstNodeIdentifier const* targetNamespaceDeclarationIdentifier = templateNamespaceInstanciation->TargetTemplateNamespaceIdentifierPointer();

                        // {TODO} Paul(2014/12/29) HERE: use the real database storage context maybe instead of the static declaration paths...
                        //// Paul(2014/12/28)  unifier... X(
                        //TInterpretContext newContext(parContext.DatabasePath(), parC

                        TOdlAstNodeNamedDeclaration const* templateNamespaceDeclaration = ResolveIdentifier(parContext, targetNamespaceDeclarationIdentifier);
                        nextChildNamespace = templateNamespaceDeclaration;
                    }

                    childNamespace = nextChildNamespace;
                }
            }

            assert(childNamespace != nullptr); // path not found.
            if (childNamespace != nullptr)
            {
                // 3) search for the final named value and resolve.
                TOdlAstNodeNamedDeclaration const* finalNamespace = childNamespace;
                std::string const& identifierToResolve = searchedNameSpaceDatabasePath.back().ToString();
                foundReference = FindIdentifierInNamespace(parContext, finalNamespace, identifierToResolve, false);
            }
        }
    }

    assert(foundReference != nullptr); // reference not resolved.

    return foundReference;
}
//-------------------------------------------------------------------------------
//*******************************************************************************
//-------------------------------------------------------------------------------
} // namespace odl

