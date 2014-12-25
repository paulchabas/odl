#pragma once


#define ODL_ENABLE_VERBOSE_DEBUG 1
#define ODL_ENABLE_VTABLE_INTO_ODL_OBJECT_FOR_DEBUG 1

#if ODL_ENABLE_VERBOSE_DEBUG
#define OnlyInEnableVerboseDebug(X) (X)
#else
#define OnlyInEnableVerboseDebug(X)
#endif




