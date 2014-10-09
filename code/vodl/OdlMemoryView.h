#ifndef INCLUDED_ODL_MEMORY_VIEW_H
# define INCLUDED_ODL_MEMORY_VIEW_H


#include <cassert>

// #include <vector>

namespace odl
{
template <typename T>
struct TConstifyPointer
{
public: 
    typedef T value_type;
    typedef const value_type const_value_type;
    typedef const value_type* const_pointer_type;
};

template <typename T>
struct TConstifyPointer<T*>
{
public: 
    typedef T* value_type;
    typedef const T* const_value_type;
    typedef const T** const_pointer_type;
};
//-------------------------------------------------------------------------------
//*******************************************************************************
//-------------------------------------------------------------------------------
template < typename T >
class TTypedMemoryView;

template < typename T >
class TTypedMemoryView< T const >;

template < typename T >
class TTypedMemoryView
{
    friend class TTypedMemoryView< T const >;
public:
    typedef T* iterator;
    typedef T const* const_iterator;

    TTypedMemoryView(T* parPointer, size_t parSize) :
        FPointer(parPointer),
        FSize(parSize)
    {
    }

    TTypedMemoryView(TTypedMemoryView< T > const& parOther) :
        FPointer(parOther.FPointer),
        FSize(parOther.FSize)
    {
    }

    T& operator [] (size_t parIndex)
    {
        assert(parIndex < FSize);
        return FPointer[parIndex];
    }

    T const& operator [] (size_t parIndex) const
    {
        assert(parIndex < FSize);
        return FPointer[parIndex];
    }

    TTypedMemoryView< T >& operator = (TTypedMemoryView< T >& parOther)
    {
        FPointer = parOther.FPointer;
        FSize = parOther.FSize;
    }

    T* pointer() { return FPointer; }
    T const* pointer() const { return FPointer; }
    size_t size() const { return FSize; }
    iterator begin() { return FPointer; }
    iterator end() { return FPointer + FSize; }
    const_iterator begin() const { return FPointer; }
    const_iterator end() const { return FPointer + FSize; }

private:
    T* FPointer;
    size_t FSize;
};
//-------------------------------------------------------------------------------
template < typename T >
class TTypedMemoryView< T const >
{
    friend class TTypedMemoryView< T >;
public:
    typedef T const* const_iterator;

    TTypedMemoryView(T const* parPointer, size_t parSize) :
        FPointer(parPointer),
        FSize(parSize)
    {
    }

    TTypedMemoryView(TTypedMemoryView< T const > const& parOther) :
        FPointer(parOther.FPointer),
        FSize(parOther.FSize)
    {
    }

    TTypedMemoryView(TTypedMemoryView< T > const& parOther) :
        FPointer(parOther.FPointer),
        FSize(parOther.FSize)
    {
    }

    T const& operator [] (size_t parIndex) const
    {
        assert(parIndex < FSize);
        return FPointer[parIndex];
    }

    TTypedMemoryView< T const >& operator = (TTypedMemoryView< T >& parOther)
    {
        FPointer = parOther.pointer();
        FSize = parOther.size();
        return *this;
    }

    TTypedMemoryView< T const >& operator = (TTypedMemoryView< T const >& parOther)
    {
        FPointer = parOther.pointer();
        FSize = parOther.size();
        return *this;
    }

    T const* pointer() const { return FPointer; }
    size_t size() const { return FSize; }
    const_iterator begin() const { return FPointer; }
    const_iterator end() const { return FPointer + FSize; }

private:
    T const* FPointer;
    size_t FSize;
};
//-------------------------------------------------------------------------------
//*******************************************************************************
//-------------------------------------------------------------------------------

//-------------------------------------------------------------------------------
} // namespace odl


#endif

