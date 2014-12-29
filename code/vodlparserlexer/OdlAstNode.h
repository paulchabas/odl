#ifndef ODL_AST_NODE_H
# define ODL_AST_NODE_H

#include "OdlTokenDatabase.h"

namespace odl
{
//-------------------------------------------------------------------------------
//*******************************************************************************
//-------------------------------------------------------------------------------
namespace TOdlAstNodeType
{
    static const int VALUE_MASK = 0x80000000;

	enum TType
	{
        UNKNOWN									= 0,
        IDENTIFIER								= 1 | VALUE_MASK,
		VALUE_STRING							= 2 | VALUE_MASK,
        VALUE_INTEGER							= 3 | VALUE_MASK,
        VALUE_FLOAT								= 4 | VALUE_MASK,
        OPERATOR								= 5,
        PROPERTY_DECLARATION					= 6,
        PROPERTY_DECLARATION_LIST				= 7,
        OBJECT_DECLARATION						= 8 | VALUE_MASK,
        NAMESPACE								= 9,
		NAMED_DECLARATION						= 10,
        OPERATION								= 11,
		VALUE_VECTOR							= 12 | VALUE_MASK,
        TEMPLATE_OBJECT_DECLARATION				= 13,
        TEMPLATE_OBJECT_INSTANCIATION			= 14 | VALUE_MASK,
        TEMPLATE_NAMESPACE_INSTANCIATION        = 15,
        TEMPLATE_DECLARATION_PARAMETER          = 16,
		TEMPLATE_DECLARATION_PARAMETER_LIST		= 17,
		TEMPLATE_INSTANCIATION_EXPRESSION_LIST	= 18,
        VALUE_BOOLEAN                           = 19 | VALUE_MASK,
	};
}
//-------------------------------------------------------------------------------
namespace TOdlAstNodeOperatorType
{
    enum TType
    {
        OPERATOR_NONE,
        OPERATOR_PLUS,
        OPERATOR_MINUS,
        OPERATOR_MULTIPLY,
        OPERATOR_DIVIDE,
        OPERATOR_MODULO,
    };
}
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
class TOdlAstNodeNamedDeclaration;
//-------------------------------------------------------------------------------
class TOdlNamedDeclarationStack
{
public:
    TOdlNamedDeclarationStack();

    void Push(TOdlAstNodeNamedDeclaration const* parNamedDeclaration);
    void Pop(TOdlAstNodeNamedDeclaration const* parNamedDeclaration);

    bool Empty() const { return FStack.empty(); }
    size_t Size() const { return FStack.size(); }

    TOdlAstNodeNamedDeclaration const* operator [] (int parIndex) const
    {
        return FStack[parIndex];
    }

    TOdlDatabasePath ToDatabasePath() const;
    std::string ToDatabasePathString() const;

private:
    std::vector< TOdlAstNodeNamedDeclaration const* > FStack;
};
//-------------------------------------------------------------------------------
//*******************************************************************************
//-------------------------------------------------------------------------------
class TOdlAstNode
{
public:
    TOdlAstNode(TOdlAstNodeType::TType parAstNodeType);
    TOdlAstNode();
    virtual ~TOdlAstNode();

    template < typename T >
    T const* CastNode() const
    {
        assert(dynamic_cast<T const*>(this) != nullptr);
        return static_cast<T const*>(this);
    }
    
    template < typename T >
    T* CastNode()
    {
        assert(dynamic_cast<T*>(this) != nullptr);
        return static_cast<T*>(this);
    }

    TOdlAstNodeType::TType AstNodeType() const { return FAstNodeType; }

