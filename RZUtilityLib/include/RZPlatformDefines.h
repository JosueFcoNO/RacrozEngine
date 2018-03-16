#pragma once

//////////////////////////////////////////////////////
//////////////////      S.O.     /////////////////////
//////////////////////////////////////////////////////

#define RZ_PLATFORM_WINDOWS 1
#define RZ_PLATFORM_LINUX 2
#define RZ_PLATFORM_OSX 3
#define RZ_PLATFORM_PSVITA 4
#define RZ_PLATFORM_ANDROID 5

//////////////////////////////////////////////////////
/////////////////// Platform /////////////////////////
//////////////////////////////////////////////////////

#ifdef _WINDOWS
#define RZ_PLATFORM RZ_PLATFORM_WINDOWS ///Compiling for windows
#endif

///////////////////////////////////////////////////////
////////////////// Architecture  //////////////////////
///////////////////////////////////////////////////////

#define RZ_ARCHITECTURE_X86 1
#define RZ_ARCHITECTURE_ARM 2
#define RZ_ARCHITECTURE_x86_32 3
#define RZ_ARCHITECTURE_x86_64 4

///////////////////////////////////////////////////////
//////////////////// Bit Size /////////////////////////
///////////////////////////////////////////////////////

#define RZ_BIT_32 1
#define RZ_BIT_64 2

///////////////////////////////////////////////////////
//////////////////// Endianess ////////////////////////
///////////////////////////////////////////////////////

#define RZ_ENDIAN_LITTLE 1
#define RZ_ENDIAN_BIG 2

#define RZ_ENDIAN RZ_ENDIAN_LITTLE

///////////////////////////////////////////////////////
//////////////////////  Mode  /////////////////////////
///////////////////////////////////////////////////////

#define RZ_MODE_DEBUG 1
#define RZ_MODE_RELEASE 2
#define RZ_MODE_PROFILE 3

///////////////////////////////////////////////////////
/////////////////// Vector Math ///////////////////////
///////////////////////////////////////////////////////

#define RZ_INTRISECS 1
#define RZ_NO_INTRISECS 2

///////////////////////////////////////////////////////
///////////////////// Compiler ////////////////////////
///////////////////////////////////////////////////////
#define RZ_COMPILER_MVSC 1
#define RZ_COMPILER_GCC 2
#define RZ_COMPILER_CLANG 3
#define RZ_COMPILER_INTEL 4

//Windows usa little, Mac usa little, linux usa los dos. Vita puede usar los dos. Macs viejas big endian, power pcs big endien, x86 little endian, arm depende


#if defined(_MSC_VER)
	#define RZ_COMPILER RZ_COMPILER_MVSC
	#define RZ_COMPILER_VER _MSC_VER
	#define RZ_THREADLOCAL _declspec(thread)
	#define RZ__FILE__ __FILE__
	#define RZ__LINE__ __LINE__
	#define RZ__FUNCTION__ __FUNCTION__
#elif defined(_GNUC__)
	#define RZ_COMPILER GE_COMPILER_GCC
	#define RZ_COMPILER_VER (((__GNUC__)*100) + (__GNUC_MINOR__*10)+__GNUC_PATCHLEVEL__)
	#define RZ_THREADLOCAL __thread
	#define RZ__FILE__ __FILE__
	#define RZ__LINE__ __LINE__
	#define RZ__FUNCTION__ __FUNCTION__
#elif defined(__INTEL_COMPILER)
	#define RZ_COMPILER RZ_COMPILER_INTEL
	#define RZ_COMP_VER __INTEL_COMPILER
	#define RZ__FILE__ __FILE__
	#define RZ__LINE__ __LINE__
	#define RZ__FUNCTION__ "Unknown"
#elif defined(__clang__)
	#define RZ_COMPILER RZ_COMPILER_CLANG
	#define RZ_COMPILER_VER __clang_major__
	#define RZ_THREADLOCAL __thread
#else
	#pragma error "No known compiler."
#endif



#if RZ_COMPILER == RZ_COMPILER_MVSC
	#if RZ_COMPILER_VER >= 1200
		#define FORCEINLINE __forceinline
	#ifndef RESTRICT
		#define RESTRICT __restrict
	#endif
#endif
#elif define (__MINGW32__)
	#if !defined(FORCEINLINE)
		#define FORCEINLINE __inline
	#ifndef RESTRICT
			#define RESTRICT
		#endif
	#endif
