
#ifndef ODL_H
#define ODL_H

#include <array>
#include <vector>
#include <string>
#include <sstream>

#include "BasicTypes.h"
#include "OdlDatabase.h"
#include "OdlExpression.h"
#include "OdlTokenDatabase.h"

namespace odl
{
//--------------------------------------------------------
//********************************************************
//--------------------------------------------------------
typedef std::string string;
//--------------------------------------------------------
//********************************************************
//--------------------------------------------------------
class TOdlVectorClass;
//--------------------------------------------------------
//********************************************************
//--------------------------------------------------------
class TPropertyBase;
//--------------------------------------------------------
//********************************************************
//--------------------------------------------------------
namespace TMetaClassType
{
	enum Type
	{
		Class,
		Value,
		Vector,
		Map
	};

} // TMetaClassType
//--------------------------------------------------------
//********************************************************
//--------------------------------------------------------
class TOdlObject;
//--------------------------------------------------------
class TMetaClassBase
{
public:
	TMetaClassBase(char const* parName, TMetaClassType::Type parMetaClassType);
    virtual ~TMetaClassBase();

    char const* Name() const { return FName.c_str(); }

	void PrintContent(std::ostringstream& parOss) const;

	// class
	virtual TOdlObject* CreateObject() const { assert(FMetaClassType == TMetaClassType::Class); return nullptr; }
    virtual void DeleteObject(TOdlObject* parObject) const { assert(false); }

    void SetParentType(TMetaClassBase const* parParentType) { FParentType = parParentType; }
    TMetaClassBase const* ParentType() const { return FParentType; }
	void RegisterProperty(TPropertyBase const* parUserProperty);
	TPropertyBase const* PropertyByName(std::string const& parPropertyName) const;
    bool IsGivenTypeCompatible(TMetaClassBase const* parMetaClassType) const;

	// vector, map
	void SetContainerKeyMetaClass(TMetaClassBase const* parKeyMetaClass);
	void SetContainerValueMetaClass(TMetaClassBase const* parValueMetaClass);

private:
    std::string FName;
	TMetaClassType::Type FMetaClassType;
	std::vector< TPropertyBase const* > FClassProperties;
	TMetaClassBase const* FContainerKeyMetaClass;
	TMetaClassBase const* FContainerValueMetaClass;
    TMetaClassBase const* FParentType; // todo: multiple inheritance.
};
//--------------------------------------------------------
//********************************************************
//--------------------------------------------------------
template < typename TUserClass >
class TMetaClass : public TMetaClassBase
{
    typedef TMetaClassBase parent_type;
    typedef TMetaClass< TUserClass > self_type;
public:
	explicit TMetaClass(char const* parName, TMetaClassType::Type parMetaClassType) :
		parent_type(parName, parMetaClassType)
	{

    }
};
//--------------------------------------------------------
template < typename TUserClass >
class TObjectMetaClass : public TMetaClass< TUserClass >
{
    typedef TMetaClass< TUserClass > parent_type;
    typedef TObjectMetaClass< TUserClass > self_type;
public:
	explicit TObjectMetaClass(char const* parName) :
		parent_type(parName, TMetaClassType::Class)
	{

    }

	virtual TOdlObject* CreateObject() const
	{ 
		return new TUserClass();
	}

    virtual void DeleteObject(TOdlObject* parObject) const
    {
        // pas de type checking donc mefie.
        delete static_cast< TUserClass* >(parObject);
    }
};
//--------------------------------------------------------
//********************************************************
//--------------------------------------------------------
template < typename T >
struct TRemovePointerForObjectTypeIFN
{
    typedef T TType;
};
//--------------------------------------------------------
template < typename T >
struct TRemovePointerForObjectTypeIFN< T* >
{
    typedef T TType;
};
//--------------------------------------------------------
// TMetaClassTraits
//--------------------------------------------------------
struct TOdlNull {};
// user objects types
template < typename TUserClass >
struct TMetaClassTraits
{ 
    typedef typename odl::TRemovePointerForObjectTypeIFN< TUserClass >::TType TUserClassNoPointer;

	static void ClassName(std::ostringstream& parOss)
	{
		parOss << GetMetaClassInstance()->Name();
	}

	static TMetaClassBase* CreateMetaClassInstance(char const* parMetaClassName)
	{
		TMetaClassBase* metaClassBase = new TObjectMetaClass< TUserClass >(parMetaClassName);
		assert(metaClassBase != nullptr);
		return metaClassBase;
	}