    void PrettyPrint(std::ostringstream& parOss) const;

private:
    void PrettyPrintWithIndentLevel(std::ostringstream& parOss, int parIndentLevel) const;

private:
    TOdlAstNodeType::TType          FAstNodeType;
};
//-------------------------------------------------------------------------------
//*******************************************************************************
//-------------------------------------------------------------------------------
class TOdlAstNodeIdentifier;
class TOdlAstNodeNamedDeclaration;
class TOdlAstNodeTemplateParameterList;
//-------------------------------------------------------------------------------
//*******************************************************************************
//-------------------------------------------------------------------------------
class TOdlAstNodeExpression : public TOdlAstNode
{
public:
    TOdlAstNodeExpression(TOdlAstNodeType::TType parOdlAstNodeType) :
        TOdlAstNode(parOdlAstNodeType)
    {
    }
};
//-------------------------------------------------------------------------------
//*******************************************************************************
//-------------------------------------------------------------------------------
class TOdlAstNodeIdentifier : public TOdlAstNodeExpression
{
    typedef TOdlAstNodeExpression parent_type;
public:
    TOdlAstNodeIdentifier(std::string const& parIdentifier) :
        parent_type(TOdlAstNodeType::IDENTIFIER),
        FIdentifier(parIdentifier),
        FResolvedReferenceWeak(nullptr),
        FIsReferenceToResolve(false)
    {

    }

    virtual ~TOdlAstNodeIdentifier()
    {

    }

    std::string const& Identifier() const { return FIdentifier; }
    
    void ResolveReference(TOdlAstNodeNamedDeclaration const* parNodeReference)
    {
        assert(parNodeReference != nullptr);
        assert(FIsReferenceToResolve);
        FResolvedReferenceWeak = parNodeReference;
        FIsReferenceToResolve = false;
    }

    TOdlAstNodeNamedDeclaration const* ResolvedReference() const { assert(!FIsReferenceToResolve); assert(FResolvedReferenceWeak != nullptr); return FResolvedReferenceWeak; }

    void SetAsReferenceToResolve() { FIsReferenceToResolve = true; }
    bool IsReferenceToResolve() const { return FIsReferenceToResolve; }

private:
    std::string FIdentifier;
    // {TODO} Paul(2014/12/23)  make a reference type to resolve.
    TOdlAstNodeNamedDeclaration const* FResolvedReferenceWeak;
    bool FIsReferenceToResolve;
};
//-------------------------------------------------------------------------------
//*******************************************************************************
//-------------------------------------------------------------------------------
class TOdlAstNodeNamedDeclaration : public TOdlAstNodeExpression
{
    typedef TOdlAstNodeExpression parent_type;
public:

    TOdlAstNodeNamedDeclaration(TOdlAstNodeType::TType parAstNodeType, 
                                TOdlAstNodeIdentifier* parIdentifier, 
                                TOdlAstNodeExpression* parExpression,
                                TOdlAstNodeTemplateParameterList* parTemplateParameterList) :
        parent_type(parAstNodeType),
        FIdentifierPointer(parIdentifier),
        FExpressionPointer(parExpression),
        FTemplateParameterListPointer(nullptr),
        FAnonymousDeclaration(false)
    {

    }

    TOdlAstNodeNamedDeclaration(TOdlAstNodeIdentifier* parIdentifier, 
                                TOdlAstNodeExpression* parExpression,
                                TOdlAstNodeTemplateParameterList* parTemplateParameterList) :
        parent_type(TOdlAstNodeType::NAMED_DECLARATION),
        FIdentifierPointer(parIdentifier),
        FExpressionPointer(parExpression),
        FTemplateParameterListPointer(nullptr),
        FAnonymousDeclaration(false)
    {
        if (parTemplateParameterList != nullptr)
            SetTemplateParameterListPointer(parTemplateParameterList);
    }

    virtual ~TOdlAstNodeNamedDeclaration();

    void SetIdentifierPointer(TOdlAstNodeIdentifier* parIdentifierPointer)
    {
        assert(FIdentifierPointer == nullptr);
        FIdentifierPointer = parIdentifierPointer;
    }

    void SetTemplateParameterListPointer(TOdlAstNodeTemplateParameterList* parTemplateParameterListPointer);

