#ifndef ODL_AST_NODE_H
# define ODL_AST_NODE_H

#include <cassert>
#include <sstream>
#include <vector>

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
        UNKNOWN                     = 0,
        IDENTIFIER                  = 1 | EXPRESSION_MASK | VALUE_MASK,
		VALUE_STRING                = 2 | EXPRESSION_MASK | VALUE_MASK,
        VALUE_INTEGER               = 3 | EXPRESSION_MASK | VALUE_MASK,
        VALUE_FLOAT                 = 4 | EXPRESSION_MASK | VALUE_MASK,
        OPERATOR                    = 5,
        PROPERTY_DECLARATION        = 6,
        PROPERTY_DECLARATION_LIST   = 7,
        OBJECT_DECLARATION          = 8 | EXPRESSION_MASK | VALUE_MASK,
        NAMESPACE                   = 9,
		NAMED_DECLARATION			= 10,
        EXPRESSION                  = 11 | EXPRESSION_MASK,
		VALUE_VECTOR				= 12 | EXPRESSION_MASK | VALUE_MASK,
        OBJECT_TEMPLATE_DECLARATION = 13 | TEMPLATE_MASK,
        OBJECT_TEMPLATE_INSTANCIATION = 14 | TEMPLATE_MASK,
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
class TOdlAstNode
{
public:
    TOdlAstNode();
    ~TOdlAstNode();

    TOdlAstNodeType::TType AstNodeType() const { return FAstNodeType; }
	TOdlAstNodeOperatorType::TType OperatorType() const { return FOperatorType; }

    void SetIdentifierPointer(TOdlAstNode* parIdentifierPointer);
    void AutoGenerateIdentifier();

    void SetAsNamespace();
    void Namespace_AppendNamedDeclaration(TOdlAstNode* parDeclaration);
    
    void SetAsExpression(TOdlAstNode* parLeftExpression,
                         TOdlAstNode* parOperator,
                         TOdlAstNode* parRightExpression);

	void SetAsVector();
	void Vector_AppendItem(TOdlAstNode* parVectorItem);

    void SetAsObjectDeclaration(TOdlAstNode* parTypeIdentifier, TOdlAstNode* parPropertyList);
	void SetAsNamedDeclaration(TOdlAstNode* parNameIdentifier, TOdlAstNode* parExpression);

    void SetAsTemplateDeclaration();
    void SetAsTemplateInstanciation();

    void SetAsPropertyDeclarationList();
    void PropertyDeclarationList_AppendPropertyDeclaration(TOdlAstNode* parPropertyDeclarationNode);
    void SetAsPropertyDeclaration(TOdlAstNode* parIdentifier, TOdlAstNode* parExpression);

    void SetAsIdentifier(std::string const& parIdentifier);
    std::string const& Identifier() const { return FValueIdentifier; }
    void SetAsStringValue(std::string const& parStringValue);
    void SetAsIntegerValue(int parIntegerValue);
    void SetAsFloatValue(float parFloatValue);

    void SetAsOperator(TOdlAstNodeOperatorType::TType parOperatorType);

	void SetAnonymousDeclaration(bool parState) { FAnonymousDeclaration = parState; }

    void PrettyPrint(std::ostringstream& parOss) const;

    std::vector< TOdlAstNode* >& PropertyList() { return FPropertyList; }
    std::vector< TOdlAstNode* >& NamespaceContent() { return FNamedDeclarationList; }
    std::vector< TOdlAstNode* >& VectorContent() { return FVectorContent; }

    std::vector< TOdlAstNode* > const& PropertyList() const { return FPropertyList; }
    std::vector< TOdlAstNode* > const& NamespaceContent() const { return FNamedDeclarationList; }
    std::vector< TOdlAstNode* > const& VectorContent() const { return FVectorContent; }
    
	TOdlAstNode* IdentifierPointer() const { assert(FIdentifierPointer != NULL); return FIdentifierPointer; }
    TOdlAstNode* IdentifierPointer_IFP() const { return FIdentifierPointer; }
	TOdlAstNode* TypeIdentifierPointer() const { assert(FTypeIdentifierPointer != NULL); return FTypeIdentifierPointer; }
    TOdlAstNode* LeftExpressionPointer() const { return FLeftExpressionPointer; }
    TOdlAstNode* OperatorExpressionPointer() const { return FOperatorExpressionPointer; }
    TOdlAstNode* RightExpressionPointer() const { return FRightExpressionPointer; }
    TOdlAstNode* ExpressionPointer() const { return FExpressionPointer; }
    TOdlAstNode* PropertyDeclarationListPointer() const { return FPropertyDeclarationListPointer; }

    int ValueInteger() const { return FValueInteger; }
    float ValueFloat() const { return FValueFloat; }
    char const* ValueString() const { return FValueString.c_str(); }

    bool IsReferenceToResolve() const { return FReferenceToResolve; }
	bool IsAnonymousDeclaration() const { return FAnonymousDeclaration; }

    // post processing
    void SetAsReferenceToResolve() { FReferenceToResolve = true; }
    bool IsValueReference() const { return FIsValueReference; }
    void ResolveReference(TOdlAstNode* parNodeReference, bool parIsValueReference);
    TOdlAstNode* ResolvedReference_ReturnNamedDeclaration() const { return FResolvedReferenceWeak; }

	void SetFullDatabasePath(TOdlDatabasePath const& parFullDatabasePath);
	TOdlDatabasePath const& FullDatabasePath() const { return FFullDatabasePath; }

private:
    void PrettyPrintWithIndentLevel(std::ostringstream& parOss, int parIndentLevel) const;

private:
    TOdlAstNodeType::TType          FAstNodeType;
    TOdlAstNodeOperatorType::TType  FOperatorType;
    std::string                     FValueIdentifier;
    std::string                     FValueString;
    int                             FValueInteger;
    float                           FValueFloat;
    
    std::vector< TOdlAstNode* >     FPropertyList;
    std::vector< TOdlAstNode* >     FNamedDeclarationList;
	std::vector< TOdlAstNode* >     FVectorContent;

    TOdlAstNode*                    FIdentifierPointer;
    TOdlAstNode*                    FTypeIdentifierPointer;
    TOdlAstNode*                    FPropertyDeclarationListPointer;
    TOdlAstNode*                    FExpressionPointer;
    
    TOdlAstNode*                    FLeftExpressionPointer;
    TOdlAstNode*                    FOperatorExpressionPointer;
    TOdlAstNode*                    FRightExpressionPointer;

    TOdlAstNode*                    FResolvedReferenceWeak;

    bool                            FAnonymousDeclaration;
    bool                            FReferenceToResolve;
    bool                            FIsValueReference;
	TOdlDatabasePath				FFullDatabasePath;
};
//-------------------------------------------------------------------------------
//*******************************************************************************
//-------------------------------------------------------------------------------

}


#endif