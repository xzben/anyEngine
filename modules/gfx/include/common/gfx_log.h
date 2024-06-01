#pragma once

#include "platformDefine.h"

#ifndef NDEBUG

#if CUR_PLATFORM == PLATFORM_WINDOWS
#define DEBUG_BREAK __debugbreak();
#else
#define DEBUG_BREAK
#endif

#define CCASSERT(condition, fmt, ...)                                                         \
    do {                                                                                      \
        if (!(condition)) {                                                                   \
            assert(false);                                                                    \
            fprintf(stderr, "[file %s \r\nfunction %s line: %d]\r\n", __FILE__, __FUNCTION__, \
                    __LINE__);                                                                \
            fprintf(stderr, fmt, __VA_ARGS__);                                                \
            fprintf(stderr, "\r\n");                                                          \
        }                                                                                     \
    } while (false)

// log
#define CCLOG(fmt, ...)                                                                         \
    do {                                                                                        \
        fprintf(stdout, "[info][file %s \r\nfunction %s line: %d]\r\n", __FILE__, __FUNCTION__, \
                __LINE__);                                                                      \
        fprintf(stdout, fmt, __VA_ARGS__);                                                      \
        fprintf(stdout, "\r\n");                                                                \
    } while (false)

#define CCWARN(fmt, ...)                                                                        \
    do {                                                                                        \
        fprintf(stderr, "[warn][file %s \r\nfunction %s line: %d]\r\n", __FILE__, __FUNCTION__, \
                __LINE__);                                                                      \
        fprintf(stderr, fmt, __VA_ARGS__);                                                      \
        fprintf(stderr, "\r\n");                                                                \
    } while (false)

#define CCERROR(fmt, ...)                                                                         \
    do {                                                                                          \
        fprintf(stderr, "[error][ file %s \r\nfunction %s line: %d]\r\n", __FILE__, __FUNCTION__, \
                __LINE__);                                                                        \
        fprintf(stderr, fmt, __VA_ARGS__);                                                        \
        fprintf(stderr, "\r\n");                                                                  \
    } while (false)
#else
#define RELEASE true
#define CCASSERT(condition, msg)

// log
#define CCLOG(fmt, ...)
#define CCWARN(fmt, ...)
#define CCERROR(fmt, ...)
#endif