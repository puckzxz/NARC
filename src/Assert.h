#pragma once

#include "Log.h"

#ifdef _WIN32
    #define NARC_ASSERT(cond, ...) { if(!(cond)) { LOG_ERROR("{0}{1}{2} {3} {4}", __FILE__,":",__LINE__, "Assertion Failed -", __VA_ARGS__); __debugbreak(); } }
#elif __linux__
    #define NARC_ASSERT(cond, ...) { if(!(cond)) { LOG_ERROR("{0}{1}{2} {3} {4}", __FILE__,":",__LINE__, "Assertion Failed -", __VA_ARGS__); __builtin_trap(); } }
#elif __APPLE__
    #define NARC_ASSERT(cond, ...) { if(!(cond)) { LOG_ERROR("{0}{1}{2} {3} {4}", __FILE__,":",__LINE__, "Assertion Failed -", __VA_ARGS__); __builtin_trap(); } }
#endif

#define NARC_ASSERT_NOT_REACHED(...) NARC_ASSERT(false, __VA_ARGS__)