    void AutoGenerateIdentifierIfNone();
    bool IsAnonymousDeclaration() const { return FAnonymousDeclaration; }
    bool IsTemplate() const { return FTemplateParameterListPointer != nullptr; }

    TOdlAstNodeIdentifier* IdentifierPointer() const { assert(FIdentifierPointer != NULL); return FIdentifierPointer; }
    TOdlAstNodeIdentifier* IdentifierPointer_IFP() const { return FIdentifierPointer; }
    TOdlAstNodeExpression* ExpressionPointer() const { return FExpressionPointer; }
    TOdlAstNodeTemplateParameterList const* TemplateParameterListPointer() const { return FTemplateParameterListPointer; }
    

    void SetFullDatabasePath(TOdlNamedDeclarationStack const& parFullDatabasePath)
    {
        FFullDatabasePath = parFullDatabasePath;
    }

    TOdlNamedDeclarationStack const& StaticDatabasePath() const { return FFullDatabasePath; }

	TOdlDatabasePath FullDatabasePath() const { return FFullDatabasePath.ToDatabasePath(); }

private:
    TOdlAstNodeIdentifier*                  FIdentifierPointer;
    TOdlAstNodeExpression*                  FExpressionPointer;
    TOdlAstNodeTemplateParameterList*       FTemplateParameterListPointer;
    TOdlNamedDeclarationStack	            FFullDatabasePath;
    bool                                    FAnonymousDeclaration;
};
//-------------------------------------------------------------------------------
//*******************************************************************************
//-------------------------------------------------------------------------------
class TOdlAstNodePropertyDeclaration : public TOdlAstNodeNamedDeclaration
{
    typedef TOdlAstNodeNamedDeclaration parent_type;
public:

    TOdlAstNodePropertyDeclaration(TOdlAstNodeIdentifier* parIdentifier, TOdlAstNodeExpression* parExpression) :
        parent_type(TOdlAstNodeType::PROPERTY_DECLARATION, parIdentifier, parExpression, nullptr)
    {

    }

private:
    
};
//-------------------------------------------------------------------------------
//*******************************************************************************
//-------------------------------------------------------------------------------
class TOdlAstNodePropertyDeclarationList : public TOdlAstNode
{
    typedef TOdlAstNode parent_type;
public:
    TOdlAstNodePropertyDeclarationList() :
        parent_type(TOdlAstNodeType::PROPERTY_DECLARATION_LIST)
    {

    }

    virtual ~TOdlAstNodePropertyDeclarationList()
    {
        for (TOdlAstNodePropertyDeclaration* propertyDeclaration: FPropertyDeclarationList)
        {
            delete propertyDeclaration;
        }
    }

    void AppendPropertyDeclaration(TOdlAstNodePropertyDeclaration* parExpression)
    {
        FPropertyDeclarationList.push_back(parExpression);
    }

    std::vector< TOdlAstNodePropertyDeclaration* > const& PropertyDeclarationList() const { return FPropertyDeclarationList; }

private:
    std::vector< TOdlAstNodePropertyDeclaration* >   FPropertyDeclarationList;
};
//-------------------------------------------------------------------------------
//*******************************************************************************
//-------------------------------------------------------------------------------
class TOdlAstNodeValue : public TOdlAstNodeExpression
{
private:
    typedef TOdlAstNodeExpression parent_type;
public:
    TOdlAstNodeValue(std::string const& parValueString) :
        parent_type(TOdlAstNodeType::VALUE_STRING),
        FValueString(parValueString),
        FValueInteger(0),
        FValueFloat(0.0f),
        FValueBoolean(false)
    {
        
    }

    TOdlAstNodeValue(float parValueFloat) :
        parent_type(TOdlAstNodeType::VALUE_FLOAT),
        FValueString(),
        FValueInteger(0),
        FValueFloat(parValueFloat),
        FValueBoolean(false)
    {
        
    }

