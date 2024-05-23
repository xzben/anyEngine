#pragma once

#include <cstdint>

#include "KeyCode.h"

enum class EventType {
    UNKNOW = 0,
    WINDOW_RESIZE,        // 窗口 resize
    WINDOW_BEFORE_CLOSE,  // 窗口 即将关闭
    CURSOR_ENTER,         // 光标进入窗口
    CURSOR_EXIT,          // 光标离开窗口
    TOUCH,                // 触摸事件
    KEYBOARD,             // 键盘事件
    MOUSE_BUTTON,         // 鼠标事件
};

enum class TouchStatus {
    BEGIN,
    MOVE,
    END,
};

enum class PressStatus {
    DOWN,
    UP,
    REPEAT,
};

struct Position {
    float x{0.f};
    float y{0.f};
};

struct Event {
    Event(EventType eventType) : type(eventType) {}
    virtual ~Event() = default;

    EventType type = EventType::UNKNOW;

    template <typename Type>
    Type& cast() {
        return dynamic_cast<Type&>(*this);
    }
    template <typename Type>
    const Type& cast() const {
        return dynamic_cast<const Type&>(*this);
    }
};

struct WindowResizeEvent : public Event {
    WindowResizeEvent() : Event(EventType::WINDOW_RESIZE) {}
    int32_t width;
    int32_t height;
};

struct TouchEvent : public Event {
    TouchEvent() : Event(EventType::TOUCH) {}
    TouchStatus status;
    Position curPos;
    Position prePos;
};

enum class MouseButton {
    LEFT = 0,
    RIGHT,
    MIDDLE,
    Count,
};

struct MouseEvent : public Event {
    MouseEvent() : Event(EventType::MOUSE_BUTTON) {}

    MouseButton button;
    PressStatus status;
};
struct KeyboardEvent : public Event {
    KeyboardEvent() : Event(EventType::KEYBOARD) {}
    PressStatus status;
    KeyCode code;
};

using EventId = uint32_t;