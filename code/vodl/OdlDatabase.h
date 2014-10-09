#ifndef ODL_DATABASE_H
#define ODL_DATABASE_H

#include "OdlSingleton.h"

#include <map>
#include <sstream>
#include <string>
#include <hash_map>



namespace odl
{
//--------------------------------------------------------
//********************************************************
//--------------------------------------------------------
class TOdlDatabasePath;
class TMetaClassBase;
class TOdlObject;
//--------------------------------------------------------
typedef std::pair< TOdlObject*, TMetaClassBase const* > TObjectAndMetaClass;
//--------------------------------------------------------
class TOdlObjectDatabase
{
public:
    TOdlObjectDatabase();
    ~TOdlObjectDatabase();    
    void StoreObject(TOdlDatabasePath const& parDatabasePath, TOdlObject* parObject, TMetaClassBase const* parMetaClassBase);
    TOdlObject* GetObject_IFP(TOdlDatabasePath const& parDatabasePath) const;
	TObjectAndMetaClass TOdlObjectDatabase::GetObjectAndMetaClass_IFP(TOdlDatabasePath const& parDatabasePath) const;

private:
    typedef std::map< TOdlDatabasePath, TObjectAndMetaClass > TObjectsByPaths;
    TObjectsByPaths FObjectsByPaths;
};
//-------------------------------------------------------------------------------
class TOdlMetaClassByName : public std::map< std::string, TMetaClassBase const* >
{
public:
    ~TOdlMetaClassByName();
};
//-------------------------------------------------------------------------------
class TOdlDatabase : public TSingleton< TOdlDatabase >
{
    typedef TSingleton< TOdlDatabase > parent_type;
public:
    TOdlDatabase();
	~TOdlDatabase();

    void RegisterMetaClass(TMetaClassBase const* parMetaClass);
	TMetaClassBase const* FindRegisteredMetaClassByName_IFP(char const* parMetaClassName) const;

	void PrintContent(std::ostringstream& parOss) const;
    
    void StoreObject(TOdlDatabasePath const& parDatabasePath, TOdlObject* parObject, TMetaClassBase const* parMetaClassBase);
    TOdlObject* GetObject(TOdlDatabasePath const& parDatabasePath) const;
    
    template < typename TObjectType >
    TObjectType* GetObject_IFP(TOdlDatabasePath const& parDatabasePath) const
    {
        TOdlObject* object = FObjectsDatabase.GetObject_IFP(parDatabasePath);
        return static_cast< TObjectType* >(object);
    }

	TObjectAndMetaClass GetObjectAndMetaClass_IFP(TOdlDatabasePath const& parDatabasePath) const;

private:
    TOdlMetaClassByName FMetaClassByName;          // field-order matters: must be destroyed after FObjectsDatabase;
    TOdlObjectDatabase FObjectsDatabase;        // field-order matters: must be destroyed before FMetaClassByName;
   
};
//--------------------------------------------------------
//********************************************************
//--------------------------------------------------------
}



#endif // ODL_DATABASE_H