#include "StdAfx.h"
#include "OdlAstNode.h"

#include <cassert>

namespace odl
{
//-------------------------------------------------------------------------------
//*******************************************************************************
//-------------------------------------------------------------------------------
static odl::TOdlAstNode* AutoGenerateObjectIdentifier()
{
	static unsigned int number = 0;
	char buffer[64];
	sprintf_s(buffer, "autoname#%010u", number);
	number++;

	odl::TOdlAstNode* autoGeneratedIdentifier = new odl::TOdlAstNode();
	autoGeneratedIdentifier->SetAsIdentifier(std::string(buffer));
	return autoGeneratedIdentifier;
}
//-------------------------------------------------------------------------------
//*******************************************************************************
//-------------------------------------------------------------------------------
TOdlAstNode::TOdlAstNode() :
    FAstNodeType(TOdlAstNodeType::UNKNOWN),
    FOperatorType(TOdlAstNodeOperatorType::OPERATOR_NONE),
    FIdentifier(),
    FValueString(),
    FValueInteger(0),
    FValueFloat(0.0f),
    FPropertyList(),
    FIdentifierPointer(nullptr),
    FTypeIdentifierPointer(nullptr),
    FPropertyDeclarationListPointer(nullptr),
    FExpressionPointer(nullptr),
    FLeftExpressionPointer(nullptr),
    FOperatorExpressionPointer(nullptr),
    FRightExpressionPointer(nullptr),
    FResolvedReferenceWeak(nullptr),
    FAnonymousObjectDeclaration(false),
    FReferenceToResolve(false),
    FIsValueReference(false)
{

}
//-------------------------------------------------------------------------------
TOdlAstNode::~TOdlAstNode()
{
    for (size_t i = 0; i < FPropertyList.size(); ++i)
    {
        TOdlAstNode const* node = FPropertyList[i];
        delete node;
    }

    for (size_t i = 0; i < FNamedDeclarationList.size(); ++i)
    {
        TOdlAstNode const* node = FNamedDeclarationList[i];
        delete node;
    }

    for (size_t i = 0; i < FVectorContent.size(); ++i)
    {
        TOdlAstNode const* node = FVectorContent[i];
        delete node;
    }
    
    delete FIdentifierPointer;
    delete FTypeIdentifierPointer;
    delete FPropertyDeclarationListPointer;
    delete FExpressionPointer;
    delete FLeftExpressionPointer;
    delete FOperatorExpressionPointer;
    delete FRightExpressionPointer;

    FResolvedReferenceWeak = nullptr; // weak reference.
}
//-------------------------------------------------------------------------------
void TOdlAstNode::SetIdentifierPointer(TOdlAstNode* parIdentifierPointer)
{
    assert(FIdentifierPointer == nullptr);
    FIdentifierPointer = parIdentifierPointer;
}
//-------------------------------------------------------------------------------
void TOdlAstNode::AutoGenerateIdentifier()
{
    assert(FIdentifierPointer == nullptr);
    FIdentifierPointer = AutoGenerateObjectIdentifier();
    FAnonymousObjectDeclaration = true;
}
//-------------------------------------------------------------------------------
void TOdlAstNode::SetAsNamespace()
{
    FAstNodeType = TOdlAstNodeType::NAMESPACE;
}
//-------------------------------------------------------------------------------
void TOdlAstNode::Namespace_AppendNamedDeclaration(TOdlAstNode* parDeclaration)
{
    assert(FAstNodeType == TOdlAstNodeType::NAMESPACE);
    FNamedDeclarationList.push_back(parDeclaration);
}
//-------------------------------------------------------------------------------
void TOdlAstNode::SetAsTemplateDeclaration()
{
    FAstNodeType = TOdlAstNodeType::OBJECT_TEMPLATE_DECLARATION;
}
//-------------------------------------------------------------------------------
void TOdlAstNode::SetAsTemplateInstanciation()
{
    FAstNodeType = TOdlAstNodeType::OBJECT_TEMPLATE_INSTANCIATION;

}
//-------------------------------------------------------------------------------
void TOdlAstNode::SetAsObjectDeclaration(TOdlAstNode* parTypeIdentifier, TOdlAstNode* parPropertyList)
{
    FAstNodeType = TOdlAstNodeType::OBJECT_DECLARATION;

    assert(parTypeIdentifier != nullptr);
    assert(parPropertyList != nullptr);

    assert(parTypeIdentifier->AstNodeType() == TOdlAstNodeType::IDENTIFIER);
    assert(parPropertyList->AstNodeType() == TOdlAstNodeType::PROPERTY_DECLARATION_LIST);

    FTypeIdentifierPointer = parTypeIdentifier;
    FPropertyDeclarationListPointer = parPropertyList;
}
//-------------------------------------------------------------------------------
void TOdlAstNode::SetAsPropertyDeclarationList()
{
    FAstNodeType = TOdlAstNodeType::PROPERTY_DECLARATION_LIST;
}
//-------------------------------------------------------------------------------
void TOdlAstNode::PropertyDeclarationList_AppendPropertyDeclaration(TOdlAstNode* parPropertyDeclarationNode)
{
    assert(FAstNodeType == TOdlAstNodeType::PROPERTY_DECLARATION_LIST);
    assert(parPropertyDeclarationNode != nullptr);
    assert(parPropertyDeclarationNode->AstNodeType() == TOdlAstNodeType::PROPERTY_DECLARATION);
    FPropertyList.push_back(parPropertyDeclarationNode);
}
//-------------------------------------------------------------------------------
void TOdlAstNode::SetAsPropertyDeclaration(TOdlAstNode* parIdentifier, TOdlAstNode* parExpression)
{
    assert(parIdentifier != nullptr);
    assert(parIdentifier->AstNodeType() == TOdlAstNodeType::IDENTIFIER);
    assert(parExpression != nullptr);
    assert(parExpression->AstNodeType() & TOdlAstNodeType::EXPRESSION);

    FAstNodeType = TOdlAstNodeType::PROPERTY_DECLARATION;
    FIdentifierPointer = parIdentifier;
    FExpressionPointer = parExpression;
}
//-------------------------------------------------------------------------------
void TOdlAstNode::SetAsIdentifier(std::string const& parIdentifier)
{
    FAstNodeType = TOdlAstNodeType::IDENTIFIER;
    FIdentifier = parIdentifier;
}
//-------------------------------------------------------------------------------
void TOdlAstNode::SetAsStringValue(std::string const& parStringValue)
{
    FAstNodeType = TOdlAstNodeType::VALUE_STRING;
    FValueString = parStringValue;
}
//-------------------------------------------------------------------------------
void TOdlAstNode::SetAsIntegerValue(int parIntegerValue)
{
    FAstNodeType = TOdlAstNodeType::VALUE_INTEGER;
    FValueInteger = parIntegerValue;
}
//-------------------------------------------------------------------------------
void TOdlAstNode::SetAsFloatValue(float parFloatValue)
{
    FAstNodeType = TOdlAstNodeType::VALUE_FLOAT;
    FValueFloat = parFloatValue;
}
//-------------------------------------------------------------------------------
void TOdlAstNode::SetAsOperator(TOdlAstNodeOperatorType::TType parOperatorType)
{
    FAstNodeType = TOdlAstNodeType::OPERATOR;
    FOperatorType = parOperatorType;
}
//-------------------------------------------------------------------------------
void TOdlAstNode::SetAsExpression(TOdlAstNode* parLeftExpression,
                                  TOdlAstNode* parOperator,
                                  TOdlAstNode* parRightExpression)
{
    FAstNodeType = TOdlAstNodeType::EXPRESSION;

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
//-------------------------------------------------------------------------------
void TOdlAstNode::SetAsVector()
{
	FAstNodeType = TOdlAstNodeType::VALUE_VECTOR;
}
//-------------------------------------------------------------------------------
void TOdlAstNode::Vector_AppendItem(TOdlAstNode* parVectorItem)
{
	assert(AstNodeType() == TOdlAstNodeType::VALUE_VECTOR);

	FVectorContent.push_back(parVectorItem);
}
//-------------------------------------------------------------------------------
void TOdlAstNode::PrettyPrint(std::ostringstream& parOss) const
{
    PrettyPrintWithIndentLevel(parOss, 0);
}
//-------------------------------------------------------------------------------
std::ostringstream& Indent(std::ostringstream& parOss, int parIndentLevel)
{
    for (int i = 0; i < parIndentLevel; ++i)
    {
        parOss << ".";
    }

    return parOss;
}
//-------------------------------------------------------------------------------
void TOdlAstNode::PrettyPrintWithIndentLevel(std::ostringstream& parOss, int parIndentLevel) const
{
    switch (FAstNodeType)
    {
	case TOdlAstNodeType::EXPRESSION:
        {
            
            bool anonymous = FIdentifierPointer == nullptr;
            if (!anonymous)
            {
                Indent(parOss, parIndentLevel);
                FIdentifierPointer->PrettyPrintWithIndentLevel(parOss, parIndentLevel);
                parOss << " is ";
                int const propertyLengthPlusSpacePlusIsPlusSpace = (int) FIdentifierPointer->Identifier().length() + 4;
                parIndentLevel += propertyLengthPlusSpacePlusIsPlusSpace;
            }

			parOss << "(" << std::endl;
			if (FLeftExpressionPointer != nullptr)
            {
                Indent(parOss, parIndentLevel + 4);
				FLeftExpressionPointer->PrettyPrintWithIndentLevel(parOss, parIndentLevel + 4);
                if (FLeftExpressionPointer->AstNodeType() != TOdlAstNodeType::EXPRESSION)
                    parOss << std::endl;
            }
			FOperatorExpressionPointer->PrettyPrintWithIndentLevel(parOss, parIndentLevel + 4);
            Indent(parOss, parIndentLevel + 4);
			FRightExpressionPointer->PrettyPrintWithIndentLevel(parOss, parIndentLevel + 4);
            parOss << std::endl;
			Indent(parOss, parIndentLevel) << ")" << std::endl;
		}
		break ;
	case TOdlAstNodeType::OPERATOR:
	{
		switch (FOperatorType)
		{
		case TOdlAstNodeOperatorType::OPERATOR_PLUS:
			{
				Indent(parOss, parIndentLevel) << '+' << std::endl;
			}
			break ;
		case TOdlAstNodeOperatorType::OPERATOR_MINUS:
			{
				Indent(parOss, parIndentLevel) << '-' << std::endl;
			}
			break ;
		case TOdlAstNodeOperatorType::OPERATOR_MULTIPLY:
			{
				Indent(parOss, parIndentLevel) << '*' << std::endl;
			}
			break ;
		case TOdlAstNodeOperatorType::OPERATOR_DIVIDE:
			{
				Indent(parOss, parIndentLevel) << '/' << std::endl;
			}
			break ;
		case TOdlAstNodeOperatorType::OPERATOR_MODULO:
			{
				Indent(parOss, parIndentLevel) << '%' << std::endl;
			}
			break ;
		default:
			assert(false);
			break ;
		};

		}	
		break ;
    case TOdlAstNodeType::IDENTIFIER:
        {
            parOss << FIdentifier;
        }
        break ;
    case TOdlAstNodeType::VALUE_STRING:
        {
            bool anonymous = FIdentifierPointer == nullptr;
            if (!anonymous)
            {
                Indent(parOss, parIndentLevel);
                FIdentifierPointer->PrettyPrintWithIndentLevel(parOss, parIndentLevel);
                parOss << " is ";
            }

			parOss << "\"";
            parOss << FValueString.c_str();
			parOss << "\"";

            if (!anonymous)
            {
                parOss << std::endl;
            }
        }
        break ;
    case TOdlAstNodeType::VALUE_INTEGER:
        {
            bool anonymous = FIdentifierPointer == nullptr;
            if (!anonymous)
            {
                Indent(parOss, parIndentLevel);
                FIdentifierPointer->PrettyPrintWithIndentLevel(parOss, parIndentLevel);
                parOss << " is ";
            }

            parOss << FValueInteger;

            if (!anonymous)
            {
                parOss << std::endl;
            }
        }
        break ;
    case TOdlAstNodeType::VALUE_FLOAT:
        {
            bool anonymous = FIdentifierPointer == nullptr;
            if (!anonymous)
            {
                Indent(parOss, parIndentLevel);
                FIdentifierPointer->PrettyPrintWithIndentLevel(parOss, parIndentLevel);
                parOss << " is ";
            }

            parOss << FValueFloat;

            if (!anonymous)
            {
                parOss << std::endl;
            }
        }
        break ;
	case TOdlAstNodeType::VALUE_VECTOR:
		{
            bool anonymous = FIdentifierPointer == nullptr;
            if (!anonymous)
            {
                Indent(parOss, parIndentLevel);
                FIdentifierPointer->PrettyPrintWithIndentLevel(parOss, parIndentLevel);
                parOss << " is ";
            }

			parOss << '[';
			for (int i = 0; i < (int) FVectorContent.size(); ++i)
            {
                // PAUL(15/05/14 18:19:11) les items sont stock�s � l'envers 
                // (description de la grammaire permet de supporter la notation [ expression , ]
                int const invI = (int) FVectorContent.size() - i - 1;
                TOdlAstNode const* item = FVectorContent[invI];
                item->PrettyPrintWithIndentLevel(parOss, parIndentLevel);
				if (i + 1 != FVectorContent.size())
					parOss << ", ";
            }
			parOss << ']';

            if (!anonymous)
            {
                parOss << std::endl;
            }
		}
		break ;
    case TOdlAstNodeType::PROPERTY_DECLARATION:
        {
            Indent(parOss, parIndentLevel);
            FIdentifierPointer->PrettyPrintWithIndentLevel(parOss, parIndentLevel);
            parOss << " = ";

            int const propertyLengthPlusSpacePlusEqualPlusSpace = (int) FIdentifierPointer->Identifier().length() + 3;
            FExpressionPointer->PrettyPrintWithIndentLevel(parOss, parIndentLevel + propertyLengthPlusSpacePlusEqualPlusSpace);
            // yuk.
            if (FExpressionPointer->AstNodeType() != TOdlAstNodeType::EXPRESSION)
            {
                parOss << std::endl;
            }
        }
        break ;
    case TOdlAstNodeType::PROPERTY_DECLARATION_LIST:
        {
            for (size_t i = 0; i < FPropertyList.size(); ++i)
            {
                TOdlAstNode const* child = FPropertyList[i];
                child->PrettyPrintWithIndentLevel(parOss, parIndentLevel);
            }
        }
        break ;
    case TOdlAstNodeType::OBJECT_DECLARATION:
        {
            bool const anonymous = FAnonymousObjectDeclaration;
            if (!anonymous)
            {
                Indent(parOss, parIndentLevel);
            }

            FIdentifierPointer->PrettyPrintWithIndentLevel(parOss, parIndentLevel);
            parOss << " is ";
            parOss << FTypeIdentifierPointer->Identifier() << std::endl;
            Indent(parOss, parIndentLevel) << "{" << std::endl;
            FPropertyDeclarationListPointer->PrettyPrintWithIndentLevel(parOss, parIndentLevel + 4);
            Indent(parOss, parIndentLevel) << "}";

            if (!anonymous)
            {
                parOss << std::endl;
            }
        }
        break ;
    case TOdlAstNodeType::NAMESPACE:
        {
            Indent(parOss, parIndentLevel) << "namespace ";
            if (FIdentifierPointer != nullptr)
            {
                FIdentifierPointer->PrettyPrintWithIndentLevel(parOss, parIndentLevel);
            }
            parOss << std::endl;
            Indent(parOss, parIndentLevel) << "{" << std::endl;
            for (size_t i = 0; i < FNamedDeclarationList.size(); ++i)
            {
                TOdlAstNode const* child = FNamedDeclarationList[i];
                child->PrettyPrintWithIndentLevel(parOss, parIndentLevel + 4);
            }
            Indent(parOss, parIndentLevel) << "}" << std::endl;
        }
        break ;
    };
}
//-------------------------------------------------------------------------------
//*******************************************************************************
//-------------------------------------------------------------------------------
} // namespace odl
