#pragma once

#ifdef _WIN32 || _WIN64
#define CITRUS_CORE_API __declspec(dllexport)
#else
#define CITRUS_CORE_API __attribute__((visibility("default")))
#endif

#include <Types.h>