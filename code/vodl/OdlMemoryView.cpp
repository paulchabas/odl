#include "StdAfx.h"
#include "OdlMemoryView.h"

#include <vector>

namespace odl
{
//-------------------------------------------------------------------------------
//*******************************************************************************
//-------------------------------------------------------------------------------
#if 0
template < typename TVectorContainerType >
TTypedMemoryView< typename TVectorContainerType::value_type > make_view(TVectorContainerType& parVector)
{
	typedef typename TVectorContainerType::value_type TValueType;
	return 	TTypedMemoryView< TValueType >(parVector.data(), parVector.size());
}
//-------------------------------------------------------------------------------
template < typename TVectorContainerType >
TTypedMemoryView< typename TVectorContainerType::value_type > make_view(TVectorContainerType const& parVector)
{
	typedef typename TVectorContainerType::value_type TValueType;
	return 	TTypedMemoryView< TValueType >(parVector.data(), parVector.size());
}
#endif



#if 0
void Test()
{
    char tab[4];

    TTypedMemoryView< char > charMemView(tab, 4);
    // TTypedMemoryView< char > testCopy = make_view(charMemView);

    //char const tab2[4] = { 4, 5, 6 };
    //TTypedMemoryView< char const > charMemViewConst(tab2, 4);
    //TTypedMemoryView< char const > testCopy2 = charMemViewConst;

    std::vector< char > bla;
    //TTypedMemoryView< char const > a0 = bla;
    // TTypedMemoryView< char > a1 = make_view(bla);
    // TTypedMemoryView< char const > a1b = make_view(bla);

    std::vector< char* > blaPointer;
    TTypedMemoryView< char* > b1 = make_view(blaPointer);
    TTypedMemoryView< char* const > b1b = make_view(blaPointer);
    // TTypedMemoryView< char const* const > b1c = make_view_const(blaPointer);

    std::vector< char const > blaConst;
	blaConst.push_back('b');
    TTypedMemoryView< char const > a2 = make_view(blaConst);
    TTypedMemoryView< char > a3 = make_view(blaConst);
	a3[0] = 'a';

	std::vector< char const > const constBlaConst = { 'b' };
    TTypedMemoryView< char const > a4 = make_view(constBlaConst);
	// TTypedMemoryView< char > a5 = make_view(constBlaConst);
	// a4[0] = 'a';
	// a5[0] = 'a';

    //std::vector< char > const constBla;
    //TTypedMemoryView< char const > a5 = constBla;

    //std::vector< char* const > const constBlaPointerConst;
    //TTypedMemoryView< char* const > a6 = constBlaPointerConst;
    //TTypedMemoryView< char* const > a6b(constBlaPointerConst);
    //TTypedMemoryView< char const* const > a7 = a6b;

    //testCopy2 = testCopy;
    // testCopy = testCopy2;
}
#endif

#if 0
struct TCallTest
{
	TCallTest()
	{
		Test();
	}
};

TCallTest callTest;
#endif
//-------------------------------------------------------------------------------
//*******************************************************************************
//-------------------------------------------------------------------------------
} // namespace odl
