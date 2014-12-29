#include "StdAfx.h"
#include "OdlInterpreter.h"

#include "Odl.h"

#include "OdlExpression.h"
#include "OdlTemplate.h"
#include "vodlParserLexer/OdlAstNode.h"
#include "OdlDatabase.h"
#include "OdlResolveIdentifier.h"


namespace odl
{
//-------------------------------------------------------------------------------
//*******************************************************************************
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
            parContext.EnterNamespace(namespaceDeclarationNode);
            std::vector< TOdlAstNodeNamedDeclaration* > const& namedDeclarations = namespaceDeclarationNode->NamespaceContent();
		    for (size_t i = 0; i < namedDeclarations.size(); ++i)
            {
                TOdlAstNode* namedDeclaration = namedDeclarations[i];
                (*parCallback)(namedDeclaration, parContext);
            }
            parContext.LeaveNamespace(namespaceDeclarationNode);
        }
        break ;
	case TOdlAstNodeType::NAMED_DECLARATION:
		{
            TOdlAstNodeNamedDeclaration const* namedDeclarationNode = parAstNode->CastNode<TOdlAstNodeNamedDeclaration>();
            #if ODL_ENABLE_VERBOSE_DEBUG
			TOdlAstNode* name = namedDeclarationNode->IdentifierPointer();
			assert(name != nullptr);
			#endif
            parContext.EnterNamespace(namedDeclarationNode);
			TOdlAstNode* expression = namedDeclarationNode->ExpressionPointer();
			(*parCallback)(expression, parContext);
            parContext.LeaveNamespace(namedDeclarationNode);
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
    case TOdlAstNodeType::TEMPLATE_NAMESPACE_INSTANCIATION:
        {
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
    #if ODL_ENABLE_VERBOSE_DEBUG
	std::string currentDatabaseNamefordebug = parContext.DatabasePath().ToString();
	#endif

    TOdlAstNodeType::TType nodeType = parAstNode->AstNodeType();
    switch (nodeType)
    {
    case TOdlAstNodeType::NAMESPACE:
        {
            TOdlAstNodeNamespaceDeclaration const* namespaceDeclarationNode = parAstNode->CastNode<TOdlAstNodeNamespaceDeclaration>();
            if (!namespaceDeclarationNode->IsTemplate())
                VisitAst(parAstNode, parContext, InstanciateObjects);
        };
        break ;
	case TOdlAstNodeType::NAMED_DECLARATION:
		{
            TOdlAstNodeNamedDeclaration const* namedDeclarationNode = parAstNode->CastNode<TOdlAstNodeNamedDeclaration>();
            if (!namedDeclarationNode->IsTemplate())
                VisitAst(parAstNode, parContext, InstanciateObjects);
		}
		break;
	case TOdlAstNodeType::TEMPLATE_OBJECT_DECLARATION:
		{
			// nothing to do, should not pass here.
            assert(false);
		}
		break ;
	case TOdlAstNodeType::TEMPLATE_OBJECT_INSTANCIATION:
		{
            TOdlAstNodeTemplateObjectInstanciation const* templateObjectInstanciationNode = parAstNode->CastNode<TOdlAstNodeTemplateObjectInstanciation>();
			std::string const& objectType = GetTemplateObjectInstanciationDeclarationTypeAsString(templateObjectInstanciationNode);

            TOdlDatabasePath const& objectNamespaceAndName = parContext.DatabasePath();
            
            // Paul(2014/12/28) Instanciation database path, to get the object back.
            templateObjectInstanciationNode->NamedDeclarationWeakRef()->SetFullDatabasePath(objectNamespaceAndName);

			TMetaClassBase const* objectMetaClass = TOdlDatabase::Instance().FindRegisteredMetaClassByName_IFP(objectType.c_str());
			TOdlObject* odlObject = objectMetaClass->CreateObject();
			TOdlDatabase::Instance().StoreObject(objectNamespaceAndName, odlObject, objectMetaClass);
		}
		break ;
    case TOdlAstNodeType::TEMPLATE_NAMESPACE_INSTANCIATION:
        {
        	#if ODL_ENABLE_VERBOSE_DEBUG
			std::string fordebug = parContext.DatabasePath().ToString();
			#endif
            TOdlAstNodeTemplateNamespaceInstanciation* templateNamespaceInstanciation = parAstNode->CastNode<TOdlAstNodeTemplateNamespaceInstanciation>();
            TOdlAstNodeNamedDeclaration* templateNamespaceNamedDeclaration = templateNamespaceInstanciation->TargetTemplateNamespaceIdentifierPointer()->ResolvedReference();
            assert(templateNamespaceNamedDeclaration != nullptr);

            TOdlAstNodeNamespaceDeclaration* templateNamespaceNamespaceDeclaration = templateNamespaceNamedDeclaration->CastNode<TOdlAstNodeNamespaceDeclaration>();

            std::vector< TOdlAstNodeNamedDeclaration* > const& namedDeclarations = templateNamespaceNamespaceDeclaration->NamespaceContent();
		    for (size_t i = 0; i < namedDeclarations.size(); ++i)
            {
                TOdlAstNode* namedDeclaration = namedDeclarations[i];
                InstanciateObjects(namedDeclaration, parContext);
            }
        }
        break;
    case TOdlAstNodeType::OBJECT_DECLARATION:
        {
            TOdlAstNodeObjectDeclaration const* objectDeclarationNode = parAstNode->CastNode<TOdlAstNodeObjectDeclaration>();
			if (!objectDeclarationNode->IsNullPtr())
			{
				std::string const& objectType = objectDeclarationNode->TypeIdentifierPointer()->Identifier();
				TMetaClassBase const* objectMetaClass = TOdlDatabase::Instance().FindRegisteredMetaClassByName_IFP(objectType.c_str());
				TOdlObject* odlObject = objectMetaClass->CreateObject();
            
				TOdlDatabasePath const& objectNamespaceAndName = parContext.DatabasePath();

                // Paul(2014/12/28) Instanciation database path, to get the object back.
                objectDeclarationNode->NamedDeclarationWeakRef()->SetFullDatabasePath(objectNamespaceAndName);

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
                                           TInterpretContext& parInterpretContext)
{
    TMetaClassBase const* propertyMetaClassBase = parPropertyBase->Type();

	TEvalExpressionContext evalExpressionContext(parInterpretContext);
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
                                          TNamedDeclarationStack& parStaticNamespaceStack,
                                          TNamedDeclarationStack& parDynamicNamespaceStack,
                                          TOdlObject* parCurrentObject, 
                                          TMetaClassBase const* parMetaClass) :
        TInterpretContext(parDatabasePath, parStaticNamespaceStack, parDynamicNamespaceStack),
        FCurrentObject(parCurrentObject),
        FMetaClassBase(parMetaClass)
    {

    }

    TOdlObject* CurrentObject() const { assert(FCurrentObject != nullptr); return FCurrentObject; }
    TMetaClassBase const* MetaClassBase() const { assert(FMetaClassBase != nullptr); return FMetaClassBase; }

private:
    TOdlObject*             FCurrentObject;
    TMetaClassBase const*   FMetaClassBase;
};
//-------------------------------------------------------------------------------
static bool IsTemplateInstanciationNamedDeclaration(TOdlAstNodeNamedDeclaration const* parNamedDeclaration)
{
    if (parNamedDeclaration->AstNodeType() == TOdlAstNodeType::NAMED_DECLARATION)
    {
        TOdlAstNodeExpression const* expression = parNamedDeclaration->ExpressionPointer();
        if (expression != nullptr)
        {
            if (expression->AstNodeType() == TOdlAstNodeType::TEMPLATE_OBJECT_INSTANCIATION)
                return true;
            if (expression->AstNodeType() == TOdlAstNodeType::TEMPLATE_NAMESPACE_INSTANCIATION)
                return true;
        }
    }

    return false;
}
//-------------------------------------------------------------------------------
void FillObjectsProperties(TOdlAstNode* parAstNode, TInterpretContext& parContext)
{
    TOdlAstNodeType::TType nodeType = parAstNode->AstNodeType();
    switch (nodeType)
    {
    case TOdlAstNodeType::NAMESPACE:
        {
            TFillObjectPropertiesInterpretContext& context = static_cast<TFillObjectPropertiesInterpretContext&>(parContext);
            TOdlAstNodeNamespaceDeclaration const* namespaceDeclarationNode = parAstNode->CastNode<TOdlAstNodeNamespaceDeclaration>();
            if (!namespaceDeclarationNode->IsTemplate())
            {
                bool const isTemplateInstanciationNamedDeclaration = IsTemplateInstanciationNamedDeclaration(namespaceDeclarationNode);
                context.EnterNamespace(namespaceDeclarationNode);
                if (isTemplateInstanciationNamedDeclaration)
                    context.EnterTemplateObjectInstanciation(namespaceDeclarationNode);
                std::vector< TOdlAstNodeNamedDeclaration* > const& namedDeclarations = namespaceDeclarationNode->NamespaceContent();
		        for (size_t i = 0; i < namedDeclarations.size(); ++i)
                {
                    TOdlAstNode* namedDeclaration = namedDeclarations[i];
                    FillObjectsProperties(namedDeclaration, parContext);
                }
                if (isTemplateInstanciationNamedDeclaration)
                    context.LeaveTemplateObjectInstanciation(namespaceDeclarationNode);
                context.LeaveNamespace(namespaceDeclarationNode);
            }
        }
        break ;
    case TOdlAstNodeType::NAMED_DECLARATION:
        {
            TFillObjectPropertiesInterpretContext& context = static_cast<TFillObjectPropertiesInterpretContext&>(parContext);
            TOdlAstNodeNamedDeclaration* namedDeclarationNode = parAstNode->CastNode<TOdlAstNodeNamedDeclaration>();
            if (!namedDeclarationNode->IsTemplate())
            {
                context.EnterNamespace(namedDeclarationNode);
                bool const isTemplateInstanciationNamedDeclaration = IsTemplateInstanciationNamedDeclaration(namedDeclarationNode);
                if (isTemplateInstanciationNamedDeclaration)
                    context.EnterTemplateObjectInstanciation(namedDeclarationNode);
                TOdlAstNode* expression = namedDeclarationNode->ExpressionPointer();
                FillObjectsProperties(expression, parContext);
                if (isTemplateInstanciationNamedDeclaration)
                    context.LeaveTemplateObjectInstanciation(namedDeclarationNode);
                context.LeaveNamespace(namedDeclarationNode);
            }
        }
        break ;
	case TOdlAstNodeType::TEMPLATE_OBJECT_DECLARATION:
		{
			// nothing to do, should not pass here.
            assert(false);
			int a = 0;
		};
		break ;
	case TOdlAstNodeType::TEMPLATE_OBJECT_INSTANCIATION:
		{
            TOdlAstNodeTemplateObjectInstanciation const* templateObjectInstanciationNode = parAstNode->CastNode<TOdlAstNodeTemplateObjectInstanciation>();
			TFillObjectPropertiesInterpretContext& context = static_cast<TFillObjectPropertiesInterpretContext&>(parContext);

			TOdlDatabasePath& templateInstanciationDatabaseName = parContext.DatabasePath();
			#if ODL_ENABLE_VERBOSE_DEBUG
			std::string fordebug = templateInstanciationDatabaseName.ToString();
			#endif

			TOdlAstNodeTemplateObjectDeclaration const* templateDeclaration = GetTemplateObjectInstanciationDeclaration(templateObjectInstanciationNode);
			#if ODL_ENABLE_VERBOSE_DEBUG
			TOdlDatabasePath templateDatabasePath = templateDeclaration->NamedDeclarationWeakRef()->FullDatabasePath();
			std::string templateDatabasePathForDebug = templateDatabasePath.ToString();
			#endif

			std::string const& objectType = GetTemplateObjectInstanciationDeclarationTypeAsString(templateObjectInstanciationNode);
			TOdlObject* object = TOdlDatabase::Instance().GetObject(templateInstanciationDatabaseName);
			TMetaClassBase const* metaClassBase = TOdlDatabase::Instance().FindRegisteredMetaClassByName_IFP(objectType.c_str());

            TFillObjectPropertiesInterpretContext newContext(context.DatabasePath(), 
                                                             context.StaticNamespaceStack(),
                                                             context.DynamicNamespaceStack(),
                                                             object, 
                                                             metaClassBase);
            
			TOdlAstNodePropertyDeclarationList* propertiesToFill = templateDeclaration->PropertyDeclarationListPointer();
            FillObjectsProperties(propertiesToFill, newContext);
		}
		break ;
    case TOdlAstNodeType::TEMPLATE_NAMESPACE_INSTANCIATION:
        {
            TOdlAstNodeTemplateNamespaceInstanciation* templateNamespaceInstanciation = parAstNode->CastNode<TOdlAstNodeTemplateNamespaceInstanciation>();
            TOdlAstNodeNamedDeclaration* templateNamespaceDeclarationNamedDeclarationNode = templateNamespaceInstanciation->TargetTemplateNamespaceIdentifierPointer()->ResolvedReference();
            TOdlAstNodeNamespaceDeclaration* templateNamespaceDeclaration = templateNamespaceDeclarationNamedDeclarationNode->CastNode<TOdlAstNodeNamespaceDeclaration>();
            
            std::vector< TOdlAstNodeNamedDeclaration* > const& namedDeclarations = templateNamespaceDeclaration->NamespaceContent();
		    for (size_t i = 0; i < namedDeclarations.size(); ++i)
            {
                TOdlAstNode* namedDeclaration = namedDeclarations[i];
                FillObjectsProperties(namedDeclaration, parContext);
            }
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
				#if ODL_ENABLE_VERBOSE_DEBUG
				std::string fordebug = objectNamespaceAndName.ToString();
				#endif

				TOdlObject* object = TOdlDatabase::Instance().GetObject(objectNamespaceAndName);
				TMetaClassBase const* metaClassBase = TOdlDatabase::Instance().FindRegisteredMetaClassByName_IFP(objectType.c_str());
				TFillObjectPropertiesInterpretContext newContext(context.DatabasePath(),
                                                                 context.StaticNamespaceStack(),
                                                                 context.DynamicNamespaceStack(),
                                                                 object, 
                                                                 metaClassBase);
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
                EvalExpressionAndStoreProperty(object, 
                                               objectMetaClassBase, 
                                               propertyBase, 
                                               expression,
                                               context);
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
            VisitAst(parAstNode, parContext, AutoNameAnomymousObjectDeclaration);
		}
		break ;
	case TOdlAstNodeType::NAMED_DECLARATION:
		{
            TOdlAstNodeNamedDeclaration* namedDeclaration = parAstNode->CastNode<TOdlAstNodeNamedDeclaration>();

            // Paul(2014/12/23)  here name objects that needs to.
            namedDeclaration->AutoGenerateIdentifierIfNone();

            TOdlAstNodeExpression* expressionPointer = namedDeclaration->ExpressionPointer();
            VisitAst(parAstNode, parContext, AutoNameAnomymousObjectDeclaration);
		}
		break ;
	case TOdlAstNodeType::TEMPLATE_OBJECT_DECLARATION:
		{
            TOdlAstNodeTemplateObjectDeclaration const* templateObjectDeclarationNode = parAstNode->CastNode<TOdlAstNodeTemplateObjectDeclaration>();
            #if ODL_ENABLE_VERBOSE_DEBUG
            std::string debug = parContext.DatabasePath().ToString();
            #endif
            VisitAst(parAstNode, parContext, AutoNameAnomymousObjectDeclaration);
		}
		break;
    case TOdlAstNodeType::TEMPLATE_OBJECT_INSTANCIATION:
        {
            TOdlAstNodeTemplateObjectInstanciation const* templateObjectInstantiationNode = parAstNode->CastNode<TOdlAstNodeTemplateObjectInstanciation>();
            #if ODL_ENABLE_VERBOSE_DEBUG
            std::string debug = parContext.DatabasePath().ToString();
            #endif
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
    TNamedDeclarationStack staticNamespaceStack;
    TNamedDeclarationStack dynamicNamespaceStack;
    TInterpretContext context(databasePath, staticNamespaceStack, dynamicNamespaceStack);
    VisitAst(parAstNode, context, AutoNameAnomymousObjectDeclaration);
}
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
            	TOdlAstNodeNamedDeclaration* foundReference = nullptr;
                foundReference = ResolveIdentifier(parContext, identifierNode);
                if (foundReference == nullptr)
                {
                    assert(false); // todo: identifiant inconnu.
                }
                else
                {
                    identifierNode->ResolveReference(foundReference);
                }
            }
        }
        break ;
    case TOdlAstNodeType::NAMESPACE:
        {
            TOdlAstNodeNamespaceDeclaration* namespaceDeclarationNode = parAstNode->CastNode<TOdlAstNodeNamespaceDeclaration>();
            VisitAst(parAstNode, parContext, ResolveValueIdentifier);
        }
        break ;
	case TOdlAstNodeType::NAMED_DECLARATION:
        {
            TOdlAstNodeNamedDeclaration* namedDeclarationNode = parAstNode->CastNode<TOdlAstNodeNamedDeclaration>();
            VisitAst(parAstNode, parContext, ResolveValueIdentifier);
        }
		break ;
	case TOdlAstNodeType::TEMPLATE_OBJECT_DECLARATION:
		{
            TOdlAstNodeTemplateObjectDeclaration* templateObjectDeclaration = parAstNode->CastNode<TOdlAstNodeTemplateObjectDeclaration>();
			// resolve resolve arguments.
            VisitAst(parAstNode, parContext, ResolveValueIdentifier);
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
    case TOdlAstNodeType::TEMPLATE_NAMESPACE_INSTANCIATION:
        {
            TOdlAstNodeTemplateNamespaceInstanciation* templateNamespaceInstanciationNode = parAstNode->CastNode<TOdlAstNodeTemplateNamespaceInstanciation>();
            TOdlAstNodeIdentifier* targetNamespaceIdentifier = templateNamespaceInstanciationNode->TargetTemplateNamespaceIdentifierPointer();
            ResolveValueIdentifier(targetNamespaceIdentifier, parContext);
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
    TNamedDeclarationStack staticNamespaceStack;
    TNamedDeclarationStack dynamicNamespaceStack;
    TInterpretContext context(databasePath, staticNamespaceStack, dynamicNamespaceStack);
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
            TNamedDeclarationStack staticNamespaceStack;
            TNamedDeclarationStack dynamicNamespaceStack;
            TInterpretContext InstanciateObjectContext(databasePath, staticNamespaceStack, dynamicNamespaceStack);
            VisitAst(parAstNode, InstanciateObjectContext, InstanciateObjects);
            
            // fill objets properties.
            assert(databasePath.empty());
            assert(staticNamespaceStack.empty());
            assert(dynamicNamespaceStack.empty());
            TFillObjectPropertiesInterpretContext fillObjectsPropertiesContext(databasePath, staticNamespaceStack, dynamicNamespaceStack, nullptr, nullptr);
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
