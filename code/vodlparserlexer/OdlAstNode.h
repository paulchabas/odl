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
    static const int EXPRESSION_MASK = 0x40000000;
    static const int TEMPLATE_MASK = 0x20000000;

	enum TType
	{
        UNKNOWN									= 0,
        IDENTIFIER								= 1 | EXPRESSION_MASK | VALUE_MASK,
		VALUE_STRING							= 2 | EXPRESSION_MASK | VALUE_MASK,
        VALUE_INTEGER							= 3 | EXPRESSION_MASK | VALUE_MASK,
        VALUE_FLOAT								= 4 | EXPRESSION_MASK | VALUE_MASK,
        OPERATOR								= 5,
        PROPERTY_DECLARATION					= 6,
        PROPERTY_DECLARATION_LIST				= 7,
        OBJECT_DECLARATION						= 8 | EXPRESSION_MASK | VALUE_MASK,
        NAMESPACE								= 9,
		NAMED_DECLARATION						= 10,
        EXPRESSION								= 11 | EXPRESSION_MASK,
		VALUE_VECTOR							= 12 | EXPRESSION_MASK | VALUE_MASK,
        OBJECT_TEMPLATE_DECLARATION				= 13 | TEMPLATE_MASK,
        OBJECT_TEMPLATE_INSTANCIATION			= 14 | TEMPLATE_MASK,
		TEMPLATE_DECLARATION_PARAMETER_LIST		= 15,
		TEMPLATE_INSTANCIATION_PARAMETER_LIST	= 16
	};
}

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
//*******************************************************************************
//-------------------------------------------------------------------------------
class TOdlAstNodeIdentifier;
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


	void BreakPoint();

    void SetIdentifierPointer(TOdlAstNodeIdentifier* parIdentifierPointer);
    void AutoGenerateIdentifier();

    void SetAsNamespace();
    void Namespace_AppendNamedDeclaration(TOdlAstNode* parDeclaration);
	void Namespace_SetTemplateParameterList(TOdlAstNode* parTemplateArgumentListPointer);

	void SetAsNamespaceTemplateInstanciation(TOdlAstNodeIdentifier* parIdentifier, TOdlAstNodeIdentifier* parTargetTemplateNamespaceIdentifierPointer, TOdlAstNode* parTemplateExpressionList);
    
    void SetAsObjectDeclaration(TOdlAstNodeIdentifier* parTypeIdentifier, TOdlAstNode* parPropertyList);
	void SetAsNullPtr();
	void SetAsNamedDeclaration(TOdlAstNodeIdentifier* parNameIdentifier, TOdlAstNode* parExpression);

    void SetAsObjectTemplateDeclaration(TOdlAstNodeIdentifier* parIdentifier, TOdlAstNodeIdentifier* parTypeIdentifier, TOdlAstNode* parTemplateParameterList, TOdlAstNode* parPropertyList);
    void SetAsTemplateInstanciation(TOdlAstNodeIdentifier* parTypeIdentifier, TOdlAstNode* parTemplateExpressionList);
	void SetAsTemplateDeclarationParameterList();
	void TemplateDeclarationParameterList_AppendParameter(TOdlAstNodeIdentifier* parIdentifier);

	void SetAsTemplateInstanciationParameterList();
	void TemplateInstanciationParameterList_AppendParameter(TOdlAstNode* parExpression);
	bool IsTemplateDeclarationParameter() const { return FIsTemplateDeclarationParameter; }
	void SetAsTemplateDeclarationParameter() { FIsTemplateDeclarationParameter = true; }


    void SetAsPropertyDeclarationList();
    void PropertyDeclarationList_AppendPropertyDeclaration(TOdlAstNode* parPropertyDeclarationNode);
    void SetAsPropertyDeclaration(TOdlAstNodeIdentifier* parIdentifier, TOdlAstNode* parExpression);

	void SetAnonymousDeclaration(bool parState) { FAnonymousDeclaration = parState; }

    void PrettyPrint(std::ostringstream& parOss) const;

    std::vector< TOdlAstNode* >& PropertyList() { return FPropertyList; }
    std::vector< TOdlAstNode* >& NamespaceContent() { return FNamedDeclarationList; }
    

    std::vector< TOdlAstNode* > const& PropertyList() const { return FPropertyList; }
    std::vector< TOdlAstNode* > const& NamespaceContent() const { return FNamedDeclarationList; }

	std::vector< TOdlAstNode* > const& TemplateParameterList() const { return FTemplateParameterList; }
    
	TOdlAstNodeIdentifier* IdentifierPointer() const { assert(FIdentifierPointer != NULL); return FIdentifierPointer; }
    TOdlAstNodeIdentifier* IdentifierPointer_IFP() const { return FIdentifierPointer; }
	TOdlAstNodeIdentifier* TypeIdentifierPointer() const { assert(FTypeIdentifierPointer != NULL); return FTypeIdentifierPointer; }

    TOdlAstNode* ExpressionPointer() const { return FExpressionPointer; }
    TOdlAstNode* PropertyDeclarationListPointer() const { return FPropertyDeclarationListPointer; }
	TOdlAstNode* TemplateParameterListPointer() const { return FTemplateParameterListPointer; }

	bool IsNullPtr() const;

    bool IsReferenceToResolve() const { return FReferenceToResolve; }
	bool IsAnonymousDeclaration() const { return FAnonymousDeclaration; }
	bool IsGlobalNamespace() const;

    // post processing
    void SetAsReferenceToResolve();
    bool IsValueReference() const { return FIsValueReference; }
    void ResolveReference(TOdlAstNode const* parNodeReference);
    TOdlAstNode const* ResolvedReference_ReturnNamedDeclaration() const { return FResolvedReferenceWeak; }

	void SetFullDatabasePath(TOdlDatabasePath const& parFullDatabasePath);
	TOdlDatabasePath const& FullDatabasePath() const { return FFullDatabasePath; }

