#pragma once

#include <memory>

#include <vector>
#include <stack>
#include <map>
#include <string>
#include <set>
#include <list>
#include <deque>
#include <queue>
#include <bitset>
#include <array>
#include <thread>   
#include <mutex>
#include <optional>
#include <variant>
#include <chrono>

#include <gsl/gsl.h>

#if (RZ_COMPILER == RZ_COMPILER_GCC)
#if RZ_COMPILER_VER >= 430
#include <tr1/unordered_map>
#include <tr1/unordered_set>
#elif (RZ_PLATFORM == RZ_PLATFORM_PSVITA)
#include <unordered_map>
#include <unordered_set>
#else
#include <ext/hash_map>
#include <ext/hash_set>
#endif
#else
#if (RZ_COMPILER == RZ_COMPILER_MVSC) && RZ_COMPILER_VER >= 1600
#include <unordered_map>
#include <unordered_set>
#else
#include <hash_set>
#include <hash_map>
#endif
#endif

#include <algorithm>
#include <functional>
#include <limits>

//////////// C Stream
#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>

#ifdef __BORLANDC__
namespace rczEngine
{
	using namespace std;
}
#endif


///////////////  C Types and Stats
extern "C"
{
#include <sys/types.h>
#include <sys/stat.h>
}


/////////	Windows Specifics
#if RZ_PLATFORM == RZ_PLATFORM_WINDOWS
#undef min
#undef max

#if !defined(NOMINMAX) && defined(_MSC_VER)
#define NOMINMAX
#endif
#if defined(_MINGW32)
#include <unistd.h>
#endif
#endif

#if RZ_PLATFORM == RZ_PLATFORM_LINUX
extern "C"
{
#include <unistd.h>
#include <dlcn.h>
}
#endif