	static TMetaClassBase const* GetMetaClassInstance()
	{
        TMetaClassBase const* metaClassBase = TUserClassNoPointer::GetMetaClassInstance();
        assert(metaClassBase != nullptr);
		return metaClassBase;
	}
};
//-------------------------------------------------------------------------------
template < typename TUserClass >
struct TMetaClassTraits< TUserClass* >
{ 
    static_assert(false && (sizeof(TUserClass) > 0), "No pointer type on metaclass.");
};
//--------------------------------------------------------
// basic types
template<> struct TMetaClassTraits< float > 
{ 
	static void ClassName(std::ostringstream& parOss)
	{
		parOss << "float";
	}

	static TMetaClassBase const* GetMetaClassInstance()
	{
		TMetaClassBase const* metaClassBase = TOdlDatabase::Instance().FindRegisteredMetaClassByName_IFP("float");
		assert(metaClassBase);
		return metaClassBase;
	}

	static TMetaClassBase* CreateMetaClassInstance(char const* parMetaClassName)
	{
		TMetaClassBase* metaClassBase = new  TMetaClass< float >(parMetaClassName, TMetaClassType::Value);
		return metaClassBase;
	}
};
//--------------------------------------------------------
template<> struct TMetaClassTraits< i32 > 
{ 
	static void ClassName(std::ostringstream& parOss)
	{
		parOss << "i32";
	}

	static TMetaClassBase const* GetMetaClassInstance()
	{
		TMetaClassBase const* metaClassBase = TOdlDatabase::Instance().FindRegisteredMetaClassByName_IFP("i32");
		assert(metaClassBase);
		return metaClassBase;
	}

	static TMetaClassBase* CreateMetaClassInstance(char const* parMetaClassName)
	{
		TMetaClassBase* metaClassBase = new  TMetaClass< float >(parMetaClassName, TMetaClassType::Value);
		return metaClassBase;
	}
};
//--------------------------------------------------------
template<> struct TMetaClassTraits< std::string > 
{ 
	static void ClassName(std::ostringstream& parOss)
	{
		parOss << "string";
	}

	static TMetaClassBase const* GetMetaClassInstance()
	{
		TMetaClassBase const* metaClassBase = TOdlDatabase::Instance().FindRegisteredMetaClassByName_IFP("string");
		assert(metaClassBase);
		return metaClassBase;
	}

	static TMetaClassBase* CreateMetaClassInstance(char const* parMetaClassName)
	{
		TMetaClassBase* metaClassBase = new  TMetaClass< float >(parMetaClassName, TMetaClassType::Value);
		return metaClassBase;
	}
};
//--------------------------------------------------------
template< typename TValue, typename TAllocator  >
struct TMetaClassTraits< std::vector< TValue, TAllocator > >
{
    typedef typename odl::TRemovePointerForObjectTypeIFN< TValue >::TType TValueNoPointer;

	static void ClassName(std::ostringstream& parOss)
	{
		parOss << "vector<";
		TMetaClassTraits< TValueNoPointer >::ClassName(parOss);
		parOss << ">";
	}

	static TMetaClassBase const* GetMetaClassInstance()
	{
		TMetaClassBase const* valueMetaClassInstance = TMetaClassTraits< TValueNoPointer >::GetMetaClassInstance();
		assert(valueMetaClassInstance != nullptr);

		std::ostringstream oss;
		ClassName(oss);
		std::string className = oss.str();
		TMetaClassBase const* metaClassBase = TOdlDatabase::Instance().FindRegisteredMetaClassByName_IFP(className.c_str());
		if (metaClassBase == nullptr)
		{
			TMetaClassBase* newMetaClassBase = CreateMetaClassInstance(className.c_str());
			newMetaClassBase->SetContainerValueMetaClass(valueMetaClassInstance);
			TOdlDatabase::Instance().RegisterMetaClass(newMetaClassBase);
			metaClassBase = newMetaClassBase;
		}
		
		return metaClassBase;
	}

	static TMetaClassBase* CreateMetaClassInstance(char const* parMetaClassName)
	{
		TMetaClassBase* metaClassBase = new TMetaClass< std::vector< TValueNoPointer > >(parMetaClassName, TMetaClassType::Vector);
		return metaClassBase;
	}

