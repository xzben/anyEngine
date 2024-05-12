#pragma once

#include <functional>

#include "common/Object.h"
#include "common.h"
#include "math/math.h"

class Window : public Object {
protected:
    Window(const std::string &title, uint32_t w, uint32_t h)
        : m_title(title), m_size(w, h){};

public:
    using CLOSE_BEFORE_HANDLE = std::function<void(Window *win)>;

    static Window *create(const std::string &title, uint32_t w, uint32_t h);

    virtual bool init()                = 0;
    virtual bool shoudleClose()        = 0;
    virtual void updateEvents()        = 0;
    virtual void *getNativeWinHandle() = 0;

    void setBeforeCloseCallback(CLOSE_BEFORE_HANDLE func) {
        m_beforeCloseCallbacks.push_back(func);
    }

    virtual const ISize &getSize() const { return m_size; }

protected:
    virtual ~Window() = default;
    void handleBeforeClose() {
        for (auto &func : m_beforeCloseCallbacks) {
            func(this);
        }
    }

protected:
    std::string m_title;
    ISize m_size;
    std::vector<CLOSE_BEFORE_HANDLE> m_beforeCloseCallbacks;
};