#else
	#define FORCEINLINE _inline
	#ifndef RESTRICT
		#define RESTRICT __restrict
	#endif
#endif


#if defined(__WIN32__) || defined (_WIN32)
	#define RZ_PLATFORM RZ_PLATFORM_WINDOWS
#elif defined(__APPLE_CC__)
	#define RZ_PLATFORM RZ_PLATFORM_OSX
#elif defined(__psp2__)
	#define RZ_PLATFORM RZ_PLATFORM_PSVITA
#else
	#define RZ_PLATFORM RZ_PLATFORM_LINUX
#endif

#if defined(__x86_64__) || defined (_x64)
	#define RZ_ARCH_TYPE RZ_ARCHITECTURE_x86_64
#else
	#define RZ_ARCH_TYPE RZ_ARCHITECTURE_x86_32
#endif




#if RZ_COMPILER == RZ_COMPILER_MVSC
#define MS_ALIGN(n) __declspec(align(n))
	#ifndef GCC_PACK
		#define GCC_PACK(n)
#endif
	#ifndef GCC_ALIGN
		#define GCC_ALIGN(n)
	#endif
#elif (RZ_COMPILER == RZ_COMPILER_GCC && RZ_PLATFORM == RZ_PLATFORM_PSVITA)
	#define MS_ALIGN(n)
	#define GCC_PACK(n)
	#define GCC_ALIGN(n) __attribute__( (__aligned(n)))
#else
	#define MS_ALIGN(n)
	#define GCC_PACK(n) __attribute__((packed, aligned(n)) )
	#define GCC_ALIGN(n) __attribute__( (aligned(n)) )
#endif


#if RZ_COMPILER == RZ_COMPILER_MVSC
	#define _NOEXPECT noexcept
#elif RZ_COMPILER == RZ_COMPILER_INTEL
	#define _NOEXCEPT noexcept
#elif RZ_COMPILER == RZ_COMPLER_GCC
	#define _NOEXCEPT noexcept
#else
	#define _NOEXCEPT
#endif


#if RZ_PLATFORM == RZ_PLATFORM_WINDOWS
	#if defined(RZ_STATIC_LIB)
		#define RZ_UTILITY_EXPORT
	#else
	#if defined(RZ_UTILITY_EXPORTS)
		#define RZ_UTILITY_EXPORT __declspec(dllexport)
	#else
	#if defined(__MINGW32)
		#define RZ_UTILITY_EXPORT
	#else
		#define RZ_UTILITY_EXPORT __declspec(dllimport)
		#endif
	#endif
#endif

#if defined(_DEBUG) || defined(DEBUG)
#define RZ_DEBUG_MODE 1
#else
#define RZ_DEBUG_MODE 0
#endif

#if RZ_COMPILER == RZ_COMPILER_INTEL
	#define RZ_THREADLOCAL __declspec(thread)
#endif
#endif

#if RZ_PLATFORM == RZ_PLATFORM_LINUX || RZ_PLATFORM == RZ_PLATFORM_OSX
#if defined(RZ_GCC_VISIBILITY)
#define RZ_UTILITY_EXPORT __attribute__ ((visibility("default")))
#else
#define RZ_UTILITY_EXPORT
#endif

#define stricmp strcasecmp

#if defined(_DEBUG) || defined(DEBUG)
#define RZ_DEBUG_MODE 1
#else
#define RZ_DEBUG_MODE 0
#endif

#if RZ_COMPILER == RZ_COMPILER_INTEL
#define RZ_THREADLOCAL __thread
#endif
#endif

#if RZ_PLATFORM == RZ_PLATFORM_PSVITA
#if defined(RZ_GCC_VISIBILITY)
#define RZ_UTILITY_EXPORT __attribute__ ((visibility("default")))
#else
#define RZ_UTILITY_EXPORT
#endif

#if defined(_DEBUG) || defined(DEBUG)
#define RZ_DEBUG_MODE 1
#else
#define RZ_DEBUG_MODE 0
#endif

#if RZ_COMPILER == RZ_COMPILER_INTEL
#define RZ_THREADLOCAL __thread
#endif
#endif

#if RZ_DEBUG_MODE
#define RZ_DEBUG_ONLY(X) X
#define RZ_ASSERT(X) assert(X)
#else
#define RZ_DEBUG_ONLT(X)
#define RZ_ASSERT(X)
#endif