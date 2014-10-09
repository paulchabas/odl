#include "StdAfx.h"
#include <stdlib.h>
#include <crtdbg.h>

#include <cassert>
#include <iostream>
#include <typeinfo>
#include <cstdlib>

#include "vOdl/BasicTypes.h"

#include <vector>
#include <map>
#include <string>

#include "vOdl/odl.h"
#include "vOdl/OdlValueDeclaration.h"

#include "vOdl/OdlParser.h"
#include "vOdl/OdlInterpreter.h"



class TTestClass : public odl::TOdlObject
{
    DECLARE_METACLASS_HEADER(TTestClass, odl::TOdlObject)
public:
    TTestClass() :
        FFloat(-1.0f)
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
        FObjectPointer(nullptr)
	{
	}

public:
    int FInteger;
    std::string FString;
	std::vector< int > FVector;
	std::vector< std::vector< float > > FVectorComposite;
	std::map< i32, std::vector< float > > FMapI32ToVectorFloat;

	std::map< i32, std::vector< std::string > > FMapI32ToVectorString;

    std::vector< TTestClass* > FVectorOfObjectPointers;
    std::vector< TTestClass > FVectorOfObjectInPlace;

    TTestClass FObjectInPlace;
    TTestClass2* FObjectPointer;
};

BEGIN_METACLASS_CPP(TTestClass)
{
	PROPERTYFIELD(FInteger)
    PROPERTYFIELD(FFloat)
}
END_METACLASS_CPP()

BEGIN_METACLASS_CPP(TTestClass2)
{
    PROPERTYFIELD(FString)
    PROPERTYFIELD(FInteger)
	PROPERTYFIELD(FVectorComposite)
	PROPERTYFIELD(FVector)
	PROPERTYFIELD(FMapI32ToVectorFloat);
	PROPERTYFIELD(FMapI32ToVectorString);

    PROPERTYFIELD(FObjectPointer);
    PROPERTYFIELD(FObjectInPlace);
    PROPERTYFIELD(FVectorOfObjectPointers);
    PROPERTYFIELD(FVectorOfObjectInPlace);
}
END_METACLASS_CPP()

void TrueMain()
{
    std::cout << "START PROGRAM" << std::endl;
    
	odl::CreateTokenDatabases();
    new odl::TOdlDatabase();

	odl::RegisterOdlMetaClasses();
    TTestClass::RegisterMetaClass();
    TTestClass2::RegisterMetaClass();

    {
	    std::ostringstream oss;
	    odl::TOdlDatabase::Instance().PrintContent(oss);
	    std::cout << oss.str() << std::endl;
    }


	TTestClass2 testClass2;

	std::cout << "FFloat: " << testClass2.FFloat << std::endl;

	odl::TMetaClassBase const* metaClassBase = TTestClass2::GetMetaClassInstance();
	odl::TPropertyBase const* propertyBase = metaClassBase->PropertyByName("FInteger");

    odl::TMetaClassBase const* intMetaClassBase = odl::TMetaClassTraits< int >::GetMetaClassInstance();

	odl::TProperty< i32 > const* propertyFloat = propertyBase->TypedPropertyIfTypeCompatible< i32 >(intMetaClassBase);
	int* integerPointer = propertyFloat->TypedPointer(&testClass2);
	*integerPointer = 2;
	std::cout << "FInteger: " << testClass2.FInteger << std::endl;

	std::cout << "PARSING" << std::endl;

    
    odl::TOdlAstNode const* odlAst = NULL;
	odl::TOdlParser odlParser;
	odlAst = odlParser.ParseFile("Data/main.odl");
   
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
        TTestClass2* object = odl::TOdlDatabase::Instance().GetObject_IFP<TTestClass2>("object0");
        if (object)
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



