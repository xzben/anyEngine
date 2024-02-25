#pragma once

#include <type_traits>

#include "gfx_defines.h"

BEGIN_GFX_NAMESPACE

template <typename T>
void compute_hash(size_t &seed, const T &v);

template <typename T>
void hash_combine(size_t &seed, const T &v) {
    seed ^= std::hash<T>{}(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

template <typename T, typename... Ts>
void hash_combine(size_t &seed, const T &v, Ts... rest) {
    hash_combine(seed, v);
    if constexpr (sizeof...(Ts) > 1) {
        hash_combine(seed, rest...);
    }
}

template <class T>
inline std::underlying_type_t<T> toNumber(const T &v) {
    return static_cast<std::underlying_type_t<T>>(v);
}

template <class T>
inline bool hasFlag(const T flags, const T flagToTest) {
    using ValueType = typename std::underlying_type<T>::type;

    CCASSERT((static_cast<ValueType>(flagToTest)
              & (static_cast<ValueType>(flagToTest) - 1))
                 == 0,
             "flag value error!");
    return (static_cast<ValueType>(flags) & static_cast<ValueType>(flagToTest))
           != 0;
}

// https://blog.csdn.net/qq_41100010/article/details/123434384
//  helper type for the visitor #4
template <class... Ts>
struct overloaded : Ts... {
    using Ts::operator()...;
};
// explicit deduction guide (not needed as of C++20)
template <class... Ts>
overloaded(Ts...) -> overloaded<Ts...>;

END_GFX_NAMESPACE

#define CC_ENUM_BITWISE_OPERATORS(T)                                           \
    constexpr bool operator!(const T v) {                                      \
        return !static_cast<std::underlying_type<T>::type>(v);                 \
    }                                                                          \
    constexpr T operator~(const T v) {                                         \
        return static_cast<T>(~static_cast<std::underlying_type<T>::type>(v)); \
    }                                                                          \
    constexpr bool operator||(const T lhs, const T rhs) {                      \
        return (static_cast<std::underlying_type<T>::type>(lhs)                \
                || static_cast<std::underlying_type<T>::type>(rhs));           \
    }                                                                          \
    constexpr bool operator&&(const T lhs, const T rhs) {                      \
        return (static_cast<std::underlying_type<T>::type>(lhs)                \
                && static_cast<std::underlying_type<T>::type>(rhs));           \
    }                                                                          \
    constexpr T operator|(const T lhs, const T rhs) {                          \
        return static_cast<T>(                                                 \
            static_cast<std::underlying_type<T>::type>(lhs)                    \
            | static_cast<std::underlying_type<T>::type>(rhs));                \
    }                                                                          \
    constexpr T operator&(const T lhs, const T rhs) {                          \
        return static_cast<T>(                                                 \
            static_cast<std::underlying_type<T>::type>(lhs)                    \
            & static_cast<std::underlying_type<T>::type>(rhs));                \
    }                                                                          \
    constexpr T operator^(const T lhs, const T rhs) {                          \
        return static_cast<T>(                                                 \
            static_cast<std::underlying_type<T>::type>(lhs)                    \
            ^ static_cast<std::underlying_type<T>::type>(rhs));                \
    }                                                                          \
    constexpr T operator+(const T lhs, const T rhs) {                          \
        return static_cast<T>(                                                 \
            static_cast<std::underlying_type<T>::type>(lhs)                    \
            + static_cast<std::underlying_type<T>::type>(rhs));                \
    }                                                                          \
    constexpr T operator+(const T lhs, bool rhs) {                             \
        return static_cast<T>(static_cast<std::underlying_type<T>::type>(lhs)  \
                              + rhs);                                          \
    }                                                                          \
    constexpr void operator|=(T &lhs, const T rhs) {                           \
        lhs =                                                                  \
            static_cast<T>(static_cast<std::underlying_type<T>::type>(lhs)     \
                           | static_cast<std::underlying_type<T>::type>(rhs)); \
    }                                                                          \
    constexpr void operator&=(T &lhs, const T rhs) {                           \
        lhs =                                                                  \
            static_cast<T>(static_cast<std::underlying_type<T>::type>(lhs)     \
                           & static_cast<std::underlying_type<T>::type>(rhs)); \
    }                                                                          \
    constexpr void operator^=(T &lhs, const T rhs) {                           \
        lhs =                                                                  \
            static_cast<T>(static_cast<std::underlying_type<T>::type>(lhs)     \
                           ^ static_cast<std::underlying_type<T>::type>(rhs)); \
    }

#ifndef NDEBUG
#define CCASSERT(condition, fmt, ...)                                   \
    do {                                                                \
        if (!(condition)) {                                             \
            assert(false);                                              \
            fprintf(stderr, "[file %s function %s line: %d]", __FILE__, \
                    __FUNCTION__, __LINE__);                            \
            fprintf(stderr, fmt, __VA_ARGS__);                          \
            fprintf(stderr, "\r\n");                                    \
            __debugbreak();                                             \
        }                                                               \
    } while (false)

// log
#define CCLOG(fmt, ...)                                                   \
    do {                                                                  \
        fprintf(stdout, "[info][file %s function %s line: %d]", __FILE__, \
                __FUNCTION__, __LINE__);                                  \
        fprintf(stdout, fmt, __VA_ARGS__);                                \
        fprintf(stdout, "\r\n");                                          \
    } while (false)

#define CCWARN(fmt, ...)                                                  \
    do {                                                                  \
        fprintf(stderr, "[warn][file %s function %s line: %d]", __FILE__, \
                __FUNCTION__, __LINE__);                                  \
        fprintf(stderr, fmt, __VA_ARGS__);                                \
        fprintf(stderr, "\r\n");                                          \
    } while (false)

#define CCERROR(fmt, ...)                                                   \
    do {                                                                    \
        fprintf(stderr, "[error][ file %s function %s line: %d]", __FILE__, \
                __FUNCTION__, __LINE__);                                    \
        fprintf(stderr, fmt, __VA_ARGS__);                                  \
        fprintf(stderr, "\r\n");                                            \
        __debugbreak();                                                     \
    } while (false)
#else
#define RELEASE true
#define CCASSERT(condition, msg)

// log
#define CCLOG(fmt, ...)
#define CCWARN(fmt, ...)
#define CCERROR(fmt, ...)
#endif