#ifndef ODL_DATABASE_H
#define ODL_DATABASE_H

#include "vodlParserLexer/BasicTypes.h"
#include "vodlParserLexer/OdlSingleton.h"

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
	TObjectAndMetaClass GetObjectAndMetaClass_IFP(TOdlDatabasePath const& parDatabasePath) const;
    TMetaClassBase const* GetObjectMetaClassBase_IFP(TOdlObject const* parObject) const;

	void DestroyContent();

private:
    typedef std::map< TOdlDatabasePath, TObjectAndMetaClass > TObjectsByPaths;
    typedef std::map< TOdlObject const*, TMetaClassBase const* > TMetaClassByObjects;
    TObjectsByPaths FObjectsByPaths;
    TMetaClassByObjects FMetaClassByObjects;
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

    void ClearObjectDatabase();

    void RegisterMetaClass(TMetaClassBase const* parMetaClass);
	TMetaClassBase const* FindRegisteredMetaClassByName_IFP(char const* parMetaClassName) const;

	void PrintRegisteredTypes(std::ostringstream& parOss) const;
    
    void StoreObject(TOdlDatabasePath const& parDatabasePath, TOdlObject* parObject, TMetaClassBase const* parMetaClassBase);
    TOdlObject* GetObject(TOdlDatabasePath const& parDatabasePath) const;
    
    template < typename TObjectType >
    TObjectType* GetObject_IFP(TOdlDatabasePath const& parDatabasePath) const
    {
        TOdlObject* object = FObjectsDatabase.GetObject_IFP(parDatabasePath);
        return static_cast< TObjectType* >(object);
    }

	TObjectAndMetaClass GetObjectAndMetaClass_IFP(TOdlDatabasePath const& parDatabasePath) const;

    TMetaClassBase const* GetObjectMetaClassBase_IFP(TOdlObject const* parObject) const;

private:
    TOdlMetaClassByName FMetaClassByName;          // field-order matters: must be destroyed after FObjectsDatabase;
    TOdlObjectDatabase FObjectsDatabase;        // field-order matters: must be destroyed before FMetaClassByName;
   
};
//--------------------------------------------------------
//********************************************************
//--------------------------------------------------------
}



#endif // ODL_DATABASE_H