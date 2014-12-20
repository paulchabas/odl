#pragma once

#include "vodlParserLexer/BasicTypes.h"

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

