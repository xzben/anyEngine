#pragma

#define SAFE_DELTE(ptr)       \
    do {                      \
        if (ptr != nullptr) { \
            delete ptr;       \
        }                     \
        ptr = nullptr;        \
    } while (false)

#define MACRO_STR(s) #s
#define MACRO_XSTR(s) MACRO_STR(s)
