#include "StdAfx.h"
#include "OdlInterpreter.h"

#include "Odl.h"

#include "OdlExpression.h"
#include "OdlTemplate.h"
#include "vodlParserLexer/OdlAstNode.h"
#include "OdlDatabase.h"


namespace odl
{
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
void VisitAst(TOdlAstNode* parAstNode, TInterpretContext& parContext, TAstOperationCallback parCallback)
{
    TOdlAstNodeType::TType nodeType = parAstNode->AstNodeType();
    switch (nodeType)
    {
	case TOdlAstNodeType::OPERATION:
		{
            TOdlAstNode* parLeft = parAstNode->CastNode<TOdlAstNodeOperation>()->LeftExpressionPointer();
            TOdlAstNode* parRight = parAstNode->CastNode<TOdlAstNodeOperation>()->RightExpressionPointer();
            if (parLeft != NULL)
                (*parCallback)(parLeft, parContext);
            (*parCallback)(parRight, parContext);
		}
		break ;
	case TOdlAstNodeType::VALUE_VECTOR:
		{
            TOdlAstNodeValueVector const* valueVectorNode = parAstNode->CastNode<TOdlAstNodeValueVector>();
            std::vector< TOdlAstNodeExpression* > const& childs = valueVectorNode->VectorContent();
            size_t childCount = childs.size();
		    for (size_t i = 0; i < childCount; ++i)
            {
                size_t invI = childCount - i - 1;
                TOdlAstNodeExpression* child = childs[invI];
                (*parCallback)(child, parContext);
            }
		}
		break ;
    case TOdlAstNodeType::PROPERTY_DECLARATION:
        {
            TOdlAstNodePropertyDeclaration* propertyDeclaration = parAstNode->CastNode<TOdlAstNodePropertyDeclaration>();
            TOdlAstNodeExpression* expression = propertyDeclaration->ExpressionPointer();
            (*parCallback)(expression, parContext);
        }
        break ;
    case TOdlAstNodeType::PROPERTY_DECLARATION_LIST:
        {
            TOdlAstNodePropertyDeclarationList* propertyDeclarationList = parAstNode->CastNode<TOdlAstNodePropertyDeclarationList>();
            std::vector< TOdlAstNodePropertyDeclaration* > const& properties = propertyDeclarationList->PropertyDeclarationList();
		    for (size_t i = 0; i < properties.size(); ++i)
            {
                TOdlAstNode* property = properties[i];
                (*parCallback)(property, parContext);
            }
        }
        break ;
    case TOdlAstNodeType::OBJECT_DECLARATION:
        {
            TOdlAstNodeObjectDeclaration const* objectDeclarationNode = parAstNode->CastNode<TOdlAstNodeObjectDeclaration>();
			if (!objectDeclarationNode->IsNullPtr())
			{
				TOdlAstNode* propertyDeclarationList = objectDeclarationNode->PropertyDeclarationListPointer();
				(*parCallback)(propertyDeclarationList, parContext);
			}
        }
        break ;
    case TOdlAstNodeType::NAMESPACE:
        {
            TOdlAstNodeNamespaceDeclaration const* namespaceDeclarationNode = parAstNode->CastNode<TOdlAstNodeNamespaceDeclaration>();
            std::vector< TOdlAstNodeNamedDeclaration* > const& namedDeclarations = namespaceDeclarationNode->NamespaceContent();
		    for (size_t i = 0; i < namedDeclarations.size(); ++i)
            {
                TOdlAstNode* namedDeclaration = namedDeclarations[i];
                (*parCallback)(namedDeclaration, parContext);
            }
        }
        break ;
	case TOdlAstNodeType::NAMED_DECLARATION:
		{
            TOdlAstNodeNamedDeclaration const* namedDeclarationNode = parAstNode->CastNode<TOdlAstNodeNamedDeclaration>();
			#ifdef ODL_ENABLE_VERBOSE_DEBUG
			TOdlAstNode* name = namedDeclarationNode->IdentifierPointer();
			assert(name != nullptr);
			#endif

			TOdlAstNode* expression = namedDeclarationNode->ExpressionPointer();
			(*parCallback)(expression, parContext);
		}
		break ;
    case TOdlAstNodeType::TEMPLATE_OBJECT_DECLARATION:
        {
            TOdlAstNodeTemplateObjectDeclaration const* templateObjectDeclarationNode = parAstNode->CastNode<TOdlAstNodeTemplateObjectDeclaration>();
			TOdlAstNode* propertyDeclarationList = templateObjectDeclarationNode->PropertyDeclarationListPointer();
			(*parCallback)(propertyDeclarationList, parContext);
        }
        break ;
    case TOdlAstNodeType::TEMPLATE_OBJECT_INSTANCIATION:
        {
            TOdlAstNodeTemplateObjectInstanciation const* templateObjectInstanciationNode = parAstNode->CastNode<TOdlAstNodeTemplateObjectInstanciation>();
            // {TODO} Paul(2014/12/27) get the template expression and find the named reference to anything else to resolve their references.
            int a = 0;
        }
        break ;
    default:
        assert(nodeType & TOdlAstNodeType::VALUE_MASK);
        break;
    };
}
//-------------------------------------------------------------------------------
static void InstanciateObjects(TOdlAstNode* parAstNode, TInterpretContext& parContext)
{
    TOdlAstNodeType::TType nodeType = parAstNode->AstNodeType();
    switch (nodeType)
    {
    case TOdlAstNodeType::NAMESPACE:
        {
            TOdlAstNodeNamespaceDeclaration const* namespaceDeclarationNode = parAstNode->CastNode<TOdlAstNodeNamespaceDeclaration>();
            TOdlAstNodeIdentifier const* identifier = namespaceDeclarationNode->IdentifierPointer_IFP();
            if (identifier != nullptr)
            {
                std::string const& namespaceName = identifier->Identifier();
                parContext.EnterNamespace(namespaceName);
            }
            VisitAst(parAstNode, parContext, InstanciateObjects);
            if (identifier != nullptr)
            {
                parContext.LeaveNamespace();
            }
        };
        break ;
	case TOdlAstNodeType::NAMED_DECLARATION:
		{
            TOdlAstNodeNamedDeclaration const* namedDeclarationNode = parAstNode->CastNode<TOdlAstNodeNamedDeclaration>();
			TOdlAstNodeIdentifier const* identifier = namedDeclarationNode->IdentifierPointer();
			std::string const& namespaceName = identifier->Identifier();
            parContext.EnterNamespace(namespaceName);
            VisitAst(parAstNode, parContext, InstanciateObjects);
			parContext.LeaveNamespace();
		}
		break;
	case TOdlAstNodeType::TEMPLATE_OBJECT_DECLARATION:
		{
			// nothing to do
			int a = 0;
		}
		break ;
	case TOdlAstNodeType::TEMPLATE_OBJECT_INSTANCIATION:
		{
            TOdlAstNodeTemplateObjectInstanciation const* templateObjectInstanciationNode = parAstNode->CastNode<TOdlAstNodeTemplateObjectInstanciation>();
			std::string const& objectType = GetTemplateObjectInstanciationDeclarationTypeAsString(templateObjectInstanciationNode);

			TMetaClassBase const* objectMetaClass = TOdlDatabase::Instance().FindRegisteredMetaClassByName_IFP(objectType.c_str());
			TOdlObject* odlObject = objectMetaClass->CreateObject();
			TOdlDatabasePath const& objectNamespaceAndName = parContext.DatabasePath();
			#ifdef ODL_ENABLE_VERBOSE_DEBUG
			std::string fordebug = objectNamespaceAndName.ToString();
			#endif
			TOdlDatabase::Instance().StoreObject(objectNamespaceAndName, odlObject, objectMetaClass);
		}
		break ;
    case TOdlAstNodeType::OBJECT_DECLARATION:
        {
            TOdlAstNodeObjectDeclaration const* objectDeclarationNode = parAstNode->CastNode<TOdlAstNodeObjectDeclaration>();
			if (!objectDeclarationNode->IsNullPtr())
			{
				std::string const& objectType = objectDeclarationNode->TypeIdentifierPointer()->Identifier();
				TMetaClassBase const* objectMetaClass = TOdlDatabase::Instance().FindRegisteredMetaClassByName_IFP(objectType.c_str());
				TOdlObject* odlObject = objectMetaClass->CreateObject();
            
				TOdlDatabasePath const& objectNamespaceAndName = parContext.DatabasePath();
				#ifdef ODL_ENABLE_VERBOSE_DEBUG
				std::string fordebug = objectNamespaceAndName.ToString();
				#endif
				TOdlDatabase::Instance().StoreObject(objectNamespaceAndName, odlObject, objectMetaClass);
				VisitAst(parAstNode, parContext, InstanciateObjects);
			}
        }
        break ;
    default:
        VisitAst(parAstNode, parContext, InstanciateObjects);
        break;
    };
}
//-------------------------------------------------------------------------------
//*******************************************************************************
//-------------------------------------------------------------------------------
static void EvalExpressionAndStoreProperty(TOdlObject* parObject, 
                                           TMetaClassBase const* parObjectMetaClassBase,
                                           TPropertyBase const* parPropertyBase, 
                                           TOdlAstNodeExpression const* parExpression,
                                           TTemplateInstanciationStack& parTemplateInstanciationStack)
{
    TMetaClassBase const* propertyMetaClassBase = parPropertyBase->Type();

	TEvalExpressionContext evalExpressionContext(parTemplateInstanciationStack);
    TOdlExpression const result = EvalExpression(evalExpressionContext, parExpression);

	if (result.Type() != TOdlExpression::UNTYPED)
	{
		bool const propertySet = parPropertyBase->SetObjectPropertyByExpression_ROK(parObject, result);
		if (!propertySet)
		{
			assert(false); // error property affectation.
		}
	}
	else
	{
		assert(false); // error expression cannot be evaluated -> circular reference ?
	}
}
//-------------------------------------------------------------------------------
//*******************************************************************************
//-------------------------------------------------------------------------------
struct TFillObjectPropertiesInterpretContext : public TInterpretContext
{
public:

