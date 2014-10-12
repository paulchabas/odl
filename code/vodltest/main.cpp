#include "StdAfx.h"
#include <stdlib.h>
#include <crtdbg.h>

#include "Windows.h"
#undef max
#undef min

#include <cassert>
#include <iostream>
#include <typeinfo>
#include <cstdlib>

#include "vOdl/BasicTypes.h"

#include <vector>
#include <map>
#include <string>

#include "vOdl/Odl.h"
#include "vOdl/OdlParser.h"
#include "vOdl/OdlInterpreter.h"



class TTestClass : public odl::TOdlObject
{
    DECLARE_METACLASS_HEADER(TTestClass, odl::TOdlObject)
public:
    TTestClass() :
		FInteger(-7),
        FFloat(-3.21f)
    {

    }

public:
    i32 FInteger;
    float FFloat;
};

class TTestClass2 : public TTestClass
{
    DECLARE_METACLASS_HEADER(TTestClass2, TTestClass)
public:
	TTestClass2() :
        FInteger(-4),
        FObjectPointer(nullptr),
		FTestClassBasePointer(nullptr),
		FTestClass2BasePointer(nullptr)
	{
	}

public:
    int FInteger;
    std::string FString;
	std::vector< int > FVector;
	std::vector< std::vector< float > > FVectorComposite;
	std::map< i32, std::vector< float > > FMapI32ToVectorFloat;

	std::map< i32, std::vector< std::string > > FMapI32ToVectorString;

	std::vector< float > FFloatVector;
	std::pair< float, float > FFloatFloatPair;

    std::vector< TTestClass* > FVectorOfObjectPointers;
    std::vector< TTestClass > FVectorOfObjectInPlace;
	std::pair< int, std::string > FIntStringPair;
	std::vector< std::pair< float, float > > FFloatFloatPairVector;
	std::pair< std::vector< float >, std::vector< int > > FFloatVectorIntVectorPair;

    TTestClass FObjectInPlace;
    TTestClass2* FObjectPointer;
	TTestClass* FTestClassBasePointer;
	TTestClass2* FTestClass2BasePointer;
};

BEGIN_METACLASS_CPP(TTestClass)
{
	PROPERTYFIELD(FInteger)
    PROPERTYFIELD(FFloat)
}
END_METACLASS_CPP()

//odl::TMetaClassBase const* TTestClass2::FMetaClassInstance = nullptr;
//void TTestClass2::RegisterMetaClass()
//{
//	TTestClass::RegisterMetaClass();
//	// odl::TMetaClassBase const* parentMetaClassIfn = odl::TMetaClassTraits< TTestClass >::GetOrCreateMetaClassInstance("TTestClass");
//
//	odl::TMetaClassBase* userClassMetaClassBase = odl::TMetaClassTraits< TTestClass2 >::CreateMetaClassInstance("TTestClass2");
//    odl::TMetaClassBase const* parentTypeMetaClassBase = odl::TMetaClassTraits< parent_type >::GetMetaClassInstance();
//    userClassMetaClassBase->SetParentType(parentTypeMetaClassBase);
//    FMetaClassInstance = userClassMetaClassBase;
//	typedef TTestClass2 TCurrentRegisterClassUserClass;
//	TTestClass2* dummyObjectPointer = nullptr;
//	dummyObjectPointer = dummyObjectPointer; // remove warning.
//
BEGIN_METACLASS_CPP(TTestClass2)
{
    PROPERTYFIELD(FString)
    PROPERTYFIELD(FInteger)
	PROPERTYFIELD(FVectorComposite)
	PROPERTYFIELD(FVector)
	PROPERTYFIELD(FMapI32ToVectorFloat);
	PROPERTYFIELD(FMapI32ToVectorString);
	PROPERTYFIELD(FFloatVector);
	PROPERTYFIELD(FFloatFloatPair);

    PROPERTYFIELD(FObjectPointer);
    PROPERTYFIELD(FObjectInPlace);
    PROPERTYFIELD(FVectorOfObjectPointers);
    PROPERTYFIELD(FVectorOfObjectInPlace);
	PROPERTYFIELD(FTestClassBasePointer);
	PROPERTYFIELD(FTestClass2BasePointer);

	PROPERTYFIELD(FIntStringPair);
	PROPERTYFIELD(FFloatFloatPairVector);
	PROPERTYFIELD(FFloatVectorIntVectorPair);
}
END_METACLASS_CPP()

void TrueMain()
{
	{
		char path[MAX_PATH] = { 0 };
		GetCurrentDirectory(MAX_PATH, path);
		std::cout << "current path: " << path << std::endl;
	}

    std::cout << "START PROGRAM" << std::endl;
    
	odl::CreateTokenDatabases();
    new odl::TOdlDatabase();

	odl::RegisterOdlMetaClasses();

	TTestClass2::RegisterMetaClass();
    TTestClass::RegisterMetaClass();


    {
	    std::ostringstream oss;
	    odl::TOdlDatabase::Instance().PrintRegisteredTypes(oss);
	    std::cout << oss.str() << std::endl;
		std::cout << "-----------------------------------" << std::endl;
    }


	//TTestClass2 testClass2;
	//std::cout << "FFloat: " << testClass2.FFloat << std::endl;
	//odl::TMetaClassBase const* metaClassBase = TTestClass2::GetMetaClassInstance();
	//odl::TPropertyBase const* propertyBase = metaClassBase->PropertyByName("FInteger");
 //   odl::TMetaClassBase const* intMetaClassBase = odl::TMetaClassTraits< int >::GetMetaClassInstance();
	//odl::TProperty< i32 > const* propertyFloat = propertyBase->TypedPropertyIfTypeCompatible< i32 >(intMetaClassBase);
	//int* integerPointer = propertyFloat->TypedPointer(&testClass2);
	//*integerPointer = 2;
	//std::cout << "FInteger: " << testClass2.FInteger << std::endl;

	std::cout << "PARSING" << std::endl;
    
    odl::TOdlAstNode const* odlAst = NULL;
	odl::TOdlParser odlParser;
	// odlAst = odlParser.ParseFile("Data/main.odl");
	odlAst = odlParser.ParseFile("Data/circular_reference.odl");
	// odlAst = odlParser.ParseFile("Data/float.odl");
   
    {
		std::cout << "-----------------------------" << std::endl;
        std::ostringstream oss;
        odlParser.PrettyPrint(oss);
        std::cout << oss.str();
    }

    {
        if (odlAst != NULL)
        {
            odlParser.InterpretAst();
        }
    }

    {
        // TTestClass2* sum1 = odl::TOdlDatabase::Instance().GetObject_IFP<TTestClass2>("tata/object");
		TTestClass2* sum1 = odl::TOdlDatabase::Instance().GetObject_IFP<TTestClass2>("root");
        if (sum1)
        {
            int a = 0;
        }
    }

	odl::TOdlDatabase::DestroyInstance();
	odl::DestroyTokenDatabases();

	system("pause");
	std::cout << "END PROGRAM" << std::endl;
}

int main()
{
    // Get the current bits

    TrueMain();

    _CrtDumpMemoryLeaks();

	return 0;
}



