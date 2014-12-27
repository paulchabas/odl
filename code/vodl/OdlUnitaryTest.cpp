#include "stdafx.h"
#include "OdlUnitaryTest.h"

#include "Odl.h"

namespace odl
{
//-------------------------------------------------------------------------------
//*******************************************************************************
//-------------------------------------------------------------------------------
class TOdlFlatObjectDumper
{
private:
    typedef std::unordered_map< odl::TOdlObject const*, std::string > TKnownObjectsAndGeneratedNames;

public:
    TOdlFlatObjectDumper() :
        FOutputStream(nullptr),
        FGenerator(0)
    {
    }

    void SetCurrentOutputStream(std::ostringstream* parOutputStream) { FOutputStream = parOutputStream; }

    void VisitObject(TOdlObject const* parObject)
    {
        assert(FOutputStream != nullptr);
        assert(FPendingTypedObjects.empty());

        TOdlObject const* currentObject = parObject;
        odl::TMetaClassBase const* currentMetaClass = odl::TOdlDatabase::Instance().GetObjectMetaClassBase_IFP(parObject);
        FKnownObjectsAndGeneratedNames[currentObject] = GenerateObjectName();
        do
        {
            std::string const& currentObjectGeneratedName = FKnownObjectsAndGeneratedNames.find(currentObject)->second;

            assert(currentMetaClass != nullptr);
            assert(currentMetaClass->MetaClassType() == TMetaClassType::Class);

            OutputStreamAssumeValid() << currentObjectGeneratedName << " is " << currentMetaClass->Name() << std::endl;
            OutputStreamAssumeValid() << '{' << std::endl;

            std::vector< TMetaClassBase const* > parentTypes;
            {
                parentTypes.push_back(currentMetaClass);
                TMetaClassBase const* parentMetaClass = currentMetaClass->ParentType();
                while (parentMetaClass != nullptr)
                {
                    parentTypes.push_back(parentMetaClass);
                    parentMetaClass = parentMetaClass->ParentType();
                }
            }

            char const* indentation = "    ";
            while (!parentTypes.empty())
            {
                TMetaClassBase const* metaClassInHierarchy = parentTypes.back();
                parentTypes.pop_back();

                std::vector< TPropertyBase const* > const& properties = metaClassInHierarchy->ClassProperties();
                for (TPropertyBase const* property : properties)
                {
                    char const* propertyName = property->Name();
                    odl::TMetaClassBase const* propertyMetaClass = property->Type();
                    // Paul(2014/12/24) i make no differences here between in-place field, and pointer field.
                    odl::TOdlExpression expression;
                    property->GetObjectPropertyExpression(currentObject, expression);

                    OutputStreamAssumeValid() << indentation << propertyName << " = ";
                    PrintPropertyExpression(expression);
                    OutputStreamAssumeValid() << std::endl;
                }
            }

            OutputStreamAssumeValid() << '}' << std::endl;
            OutputStreamAssumeValid() << std::endl;

            if (!FPendingTypedObjects.empty())
            {
                std::pair< TOdlObject const*, TMetaClassBase const* > objectAndMetaClass = FPendingTypedObjects.front();
                currentObject = objectAndMetaClass.first;
                currentMetaClass = objectAndMetaClass.second;
                // pop_front
                FPendingTypedObjects.erase(FPendingTypedObjects.begin());
            }
            else
            {
                currentObject = nullptr;
                currentMetaClass = nullptr;
            }
        } 
        while (currentObject != nullptr);

        assert(FPendingTypedObjects.empty());
    }

private:
    std::string GenerateObjectName()
    {
        char buffer[16];
        sprintf_s(buffer, 16, "object%06d", FGenerator++);
        return std::string(buffer);
    }

    std::ostringstream& OutputStreamAssumeValid() { assert(FOutputStream != nullptr); return *FOutputStream; }