    TOdlAstNodeValue(int parValueInteger) :
        parent_type(TOdlAstNodeType::VALUE_INTEGER),
        FValueString(),
        FValueInteger(parValueInteger),
        FValueFloat(0.0f),
        FValueBoolean(false)
    {
        
    }

    TOdlAstNodeValue(bool parValueBoolean) :
        parent_type(TOdlAstNodeType::VALUE_BOOLEAN),
        FValueString(),
        FValueInteger(0),
        FValueFloat(0.0f),
        FValueBoolean(parValueBoolean)
    {
        
    }

    int ValueInteger() const { return FValueInteger; }
    float ValueFloat() const { return FValueFloat; }
    std::string const& ValueString() const { return FValueString; }
    bool ValueBoolean() const { return FValueBoolean; }

private:
    std::string                     FValueString;
    int                             FValueInteger;
    float                           FValueFloat;
    bool                            FValueBoolean;
};
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
class TOdlAstNodeValueVector : public TOdlAstNodeExpression
{
    typedef TOdlAstNodeExpression parent_type;
public:
    TOdlAstNodeValueVector() :
        parent_type(TOdlAstNodeType::VALUE_VECTOR)
    {

    }

    virtual ~TOdlAstNodeValueVector()
    {
        for (size_t i = 0; i < FVectorContent.size(); ++i)
        {
            TOdlAstNode const* node = FVectorContent[i];
            delete node;
        }
    }

    void Vector_AppendItem(TOdlAstNodeExpression* parVectorItem)
    {
        FVectorContent.push_back(parVectorItem);
    }

    std::vector< TOdlAstNodeExpression* >& VectorContent() { return FVectorContent; }
    std::vector< TOdlAstNodeExpression* > const& VectorContent() const { return FVectorContent; }

    
private:
    std::vector< TOdlAstNodeExpression* >  FVectorContent;
};
//-------------------------------------------------------------------------------
//*******************************************************************************
//-------------------------------------------------------------------------------
class TOdlAstNodeTypedSyntax : public TOdlAstNodeExpression
{
    typedef TOdlAstNodeExpression parent_type;
public:
    TOdlAstNodeTypedSyntax(TOdlAstNodeType::TType parOdlAstNodeType, TOdlAstNodeIdentifier* parTypeIdentifier) :
        parent_type(parOdlAstNodeType),
        FTypeIdentifierPointer(parTypeIdentifier)
    {
        assert((parTypeIdentifier != nullptr) ? (parTypeIdentifier->AstNodeType() == TOdlAstNodeType::IDENTIFIER) : true);
    }

    virtual ~TOdlAstNodeTypedSyntax()
    {
        delete FTypeIdentifierPointer;
    }

    TOdlAstNodeIdentifier* TypeIdentifierPointer() const { assert(FTypeIdentifierPointer != NULL); return FTypeIdentifierPointer; }

    bool IsNullPtr() const
    {
        if (FTypeIdentifierPointer == nullptr)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

private:
    TOdlAstNodeIdentifier*  FTypeIdentifierPointer;    
};
//-------------------------------------------------------------------------------
//*******************************************************************************
//-------------------------------------------------------------------------------
class TOdlAstNodeTemplateParameter : public TOdlAstNodeNamedDeclaration
{
    typedef TOdlAstNodeNamedDeclaration parent_type;
public:
    TOdlAstNodeTemplateParameter(TOdlAstNodeIdentifier* parIdentifier, TOdlAstNodeExpression* parExpression) :
        parent_type(TOdlAstNodeType::TEMPLATE_DECLARATION_PARAMETER, parIdentifier, parExpression, nullptr),
        FTemplateParameterIndex(-1),
        FTemplateHolderWeakReference(nullptr)
    {

    }

    void SetTemplateParameterIndex(i32 parIndex)
    {
        assert(parIndex >= 0);
        FTemplateParameterIndex = parIndex;
    }

