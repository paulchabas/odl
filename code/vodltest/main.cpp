#include "StdAfx.h"

#include <crtdbg.h>
#include "Windows.h"
#undef max
#undef min

#include "vOdl/Odl.h"
#include "vOdl/OdlUnitaryTest.h"

void TrueMain()
{
	{
		char path[MAX_PATH] = { 0 };
		GetCurrentDirectory(MAX_PATH, path);
		std::cout << "current path: " << path << std::endl;
	}

    std::cout << "START PROGRAM" << std::endl;
    
    odl::StartOdlTests();
    // odl::TOdlUnitaryTest("Data/Tests/test0.odl", "PropertyLoading", 1);
    odl::TOdlUnitaryTest("Data/Tests/test1.odl", "CodingTemplate", 1);
    odl::EndOdlTests();

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



