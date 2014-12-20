#ifndef ODL_SINGLETON_H
# define ODL_SINGLETON_H


#include <cassert>

namespace odl
{

template < typename T >
class TSingleton
{
public:
    TSingleton() { FInstance = 0; }

protected:
    explicit TSingleton(T* parInstance)
    {
        assert(FInstance == 0);
        assert(parInstance != 0);
        FInstance = parInstance;
    }

    ~TSingleton()
    {

    }

public:
    static T& Instance() { return *FInstance; }
    static void DestroyInstance() 
    {
        delete FInstance;
        FInstance = 0;
    }

private:
    static T* FInstance;
};

template < typename T >
T* TSingleton< T >::FInstance = 0;

}



#endif