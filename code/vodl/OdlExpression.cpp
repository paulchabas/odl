#include "stdafx.h"
#include "OdlExpression.h"

#include "Odl.h"
#include "vOdlParserLexer/OdlTokenDatabase.h"

namespace odl
{
//-------------------------------------------------------------------------------
//*******************************************************************************
//-------------------------------------------------------------------------------
TOdlExpression::TOdlExpression()
{
    memset(&FValueUnion, 0, sizeof(TValueUnion));
}
//-------------------------------------------------------------------------------
TOdlExpression::TOdlExpression(TOdlExpressionNullTag const&) :
    FType(TOdlExpression::NULLEXP),
    FMetaClassBase(nullptr)
{

}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------
TOdlExpression::TOdlExpression(int parValueInteger) :
    FType(TOdlExpression::INTEGER),
    FMetaClassBase(TMetaClassTraits< int >::GetMetaClassInstance())
{
    memset(&FValueUnion, 0, sizeof(TValueUnion));

    FValueUnion.FInteger = parValueInteger;
}
//-------------------------------------------------------------------------------
TOdlExpression::TOdlExpression(float parFloatValue) :
    FType(TOdlExpression::FLOAT),
    FMetaClassBase(TMetaClassTraits< float >::GetMetaClassInstance())
{
    memset(&FValueUnion, 0, sizeof(TValueUnion));

    FValueUnion.FFloat = parFloatValue;
}
//-------------------------------------------------------------------------------
TOdlExpression::TOdlExpression(char const* parString) :
    FType(TOdlExpression::STRING),
    FMetaClassBase(TMetaClassTraits< odl::string >::GetMetaClassInstance())
{
    memset(&FValueUnion, 0, sizeof(TValueUnion));
    FValueUnion.FString = parString;
}
//-------------------------------------------------------------------------------
TOdlExpression::TOdlExpression(TOdlObject* parObject, TMetaClassBase const* parObjectMetaClassBase) :
    FType(TOdlExpression::OBJECT),
    FMetaClassBase(parObjectMetaClassBase)
{
    memset(&FValueUnion, 0, sizeof(TValueUnion));

    FValueUnion.FObject = parObject;
}
//-------------------------------------------------------------------------------
TOdlExpression::TOdlExpression(TOdlExpression* parVectorValues, size_t parVectorCount, TMetaClassBase const* parVectorValueMetaClassBase) :
    FType(TOdlExpression::VECTOR),
    FMetaClassBase(parVectorValueMetaClassBase)
{
    memset(&FValueUnion, 0, sizeof(TValueUnion));

    FValueUnion.FValueArray.FVectorValues = parVectorValues;
    FValueUnion.FValueArray.FVectorCount = parVectorCount;
}
//-------------------------------------------------------------------------------
//*******************************************************************************
//-------------------------------------------------------------------------------
bool ExpressionTypeCompatible(TOdlExpression const& parLeft, TOdlExpression const& parRight)
{
    if (parLeft.Type() == parRight.Type())
    {
        if (parLeft.Type() == TOdlExpression::OBJECT)
        {
			return false;
        }
        else if (parLeft.Type() == TOdlExpression::VECTOR)
        {
            if (parLeft.MetaClassBase() == parRight.MetaClassBase())
            {
                return true;
            }
        }

        return true;
    }
	else if (parLeft.Type() == TOdlExpression::FLOAT)
	{
		if (parRight.Type() == TOdlExpression::INTEGER)
			return true;
	}
	else if (parLeft.Type() == TOdlExpression::INTEGER)
	{
		if (parRight.Type() == TOdlExpression::FLOAT)
			return true;
	}

    return false;
}
//-------------------------------------------------------------------------------
TOdlExpression EvalOperationPlus(TOdlExpression& parLeft, TOdlExpression& parRight)
{
    assert(ExpressionTypeCompatible(parLeft, parRight));

    if (parLeft.Type() == TOdlExpression::INTEGER &&
        parRight.Type() == TOdlExpression::INTEGER)
    {
        return TOdlExpression(parLeft.ValueUnion().FInteger + parRight.ValueUnion().FInteger);
    }

    if (parLeft.Type() == TOdlExpression::FLOAT &&
        parRight.Type() == TOdlExpression::FLOAT)
    {
        return TOdlExpression(parLeft.ValueUnion().FFloat + parRight.ValueUnion().FFloat);
    }

	if (parLeft.Type() == TOdlExpression::FLOAT && 
		parRight.Type() == TOdlExpression::INTEGER)
	{
		return TOdlExpression(parLeft.ValueUnion().FFloat + parRight.ValueUnion().FInteger);
	}

	if (parLeft.Type() == TOdlExpression::INTEGER && 
		parRight.Type() == TOdlExpression::FLOAT)
	{
		return TOdlExpression(parLeft.ValueUnion().FInteger + parRight.ValueUnion().FFloat);
	}

    if (parLeft.Type() == TOdlExpression::STRING &&
        parRight.Type() == TOdlExpression::STRING)
    {
        char const* leftString = parLeft.ValueUnion().FString;
        char const* rightString = parRight.ValueUnion().FString;

        size_t const leftLength = strlen(leftString);
        size_t const rightLength = strlen(rightString);
        size_t const newLength = leftLength + rightLength;
        char* newString = new char[newLength + 1];
        for (size_t i = 0; i < leftLength; ++i)
        {
            newString[i] = leftString[i];
        }

        for (size_t i = 0; i < rightLength; ++i)
        {
            newString[leftLength + i] = rightString[i];
        }
        newString[newLength] = '\0';
        return TOdlExpression(newString);
    }

    if (parLeft.Type() == TOdlExpression::VECTOR &&
        parRight.Type() == TOdlExpression::VECTOR)
    {
        size_t sizeLeft = parLeft.ValueUnion().FValueArray.FVectorCount;
        size_t sizeRight = parRight.ValueUnion().FValueArray.FVectorCount;
        size_t newContentSize = sizeLeft + sizeRight;
        TOdlExpression* newContent = nullptr;
        if (newContentSize > 0)
        {
            newContent = new TOdlExpression[newContentSize];
            for (size_t i = 0; i < sizeLeft; ++i)
            {
                newContent[i].ExplicitMove(parLeft.ValueUnion().FValueArray.FVectorValues[i]);
            }
        
            size_t readIndex = 0;
            for (size_t i = sizeLeft; i < newContentSize; ++i)
            {
                newContent[i].ExplicitMove(parRight.ValueUnion().FValueArray.FVectorValues[readIndex++]);
            }
        }
        
        return TOdlExpression(newContent, newContentSize, nullptr);
    }

	assert(false); // undefined operation

    return TOdlExpression();
}
//-------------------------------------------------------------------------------
TOdlExpression EvalOperationMinus(TOdlExpression& parLeft, TOdlExpression& parRight)
{
    assert(ExpressionTypeCompatible(parLeft, parRight));

    if (parLeft.Type() == TOdlExpression::INTEGER &&
        parRight.Type() == TOdlExpression::INTEGER)
    {
        return TOdlExpression(parLeft.ValueUnion().FInteger - parRight.ValueUnion().FInteger);
    }

    if (parLeft.Type() == TOdlExpression::FLOAT &&
        parRight.Type() == TOdlExpression::FLOAT)
    {
        return TOdlExpression(parLeft.ValueUnion().FFloat - parRight.ValueUnion().FFloat);
    }

	if (parLeft.Type() == TOdlExpression::FLOAT && 
		parRight.Type() == TOdlExpression::INTEGER)
	{
		return TOdlExpression(parLeft.ValueUnion().FFloat - parRight.ValueUnion().FInteger);
	}

	if (parLeft.Type() == TOdlExpression::INTEGER && 
		parRight.Type() == TOdlExpression::FLOAT)
	{
		return TOdlExpression(parLeft.ValueUnion().FInteger - parRight.ValueUnion().FFloat);
	}

	assert(false); // undefined operation

    return TOdlExpression();
}
//-------------------------------------------------------------------------------
TOdlExpression EvalOperationMultiply(TOdlExpression& parLeft, TOdlExpression& parRight)
{
    assert(ExpressionTypeCompatible(parLeft, parRight));

    if (parLeft.Type() == TOdlExpression::INTEGER &&
        parRight.Type() == TOdlExpression::INTEGER)
    {
        return TOdlExpression(parLeft.ValueUnion().FInteger * parRight.ValueUnion().FInteger);
    }

    if (parLeft.Type() == TOdlExpression::FLOAT &&
        parRight.Type() == TOdlExpression::FLOAT)
    {
        return TOdlExpression(parLeft.ValueUnion().FFloat * parRight.ValueUnion().FFloat);
    }

	if (parLeft.Type() == TOdlExpression::FLOAT && 
		parRight.Type() == TOdlExpression::INTEGER)
	{
		return TOdlExpression(parLeft.ValueUnion().FFloat * parRight.ValueUnion().FInteger);
	}

	if (parLeft.Type() == TOdlExpression::INTEGER && 
		parRight.Type() == TOdlExpression::FLOAT)
	{
		return TOdlExpression(parLeft.ValueUnion().FInteger * parRight.ValueUnion().FFloat);
	}

	assert(false); // undefined operation

    return TOdlExpression();
}
//-------------------------------------------------------------------------------
TOdlExpression EvalOperationDivide(TOdlExpression& parLeft, TOdlExpression& parRight)
{
    assert(ExpressionTypeCompatible(parLeft, parRight));

    if (parLeft.Type() == TOdlExpression::INTEGER &&
        parRight.Type() == TOdlExpression::INTEGER)
    {
        return TOdlExpression(parLeft.ValueUnion().FInteger / parRight.ValueUnion().FInteger);
    }

    if (parLeft.Type() == TOdlExpression::FLOAT &&
        parRight.Type() == TOdlExpression::FLOAT)
    {
        return TOdlExpression(parLeft.ValueUnion().FFloat / parRight.ValueUnion().FFloat);
    }

	if (parLeft.Type() == TOdlExpression::FLOAT && 
		parRight.Type() == TOdlExpression::INTEGER)
	{
		return TOdlExpression(parLeft.ValueUnion().FFloat / parRight.ValueUnion().FInteger);
	}

	if (parLeft.Type() == TOdlExpression::INTEGER && 
		parRight.Type() == TOdlExpression::FLOAT)
	{
		return TOdlExpression(parLeft.ValueUnion().FInteger / parRight.ValueUnion().FFloat);
	}

	assert(false); // undefined operation

    return TOdlExpression();
}
//-------------------------------------------------------------------------------
TOdlExpression EvalOperationModulo(TOdlExpression& parLeft, TOdlExpression& parRight)
{
    assert(ExpressionTypeCompatible(parLeft, parRight));

    if (parLeft.Type() == TOdlExpression::INTEGER &&
        parRight.Type() == TOdlExpression::INTEGER)
    {
        return TOdlExpression(parLeft.ValueUnion().FInteger % parRight.ValueUnion().FInteger);
    }

    if (parLeft.Type() == TOdlExpression::FLOAT &&
        parRight.Type() == TOdlExpression::FLOAT)
    {
		float const result = std::fmodf(parLeft.ValueUnion().FFloat, parRight.ValueUnion().FFloat);
        return TOdlExpression(result);
    }

	if (parLeft.Type() == TOdlExpression::FLOAT && 
		parRight.Type() == TOdlExpression::INTEGER)
	{
		float const result = std::fmodf(parLeft.ValueUnion().FFloat, (float) parRight.ValueUnion().FInteger);
		return TOdlExpression(result);
	}

	if (parLeft.Type() == TOdlExpression::INTEGER && 
		parRight.Type() == TOdlExpression::FLOAT)
	{
		float const result = std::fmodf((float) parLeft.ValueUnion().FInteger, parRight.ValueUnion().FFloat);
		return TOdlExpression(result);
	}

	assert(false); // undefined operation

    return TOdlExpression();
}
//-------------------------------------------------------------------------------
TOdlExpression EvalExpression(TEvalExpressionContext& parContext, TOdlAstNode const* parExpression)
{
	TScopedCheckCircularReferenceCheck scopedCheck(parContext, parExpression);

	if (scopedCheck.CheckFailed())
		return TOdlExpression();

    TOdlAstNodeType::TType expressionType = parExpression->AstNodeType();
    assert(expressionType & TOdlAstNodeType::EXPRESSION_MASK);
    if (expressionType & TOdlAstNodeType::VALUE_MASK)
    {
        switch (expressionType)
        {
        case TOdlAstNodeType::VALUE_INTEGER:
            {
                return TOdlExpression(parExpression->ValueInteger());
            }
            break ;
        case TOdlAstNodeType::VALUE_FLOAT:
            {
                return TOdlExpression(parExpression->ValueFloat());
            }
            break ;
        case TOdlAstNodeType::VALUE_STRING:
            {
                char const* theString = parExpression->ValueString();
                size_t const stringLength = strlen(theString);
                char* newString = new char[stringLength + 1];
                for (size_t i = 0; i < stringLength; ++i)
                {
                    newString[i] = theString[i];
                }
                newString[stringLength] = '\0';
                return TOdlExpression(newString);
            }
            break ;
        case TOdlAstNodeType::OBJECT_DECLARATION:
			{
				// search nullptr
				if (!parExpression->IsNullPtr())
				{
					// PAUL(27/05/14 17:57:57) yuk.
					TOdlDatabasePath const& databasePath = parExpression->FullDatabasePath();
					std::string const& objectType = parExpression->TypeIdentifierPointer()->Identifier();

					#ifdef ODL_ENABLE_VERBOSE_DEBUG
					std::string forDebug1 = databasePath.ToString();
					#endif

					TOdlObject* object = TOdlDatabase::Instance().GetObject(databasePath);
					TMetaClassBase const* objectMetaClassBase = TOdlDatabase::Instance().FindRegisteredMetaClassByName_IFP(objectType.c_str());
					return TOdlExpression(object, objectMetaClassBase);
				}
				else
				{
					return TOdlExpression(TOdlExpressionNullTag());
				}
            }
            break ;
        case TOdlAstNodeType::VALUE_VECTOR:
            {
                std::vector< TOdlAstNode* > const& childs = parExpression->VectorContent();
                size_t childCount = childs.size();

                TOdlExpression* vectorContent = nullptr;
                if (childCount > 0)
                {
                    vectorContent = new TOdlExpression[childCount];
		            for (size_t i = 0; i < childCount; ++i)
                    {
                        size_t invI = childCount - i - 1;
                        TOdlAstNode const* child = childs[invI];
                    
                        vectorContent[i] = EvalExpression(parContext, child);
                    }
                }

                return TOdlExpression(vectorContent, childCount, nullptr);
            }
            break ;
		case TOdlAstNodeType::IDENTIFIER:
			{
				// identifier: object name of link
				#if ODL_ENABLE_VERBOSE_DEBUG
		        std::string const& objectName = parExpression->Identifier();
				#endif
               
				TOdlAstNode* namedDeclaration = parExpression->ResolvedReference_ReturnNamedDeclaration();
                TOdlAstNode* namedDeclarationExpression = namedDeclaration->ExpressionPointer();
				if (namedDeclarationExpression == nullptr)
				{

				}

				assert(namedDeclarationExpression != nullptr);
                return EvalExpression(parContext, namedDeclarationExpression);

			}
			break ;
        default:
            assert(false); // unknown value type.
            break ;
        };
    }
    else
    {
        assert(expressionType == TOdlAstNodeType::EXPRESSION);
        TOdlAstNodeOperatorType::TType nodeOperator = parExpression->OperatorExpressionPointer()->OperatorType();
        switch (nodeOperator)
        {
        case TOdlAstNodeOperatorType::OPERATOR_PLUS:
            {
                TOdlAstNode const* left = parExpression->LeftExpressionPointer();
                TOdlAstNode const* right = parExpression->RightExpressionPointer();

                TOdlExpression leftResult = EvalExpression(parContext, left);
				if (leftResult.Type() == TOdlExpression::UNTYPED)
					return TOdlExpression();
                TOdlExpression rightResult = EvalExpression(parContext, right);
				if (rightResult.Type() == TOdlExpression::UNTYPED)
					return TOdlExpression();

                if (ExpressionTypeCompatible(leftResult, rightResult))
                {
                    return EvalOperationPlus(leftResult, rightResult);
                }
                
            }
            break ;
        case TOdlAstNodeOperatorType::OPERATOR_MINUS:
            {
				TOdlAstNode const* left = parExpression->LeftExpressionPointer();
                TOdlAstNode const* right = parExpression->RightExpressionPointer();

				TOdlExpression leftResult(0);
				if (left != nullptr)
				{
					leftResult = EvalExpression(parContext, left);
					if (leftResult.Type() == TOdlExpression::UNTYPED)
						return TOdlExpression();
				}
                TOdlExpression rightResult = EvalExpression(parContext, right);
				if (rightResult.Type() == TOdlExpression::UNTYPED)
					return TOdlExpression();

				if (ExpressionTypeCompatible(leftResult, rightResult))
				{
					return EvalOperationMinus(leftResult, rightResult);
				}
            }
            break ;

        case TOdlAstNodeOperatorType::OPERATOR_MULTIPLY:
            {
                TOdlAstNode const* left = parExpression->LeftExpressionPointer();
                TOdlAstNode const* right = parExpression->RightExpressionPointer();

				TOdlExpression leftResult = EvalExpression(parContext, left);
				if (leftResult.Type() == TOdlExpression::UNTYPED)
					return TOdlExpression();
                TOdlExpression rightResult = EvalExpression(parContext, right);
				if (rightResult.Type() == TOdlExpression::UNTYPED)
					return TOdlExpression();

				if (ExpressionTypeCompatible(leftResult, rightResult))
				{
					return EvalOperationMultiply(leftResult, rightResult);
				}
            }
            break ;

        case TOdlAstNodeOperatorType::OPERATOR_DIVIDE:
            {
                TOdlAstNode const* left = parExpression->LeftExpressionPointer();
                TOdlAstNode const* right = parExpression->RightExpressionPointer();

				TOdlExpression leftResult = EvalExpression(parContext, left);
				if (leftResult.Type() == TOdlExpression::UNTYPED)
					return TOdlExpression();
                TOdlExpression rightResult = EvalExpression(parContext, right);
				if (rightResult.Type() == TOdlExpression::UNTYPED)
					return TOdlExpression();

				if (ExpressionTypeCompatible(leftResult, rightResult))
				{
					return EvalOperationDivide(leftResult, rightResult);
				}
            }
            break ;

        case TOdlAstNodeOperatorType::OPERATOR_MODULO:
            {
                TOdlAstNode const* left = parExpression->LeftExpressionPointer();
                TOdlAstNode const* right = parExpression->RightExpressionPointer();

				TOdlExpression leftResult = EvalExpression(parContext, left);
				if (leftResult.Type() == TOdlExpression::UNTYPED)
					return TOdlExpression();
                TOdlExpression rightResult = EvalExpression(parContext, right);
				if (rightResult.Type() == TOdlExpression::UNTYPED)
					return TOdlExpression();
				if (ExpressionTypeCompatible(leftResult, rightResult))
				{
					return EvalOperationModulo(leftResult, rightResult);
				}
            }
            break ;

        default:
            assert(false);
            break ;
        };
    }
    
    return TOdlExpression();
}
//-------------------------------------------------------------------------------
//*******************************************************************************
//-------------------------------------------------------------------------------
TEvalExpressionContext::TEvalExpressionContext(TTemplateDeclarations& parTemplateDeclarations) :
	FTemplateDeclarations(parTemplateDeclarations)
{

}
//------------------------------------------------------------------------------
bool TEvalExpressionContext::AddToCircularReferenceCheck(TOdlAstNode const* parAstNode)
{
	auto it = std::find(FCircularReferenceCheck.begin(), FCircularReferenceCheck.end(), parAstNode);
	bool const checkSucceeded = it == FCircularReferenceCheck.end();
	FCircularReferenceCheck.push_back(parAstNode);
	if (checkSucceeded)
	{
		return true;
	}
	else
	{
		return false;
	}
}
//-------------------------------------------------------------------------------
void TEvalExpressionContext::RemoveToCircularReferenceCheck(TOdlAstNode const* parAstNode)
{
	assert(FCircularReferenceCheck.back() == parAstNode);
	FCircularReferenceCheck.pop_back();
}
//-------------------------------------------------------------------------------
//*******************************************************************************
//-------------------------------------------------------------------------------
} // odl