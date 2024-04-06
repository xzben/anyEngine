#pragma once

class NoCopy {
public:
    NoCopy() = default;

    NoCopy(const NoCopy &rhs)          = delete;
    NoCopy(const NoCopy &&rhs)         = delete;
    void operator=(const NoCopy &rhs)  = delete;
    void operator=(const NoCopy &&rhs) = delete;
};