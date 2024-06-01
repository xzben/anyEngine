#pragma once

#define PLATFORM_UNKNOWN 0
#define PLATFORM_WINDOWS 1
#define PLATFORM_ANDROID 2
#define PLATFORM_IOS 3
#define PLATFORM_MAC 4

#if defined(__WIN32) || defined(WINDOWS)
#define CUR_PLATFORM PLATFORM_WINDOWS
#elif defined(__APPLE__)
#include <TargetConditionals.h>
#if TARGET_OS_MAC
#define CUR_PLATFORM PLATFORM_MAC
#elif TARGET_OS_IPHONE
#define CUR_PLATFORM PLATFORM_IOS
#else
#define CUR_PLATFORM PLATFORM_UNKNOWN
static_assert(false);  // unoknow platform
#endif
#elif defined(__ANDROID__)
#define CUR_PLATFORM PLATFORM_ANDROID
#endif