	static TMetaClassBase const* ContainerValueMetaClass_IFP() 
	{ 
		return TMetaClassTraits< TValueNoPointer >::GetMetaClassInstance(); 
	}
};
//--------------------------------------------------------
template< typename TKey, typename TValue, typename TCompare, typename TAllocator  >
struct TMetaClassTraits< std::map< TKey, TValue, TCompare, TAllocator > >
{ 
    typedef typename odl::TRemovePointerForObjectTypeIFN< TKey >::TType TKeyNoPointer;
    typedef typename odl::TRemovePointerForObjectTypeIFN< TValue >::TType TValueNoPointer;

	static void ClassName(std::ostringstream& parOss)
	{
		parOss << "map<";
		TMetaClassTraits< TKeyNoPointer >::ClassName(parOss);
		parOss << ",";
		TMetaClassTraits< TValueNoPointer >::ClassName(parOss);
		parOss << ">";
	}

	static TMetaClassBase const* GetMetaClassInstance()
	{
		TMetaClassBase const* keyMetaClassInstance = TMetaClassTraits< TKeyNoPointer >::GetMetaClassInstance();
		assert(keyMetaClassInstance != nullptr);

		TMetaClassBase const* valueMetaClassInstance = TMetaClassTraits< TValueNoPointer >::GetMetaClassInstance();
		assert(valueMetaClassInstance != nullptr);

		std::ostringstream oss;
		ClassName(oss);
		std::string className = oss.str();
		TMetaClassBase const* metaClassBase = TOdlDatabase::Instance().FindRegisteredMetaClassByName_IFP(className.c_str());
		if (metaClassBase == nullptr)
		{
			TMetaClassBase* newMetaClassBase = CreateMetaClassInstance(className.c_str());
			newMetaClassBase->SetContainerKeyMetaClass(keyMetaClassInstance);
			newMetaClassBase->SetContainerValueMetaClass(keyMetaClassInstance);
			TOdlDatabase::Instance().RegisterMetaClass(newMetaClassBase);
			metaClassBase = newMetaClassBase;
		}
		
		return metaClassBase;
	}

	static TMetaClassBase* CreateMetaClassInstance(char const* parMetaClassName)
	{
		TMetaClassBase* metaClassBase = new TMetaClass< std::map< TKey, TValue, TCompare > >(parMetaClassName, TMetaClassType::Map);
		return metaClassBase;
	}

