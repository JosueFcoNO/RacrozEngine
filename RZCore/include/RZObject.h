#pragma once
#define INVALID_RESOURCE -1
#define MISSING_RESOURCE -2

namespace rczEngine
{
	typedef int32 ResourceHandle;
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