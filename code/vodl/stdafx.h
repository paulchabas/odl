#pragma once

#include "vodlParserLexer/BasicTypes.h"

#define ODL_ENABLE_VERBOSE_DEBUG 1

#if ODL_ENABLE_VERBOSE_DEBUG
#define OnlyInEnableVerboseDebug(X) (X)
#else
#define OnlyInEnableVerboseDebug(X)
#endif