    void SetTemplateHolderDeclarationWeakReference(TOdlAstNodeNamedDeclaration const* parTemplateHolderWeakReference)
    {
        FTemplateHolderWeakReference = parTemplateHolderWeakReference;
    }

    TOdlAstNodeNamedDeclaration const* TemplateHolderWeakReference() const { assert(FTemplateHolderWeakReference != nullptr); return FTemplateHolderWeakReference; }

    i32 TemplateParameterIndex() const { assert(FTemplateParameterIndex >= 0); return FTemplateParameterIndex; }

private:
    // Paul(2014/12/26) bof: each parameters known the template holder.
    TOdlAstNodeNamedDeclaration const* FTemplateHolderWeakReference;
    i32 FTemplateParameterIndex;
};
//-------------------------------------------------------------------------------
//*******************************************************************************
//-------------------------------------------------------------------------------
class TOdlAstNodeTemplateParameterList : public TOdlAstNode
{
    typedef TOdlAstNode parent_type;
public:
    TOdlAstNodeTemplateParameterList() :
        parent_type(TOdlAstNodeType::TEMPLATE_DECLARATION_PARAMETER_LIST)
    {

    }

    virtual ~TOdlAstNodeTemplateParameterList()
    {
        for (TOdlAstNodeTemplateParameter* parameter : FTemplateParameterList)
        {
            delete parameter;
        }
    }

    void AppendTemplateParameter(TOdlAstNodeTemplateParameter* parNamedDeclaration)
    {
        parNamedDeclaration->SetTemplateParameterIndex((i32) FTemplateParameterList.size());
        FTemplateParameterList.push_back(parNamedDeclaration);
    }

    std::vector< TOdlAstNodeTemplateParameter* > const& TemplateParameterList() const { return FTemplateParameterList; }

private:
    std::vector< TOdlAstNodeTemplateParameter* >     FTemplateParameterList;
};
//-------------------------------------------------------------------------------
//*******************************************************************************
//-------------------------------------------------------------------------------
class TOdlAstNodeTemplateExpressionList : public TOdlAstNode
{
    typedef TOdlAstNode parent_type;
public:
    TOdlAstNodeTemplateExpressionList() :
        parent_type(TOdlAstNodeType::TEMPLATE_INSTANCIATION_EXPRESSION_LIST)
    {

    }

    virtual ~TOdlAstNodeTemplateExpressionList()
    {
        for (TOdlAstNodeExpression* expression : FTemplateParametersExpressionList)
        {
            delete expression;
        }
    }

    void AppendTemplateExpressionParameter(TOdlAstNodeExpression* parExpression)
    {
        FTemplateParametersExpressionList.push_back(parExpression);
    }

    std::vector< TOdlAstNodeExpression* > const& TemplateParametersExpressionList() const { return FTemplateParametersExpressionList; }

    TOdlAstNodeExpression const* ExpressionByIndex(size_t parIndex) const
    {
        assert(!FTemplateParametersExpressionList.empty());
        assert(parIndex < FTemplateParametersExpressionList.size()); // Paul(2014/12/27) ERROR not enough parameters
        return FTemplateParametersExpressionList[parIndex];
    }

private:
    std::vector< TOdlAstNodeExpression* >   FTemplateParametersExpressionList;
};
//-------------------------------------------------------------------------------
//*******************************************************************************
//-------------------------------------------------------------------------------
class TOdlAstNodeTemplateObjectDeclaration : public TOdlAstNodeTypedSyntax
{
    typedef TOdlAstNodeTypedSyntax parent_type;
public:

    TOdlAstNodeTemplateObjectDeclaration(TOdlAstNodeIdentifier* parTypeIdentifier, TOdlAstNodePropertyDeclarationList* parPropertyDeclarationList) :
        parent_type(TOdlAstNodeType::TEMPLATE_OBJECT_DECLARATION, parTypeIdentifier),
        FNamedDeclarationWeakRef(nullptr),
        FPropertyDeclarationListPointer(parPropertyDeclarationList)
    {
        assert(parTypeIdentifier != nullptr); // Paul(2014/12/21)  cannot be null in that case.
        assert(FPropertyDeclarationListPointer != nullptr);
        assert(FPropertyDeclarationListPointer->AstNodeType() == TOdlAstNodeType::PROPERTY_DECLARATION_LIST);
    }

