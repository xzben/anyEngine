#pragma once

#include <functional>
#include <unordered_map>

#include "common.h"
#include "common/Object.h"
#include "event/EventDispatcher.h"
#include "math/math.h"

class Window : public Object, public EventDispatcher {
protected:
    Window(const std::string &title, uint32_t w, uint32_t h) : m_title(title), m_size(w, h){};

public:
    using CLOSE_BEFORE_HANDLE = std::function<void(Window *win)>;
    using RESIZE_HANDLE       = std::function<void(uint32_t width, uint32_t height)>;

    static Window *create(const std::string &title, uint32_t w, uint32_t h);

    virtual bool init()                = 0;
    virtual bool shoudleClose()        = 0;
    virtual void updateEvents()        = 0;
    virtual void *getNativeWinHandle() = 0;

    virtual const ISize &getSize() const { return m_size; }

protected:
    virtual ~Window() = default;

protected:
    std::string m_title;
    ISize m_size;

    std::unordered_map<uint32_t, CLOSE_BEFORE_HANDLE> m_beforeCloseCallbacks;
    std::unordered_map<uint32_t, RESIZE_HANDLE> m_resizeCallbacks;
    uint32_t m_eventId{0};
};