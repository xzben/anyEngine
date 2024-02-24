#pragma once

#define PLATFORM_UNKNOWN 0
#define PLATFORM_WINDOWS 1
#define PLATFORM_ANDROID 2
#define PLATFORM_IOS 3
#define PLATFORM_MAC 4

#define CUR_PLATFORM PLATFORM_WINDOWS

#define SAFE_DELTE(ptr)     \
    do                      \
    {                       \
        if (ptr != nullptr) \
        {                   \
            delete ptr;     \
        }                   \
        ptr = nullptr;      \
    } while (false)