    void PrintPropertyExpression(odl::TOdlExpression const& parExpression)
    {
        switch (parExpression.Type())
        {
        case odl::TOdlExpression::NULLEXP:
            {
                OutputStreamAssumeValid() << "nullptr";
            }
            break;
        case odl::TOdlExpression::OBJECT:
            {
                odl::TOdlObject const* objectPointer = parExpression.ValueUnion().FObject;
                if (objectPointer == nullptr)
                {
                    OutputStreamAssumeValid() << "nullptr";
                }
                else
                {
                    TKnownObjectsAndGeneratedNames::const_iterator it = FKnownObjectsAndGeneratedNames.find(objectPointer);
                    std::string objectName;
                    if (it == FKnownObjectsAndGeneratedNames.end())
                    {
                        objectName = GenerateObjectName();
                        FKnownObjectsAndGeneratedNames[objectPointer] = objectName;
                        std::pair< TOdlObject const*, TMetaClassBase const* > objectAndMetaClass(objectPointer, parExpression.MetaClassBase());
                        FPendingTypedObjects.push_back(objectAndMetaClass);                    
                    }
                    else
                    {
                        objectName = it->second;
                    }

                    OutputStreamAssumeValid() << objectName;
                }
            }
            break;
        case odl::TOdlExpression::VECTOR:
            {
                OutputStreamAssumeValid() << "[ ";

                u32 const expressionCount = parExpression.ValueUnion().FValueArray.FVectorCount;
                TOdlExpression const* expressionArray = parExpression.ValueUnion().FValueArray.FVectorValues;
                for (u32 i = 0; i < expressionCount; ++i)
                {
                    TOdlExpression const& itemExpression = expressionArray[i];
                    PrintPropertyExpression(itemExpression);
                    if (i + 1 < expressionCount)
                        OutputStreamAssumeValid() << ", ";
                }

                OutputStreamAssumeValid() << " ]";
            }
            break;
        case odl::TOdlExpression::BOOLEAN:
            {
                if (parExpression.ValueUnion().FBoolean)
                    OutputStreamAssumeValid() << "true";
                else
                    OutputStreamAssumeValid() << "false";
            }
            break;
        case odl::TOdlExpression::FLOAT:
            {
                OutputStreamAssumeValid() << parExpression.ValueUnion().FFloat;
            }
            break;
        case odl::TOdlExpression::INTEGER:
            {
                OutputStreamAssumeValid() << parExpression.ValueUnion().FInteger;
            }
            break;
        case odl::TOdlExpression::STRING:
            {
                OutputStreamAssumeValid() << '"' << parExpression.ValueUnion().FString << '"';
            }
            break;
        default:
            {
                assert(false);
            }
            break;
        };
    }

private:
    TKnownObjectsAndGeneratedNames FKnownObjectsAndGeneratedNames;
    std::vector< std::pair< odl::TOdlObject const*, odl::TMetaClassBase const* > > FPendingTypedObjects;
    std::ostringstream* FOutputStream;
    u32 FGenerator;
};
//-------------------------------------------------------------------------------
//*******************************************************************************
//-------------------------------------------------------------------------------
class TOdlTestClassBase : public odl::TOdlObject
{
    DECLARE_METACLASS_HEADER(TOdlTestClassBase, odl::TOdlObject)
public:
    TOdlTestClassBase() :
        FBool(false),
		FInteger(-7),
        FFloat(-3.21f)
    {

    }

public:
    bool FBool;
    i32 FInteger;
    float FFloat;
};
//-------------------------------------------------------------------------------
BEGIN_METACLASS_CPP(TOdlTestClassBase)
{
    PROPERTYFIELD(FBool);
	PROPERTYFIELD(FInteger)
    PROPERTYFIELD(FFloat)
}
END_METACLASS_CPP()
//-------------------------------------------------------------------------------
//*******************************************************************************
//-------------------------------------------------------------------------------
class TOdlTestClassChild : public TOdlTestClassBase
{
    DECLARE_METACLASS_HEADER(TOdlTestClassChild, TOdlTestClassBase)
public:
	TOdlTestClassChild() :
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
    std::vector< TOdlTestClassBase* > FVectorOfObjectPointers;
    std::vector< TOdlTestClassBase > FVectorOfObjectInPlace;
	std::pair< int, std::string > FIntStringPair;
	std::vector< std::pair< float, float > > FFloatFloatPairVector;
	std::pair< std::vector< float >, std::vector< int > > FFloatVectorIntVectorPair;
    TOdlTestClassBase FObjectInPlace;
    TOdlTestClassChild* FObjectPointer;
	TOdlTestClassBase* FTestClassBasePointer;
	TOdlTestClassChild* FTestClass2BasePointer;
};
//-------------------------------------------------------------------------------
BEGIN_METACLASS_CPP(TOdlTestClassChild)
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
//-------------------------------------------------------------------------------
//*******************************************************************************
//-------------------------------------------------------------------------------
class TOdlTestResult : public odl::TOdlObject
{
    DECLARE_METACLASS_HEADER(TOdlTestResult, TOdlObject)
public:
    
    TOdlTestResult() :
        FTestedObject(nullptr)
    {

    }

