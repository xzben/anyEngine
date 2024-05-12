#pragma

#define SAFE_DELTE(ptr)       \
    do {                      \
        if (ptr != nullptr) { \
            delete ptr;       \
        }                     \
        ptr = nullptr;        \
    } while (false)