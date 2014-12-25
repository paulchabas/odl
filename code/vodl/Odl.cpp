#include "StdAfx.h"
#include "Odl.h"


namespace odl
{
BEGIN_METACLASS_CPP(TOdlObject)
END_METACLASS_CPP()
//--------------------------------------------------------
//********************************************************
//--------------------------------------------------------
DECLARE_VALUE_METACLASS(bool)
DECLARE_VALUE_METACLASS(i32)
DECLARE_VALUE_METACLASS(float)
namespace
{
    using std::string;
    DECLARE_VALUE_METACLASS(string)
}
//--------------------------------------------------------
void RegisterOdlMetaClasses()
{
    using std::string;

    TOdlObject::RegisterMetaClass();
    REGISTER_VALUE_METACLASS(bool)
	REGISTER_VALUE_METACLASS(i32)
	REGISTER_VALUE_METACLASS(float)
	REGISTER_VALUE_METACLASS(string)
}
//--------------------------------------------------------
//********************************************************
//--------------------------------------------------------
TMetaClassBase::TMetaClassBase(char const* parName, TMetaClassType::Type parMetaClassType) :
    FName(parName),
	FMetaClassType(parMetaClassType),
	FClassProperties(),
	FContainerKeyMetaClass(nullptr),
	FContainerValueMetaClass(nullptr),
    FParentType(nullptr)
{

}
//-------------------------------------------------------------------------------
TMetaClassBase::~TMetaClassBase()
{
    for(TPropertyBase const* property : FClassProperties)
    {
        delete property;
    }
}
//-------------------------------------------------------------------------------
void TMetaClassBase::PrintContent(std::ostringstream& parOss) const
{
	if (FMetaClassType == TMetaClassType::Class)
	{
		parOss << "class " << Name() << std::endl;
		parOss << "(" << std::endl;
		for (u32 i = 0; i < FClassProperties.size(); ++i)
		{
			TPropertyBase const* property = FClassProperties[i];
			parOss << "    " << property->Name() << ": ";
			parOss << property->TypeName();
			parOss << std::endl;
		}
		parOss << ")" << std::endl;
	}
	else if (FMetaClassType == TMetaClassType::Value)
	{
		parOss << "value: " << Name() << std::endl;
	}
	else if (FMetaClassType == TMetaClassType::Vector)
	{
		parOss << "vector: " << Name() << std::endl;
	}
	else if (FMetaClassType == TMetaClassType::Map)
	{
		parOss << "map: " << Name() << std::endl;
	}
	else if (FMetaClassType == TMetaClassType::Pair)
	{
		parOss << "pair: " << Name() << std::endl;
	}
}
//--------------------------------------------------------
void TMetaClassBase::RegisterProperty(TPropertyBase const* parUserProperty)
{
	assert(FMetaClassType == TMetaClassType::Class);
	FClassProperties.push_back(parUserProperty);
}
//--------------------------------------------------------
TPropertyBase const* TMetaClassBase::PropertyByName(std::string const& parPropertyName) const
{
	for (size_t i = 0; i < FClassProperties.size(); ++i)
	{
		TPropertyBase const* property = FClassProperties[i];
		if (property->Name() == parPropertyName)
		{
			return property;
		}
	}

	if (FParentType != nullptr)
	{
		return FParentType->PropertyByName(parPropertyName);
	}

	return nullptr;
}
//-------------------------------------------------------------------------------
bool TMetaClassBase::IsGivenTypeCompatible(TMetaClassBase const* parMetaClassBase) const
{
    assert(parMetaClassBase != nullptr);
    if (this == parMetaClassBase)
    {
        return true;
    }
    else
    {
        if (parMetaClassBase != nullptr)
        {
            TMetaClassBase const* parentType = parMetaClassBase->ParentType();
            if (parentType != nullptr)
            {
                return IsGivenTypeCompatible(parentType);
            }
        }
    }

    return false;
}
//--------------------------------------------------------
void TMetaClassBase::SetContainerValueMetaClass(TMetaClassBase const* parValueMetaClass)
{
	assert(FMetaClassType == TMetaClassType::Vector || FMetaClassType == TMetaClassType::Map || FMetaClassType == TMetaClassType::Pair);
	FContainerValueMetaClass = parValueMetaClass;
}
//--------------------------------------------------------
void TMetaClassBase::SetContainerKeyMetaClass(TMetaClassBase const* parKeyMetaClass)
{
	assert(FMetaClassType == TMetaClassType::Vector || FMetaClassType == TMetaClassType::Map || FMetaClassType == TMetaClassType::Pair);
	FContainerKeyMetaClass = parKeyMetaClass;
}
//-------------------------------------------------------------------------------
//*******************************************************************************
//-------------------------------------------------------------------------------
void StartOdl()
{
	CreateTokenDatabases();
    new TOdlDatabase();
	RegisterOdlMetaClasses();
}
//-------------------------------------------------------------------------------
void EndOdl()
{
    TOdlDatabase::DestroyInstance();
	DestroyTokenDatabases();
}
//--------------------------------------------------------
//********************************************************
//--------------------------------------------------------
}