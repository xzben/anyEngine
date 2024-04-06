#pragma once

#include "common/common.h"
#include "math/math.h"

class Window {
protected:
    Window(const std::string &title, uint32_t w, uint32_t h)
        : m_title(title), m_size(w, h){};

public:
    static Window *create(const std::string &title, uint32_t w, uint32_t h);
    virtual ~Window()           = default;
    virtual bool init()         = 0;
    virtual bool shoudleClose() = 0;
    virtual void updateEvents() = 0;
    virtual const ISize &getSize() const { return m_size; }

protected:
    std::string m_title;
    ISize m_size;
};