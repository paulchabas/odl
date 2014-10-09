#include "StdAfx.h"
#include "OdlTokenDatabase.h"


namespace odl
{
//-------------------------------------------------------------------------------
//*******************************************************************************
//-------------------------------------------------------------------------------
void CreateTokenDatabases()
{
	new TTokenDatabase< TTokenDomain::MISC >();
	new TTokenDatabase< TTokenDomain::ODL_DATABASE_NAME >();


    //TTokenPath< TTokenDomain::MISC > pathTest0("");
    //TTokenPath< TTokenDomain::MISC > pathTest1("bleak");
    //TTokenPath< TTokenDomain::MISC > pathTest2("bleak/");
    //TTokenPath< TTokenDomain::MISC > pathTest3("bleak/groo");
    //TTokenPath< TTokenDomain::MISC > pathTest4("bleak/groo/gnee");
}
//-------------------------------------------------------------------------------
void DestroyTokenDatabases()
{
	TTokenDatabase< TTokenDomain::MISC >::DestroyInstance();
	TTokenDatabase< TTokenDomain::ODL_DATABASE_NAME >::DestroyInstance();
}
//-------------------------------------------------------------------------------
//*******************************************************************************
//-------------------------------------------------------------------------------
}