	static TMetaClassBase const* ContainerValueMetaClass_IFP() 
	{ 
		return TMetaClassTraits< TValueNoPointer >::GetMetaClassInstance(); 
	}
};
//-------------------------------------------------------------------------------
template<> struct TMetaClassTraits< TOdlNull >
{
    static TMetaClassBase const* GetMetaClassInstance()
    {
        return nullptr;
    }
};
//-------------------------------------------------------------------------------
//*******************************************************************************
//-------------------------------------------------------------------------------
template < typename TValue, typename TAllocator >
bool SetValue(std::vector< TValue, TAllocator >& outVector, TOdlExpression const& parExpression)
{
    if (parExpression.Type() == TOdlExpression::VECTOR)
    {
        // PAUL(26/05/14 18:34:31) pas de check sur les types des vecteurs.
        // on voit ca au set de la valeur.
        // faut check la compatibilite avec les maps tousssa peut etre quand m�me...
        size_t const elementCount = parExpression.ValueUnion().FValueArray.FVectorCount;
        outVector.reserve(outVector.size() + elementCount);

        for (size_t i = 0; i < elementCount; ++i)
        {
            outVector.push_back(TValue());
            TOdlExpression const& subExpression = parExpression.ValueUnion().FValueArray.FVectorValues[i];
            if (!SetValue(outVector.back(), subExpression))
            {
                assert(false);
            }
        }
        return true;
    }
    
    assert(false); // {TODO} invalid type
    return false;
}
//-------------------------------------------------------------------------------
template < typename TKey, typename TValue, typename TCompare, typename TAllocator >
bool SetValue(std::map< TKey, TValue, TCompare, TAllocator >& outMap, TOdlExpression const& parExpression)
{
    if (parExpression.Type() == TOdlExpression::VECTOR)
    {
        if (parExpression.ValueUnion().FValueArray.FVectorCount == 2)
        {
            TKey key;
            TValue value;
            if (SetValue(key, parExpression.ValueUnion().FValueArray.FVectorValues[0]))
            {
                if (SetValue(value, parExpression.ValueUnion().FValueArray.FVectorValues[1]))
                {
                    outMap[key] = value;
                }
            }
        }
        else
        {
            assert(false); // map non compose de pair.
        }
    }
    else
    {
        assert(false); // map non compose de pair.
    }

    return false;
}
//-------------------------------------------------------------------------------
inline bool SetValue(float& outFloat, TOdlExpression const& parExpression)
{
    if (parExpression.Type() == TOdlExpression::FLOAT)
    {
        outFloat = parExpression.ValueUnion().FFloat;
        return true;
    }
    assert(false); // {TODO} invalid type
    return false;
}
//-------------------------------------------------------------------------------
inline bool SetValue(int& outInt, TOdlExpression const& parExpression)
{
    if (parExpression.Type() == TOdlExpression::INTEGER)
    {
        outInt = parExpression.ValueUnion().FInteger;
        return true;
    }
    assert(false); // {TODO} invalid type
    return false;
}
//-------------------------------------------------------------------------------
inline bool SetValue(std::string& outString, TOdlExpression const& parExpression)
{
    if (parExpression.Type() == TOdlExpression::STRING)
    {
        outString = parExpression.ValueUnion().FString;
        return true;
    }
    assert(false); // {TODO} invalid type
    return false;
}
//-------------------------------------------------------------------------------
template < typename TUserClass >
inline bool SetValue(TUserClass*& parUserClassPointer, TOdlExpression const& parExpression)
{
    if (parExpression.Type() == TOdlExpression::OBJECT)
    {
        TMetaClassBase const* userMetaClassBase = TMetaClassTraits< TUserClass >::GetMetaClassInstance();
        if (userMetaClassBase->IsGivenTypeCompatible(parExpression.MetaClassBase()))
        {
            parUserClassPointer = static_cast<TUserClass*>(parExpression.ValueUnion().FObject);
            return true;
        }
        else
        {
            assert(false); // invalid type
        }
    }

    assert(false); // invalid type

    return false;
}
//-------------------------------------------------------------------------------
template < typename TUserClass >
inline bool SetValue(TUserClass& parUserClassInPlace, TOdlExpression const& parExpression)
{
    if (parExpression.Type() == TOdlExpression::OBJECT)
    {
        TMetaClassBase const* userMetaClassBase = TMetaClassTraits< TUserClass >::GetMetaClassInstance();
        if (userMetaClassBase->IsGivenTypeCompatible(parExpression.MetaClassBase()))
        {
            parUserClassInPlace = *static_cast<TUserClass*>(parExpression.ValueUnion().FObject);
            return true;
        }
        else
        {
            assert(false); // invalid type
        }
    }

    assert(false); // invalid type
    return false;
}
//--------------------------------------------------------
//********************************************************
//--------------------------------------------------------
template < typename TPropertyType >
class TProperty;
//--------------------------------------------------------
class TPropertyBase
{
public:
	TPropertyBase(char const* parName,
				  TMetaClassBase const* parMetaClassBase,
				  size_t parPropertyOffset) :
		FName(parName),
		FType(parMetaClassBase),
		FPropertyOffset(parPropertyOffset)
	{

	}

	char const* Name() const { return FName; }
	TMetaClassBase const* Type() const { return FType; }
	char const* TypeName() const { return FType->Name(); }

	template < typename TPropertyType >
	TProperty< TPropertyType > const* TypedPropertyIfTypeCompatible(TMetaClassBase const* parRequestedType) const
	{
		if (FType->IsGivenTypeCompatible(parRequestedType))
		{
			return static_cast< TProperty< TPropertyType > const* >(this);
		}

		return nullptr;
	}

    virtual bool SetObjectPropertyByExpression_ROK(TOdlObject* parObject, TOdlExpression const& parExpression) const = 0;

protected:
	size_t PropertyOffset() const { return FPropertyOffset; }

private:
	char const* FName;
	TMetaClassBase const* FType;
	size_t FPropertyOffset;
};
//--------------------------------------------------------
template < typename TPropertyType >
class TTypedProperty : public TPropertyBase
{
    typedef TPropertyBase parent_type;
public:
    TTypedProperty(char const* parName,
		           TMetaClassBase const* parMetaClassBase,
			       size_t parPropertyOffset) :
		parent_type(parName, parMetaClassBase, parPropertyOffset)
	{
	}

	TPropertyType* TypedPointer(TOdlObject* parObject) const
	{
		typedef TPropertyType TOdlObject::*TPointerToMemberType;

		size_t const propertyOffset = PropertyOffset();
		TPointerToMemberType pointerToMember = (*((TPointerToMemberType*)&propertyOffset));
		TPropertyType& truePropertyPointer = parObject->*pointerToMember;
		return &truePropertyPointer;
	}
};
//-------------------------------------------------------------------------------
// ObjectInPlace or Unmatched case.
template < typename TPropertyType >
class TProperty : public TTypedProperty< TPropertyType >
{
	typedef TTypedProperty< TPropertyType > parent_type;
public:
	TProperty(char const* parName,
		      TMetaClassBase const* parMetaClassBase,
			  size_t parPropertyOffset) :
		parent_type(parName, parMetaClassBase, parPropertyOffset)
	{
	}

