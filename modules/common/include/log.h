#pragma once

#include <cassert>

#include "platformDefine.h"

#ifndef NDEBUG

#if CUR_PLATFORM == PLATFORM_WINDOWS
#define DEBUG_BREAK __debugbreak();
#else
#define DEBUG_BREAK
#endif

#define CCASSERT(condition, fmt, ...)                                                          \
    do {                                                                                       \
        if (!(condition)) {                                                                    \
            assert(false);                                                                     \
            fprintf(stderr, "[file] %s \r\n[function] %s \r\n[line] %d\r\n[ERROR]" fmt "\r\n", \
                    __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__);                          \
        }                                                                                      \
    } while (false)

// log
#define CCLOG(fmt, ...)                                                                            \
    do {                                                                                           \
        fprintf(stderr, "[file] %s \r\n[function] %s \r\n[line] %d\r\n[LOG]" fmt "\r\n", __FILE__, \
                __FUNCTION__, __LINE__, ##__VA_ARGS__);                                            \
    } while (false)

#define CCWARN(fmt, ...)                                                                  \
    do {                                                                                  \
        fprintf(stderr, "[file] %s \r\n[function] %s \r\n[line] %d\r\n[WARN]" fmt "\r\n", \
                __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__);                         \
    } while (false)

#define CCERROR(fmt, ...)                                                                  \
    do {                                                                                   \
        assert(false);                                                                     \
        fprintf(stderr, "[file] %s \r\n[function] %s \r\n[line] %d\r\n[ERROR]" fmt "\r\n", \
                __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__);                          \
    } while (false)
#else
#define RELEASE true
#define CCASSERT(condition, msg)

// log
#define CCLOG(fmt, ...)
#define CCWARN(fmt, ...)
#define CCERROR(fmt, ...)
#endif