    virtual ~TOdlAstNodeTemplateObjectDeclaration()
    {
        delete FPropertyDeclarationListPointer;
    }

    void SetNamedDeclarationWeakReference(TOdlAstNodeNamedDeclaration* parNamedDeclaration)
    {
        assert(FNamedDeclarationWeakRef == nullptr);
        FNamedDeclarationWeakRef = parNamedDeclaration;
    };

    TOdlAstNodeNamedDeclaration* NamedDeclarationWeakRef() const { return FNamedDeclarationWeakRef; }

    TOdlAstNodeIdentifier const* IdentifierPointer() const 
    { 
        assert(FNamedDeclarationWeakRef != nullptr);
        return FNamedDeclarationWeakRef->IdentifierPointer(); 
    }

    TOdlAstNodePropertyDeclarationList* PropertyDeclarationListPointer() const { return FPropertyDeclarationListPointer; }
    TOdlAstNodeTemplateParameterList const*   TemplateParameterListPointer() const
    { 
        assert(FNamedDeclarationWeakRef != nullptr);
        return FNamedDeclarationWeakRef->TemplateParameterListPointer(); 
    }

private:
    TOdlAstNodeNamedDeclaration*        FNamedDeclarationWeakRef;
    TOdlAstNodePropertyDeclarationList* FPropertyDeclarationListPointer;
};
//-------------------------------------------------------------------------------
//*******************************************************************************
//-------------------------------------------------------------------------------
class TOdlAstNodeTemplateObjectInstanciation : public TOdlAstNodeTypedSyntax
{
    typedef TOdlAstNodeTypedSyntax parent_type;
public:
    TOdlAstNodeTemplateObjectInstanciation(TOdlAstNodeIdentifier* parTypeIdentifier, TOdlAstNodeTemplateExpressionList* parExpressionListPointer) :
        parent_type(TOdlAstNodeType::TEMPLATE_OBJECT_INSTANCIATION, parTypeIdentifier),
        FExpressionListPointer(parExpressionListPointer),
        FNamedDeclarationWeakRef(nullptr)
    {
        assert(FExpressionListPointer != nullptr);
    }

    virtual ~TOdlAstNodeTemplateObjectInstanciation()
    {
        delete FExpressionListPointer;
    }

    void SetNamedDeclarationWeakReference(TOdlAstNodeNamedDeclaration* parNamedDeclaration)
    {
        assert(FNamedDeclarationWeakRef == nullptr);
        FNamedDeclarationWeakRef = parNamedDeclaration;
    };

    TOdlAstNodeTemplateExpressionList const* TemplateExpressionListPointer() const { return FExpressionListPointer; }
    TOdlAstNodeNamedDeclaration* NamedDeclarationWeakRef() const { assert(FNamedDeclarationWeakRef != nullptr); return FNamedDeclarationWeakRef; }

private:
    TOdlAstNodeTemplateExpressionList* FExpressionListPointer;
    TOdlAstNodeNamedDeclaration* FNamedDeclarationWeakRef;
};
//-------------------------------------------------------------------------------
//*******************************************************************************
//-------------------------------------------------------------------------------
class TOdlAstNodeObjectDeclaration : public TOdlAstNodeTypedSyntax
{
    typedef TOdlAstNodeTypedSyntax parent_type;
public:

    TOdlAstNodeObjectDeclaration() :
        parent_type(TOdlAstNodeType::OBJECT_DECLARATION, nullptr),
        FNamedDeclarationWeakRef(nullptr),
        FPropertyDeclarationListPointer(nullptr)
    {

    }