    bool CheckTestResult(u32 parVerboseLevel)
    {
        std::ostringstream oss;
        FOdlFlatObjectDumper.SetCurrentOutputStream(&oss);

        if (FTestedObject != nullptr)
        {
            FOdlFlatObjectDumper.VisitObject(FTestedObject);
        }
        FOdlFlatObjectDumper.SetCurrentOutputStream(nullptr);

        std::string result = oss.str();

        if (!FDumpOutputFilename.empty())
        {
            std::ofstream outputStream(FDumpOutputFilename.c_str());
            outputStream << result;
        }

        std::ostringstream oss2;
        if (!FExpectedOutputFilename.empty())
        {
            std::ifstream inputStream(FExpectedOutputFilename.c_str());

            while (!inputStream.eof())
            {
                char c = inputStream.get();
                if (c == EOF)
                {
                    break ;
                }
                oss2 << c;
            }
        }

        std::string expected = oss2.str();
        if (expected == result)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

private:
    std::string FDumpOutputFilename;
    odl::TOdlTestClassBase* FTestedObject;
    std::string FExpectedOutputFilename;
    odl::TOdlFlatObjectDumper FOdlFlatObjectDumper;
};
//-------------------------------------------------------------------------------
BEGIN_METACLASS_CPP(TOdlTestResult)
{
    PROPERTYFIELD(FDumpOutputFilename)
    PROPERTYFIELD(FTestedObject);
    PROPERTYFIELD(FExpectedOutputFilename)
}
END_METACLASS_CPP()
//-------------------------------------------------------------------------------
//*******************************************************************************
//-------------------------------------------------------------------------------
void StartOdlTests()
{
    StartOdl();
    
    TOdlTestClassChild::RegisterMetaClass();
    TOdlTestClassBase::RegisterMetaClass();
    TOdlTestResult::RegisterMetaClass();

    std::ostringstream oss;
	odl::TOdlDatabase::Instance().PrintRegisteredTypes(oss);
	std::cout << oss.str() << std::endl;
}
//-------------------------------------------------------------------------------
void EndOdlTests()
{
    EndOdl();
}
//-------------------------------------------------------------------------------
//*******************************************************************************
//-------------------------------------------------------------------------------
TOdlUnitaryTest::TOdlUnitaryTest(char const* parFilename, char const* parTestName, u32 parVerboseLevel) :
    FFilename(parFilename),
    FTestName(parTestName),
    FVerboseLevel(parVerboseLevel)
{
    if (parVerboseLevel > 1)
        std::cout << "BEGIN TEST: [" << parTestName << "]" << std::endl;
    if (FVerboseLevel > 1)
        std::cout << "* PARSING" << std::endl;
    
    odl::TOdlAstNode const* odlAst = NULL;
	odl::TOdlParser odlParser;
	odlAst = odlParser.ParseFile(parFilename);

    if (odlAst == nullptr)
    {
        if (FVerboseLevel > 1)
            std::cout << "error: " << parFilename << " file not found" << std::endl;
    }

    if (FVerboseLevel > 1)
    {
        std::cout << "* PRETTY PRINT AST" << std::endl;
        std::ostringstream oss;
        odlParser.PrettyPrint(oss);
        std::cout << oss.str();
    }

    if (FVerboseLevel > 1)
    {
        std::cout << "* INTERPRETING AST" << std::endl;
    }
    if (odlAst != nullptr)
    {
        odlParser.InterpretAst();
    }

    char const* testResultCode = "[KO]";
    if (odlAst != nullptr)
    {
        if (FVerboseLevel > 1)
            std::cout << "* TESTING" << std::endl;
		odl::TOdlTestResult* testResultObject = odl::TOdlDatabase::Instance().GetObject_IFP<odl::TOdlTestResult>("testResult");
        if (testResultObject != nullptr)
        {
            if (testResultObject->CheckTestResult(parVerboseLevel))
                testResultCode = "[OK]";
        }
    }


    if (parVerboseLevel > 1)
        std::cout << "END TEST:";
    if (parVerboseLevel > 0)
        std::cout << "test: " << "[" << parTestName << "] " << testResultCode << std::endl;
}
//-------------------------------------------------------------------------------
TOdlUnitaryTest::~TOdlUnitaryTest()
{
    odl::TOdlDatabase::Instance().ClearObjectDatabase();
}
//-------------------------------------------------------------------------------
//*******************************************************************************
//-------------------------------------------------------------------------------
} // odl

