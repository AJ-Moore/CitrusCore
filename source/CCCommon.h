#pragma once

#if defined(_WIN32) || defined(_WIN64)
#define CITRUS_CORE_API __declspec(dllexport)
#else
#define CITRUS_CORE_API __attribute__((visibility("default")))
#endif

#include <Types.h>
#include <memory>
#include <string>

// Common containers
#include <unordered_map>

#include <Util/Logger.h>
