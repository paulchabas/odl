#pragma once

#include <cassert>
#include <sstream>
#include <vector>
#include <set>
#include <map>
#include <sstream>
#include <string>
#include <hash_map>

#define ODL_ENABLE_VERBOSE_DEBUG 1

#if ODL_ENABLE_VERBOSE_DEBUG
#define OnlyInEnableVerboseDebug(X) (X)
#else
#define OnlyInEnableVerboseDebug(X)
#endif

#ifdef _DEBUG
   #ifndef DBG_NEW
      #define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
      #define new DBG_NEW
   #endif
#endif  // _DEBUG

