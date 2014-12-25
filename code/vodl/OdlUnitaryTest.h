#pragma once

#include "vOdlParserLexer/BasicTypes.h"

namespace odl
{
//-------------------------------------------------------------------------------
//*******************************************************************************
//-------------------------------------------------------------------------------
class TOdlUnitaryTest
{
public:
    TOdlUnitaryTest(char const* parFilename, 
                    char const* parTestName,
                    u32 parVerboseLevel);
    ~TOdlUnitaryTest();

private:
    char const* FFilename;
    char const* FTestName;
    u32 const FVerboseLevel;
};
//-------------------------------------------------------------------------------
void StartOdlTests();
void EndOdlTests();
//-------------------------------------------------------------------------------
//*******************************************************************************
//-------------------------------------------------------------------------------
} // odl



