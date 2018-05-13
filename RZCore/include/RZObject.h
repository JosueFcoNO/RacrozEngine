#pragma once
#define INVALID_RESOURCE NULL

namespace rczEngine
{
	typedef size_t ResourceHandle;
	typedef int32 ObjectID;

	class RZ_UTILITY_EXPORT Object
	{
	public:
		virtual ~Object() {};
		Object() { static int32 ObjectIndex = 0;  m_ObjID = ObjectIndex++; };

	protected:
		ObjectID m_ObjID = -1;
	};

}