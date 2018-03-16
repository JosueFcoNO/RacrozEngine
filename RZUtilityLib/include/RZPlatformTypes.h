#pragma once
#include "RZPlatformDefines.h"

namespace rczEngine
{
	typedef unsigned char uint8;
	typedef unsigned short uint16;
	typedef unsigned int uint32;

#if RZ_COMPILER == RZ_COMPILER_MVSC
	typedef unsigned __int64 uint64;
#else
	typedef unsigned long uint64;
#endif


	typedef signed char int8;
	typedef signed short int16;
	typedef signed int int32;
#if RZ_COMPILER == RZ_COMPILER_MVSC
	typedef __int64 int64;
#else
	typedef long int64;
#endif




#if RZ_COMPILER == RZ_COMPILER_MVSC || RZ_PLATFORM == RZ_PLATFORM_PSVITA
		typedef wchar_t UNICHAR;
#else
		typedef unsigned short UNICHAR;
#endif
	typedef char ANSICHAR;

	typedef int32 TYPE_OF_NULL;

#if ((RZ_ARCH_TYPE == RZ_ARCHITECTURE_x86_64) && (RZ_COMPILER == RZ_COMPILER_MVSC))
	typedef unsigned __int64 SIZE_T;
#else
	typedef unsigned long SIZE_T;
#endif


}