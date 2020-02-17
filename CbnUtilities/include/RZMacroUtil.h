#pragma once

#if RZ_COMPILER == RZ_COMPILER_MVSC
	#define MULTI_LINE_MACREO_BEGIN do{
	#define MULTI_LINE_MACRO_END \
	__pragma(warning(push))\
	__pragma(warning(disable:4127))\
	} while(0)\
	__pragma(warning(pop))
#else
	#define MULTI_LINE_MACRO_BEGIN do{
	#define MULTI_LINE_MACRO_END } while(0)
#endif

#ifndef RZ_UNREFERENCED_PARAMETER
#define RZ_UNREFERENCED_PARAMETER(P) (void)P
#endif

#pragma warning (disable: 4201) //Anonymous struct warning.
#pragma warning (disable: 26446) //gsl::at() warning.
#pragma warning (disable: 26482) //Array indexing with constexprs