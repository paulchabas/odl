#include "stdafx.h"
#include "OdlExpression.h"

#include "Odl.h"
#include "OdlTokenDatabase.h"

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
            if (parLeft.MetaClassBase() == parRight.MetaClassBase())
            {
                return true;
            }
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


    return TOdlExpression();
}
//-------------------------------------------------------------------------------
TOdlExpression EvalExpression(TOdlAstNode const* parExpression, TOdlDatabasePath const& parDatabasePath)
{
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
                std::string const& objectName = parExpression->IdentifierPointer()->Identifier();
                std::string const& objectType = parExpression->TypeIdentifierPointer()->Identifier();

                // PAUL(27/05/14 17:57:57) yuk.
                TOdlDatabasePath workingDatabasePath(parDatabasePath);
                workingDatabasePath.push_back(TOdlDatabaseToken(objectName));

                TOdlObject* object = TOdlDatabase::Instance().GetObject(workingDatabasePath);
                TMetaClassBase const* objectMetaClassBase = TOdlDatabase::Instance().FindRegisteredMetaClassByName_IFP(objectType.c_str());
                return TOdlExpression(object, objectMetaClassBase);
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
                    
                        vectorContent[i] = EvalExpression(child, parDatabasePath);
                    }
                }

                return TOdlExpression(vectorContent, childCount, nullptr);
            }
            break ;
		case TOdlAstNodeType::IDENTIFIER:
			{
				// identifier: object or value constant
		        std::string const& objectName = parExpression->Identifier();

                // object reference
                if (!parExpression->IsValueReference())
                {
                    // PAUL(27/05/14 17:57:57) yuk.
                    TObjectAndMetaClass objectAndMetaClass;
                    {
                        TOdlDatabasePath workingDatabasePath(parDatabasePath);
                        workingDatabasePath.push_back(objectName);
                        objectAndMetaClass = TOdlDatabase::Instance().GetObjectAndMetaClass_IFP(workingDatabasePath);
                        while (objectAndMetaClass.first == nullptr)
                        {
                            // yuk.
                            if (!workingDatabasePath.SetCurrentNameInParentNamespace_ROK())
                            {
                                assert(false); // {TODO} object not found 
                                break ;
                            }
                            objectAndMetaClass = TOdlDatabase::Instance().GetObjectAndMetaClass_IFP(workingDatabasePath);
                        }
                    }

		            if (objectAndMetaClass.first != nullptr)
					    return TOdlExpression(objectAndMetaClass.first, objectAndMetaClass.second);
				    else
				    {
					    assert(false); // {TODO}  object not found.
				    }
                }
                else // value reference
                {
                    TOdlAstNode* resolvedReference = parExpression->ResolvedReference();
                    return EvalExpression(resolvedReference, parDatabasePath);
                }
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

                TOdlExpression leftResult = EvalExpression(left, parDatabasePath);
                TOdlExpression rightResult = EvalExpression(right, parDatabasePath);

                if (ExpressionTypeCompatible(leftResult, rightResult))
                {
                    return EvalOperationPlus(leftResult, rightResult);
                }
                
            }
            break ;
        case TOdlAstNodeOperatorType::OPERATOR_MINUS:
            {
                int a = 0; // {TODO} operator
            }
            break ;

        case TOdlAstNodeOperatorType::OPERATOR_MULTIPLY:
            {
                int a = 0; // {TODO} operator
            }
            break ;

        case TOdlAstNodeOperatorType::OPERATOR_DIVIDE:
            {
                int a = 0; // {TODO} operator
            }
            break ;

        case TOdlAstNodeOperatorType::OPERATOR_MODULO:
            {
                int a = 0; // {TODO} operator
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
} // odl