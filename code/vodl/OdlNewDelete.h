#ifndef ODL_NEW_DELETE_H
#define ODL_NEW_DELETE_H



namespace odl
{
	class TDomain
	{
	public:


	};

};

#define ODL_NEW(Domain) T ## Domain::new

#endif // ODL_NEW_DELETE_H