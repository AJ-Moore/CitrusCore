#pragma once

#include <CCCommon.h>

#ifdef RENDERER_PS5
CITRUS_CORE_API typedef uint8_t	uint8;
CITRUS_CORE_API typedef signed char sint8;
CITRUS_CORE_API typedef uint16_t uint16;
CITRUS_CORE_API typedef signed short sint16;
CITRUS_CORE_API typedef uint32_t uint32;
CITRUS_CORE_API typedef unsigned int uint;
CITRUS_CORE_API typedef unsigned long int uint64;
CITRUS_CORE_API typedef signed int sint32;
CITRUS_CORE_API typedef signed int int32;
#else
CITRUS_CORE_API typedef unsigned char	uint8;
CITRUS_CORE_API typedef signed char sint8;
CITRUS_CORE_API typedef unsigned short uint16;
CITRUS_CORE_API typedef signed short sint16;
CITRUS_CORE_API typedef unsigned int uint32;
CITRUS_CORE_API typedef unsigned int uint;
CITRUS_CORE_API typedef unsigned long int uint64;
CITRUS_CORE_API typedef signed int sint32;
CITRUS_CORE_API typedef signed int int32;
#endif

using Datestamp = uint64;