    virtual bool SetObjectPropertyByExpression_ROK(TOdlObject* parObject, TOdlExpression const& parExpression) const override
	{
        TPropertyType* objectInPlace = TypedPointer(parObject);
        if (SetValue(*objectInPlace, parExpression))
            return true;
        assert(false); // failed set object in place.
        return false;
    };
};

//-------------------------------------------------------------------------------
// ObjectPointer
template < typename TPropertyType >
class TProperty< TPropertyType* > : public TTypedProperty< TPropertyType* >
{
	typedef TTypedProperty< TPropertyType* > parent_type;
public:
    TProperty(char const* parName,
		      TMetaClassBase const* parMetaClassBase,
			  size_t parPropertyOffset) :
		parent_type(parName, parMetaClassBase, parPropertyOffset)
	{
	}


    virtual bool SetObjectPropertyByExpression_ROK(TOdlObject* parObject, TOdlExpression const& parExpression) const override
	{
        TPropertyType** objectPointer = TypedPointer(parObject);
        if (SetValue(*objectPointer, parExpression))
            return true;
        assert(false); // failed set object pointer
        return false;
    };

};
//--------------------------------------------------------
template < >
class TProperty< int > : public TTypedProperty< int >
{
	typedef TTypedProperty< int > parent_type;
public:
    TProperty(char const* parName,
		      TMetaClassBase const* parMetaClassBase,
			  size_t parPropertyOffset) :
		parent_type(parName, parMetaClassBase, parPropertyOffset)
	{
	}


    virtual bool SetObjectPropertyByExpression_ROK(TOdlObject* parObject, TOdlExpression const& parExpression) const override
	{
        int* intPointer = TypedPointer(parObject);
        if (SetValue(*intPointer, parExpression))
            return true;
        assert(false); // failed set object pointer
        return false;
    };

};
//--------------------------------------------------------
template < >
class TProperty< float > : public TTypedProperty< float >
{
	typedef TTypedProperty< float > parent_type;
public:
    TProperty(char const* parName,
		      TMetaClassBase const* parMetaClassBase,
			  size_t parPropertyOffset) :
		parent_type(parName, parMetaClassBase, parPropertyOffset)
	{
	}


    virtual bool SetObjectPropertyByExpression_ROK(TOdlObject* parObject, TOdlExpression const& parExpression) const override
	{
        float* floatPointer = TypedPointer(parObject);
        if (SetValue(*floatPointer, parExpression))
            return true;
        assert(false); // failed set float
        return false;
    };

};
//-------------------------------------------------------------------------------
template < >
class TProperty< std::string > : public TTypedProperty< std::string >
{
	typedef TTypedProperty< std::string > parent_type;
public:
    TProperty(char const* parName,
		      TMetaClassBase const* parMetaClassBase,
			  size_t parPropertyOffset) :
		parent_type(parName, parMetaClassBase, parPropertyOffset)
	{
	}


    virtual bool SetObjectPropertyByExpression_ROK(TOdlObject* parObject, TOdlExpression const& parExpression) const override
	{
        std::string* stringPointer = TypedPointer(parObject);
        if (SetValue(*stringPointer, parExpression))
            return true;
        assert(false); // failed set string
        return false;
    };

};
//-------------------------------------------------------------------------------
//*******************************************************************************
//-------------------------------------------------------------------------------
template < typename TValue, typename TAllocator >
class TProperty< std::vector< TValue, TAllocator > > : public TTypedProperty< std::vector< TValue, TAllocator > >
{
    typedef std::vector< TValue, TAllocator > TVectorType;
	typedef TTypedProperty< std::vector< TValue, TAllocator > > parent_type;
public:
    TProperty(char const* parName,
		      TMetaClassBase const* parMetaClassBase,
			  size_t parPropertyOffset) :
		parent_type(parName, parMetaClassBase, parPropertyOffset)
	{
	}


