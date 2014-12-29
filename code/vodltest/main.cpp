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
    odl::TOdlUnitaryTest("Data/Tests/simple_properties_loading.odl", "SimplePropertiesLoading", 1);
    odl::TOdlUnitaryTest("Data/Tests/name_resolution.odl", "NameResolution", 1);
    odl::TOdlUnitaryTest("Data/Tests/simple_template_object.odl", "SimpleTemplateObject", 1);
    odl::TOdlUnitaryTest("Data/Tests/simple_template_namespace.odl", "SimpleTemplateNamespace", 1);
    odl::TOdlUnitaryTest("Data/Tests/one_recursion_template_namespace.odl", "OneRecursionTemplateNamespace", 1);
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



