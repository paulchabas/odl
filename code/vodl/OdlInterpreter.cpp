#include "StdAfx.h"
#include "OdlInterpreter.h"

#include "OdlExpression.h"
#include "Odl.h"
#include "OdlAstNode.h"
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
	case TOdlAstNodeType::EXPRESSION:
		{
            TOdlAstNode* parLeft = parAstNode->LeftExpressionPointer();
            TOdlAstNode* parRight = parAstNode->RightExpressionPointer();
            if (parLeft != NULL)
                (*parCallback)(parLeft, parContext);
            (*parCallback)(parRight, parContext);
		}
		break ;
	case TOdlAstNodeType::VALUE_VECTOR:
		{
            std::vector< TOdlAstNode* >& childs = parAstNode->VectorContent();
            size_t childCount = childs.size();
		    for (size_t i = 0; i < childCount; ++i)
            {
                size_t invI = childCount - i - 1;
                TOdlAstNode* child = childs[invI];
                (*parCallback)(child, parContext);
            }
		}
		break ;
    case TOdlAstNodeType::PROPERTY_DECLARATION:
        {
            TOdlAstNode* expression = parAstNode->ExpressionPointer();
            (*parCallback)(expression, parContext);
        }
        break ;
    case TOdlAstNodeType::PROPERTY_DECLARATION_LIST:
        {
            std::vector< TOdlAstNode* >& properties = parAstNode->PropertyList();
		    for (size_t i = 0; i < properties.size(); ++i)
            {
                TOdlAstNode* property = properties[i];
                (*parCallback)(property, parContext);
            }
        }
        break ;
    case TOdlAstNodeType::OBJECT_DECLARATION:
        {
			if (!parAstNode->IsNullPtr())
			{
				TOdlAstNode* propertyDeclarationList = parAstNode->PropertyDeclarationListPointer();
				(*parCallback)(propertyDeclarationList, parContext);
			}
        }
        break ;
    case TOdlAstNodeType::NAMESPACE:
        {
            std::vector< TOdlAstNode* >& namedDeclarations = parAstNode->NamespaceContent();
		    for (size_t i = 0; i < namedDeclarations.size(); ++i)
            {
                TOdlAstNode* namedDeclaration = namedDeclarations[i];
                (*parCallback)(namedDeclaration, parContext);
            }
        }
        break ;
	case TOdlAstNodeType::NAMED_DECLARATION:
		{
			#ifdef ODL_ENABLE_VERBOSE_DEBUG
			TOdlAstNode* name = parAstNode->IdentifierPointer();
			assert(name != nullptr);
			#endif

			TOdlAstNode* expression = parAstNode->ExpressionPointer();
			(*parCallback)(expression, parContext);
		}
		break ;
    case TOdlAstNodeType::OBJECT_TEMPLATE_DECLARATION:
        {
            // todo.
            int a = 0;
        }
        break ;
    case TOdlAstNodeType::OBJECT_TEMPLATE_INSTANCIATION:
        {
            // todo.
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
            TOdlAstNode const* identifier = parAstNode->IdentifierPointer_IFP();
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
			TOdlAstNode const* identifier = parAstNode->IdentifierPointer();
			std::string const& namespaceName = identifier->Identifier();
            parContext.EnterNamespace(namespaceName);
            VisitAst(parAstNode, parContext, InstanciateObjects);
			parContext.LeaveNamespace();
		}
		break;
    case TOdlAstNodeType::OBJECT_DECLARATION:
        {
			if (!parAstNode->IsNullPtr())
			{
				std::string autonamedObjectOrEmptyString = parAstNode->IdentifierPointer_IFP() ? parAstNode->IdentifierPointer_IFP()->Identifier() : "";
				std::string const& objectType = parAstNode->TypeIdentifierPointer()->Identifier();
				TMetaClassBase const* objectMetaClass = TOdlDatabase::Instance().FindRegisteredMetaClassByName_IFP(objectType.c_str());
				TOdlObject* odlObject = objectMetaClass->CreateObject();
            
				if (!autonamedObjectOrEmptyString.empty())
					parContext.EnterNamespace(autonamedObjectOrEmptyString);
				TOdlDatabasePath const& objectNamespaceAndName = parContext.DatabasePath();
				#ifdef ODL_ENABLE_VERBOSE_DEBUG
				std::string fordebug = objectNamespaceAndName.ToString();
				#endif
				TOdlDatabase::Instance().StoreObject(objectNamespaceAndName, odlObject, objectMetaClass);
				VisitAst(parAstNode, parContext, InstanciateObjects);
				if (!autonamedObjectOrEmptyString.empty())
					parContext.LeaveNamespace();
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
                                           TOdlAstNode const* parExpression,
                                           TOdlDatabasePath const& parDatabasePath)
{
    TMetaClassBase const* propertyMetaClassBase = parPropertyBase->Type();

    TOdlExpression const result = EvalExpression(parExpression, parDatabasePath);
    bool const propertySet = parPropertyBase->SetObjectPropertyByExpression_ROK(parObject, result);
    if (!propertySet)
    {
        assert(false);
    }
}
//-------------------------------------------------------------------------------
//*******************************************************************************
//-------------------------------------------------------------------------------
struct TFillObjectPropertiesInterpretContext : public TInterpretContext
{
public:
    TFillObjectPropertiesInterpretContext(TOdlDatabasePath& parDatabasePath) :
        TInterpretContext(parDatabasePath),
        FCurrentObject(nullptr),
        FMetaClassBase(nullptr)
    {
    }

    TFillObjectPropertiesInterpretContext(TOdlDatabasePath& parDatabasePath,
                                          TOdlObject* parCurrentObject, 
                                          TMetaClassBase const* parMetaClass) :
        TInterpretContext(parDatabasePath),
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
void FillObjectsProperties(TOdlAstNode* parAstNode, TInterpretContext& parContext)
{
    TOdlAstNodeType::TType nodeType = parAstNode->AstNodeType();
    switch (nodeType)
    {
    case TOdlAstNodeType::NAMESPACE:
        {
            TFillObjectPropertiesInterpretContext& context = static_cast<TFillObjectPropertiesInterpretContext&>(parContext);
            TOdlAstNode const* identifier = parAstNode->IdentifierPointer_IFP();;
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
            TFillObjectPropertiesInterpretContext& context = static_cast<TFillObjectPropertiesInterpretContext&>(parContext);
            TOdlAstNode const* identifier = parAstNode->IdentifierPointer_IFP();;
            assert(identifier != nullptr);
            std::string const& namespaceName = identifier->Identifier();
            context.EnterNamespace(namespaceName);
            VisitAst(parAstNode, parContext, FillObjectsProperties);
			context.LeaveNamespace();
        }
        break ;
    case TOdlAstNodeType::OBJECT_DECLARATION:
		{
			if (!parAstNode->IsNullPtr())
			{
				TFillObjectPropertiesInterpretContext& context = static_cast<TFillObjectPropertiesInterpretContext&>(parContext);

				std::string autonamedObjectOrEmptyString = parAstNode->IdentifierPointer_IFP() ? parAstNode->IdentifierPointer_IFP()->Identifier() : "";
				std::string const& objectType = parAstNode->TypeIdentifierPointer()->Identifier();
				TMetaClassBase const* objectMetaClass = TOdlDatabase::Instance().FindRegisteredMetaClassByName_IFP(objectType.c_str());
				TOdlObject* odlObject = objectMetaClass->CreateObject();
            
				if (!autonamedObjectOrEmptyString.empty())
					parContext.EnterNamespace(autonamedObjectOrEmptyString);
				TOdlDatabasePath& objectNamespaceAndName = parContext.DatabasePath();
				#ifdef ODL_ENABLE_VERBOSE_DEBUG
				std::string fordebug = objectNamespaceAndName.ToString();
				#endif

				TOdlObject* object = TOdlDatabase::Instance().GetObject(objectNamespaceAndName);
				TMetaClassBase const* metaClassBase = TOdlDatabase::Instance().FindRegisteredMetaClassByName_IFP(objectType.c_str());
				TFillObjectPropertiesInterpretContext newContext(objectNamespaceAndName, object, metaClassBase);
				VisitAst(parAstNode, newContext, FillObjectsProperties);
				if (!autonamedObjectOrEmptyString.empty())
					parContext.LeaveNamespace();
			}
        };
        break ;
    case TOdlAstNodeType::PROPERTY_DECLARATION:
        {
            TFillObjectPropertiesInterpretContext& context = static_cast< TFillObjectPropertiesInterpretContext& >(parContext);
            TOdlDatabasePath const& odlDatabasePath = context.DatabasePath();

			#if ODL_ENABLE_VERBOSE_DEBUG
			std::string debugString = odlDatabasePath.ToString();
			#endif

            TOdlObject* object = context.CurrentObject();
            TMetaClassBase const* objectMetaClassBase = context.MetaClassBase();
            std::string const& propertyName = parAstNode->IdentifierPointer()->Identifier();
            TPropertyBase const* propertyBase = objectMetaClassBase->PropertyByName(propertyName);
            TOdlAstNode const* expression = parAstNode->ExpressionPointer();
            EvalExpressionAndStoreProperty(object, objectMetaClassBase, propertyBase, expression, odlDatabasePath);

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
			std::string namespaceName = parAstNode->IdentifierPointer_IFP() ? parAstNode->IdentifierPointer_IFP()->Identifier() : std::string();
			if (!namespaceName.empty())
				parContext.EnterNamespace(namespaceName);
			VisitAst(parAstNode, parContext, AutoNameAnomymousObjectDeclaration);
			if (!namespaceName.empty())
				parContext.LeaveNamespace();
		}
		break ;
	case TOdlAstNodeType::NAMED_DECLARATION:
		{
			std::string namedDeclarationName = parAstNode->IdentifierPointer()->Identifier();
			parContext.EnterNamespace(namedDeclarationName);
			TOdlAstNode* expressionPointer = parAstNode->ExpressionPointer();
			if (expressionPointer->AstNodeType() == TOdlAstNodeType::OBJECT_DECLARATION)
			{
				if (expressionPointer->IsNullPtr())
					int a = 0;

				expressionPointer->SetAnonymousDeclaration(false);
			}
			VisitAst(parAstNode, parContext, AutoNameAnomymousObjectDeclaration);
			parContext.LeaveNamespace();
		}
		break ;
    case TOdlAstNodeType::OBJECT_DECLARATION:
        {
			// ... object declaration store name in IdentifierPointer...
		    // maybe should it be a named_declaration node construction as well.
            bool const isAnonymousObject = parAstNode->IsAnonymousDeclaration();
            if (isAnonymousObject)
            {
                parAstNode->AutoGenerateIdentifier();
            }
			std::string objectName = parAstNode->IdentifierPointer_IFP() ? parAstNode->IdentifierPointer_IFP()->Identifier() : std::string();
			if (!objectName.empty())
				parContext.EnterNamespace(objectName);
			#if ODL_ENABLE_VERBOSE_DEBUG
			std::string debug = parContext.DatabasePath().ToString();
			#endif
			parAstNode->SetFullDatabasePath(parContext.DatabasePath());

			VisitAst(parAstNode, parContext, AutoNameAnomymousObjectDeclaration);

			if (!objectName.empty())
				parContext.LeaveNamespace();
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
            if (parAstNode->IsReferenceToResolve())
            {
				TOdlAstNode const* foundReference = nullptr;

				TResolveValueIdentifierInterpretContext const& context = static_cast< TResolveValueIdentifierInterpretContext const& >(parContext);
				std::vector< TOdlAstNode* > const& parentNamespaces = context.Parents();
				size_t const parentNamespaceCount = parentNamespaces.size();

                std::string const& fullIdentifierToResolve = parAstNode->Identifier();

				TOdlDatabasePath searchedNameSpaceDatabasePath(fullIdentifierToResolve.c_str());
				if (searchedNameSpaceDatabasePath.size() <= 1)
				{
					// research name resolution in parent namespaces recursively
					std::string const& identifierToResolve = searchedNameSpaceDatabasePath.back().ToString();

					for (size_t i = 0; i < parentNamespaceCount && (foundReference == nullptr); ++i)
					{
						size_t const invI = parentNamespaceCount - i - 1;

						TOdlAstNode* namespaceCandidate = parentNamespaces[invI];
						std::vector< TOdlAstNode* > const& namespaceContent = namespaceCandidate->NamespaceContent();
						for (size_t j = 0; j < namespaceContent.size(); ++j)
						{
							TOdlAstNode* candidateNode = namespaceContent[j];
							std::string const& declarationIdentifier = candidateNode->IdentifierPointer()->Identifier();
							if (identifierToResolve == declarationIdentifier)
							{
								#if ODL_ENABLE_VERBOSE_DEBUG
								std::string pathDebug = context.DatabasePath().ToString();
								#endif
								parAstNode->ResolveReference(candidateNode);
								foundReference = candidateNode;
								break ;
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

							std::vector< TOdlAstNode* > const& rootNamespaceContent = rootNamespaceCandidate->NamespaceContent();
							for (size_t j = 0; j < rootNamespaceContent.size(); ++j)
							{
								TOdlAstNode* namedDeclarationNode = rootNamespaceContent[j];
								std::string const& namedDeclarationName = namedDeclarationNode->IdentifierPointer()->Identifier();

								if (namedDeclarationName == rootNamespaceToFind)
								{
									rootNamespace = rootNamespaceCandidate;
									goto rootNamespaceDone;
								}
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
								std::vector< TOdlAstNode* > const& childNamespaceContent =  childNamespace->NamespaceContent();
								for (size_t j = 0; j < childNamespaceContent.size(); ++j)
								{
									TOdlAstNode* candidateNamespace = childNamespaceContent[j];
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

								if (!childNamespaceFound)
									assert(false); // cannot find tutu in namespace tata for example: toto/tata/tutu/object
							}
						}

						if (childNamespace != nullptr)
						{
							// 3) search for the final named value and resolve.
							TOdlAstNode const* finalNamespace = childNamespace;
							std::string const& identifierToResolve = searchedNameSpaceDatabasePath.back().ToString();
							std::vector< TOdlAstNode* > const& namespaceContent = finalNamespace->NamespaceContent();
							for (size_t j = 0; j < namespaceContent.size(); ++j)
							{
								TOdlAstNode* candidateNode = namespaceContent[j];
								std::string const& declarationIdentifier = candidateNode->IdentifierPointer()->Identifier();
								if (candidateNode->AstNodeType() == TOdlAstNodeType::NAMED_DECLARATION)
								{
									if (identifierToResolve == declarationIdentifier)
									{
										#if ODL_ENABLE_VERBOSE_DEBUG
										std::string pathDebug = context.DatabasePath().ToString();
										#endif
										parAstNode->ResolveReference(candidateNode);
										foundReference = candidateNode;
										break ;
									}
								}
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
            TResolveValueIdentifierInterpretContext& context = static_cast< TResolveValueIdentifierInterpretContext& >(parContext);
            context.PushParentNode(parAstNode);
			std::string namespaceName = parAstNode->IdentifierPointer_IFP() != nullptr ? parAstNode->IdentifierPointer_IFP()->Identifier() : std::string();
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
            TResolveValueIdentifierInterpretContext& context = static_cast< TResolveValueIdentifierInterpretContext& >(parContext);
            context.PushParentNode(parAstNode);
			std::string const& namespaceName = parAstNode->IdentifierPointer()->Identifier();
			if (!namespaceName.empty())
				context.EnterNamespace(namespaceName);
            VisitAst(parAstNode, parContext, ResolveValueIdentifier);
			if (!namespaceName.empty())
				context.LeaveNamespace();
			context.PopParentNode();
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
            TFillObjectPropertiesInterpretContext fillObjectsPropertiesContext(databasePath);
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