private:
    void PrettyPrintWithIndentLevel(std::ostringstream& parOss, int parIndentLevel) const;

private:
    TOdlAstNodeType::TType          FAstNodeType;

    std::vector< TOdlAstNode* >     FPropertyList;
    std::vector< TOdlAstNode* >     FNamedDeclarationList;
	std::vector< TOdlAstNode* >     FTemplateParameterList;

    TOdlAstNodeIdentifier*          FIdentifierPointer;
    TOdlAstNodeIdentifier*          FTypeIdentifierPointer;
	TOdlAstNodeIdentifier*			FTargetTemplateNamespaceIdentifierPointer;
    TOdlAstNode*                    FPropertyDeclarationListPointer;
    TOdlAstNode*                    FExpressionPointer;
	TOdlAstNode*					FTemplateParameterListPointer;
    
    TOdlAstNode const*              FResolvedReferenceWeak;

    bool                            FAnonymousDeclaration;
    bool                            FReferenceToResolve;
    bool                            FIsValueReference;
	bool							FIsTemplateDeclarationParameter;
	TOdlDatabasePath				FFullDatabasePath;
};
//-------------------------------------------------------------------------------
//*******************************************************************************
//-------------------------------------------------------------------------------
class TOdlAstNodeExpression : public TOdlAstNode
{
public:
    TOdlAstNodeExpression(TOdlAstNodeType::TType parOdlAstNodeType) :
        TOdlAstNode(parOdlAstNodeType)
    {
        assert(parOdlAstNodeType & TOdlAstNodeType::EXPRESSION_MASK);
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
        FIdentifier(parIdentifier)
    {

    }

    virtual ~TOdlAstNodeIdentifier()
    {

    }

    std::string const& Identifier() const { return FIdentifier; }

private:
    std::string FIdentifier;
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
        FValueFloat(0.0f)
    {
        
    }

    TOdlAstNodeValue(float parValueFloat) :
        parent_type(TOdlAstNodeType::VALUE_FLOAT),
        FValueString(),
        FValueInteger(0),
        FValueFloat(parValueFloat)
    {
        
    }

    TOdlAstNodeValue(int parValueInteger) :
        parent_type(TOdlAstNodeType::VALUE_INTEGER),
        FValueString(),
        FValueInteger(parValueInteger),
        FValueFloat(0.0f)
    {
        
    }


    int ValueInteger() const { return FValueInteger; }
    float ValueFloat() const { return FValueFloat; }
    std::string const& ValueString() const { return FValueString; }

private:
    std::string                     FValueString;
    int                             FValueInteger;
    float                           FValueFloat;
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
        parent_type(TOdlAstNodeType::EXPRESSION),
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
    
        assert(parLeftExpression != nullptr ? parLeftExpression->AstNodeType() & TOdlAstNodeType::EXPRESSION_MASK : true);
        assert(parRightExpression->AstNodeType() & TOdlAstNodeType::EXPRESSION_MASK);
        assert(parOperator->AstNodeType() == TOdlAstNodeType::OPERATOR);

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