    TOdlAstNodeObjectDeclaration(TOdlAstNodeIdentifier* parTypeIdentifier, TOdlAstNode* parPropertyDeclarationListPointer) :
        parent_type(TOdlAstNodeType::OBJECT_DECLARATION, parTypeIdentifier),
        FNamedDeclarationWeakRef(nullptr),
        FPropertyDeclarationListPointer(parPropertyDeclarationListPointer)
    {
        assert(parTypeIdentifier != nullptr);
        assert(FPropertyDeclarationListPointer != nullptr);
        assert(FPropertyDeclarationListPointer->AstNodeType() == TOdlAstNodeType::PROPERTY_DECLARATION_LIST);
    }

    virtual ~TOdlAstNodeObjectDeclaration()
    {
        delete FPropertyDeclarationListPointer;
    }

    void SetNamedDeclarationWeakReference(TOdlAstNodeNamedDeclaration* parNamedDeclaration)
    {
        assert(FNamedDeclarationWeakRef == nullptr);
        FNamedDeclarationWeakRef = parNamedDeclaration;
    };

    TOdlAstNodeNamedDeclaration* NamedDeclarationWeakRef() const { return FNamedDeclarationWeakRef; }
    
    TOdlAstNodeIdentifier* IdentifierPointer_IFP() const 
    { 
        if (FNamedDeclarationWeakRef != nullptr)
        {
            return FNamedDeclarationWeakRef->IdentifierPointer();
        }

        return nullptr;
    }

    TOdlAstNode* PropertyDeclarationListPointer() const { return FPropertyDeclarationListPointer; }

private:
    TOdlAstNodeNamedDeclaration* FNamedDeclarationWeakRef;
    TOdlAstNode* FPropertyDeclarationListPointer;
};
//-------------------------------------------------------------------------------
//*******************************************************************************
//-------------------------------------------------------------------------------
class TOdlAstNodeNamespaceDeclaration : public TOdlAstNodeNamedDeclaration
{
    typedef TOdlAstNodeNamedDeclaration parent_type;
public:
    TOdlAstNodeNamespaceDeclaration() :
        parent_type(TOdlAstNodeType::NAMESPACE, nullptr, nullptr, nullptr)
    {
    }

    virtual ~TOdlAstNodeNamespaceDeclaration()
    {
        for (TOdlAstNode* namedDeclarationNode : FNamedDeclarationList)
        {
            delete namedDeclarationNode;
        }
    }

    bool IsGlobalNamespace() const { return IdentifierPointer_IFP() == nullptr; }

    std::vector< TOdlAstNodeNamedDeclaration* >& NamespaceContent() { return FNamedDeclarationList; }
    std::vector< TOdlAstNodeNamedDeclaration* > const& NamespaceContent() const { return FNamedDeclarationList; }

    void AppendNamedDeclaration(TOdlAstNodeNamedDeclaration* parDeclaration)
    {
        FNamedDeclarationList.push_back(parDeclaration);
    }

private:
    std::vector< TOdlAstNodeNamedDeclaration* >     FNamedDeclarationList;
};
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
class TOdlAstNodeTemplateNamespaceInstanciation : public TOdlAstNodeExpression
{
    typedef TOdlAstNodeExpression parent_type;
public:
    TOdlAstNodeTemplateNamespaceInstanciation(TOdlAstNodeIdentifier* parTargetTemplateNamespaceIdentifierPointer, 
                                              TOdlAstNodeTemplateExpressionList* parTemplateExpressionList) :
        parent_type(TOdlAstNodeType::TEMPLATE_NAMESPACE_INSTANCIATION)
    {
        assert(parTargetTemplateNamespaceIdentifierPointer != nullptr);
        assert(parTemplateExpressionList != nullptr);

        assert(parTargetTemplateNamespaceIdentifierPointer->AstNodeType() == TOdlAstNodeType::IDENTIFIER);
        assert(parTemplateExpressionList->AstNodeType() == TOdlAstNodeType::TEMPLATE_INSTANCIATION_EXPRESSION_LIST);

        FTargetTemplateNamespaceIdentifierPointer = parTargetTemplateNamespaceIdentifierPointer;
        FTemplateExpressionListPointer = parTemplateExpressionList;
    }

