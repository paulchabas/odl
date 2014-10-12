#ifndef ODL_EXPRESSION_H
# define ODL_EXPRESSION_H


#include "OdlAstNode.h"

namespace odl
{
//-------------------------------------------------------------------------------
//*******************************************************************************
//-------------------------------------------------------------------------------
class TOdlObject;
class TMetaClassBase;
class TOdlDatabasePath;
//-------------------------------------------------------------------------------
//*******************************************************************************
//-------------------------------------------------------------------------------
struct TOdlExpressionNullTag {};
//-------------------------------------------------------------------------------
class TOdlExpression
{
public:
    enum TType
    {
        UNTYPED,
        INTEGER,
        FLOAT,
        STRING,
        OBJECT,
		NULLEXP,
        VECTOR,
    };

    union TValueUnion
    {
        int FInteger;
        float FFloat;
        char const* FString;
        TOdlObject* FObject;
        struct TValueArray
        {
            TOdlExpression*  FVectorValues;
            size_t              FVectorCount;
        } FValueArray;
    };

private:
    // non copyable: = delete syntax doesn't seems to work. (visual studio 2012)
    TOdlExpression(TOdlExpression const&) {};
    TOdlExpression& operator = (TOdlExpression const&) {};
public:

    TOdlExpression();
	TOdlExpression(TOdlExpressionNullTag const&);
	TOdlExpression(int parValueInteger);
    TOdlExpression(float parFloatValue);
    TOdlExpression(char const* parString);
    TOdlExpression(TOdlObject* parObject, TMetaClassBase const* parObjectMetaClassBase);
    TOdlExpression(TOdlExpression* parVectorValues, size_t parVectorCount, TMetaClassBase const* parVectorValueMetaClassBase);

    ~TOdlExpression()
    {
        if (FType == TOdlExpression::VECTOR)
        {
            if (FValueUnion.FValueArray.FVectorValues != nullptr)
            {
                delete [] FValueUnion.FValueArray.FVectorValues;
            }
        }
        else if (FType == TOdlExpression::STRING)
        {
            if (FValueUnion.FString != nullptr)
            {
                delete [] FValueUnion.FString;
            }
        }
    }

    TOdlExpression(TOdlExpression&& rhs)
    {
        *this = rhs;
    }

    TOdlExpression& TOdlExpression::operator=(TOdlExpression&& rhs)
    {
        ExplicitMove(rhs);
        return *this;
    }
    
    TOdlExpression::TType    Type() const { return FType; }
    TMetaClassBase const*       MetaClassBase() const { return FMetaClassBase; }
    TValueUnion const&          ValueUnion() const { return FValueUnion; }

    void ExplicitMove(TOdlExpression& rhs)
    {
        FType = rhs.FType;
        FValueUnion = rhs.FValueUnion;
        FMetaClassBase = rhs.FMetaClassBase;
        
        rhs.FType = TOdlExpression::UNTYPED;
        memset(&rhs.FValueUnion, 0, sizeof(TValueUnion));
        rhs.FMetaClassBase = nullptr;
    }

private:
    TOdlExpression::TType		FType;
    TValueUnion                 FValueUnion;
    TMetaClassBase const*       FMetaClassBase;
};
//-------------------------------------------------------------------------------
struct TEvalExpressionContext
{
	bool AddToCircularReferenceCheck(TOdlAstNode const* parAstNode);
	void RemoveToCircularReferenceCheck(TOdlAstNode const* parAstNode);

private:
	std::vector< TOdlAstNode const* > FCircularReferenceCheck;
};
//-------------------------------------------------------------------------------
struct TScopedCheckCircularReferenceCheck
{
	TScopedCheckCircularReferenceCheck(TEvalExpressionContext& parContext, TOdlAstNode const* parExpression) :
		FContext(parContext),
		FExpression(parExpression),
		FCheckResult(true)
	{
		assert(parExpression != nullptr);
		FCheckResult = FContext.AddToCircularReferenceCheck(parExpression);
	}

	bool CheckFailed() const { return !FCheckResult; }

	~TScopedCheckCircularReferenceCheck()
	{
		FContext.RemoveToCircularReferenceCheck(FExpression);
	}

private:
	TEvalExpressionContext& FContext;
	TOdlAstNode const* FExpression;
	bool FCheckResult;
};
//-------------------------------------------------------------------------------
TOdlExpression EvalExpression(TEvalExpressionContext& parContext, TOdlAstNode const* parExpression, TOdlDatabasePath const& parDatabasePath);
//-------------------------------------------------------------------------------
//*******************************************************************************
//-------------------------------------------------------------------------------
} // namespace odl


#endif
