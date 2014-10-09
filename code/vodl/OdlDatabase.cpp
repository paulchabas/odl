#include "StdAfx.h"
#include "OdlDatabase.h"


#include "odl.h"

namespace odl
{
//--------------------------------------------------------
//********************************************************
//--------------------------------------------------------
TOdlObjectDatabase::TOdlObjectDatabase()
{
}
//--------------------------------------------------------
TOdlObjectDatabase::~TOdlObjectDatabase()
{
	for (TObjectsByPaths::iterator it = FObjectsByPaths.begin();
		 it != FObjectsByPaths.end();
		 ++it)
	{
		TObjectAndMetaClass const& objectAndMetaClass = it->second;
        TOdlObject* object = objectAndMetaClass.first;
        TMetaClassBase const* objectMetaClass = objectAndMetaClass.second;
        
        objectMetaClass->DeleteObject(object);
	}
	FObjectsByPaths.clear();
}
//--------------------------------------------------------
void TOdlObjectDatabase::StoreObject(TOdlDatabasePath const& parDatabasePath, TOdlObject* parObject, TMetaClassBase const* parMetaClassBase)
{
    assert(parMetaClassBase != nullptr);
    
	std::string forDebug = parDatabasePath.ToString();

    TObjectsByPaths::iterator it = FObjectsByPaths.lower_bound(parDatabasePath);
    if (it != FObjectsByPaths.end())
    {
        if (it->first == parDatabasePath)
        {
			// object doublon in database !! ??
            assert(false);
        }
    }
    // = FObjectsByPaths.find(parDatabasePath);
    //if (it == FObjectsByPaths
    // == FObjectsByPaths
    FObjectsByPaths[parDatabasePath] = TObjectAndMetaClass(parObject, parMetaClassBase);
}
//--------------------------------------------------------
TOdlObject* TOdlObjectDatabase::GetObject_IFP(TOdlDatabasePath const& parDatabasePath) const
{
    TObjectsByPaths::const_iterator it = FObjectsByPaths.find(parDatabasePath);
    if (it != FObjectsByPaths.end())
    {
        TObjectAndMetaClass const& objectAndMetaClass = it->second;
        return objectAndMetaClass.first;
    }

    return nullptr;
}
//-------------------------------------------------------------------------------
TObjectAndMetaClass TOdlObjectDatabase::GetObjectAndMetaClass_IFP(TOdlDatabasePath const& parDatabasePath) const
{
    TObjectsByPaths::const_iterator it = FObjectsByPaths.find(parDatabasePath);
    if (it != FObjectsByPaths.end())
    {
        TObjectAndMetaClass const& objectAndMetaClass = it->second;
        return objectAndMetaClass;
    }

    return TObjectAndMetaClass(nullptr, nullptr);
}
//-------------------------------------------------------------------------------
//*******************************************************************************
//-------------------------------------------------------------------------------
TOdlMetaClassByName::~TOdlMetaClassByName()
{
    for (iterator it = begin();
		 it != end();
		 ++it)
	{
		TMetaClassBase const* metaClass = it->second;
		delete metaClass;
	}
	clear();
}
//--------------------------------------------------------
//********************************************************
//--------------------------------------------------------
TOdlDatabase::TOdlDatabase() :
    parent_type(this),
    FMetaClassByName(),          // field-order matters: must be destroyed after FObjectsDatabase;
    FObjectsDatabase()        // field-order matters: must be destroyed before FMetaClassByName;
{

}
//--------------------------------------------------------
TOdlDatabase::~TOdlDatabase()
{

}
//--------------------------------------------------------
TMetaClassBase const* TOdlDatabase::FindRegisteredMetaClassByName_IFP(char const* parMetaClassName) const
{
	TOdlMetaClassByName::const_iterator it = FMetaClassByName.find(parMetaClassName);
	if (it != FMetaClassByName.end())
	{
		return it->second;
	}

	return nullptr;
}
//--------------------------------------------------------
void TOdlDatabase::RegisterMetaClass(TMetaClassBase const* parMetaClass)
{
	assert(parMetaClass != nullptr);
	assert(FindRegisteredMetaClassByName_IFP(parMetaClass->Name()) == nullptr);

    FMetaClassByName[parMetaClass->Name()] = parMetaClass;
}
//--------------------------------------------------------
void TOdlDatabase::PrintRegisteredTypes(std::ostringstream& parOss) const
{
	parOss << "|----------------------------------------------------" << std::endl;
	parOss << "| **        ODL DATABASE REGISTERED TYPES          **" << std::endl;
	parOss << "|----------------------------------------------------" << std::endl;
	for (TOdlMetaClassByName::const_iterator it = FMetaClassByName.begin(); it != FMetaClassByName.end(); ++it)
	{
		TMetaClassBase const* metaClass = it->second;
		parOss << "|";
		metaClass->PrintContent(parOss);
		if (std::next(it) != FMetaClassByName.end())
			parOss << "|----------------------------------------------------" << std::endl;
	}
	parOss << "|----------------------------------------------------" << std::endl;
	parOss << "|****************************************************" << std::endl;
	parOss << "|----------------------------------------------------" << std::endl;
}
//--------------------------------------------------------
void TOdlDatabase::StoreObject(TOdlDatabasePath const& parDatabasePath, TOdlObject* parObject, TMetaClassBase const* parMetaClassBase)
{
    FObjectsDatabase.StoreObject(parDatabasePath, parObject, parMetaClassBase);
}
//-------------------------------------------------------------------------------
TOdlObject* TOdlDatabase::GetObject(TOdlDatabasePath const& parDatabasePath) const
{
	#if ODL_ENABLE_VERBOSE_DEBUG
	std::string databasePath = parDatabasePath.ToString();
	#endif
    TOdlObject* object = FObjectsDatabase.GetObject_IFP(parDatabasePath);
    assert(object != nullptr);
    return object;
}
//-------------------------------------------------------------------------------
TObjectAndMetaClass TOdlDatabase::GetObjectAndMetaClass_IFP(TOdlDatabasePath const& parDatabasePath) const
{
	return FObjectsDatabase.GetObjectAndMetaClass_IFP(parDatabasePath);
}
//--------------------------------------------------------
//********************************************************
//--------------------------------------------------------
}