    virtual ~TOdlAstNodeTemplateNamespaceInstanciation()
    {
        delete FTargetTemplateNamespaceIdentifierPointer;
        delete FTemplateExpressionListPointer;
    }

    void SetNamedDeclarationWeakReference(TOdlAstNodeNamedDeclaration const* parNamedDeclarationWeakReference)
    {
        FNamedDeclarationWeakReference = parNamedDeclarationWeakReference;
    }

    TOdlAstNodeNamedDeclaration const* NamedDeclarationWeakReference() const { return FNamedDeclarationWeakReference; }
    TOdlAstNodeIdentifier* TargetTemplateNamespaceIdentifierPointer() const { return FTargetTemplateNamespaceIdentifierPointer; }
    TOdlAstNodeTemplateExpressionList const* TemplateExpressionListPointer() const { return FTemplateExpressionListPointer; }

private:
    TOdlAstNodeIdentifier* FTargetTemplateNamespaceIdentifierPointer;
    TOdlAstNodeTemplateExpressionList* FTemplateExpressionListPointer;
    TOdlAstNodeNamedDeclaration const* FNamedDeclarationWeakReference;
};
//-------------------------------------------------------------------------------
//*******************************************************************************
//-------------------------------------------------------------------------------
class TOdlAstNodeOperator : public TOdlAstNode
{
public:
    TOdlAstNodeOperator(TOdlAstNodeOperatorType::TType parOperatorType) :
        TOdlAstNode(TOdlAstNodeType::OPERATOR),
        FOperatorType(parOperatorType)
    {

    }

    TOdlAstNodeOperatorType::TType OperatorType() const { return FOperatorType; }

private:
    TOdlAstNodeOperatorType::TType  FOperatorType;
};
//-------------------------------------------------------------------------------
//*******************************************************************************
//-------------------------------------------------------------------------------
class TOdlAstNodeOperation : public TOdlAstNodeExpression
{
    typedef TOdlAstNodeExpression parent_type;
public:
    TOdlAstNodeOperation(TOdlAstNodeExpression* parLeftExpression,
                         TOdlAstNodeOperator* parOperator,
                         TOdlAstNodeExpression* parRightExpression) :
        parent_type(TOdlAstNodeType::OPERATION),
        FLeftExpressionPointer(nullptr),
        FOperatorExpressionPointer(nullptr),
        FRightExpressionPointer(nullptr)
    {
        assert(parRightExpression != nullptr);
        assert(parOperator != nullptr);
        if (parOperator->OperatorType() != TOdlAstNodeOperatorType::OPERATOR_MINUS)
        {
            assert(parLeftExpression != nullptr);
        }
    
        FLeftExpressionPointer = parLeftExpression;
        FOperatorExpressionPointer = parOperator;
        FRightExpressionPointer = parRightExpression;    
    }

    virtual ~TOdlAstNodeOperation()
    {
        delete FLeftExpressionPointer;
        delete FOperatorExpressionPointer;
        delete FRightExpressionPointer;
    }

    TOdlAstNodeExpression* LeftExpressionPointer() const { return FLeftExpressionPointer; }
    TOdlAstNodeOperator* OperatorExpressionPointer() const { return FOperatorExpressionPointer; }
    TOdlAstNodeExpression* RightExpressionPointer() const { return FRightExpressionPointer; }

private:
    TOdlAstNodeExpression*  FLeftExpressionPointer;
    TOdlAstNodeOperator*    FOperatorExpressionPointer;
    TOdlAstNodeExpression*  FRightExpressionPointer;
};
//-------------------------------------------------------------------------------
//*******************************************************************************
//-------------------------------------------------------------------------------
}


#endif