    virtual bool SetObjectPropertyByExpression_ROK(TOdlObject* parObject, TOdlExpression const& parExpression) const override
	{
        TVectorType* vectorPointer = TypedPointer(parObject);
        if (SetValue(*vectorPointer, parExpression))
            return true;
        assert(false); // set vector failed.
        return false;
    };
};
//-------------------------------------------------------------------------------
template< typename TKey, typename TValue, typename TCompare, typename TAllocator  >
class TProperty< std::map< TKey, TValue, TCompare, TAllocator > > : public TTypedProperty< std::map< TKey, TValue, TCompare, TAllocator > >
{
    typedef std::map< TKey, TValue, TCompare, TAllocator > TMapType;
	typedef TTypedProperty< std::map< TKey, TValue, TCompare, TAllocator > > parent_type;
public:
    TProperty(char const* parName,
		      TMetaClassBase const* parMetaClassBase,
			  size_t parPropertyOffset) :
		parent_type(parName, parMetaClassBase, parPropertyOffset)
	{
	}


    virtual bool SetObjectPropertyByExpression_ROK(TOdlObject* parObject, TOdlExpression const& parExpression) const override
	{
        TMapType* mapType = TypedPointer(parObject);
        if (SetValue(*mapType, parExpression))
            return true;
        return false;
    };
};
//--------------------------------------------------------
//********************************************************
//--------------------------------------------------------
} // namespace odl

#define DECLARE_METACLASS_HEADER(TUserClass, TParentType)																	\
	private:																												\
    typedef TParentType parent_type;                                                                                        \
	static odl::TMetaClassBase const* FMetaClassInstance;																	\
	public:																													\
	static odl::TMetaClassBase const* GetMetaClassInstance() { return FMetaClassInstance; }									\
	static void RegisterMetaClass();																						\

#define BEGIN_METACLASS_CPP(TUserClass)																										\
	odl::TMetaClassBase const* TUserClass::FMetaClassInstance = nullptr;																	\
	void TUserClass::RegisterMetaClass()																									\
	{																																		\
		odl::TMetaClassBase* userClassMetaClassBase = odl::TMetaClassTraits< TUserClass >::CreateMetaClassInstance(#TUserClass);			\
        odl::TMetaClassBase const* parentTypeMetaClassBase = odl::TMetaClassTraits< parent_type >::GetMetaClassInstance();                  \
        userClassMetaClassBase->SetParentType(parentTypeMetaClassBase);                                                                     \
        FMetaClassInstance = userClassMetaClassBase;																						\
		typedef TUserClass TCurrentRegisterClassUserClass;																					\
		TUserClass* dummyObjectPointer = nullptr;																							\
		dummyObjectPointer = dummyObjectPointer; // remove warning.

#define END_METACLASS_CPP()																													\
		odl::TOdlDatabase::Instance().RegisterMetaClass(userClassMetaClassBase);															\
	}

#define DECLARE_VALUE_METACLASS(TValueType)																						\
	inline void odl_ ## TValueType ## RegisterMetaClass()																		\
	{																															\
		odl::TMetaClassBase* metaClassBase = odl::TMetaClassTraits< TValueType >::CreateMetaClassInstance(#TValueType);			\
		odl::TOdlDatabase::Instance().RegisterMetaClass(metaClassBase);															\
	}

#define REGISTER_VALUE_METACLASS(TValueType)																				\
	odl::odl_ ## TValueType ## RegisterMetaClass();																			\

#define PROPERTYFIELD(PropertyMember)																												\
	{																																				\
		typedef decltype(dummyObjectPointer->PropertyMember) TPropertyType;																			\
        typedef odl::TRemovePointerForObjectTypeIFN< TPropertyType >::TType TPropertyTypeNoPointer;                                                 \
		odl::TMetaClassBase const* metaClassBaseInstance = odl::TMetaClassTraits< TPropertyTypeNoPointer >::GetMetaClassInstance();					\
		size_t const propertyOffset = (size_t)&static_cast<TCurrentRegisterClassUserClass*>(nullptr)->PropertyMember;					            \
		odl::TProperty< TPropertyType >* property = new odl::TProperty< TPropertyType >(#PropertyMember, metaClassBaseInstance, propertyOffset);	\
		userClassMetaClassBase->RegisterProperty(property);																							\
	}

namespace odl
{
//--------------------------------------------------------
//********************************************************
//--------------------------------------------------------
class TOdlObject
{
public:
    TOdlObject() {};

    DECLARE_METACLASS_HEADER(TOdlObject, TOdlNull)
};
//--------------------------------------------------------
//********************************************************
//--------------------------------------------------------
void RegisterOdlMetaClasses();
//--------------------------------------------------------
//********************************************************
//--------------------------------------------------------
} // namespace odl

#endif // ODL_H