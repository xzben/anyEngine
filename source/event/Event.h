#pragma once

#include <cstdint>

#include "KeyCode.h"

enum class EventType {
    WINDOW_RESIZE,
    WINDOW_BEFORE_CLOSE,
    INPUT,
};

enum class InputEventType {
    TOUCH,
    KEYBOARD,
};

enum class TouchStatus {
    BEGIN,
    MOVE,
    END,
    CANCEL,
};

enum class KeyboardStatus {
    DOWN,
    UP,
    REPEAT,
};

struct Position {
    float x;
    float y;
};

struct Event {
    EventType type;
};

struct WindowResizeEvent : public Event {
    uint32_t width;
    uint32_t height;
};

struct WindowBeforeClose : public Event {};

struct TouchEvent : public Event {
    TouchStatus status;
    Position curPos;
    Position prePos;
};

struct KeyboardEvent : public Event {
    KeyboardStatus status;
    KeyCode code;
};

using EventId = uint32_t;