	TFillObjectPropertiesInterpretContext(TOdlDatabasePath& parDatabasePath,
                                          TOdlObject* parCurrentObject, 
                                          TMetaClassBase const* parMetaClass,
                                          TTemplateInstanciationStack& parTemplateInstanciationStack) :
        TInterpretContext(parDatabasePath),
        FCurrentObject(parCurrentObject),
        FMetaClassBase(parMetaClass),
		FTemplateInstanciationStack(parTemplateInstanciationStack)
    {

    }

    TOdlObject* CurrentObject() const { assert(FCurrentObject != nullptr); return FCurrentObject; }
    TMetaClassBase const* MetaClassBase() const { assert(FMetaClassBase != nullptr); return FMetaClassBase; }
    TTemplateInstanciationStack& TemplateInstanciationNodeStack() { return FTemplateInstanciationStack; }

private:
    TOdlObject*             FCurrentObject;
    TMetaClassBase const*   FMetaClassBase;
    TTemplateInstanciationStack& FTemplateInstanciationStack;
};
//-------------------------------------------------------------------------------
void FillObjectsProperties(TOdlAstNode* parAstNode, TInterpretContext& parContext)
{
    TOdlAstNodeType::TType nodeType = parAstNode->AstNodeType();
    switch (nodeType)
    {
    case TOdlAstNodeType::NAMESPACE:
        {
            TOdlAstNodeNamespaceDeclaration const* namespaceDeclarationNode = parAstNode->CastNode<TOdlAstNodeNamespaceDeclaration>();
            TFillObjectPropertiesInterpretContext& context = static_cast<TFillObjectPropertiesInterpretContext&>(parContext);
            TOdlAstNodeIdentifier const* identifier = namespaceDeclarationNode->IdentifierPointer_IFP();
            if (identifier != nullptr)
            {
                std::string const& namespaceName = identifier->Identifier();
                context.EnterNamespace(namespaceName);
            }
            VisitAst(parAstNode, parContext, FillObjectsProperties);
            if (identifier != nullptr)
            {
                context.LeaveNamespace();
            }
        }
        break ;
    case TOdlAstNodeType::NAMED_DECLARATION:
        {
            TOdlAstNodeNamedDeclaration* namedDeclarationNode = parAstNode->CastNode<TOdlAstNodeNamedDeclaration>();
            TFillObjectPropertiesInterpretContext& context = static_cast<TFillObjectPropertiesInterpretContext&>(parContext);
            TOdlAstNodeIdentifier const* identifier = namedDeclarationNode->IdentifierPointer_IFP();
            assert(identifier != nullptr);
            std::string const& namespaceName = identifier->Identifier();
            context.EnterNamespace(namespaceName);
            VisitAst(parAstNode, parContext, FillObjectsProperties);
			context.LeaveNamespace();
        }
        break ;
	case TOdlAstNodeType::TEMPLATE_OBJECT_DECLARATION:
		{
			// nothing to do.
			int a = 0;
		};
		break ;
	case TOdlAstNodeType::TEMPLATE_OBJECT_INSTANCIATION:
		{
            TOdlAstNodeTemplateObjectInstanciation const* templateObjectInstanciationNode = parAstNode->CastNode<TOdlAstNodeTemplateObjectInstanciation>();
			TFillObjectPropertiesInterpretContext& context = static_cast<TFillObjectPropertiesInterpretContext&>(parContext);

			TOdlDatabasePath& templateInstanciationDatabaseName = parContext.DatabasePath();
			#ifdef ODL_ENABLE_VERBOSE_DEBUG
			std::string fordebug = templateInstanciationDatabaseName.ToString();
			#endif

			TOdlAstNodeTemplateObjectDeclaration const* templateDeclaration = GetTemplateObjectInstanciationDeclaration(templateObjectInstanciationNode);
			#ifdef ODL_ENABLE_VERBOSE_DEBUG
			TOdlDatabasePath templateDatabasePath = templateDeclaration->NamedDeclarationWeakRef()->FullDatabasePath();
			std::string templateDatabasePathForDebug = templateDatabasePath.ToString();
			#endif

			std::string const& objectType = GetTemplateObjectInstanciationDeclarationTypeAsString(templateObjectInstanciationNode);
			TOdlObject* object = TOdlDatabase::Instance().GetObject(templateInstanciationDatabaseName);
			TMetaClassBase const* metaClassBase = TOdlDatabase::Instance().FindRegisteredMetaClassByName_IFP(objectType.c_str());

            TFillObjectPropertiesInterpretContext newContext(templateInstanciationDatabaseName, object, metaClassBase, context.TemplateInstanciationNodeStack());
            newContext.TemplateInstanciationNodeStack().EnterTemplateObjectInstanciation(templateObjectInstanciationNode);
			TOdlAstNodePropertyDeclarationList* propertiesToFill = templateDeclaration->PropertyDeclarationListPointer();
			VisitAst(propertiesToFill, newContext, FillObjectsProperties);
            newContext.TemplateInstanciationNodeStack().LeaveTemplateObjectInstanciation(templateObjectInstanciationNode);
		}
		break ;
    case TOdlAstNodeType::OBJECT_DECLARATION:
		{
            TOdlAstNodeObjectDeclaration const* objectDeclarationNode = parAstNode->CastNode<TOdlAstNodeObjectDeclaration>();
			if (!objectDeclarationNode->IsNullPtr())
			{
            	TFillObjectPropertiesInterpretContext& context = static_cast<TFillObjectPropertiesInterpretContext&>(parContext);
				std::string const& objectType = objectDeclarationNode->TypeIdentifierPointer()->Identifier();
            
				TOdlDatabasePath& objectNamespaceAndName = parContext.DatabasePath();
				#ifdef ODL_ENABLE_VERBOSE_DEBUG
				std::string fordebug = objectNamespaceAndName.ToString();
				#endif

				TOdlObject* object = TOdlDatabase::Instance().GetObject(objectNamespaceAndName);
				TMetaClassBase const* metaClassBase = TOdlDatabase::Instance().FindRegisteredMetaClassByName_IFP(objectType.c_str());
				TFillObjectPropertiesInterpretContext newContext(objectNamespaceAndName, object, metaClassBase, context.TemplateInstanciationNodeStack());
				VisitAst(parAstNode, newContext, FillObjectsProperties);
			}
        };
        break ;
    case TOdlAstNodeType::PROPERTY_DECLARATION:
        {
            TOdlAstNodePropertyDeclaration const* propertyDeclarationNode = parAstNode->CastNode<TOdlAstNodePropertyDeclaration>();    

            TFillObjectPropertiesInterpretContext& context = static_cast< TFillObjectPropertiesInterpretContext& >(parContext);
            TOdlDatabasePath const& odlDatabasePath = context.DatabasePath();

			#if ODL_ENABLE_VERBOSE_DEBUG
			std::string debugString = odlDatabasePath.ToString();
			#endif

            TOdlObject* object = context.CurrentObject();
            TMetaClassBase const* objectMetaClassBase = context.MetaClassBase();
            std::string const& propertyName = propertyDeclarationNode->IdentifierPointer()->Identifier();
            TPropertyBase const* propertyBase = objectMetaClassBase->PropertyByName(propertyName);
            if (propertyBase != nullptr)
            {
                TOdlAstNodeExpression const* expression = propertyDeclarationNode->ExpressionPointer();
                EvalExpressionAndStoreProperty(object, objectMetaClassBase, propertyBase, expression, context.TemplateInstanciationNodeStack());
            }
            else
            {
                assert(false); // property not found.
            }

            VisitAst(parAstNode, context, FillObjectsProperties);
        }
        break ;
    default:
        VisitAst(parAstNode, parContext, FillObjectsProperties);
        break;
    };
}
//-------------------------------------------------------------------------------
void AutoNameAnomymousObjectDeclaration(TOdlAstNode* parAstNode, TInterpretContext& parContext)
{
    TOdlAstNodeType::TType nodeType = parAstNode->AstNodeType();
    switch (nodeType)
    {
	case TOdlAstNodeType::NAMESPACE:
		{
            TOdlAstNodeNamespaceDeclaration const* namespaceDeclarationNode = parAstNode->CastNode<TOdlAstNodeNamespaceDeclaration>();
            std::string namespaceName = namespaceDeclarationNode->IdentifierPointer_IFP() ? namespaceDeclarationNode->IdentifierPointer_IFP()->Identifier() : std::string();
            // Paul(2014/12/23)  root namespace is empty.
            if (!namespaceName.empty())
            	parContext.EnterNamespace(namespaceName);
            VisitAst(parAstNode, parContext, AutoNameAnomymousObjectDeclaration);
            // Paul(2014/12/23)  root namespace is empty.
            if (!namespaceName.empty())
            	parContext.LeaveNamespace();
		}
		break ;
	case TOdlAstNodeType::NAMED_DECLARATION:
		{
            TOdlAstNodeNamedDeclaration* namedDeclaration = parAstNode->CastNode<TOdlAstNodeNamedDeclaration>();

            // Paul(2014/12/23)  here name objects that needs to.
            namedDeclaration->AutoGenerateIdentifierIfNone();

            std::string namedDeclarationName = namedDeclaration->IdentifierPointer()->Identifier();
            parContext.EnterNamespace(namedDeclarationName);
            TOdlAstNodeExpression* expressionPointer = namedDeclaration->ExpressionPointer();
            VisitAst(parAstNode, parContext, AutoNameAnomymousObjectDeclaration);
            parContext.LeaveNamespace();
		}
		break ;
	case TOdlAstNodeType::TEMPLATE_OBJECT_DECLARATION:
		{
            TOdlAstNodeTemplateObjectDeclaration const* templateObjectDeclarationNode = parAstNode->CastNode<TOdlAstNodeTemplateObjectDeclaration>();
            #if ODL_ENABLE_VERBOSE_DEBUG
            std::string debug = parContext.DatabasePath().ToString();
            #endif
            // Paul(2014/12/23) SetFullDatabasePath for debug.
            templateObjectDeclarationNode->NamedDeclarationWeakRef()->SetFullDatabasePath(parContext.DatabasePath());
            VisitAst(parAstNode, parContext, AutoNameAnomymousObjectDeclaration);
		}
		break;
    case TOdlAstNodeType::TEMPLATE_OBJECT_INSTANCIATION:
        {
            TOdlAstNodeTemplateObjectInstanciation const* templateObjectInstantiationNode = parAstNode->CastNode<TOdlAstNodeTemplateObjectInstanciation>();
            #if ODL_ENABLE_VERBOSE_DEBUG
            std::string debug = parContext.DatabasePath().ToString();
            #endif
            // Paul(2014/12/23) SetFullDatabasePath for debug.
            templateObjectInstantiationNode->NamedDeclarationWeakRef()->SetFullDatabasePath(parContext.DatabasePath());
            VisitAst(parAstNode, parContext, AutoNameAnomymousObjectDeclaration);
        };
        break ;
    case TOdlAstNodeType::OBJECT_DECLARATION:
        {
            TOdlAstNodeObjectDeclaration* objectDeclarationNode = parAstNode->CastNode<TOdlAstNodeObjectDeclaration>();
           
            if (!objectDeclarationNode->IsNullPtr())
            {
                TOdlAstNodeNamedDeclaration* objectDeclarationNamedObject = objectDeclarationNode->NamedDeclarationWeakRef();
                std::string const& objectName = objectDeclarationNamedObject->IdentifierPointer()->Identifier();
                #if ODL_ENABLE_VERBOSE_DEBUG
                std::string debug = parContext.DatabasePath().ToString();
                #endif
                // Paul(2014/12/23) SetFullDatabasePath for debug.
                objectDeclarationNamedObject->SetFullDatabasePath(parContext.DatabasePath());

                VisitAst(parAstNode, parContext, AutoNameAnomymousObjectDeclaration);
            }
        }
        break ;
    default:
		VisitAst(parAstNode, parContext, AutoNameAnomymousObjectDeclaration);
        break ;
    };
}
//-------------------------------------------------------------------------------
static void AutoNameAnomymousObjectDeclarations(TOdlAstNode* parAstNode)
{
    TOdlDatabasePath databasePath;
    TInterpretContext context(databasePath);
    VisitAst(parAstNode, context, AutoNameAnomymousObjectDeclaration);
}
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
void ResolveValueIdentifier(TOdlAstNode* parAstNode, TInterpretContext& parContext)
{
    TOdlAstNodeType::TType nodeType = parAstNode->AstNodeType();

    // PAUL(27/05/14 19:28:29) looking for named expressions.
    switch (nodeType)
    {
    case TOdlAstNodeType::IDENTIFIER:
        {
            TOdlAstNodeIdentifier* identifierNode = parAstNode->CastNode<TOdlAstNodeIdentifier>();
            if (identifierNode->IsReferenceToResolve())
            {
            	TOdlAstNode const* foundReference = nullptr;

            	TResolveValueIdentifierInterpretContext const& context = static_cast< TResolveValueIdentifierInterpretContext const& >(parContext);
            	std::vector< TOdlAstNode* > const& parentNamespaces = context.Parents();
            	size_t const parentNamespaceCount = parentNamespaces.size();

                std::string const& fullIdentifierToResolve = parAstNode->CastNode<TOdlAstNodeIdentifier>()->Identifier();

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
										break ;
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
									break ;
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
                            else
                            {
                                // {TODO} Paul(2014/12/21) est-ce que c'est une erreur ou pas ? gerer correctement les named values.
                                assert(false);
                            }
						}
					rootNamespaceDone:
						int a = 0;
					}
															
					if (rootNamespace != nullptr)
					{
						// 2) search childs namespace of root namespace.
						TOdlAstNode const* childNamespace = rootNamespace;
						{
							for (int i = 0; i < (int) searchedNameSpaceDatabasePath.size() - 1; ++i)
							{
								std::string const& searchedChildNamespace = searchedNameSpaceDatabasePath[i].ToString();

								bool childNamespaceFound = false;

                                if (childNamespace->AstNodeType() == TOdlAstNodeType::NAMESPACE)
                                {
                                    TOdlAstNodeNamespaceDeclaration const* childNamespaceDeclaration = childNamespace->CastNode<TOdlAstNodeNamespaceDeclaration>();
							        std::vector< TOdlAstNodeNamedDeclaration* > const& childNamespaceContent =  childNamespaceDeclaration->NamespaceContent();
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
										        break ;
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
										    break ;
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
                
                if (foundReference == nullptr)
                {
                    assert(false); // todo: identifiant inconnu.
                }
            }
        }
        break ;
    case TOdlAstNodeType::NAMESPACE:
        {
            TOdlAstNodeNamespaceDeclaration* namespaceDeclarationNode = parAstNode->CastNode<TOdlAstNodeNamespaceDeclaration>();
            TResolveValueIdentifierInterpretContext& context = static_cast< TResolveValueIdentifierInterpretContext& >(parContext);
            context.PushParentNode(namespaceDeclarationNode);
			std::string namespaceName = namespaceDeclarationNode->IdentifierPointer_IFP() != nullptr ? namespaceDeclarationNode->IdentifierPointer_IFP()->Identifier() : std::string();
			if (!namespaceName.empty())
				context.EnterNamespace(namespaceName);
            VisitAst(parAstNode, parContext, ResolveValueIdentifier);
			if (!namespaceName.empty())
				context.LeaveNamespace();
            context.PopParentNode();
        }
        break ;
	case TOdlAstNodeType::NAMED_DECLARATION:
        {
            TOdlAstNodeNamedDeclaration* namedDeclarationNode = parAstNode->CastNode<TOdlAstNodeNamedDeclaration>();
            TResolveValueIdentifierInterpretContext& context = static_cast< TResolveValueIdentifierInterpretContext& >(parContext);
            context.PushParentNode(parAstNode);
			std::string const& namespaceName = namedDeclarationNode->IdentifierPointer()->Identifier();
			if (!namespaceName.empty())
				context.EnterNamespace(namespaceName);
            VisitAst(parAstNode, parContext, ResolveValueIdentifier);
			if (!namespaceName.empty())
				context.LeaveNamespace();
			context.PopParentNode();
        }
		break ;
	case TOdlAstNodeType::TEMPLATE_OBJECT_DECLARATION:
		{
            TOdlAstNodeTemplateObjectDeclaration* templateObjectDeclaration = parAstNode->CastNode<TOdlAstNodeTemplateObjectDeclaration>();
			// resolve resolve arguments.
			TResolveValueIdentifierInterpretContext& context = static_cast< TResolveValueIdentifierInterpretContext& >(parContext);
            context.PushParentNode(templateObjectDeclaration);
            VisitAst(parAstNode, parContext, ResolveValueIdentifier);
			context.PopParentNode();
		};
		break;
	case TOdlAstNodeType::TEMPLATE_OBJECT_INSTANCIATION:
	    {
            TOdlAstNodeTemplateObjectInstanciation const* templateObjectInstanciationNode = parAstNode->CastNode<TOdlAstNodeTemplateObjectInstanciation>();

			ResolveValueIdentifier(templateObjectInstanciationNode->TypeIdentifierPointer(), parContext);

			std::vector< TOdlAstNodeExpression* > const& templateParameters = templateObjectInstanciationNode->TemplateExpressionListPointer()->TemplateExpressionParameterList();
			for (size_t i = 0; i < templateParameters.size(); ++i)
			{
				TOdlAstNodeExpression* templateParameter = templateParameters[i];
				ResolveValueIdentifier(templateParameter, parContext);
			}
		}
		break ;
    default:
        VisitAst(parAstNode, parContext, ResolveValueIdentifier);
        break ;
    };
}
//-------------------------------------------------------------------------------
static void ResolveValueIdentifiers(TOdlAstNode* parAstNode)
{
    TOdlDatabasePath databasePath;
    TResolveValueIdentifierInterpretContext context(databasePath);
    ResolveValueIdentifier(parAstNode, context);
}
//-------------------------------------------------------------------------------
void PostProcessAst(TOdlAstNode* parRootNode)
{
    // auto name
    AutoNameAnomymousObjectDeclarations(parRootNode);

    // link named value nodes... box ?
    ResolveValueIdentifiers(parRootNode);
}
//-------------------------------------------------------------------------------
void InterpretAst(TOdlAstNode* parAstNode)
{
    TOdlAstNodeType::TType nodeType = parAstNode->AstNodeType();
    switch (nodeType)
    {
    case TOdlAstNodeType::NAMESPACE:
        {
            // create objets
            TOdlDatabasePath databasePath;
            TInterpretContext InstanciateObjectContext(databasePath);
            VisitAst(parAstNode, InstanciateObjectContext, InstanciateObjects);
            
            // fill objets properties.
            assert(databasePath.empty());
            TTemplateInstanciationStack templateInstanciationStack;
            TFillObjectPropertiesInterpretContext fillObjectsPropertiesContext(databasePath, nullptr, nullptr, templateInstanciationStack);
            VisitAst(parAstNode, fillObjectsPropertiesContext, FillObjectsProperties);
        };
        break ;
    default:
        assert(false);
        break ;
    }
}
//-------------------------------------------------------------------------------
//*******************************************************************************
//-------------------------------------------------------------------------------
} // namespace odl
