#include "stdafx.h"
#include "OdlTemplate.h"


namespace odl
{
//-------------------------------------------------------------------------------
//*******************************************************************************
//-------------------------------------------------------------------------------
TOdlAstNodeTemplateObjectDeclaration const* GetTemplateObjectInstanciationDeclaration(TOdlAstNodeTemplateObjectInstanciation const* parTemplateObjectInstanciationNode)
{
    assert(parTemplateObjectInstanciationNode->AstNodeType() == TOdlAstNodeType::TEMPLATE_OBJECT_INSTANCIATION);
    TOdlAstNodeTemplateObjectInstanciation const* templateObjectInstanciationNode = parTemplateObjectInstanciationNode->CastNode<TOdlAstNodeTemplateObjectInstanciation>();

    TOdlAstNodeIdentifier const* typeIdentifierPointer = templateObjectInstanciationNode->TypeIdentifierPointer();
    assert(typeIdentifierPointer != nullptr);
    TOdlAstNodeNamedDeclaration const* resolvedNamedDeclaration = typeIdentifierPointer->ResolvedReference();
    assert(resolvedNamedDeclaration != nullptr);
    assert(resolvedNamedDeclaration->AstNodeType() == TOdlAstNodeType::NAMED_DECLARATION);
    TOdlAstNodeExpression const* namedDeclarationExpression = resolvedNamedDeclaration->ExpressionPointer();
    assert(namedDeclarationExpression != nullptr);
    assert(namedDeclarationExpression->AstNodeType() == TOdlAstNodeType::TEMPLATE_OBJECT_DECLARATION);

    TOdlAstNodeTemplateObjectDeclaration const* templateObjectDeclaration = namedDeclarationExpression->CastNode<TOdlAstNodeTemplateObjectDeclaration>();
    return templateObjectDeclaration;
}
//-------------------------------------------------------------------------------
std::string const& GetTemplateObjectInstanciationDeclarationTypeAsString(TOdlAstNodeTemplateObjectInstanciation const* parTemplateObjectInstanciationNode)
{
    TOdlAstNodeTemplateObjectDeclaration const* templateObjectDeclaration = GetTemplateObjectInstanciationDeclaration(parTemplateObjectInstanciationNode);
    std::string const& objectType = templateObjectDeclaration->TypeIdentifierPointer()->Identifier();
    return objectType;
}
//-------------------------------------------------------------------------------
//*******************************************************************************
//-------------------------------------------------------------------------------
} // odl


