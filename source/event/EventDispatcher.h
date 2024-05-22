#pragma once

#include <unordered_map>
#include <vector>

#include "Event.h"

class EventDispatcher {
public:
    using EventCallback = std::function<void(const Event& event)>;

    EventId addEventListener(EventType event, EventCallback func) {
        auto& handles = m_events[event];
        m_eventId++;
        handles.emplace_back(m_eventId, func);
    }

    void dispatchEvent(EventType event, const Event& event) {
        auto it = m_events.find(event);
        if (it == m_events.end()) {
            return;
        }
        m_dispatching = true;
        auto& handles = m_events[event];

        for (auto& item : handles) {
            item.func(event);
        }
    }

    void removeEventHandle(EventType event, EventId eventId) {
        auto it = m_events.find(event);
        if (it == m_events.end()) {
            return;
        }
        auto& handles = m_events[event];

        if (m_dispatching) {
            for (auto& item : handles) {
                if (item.eventId == eventId) {
                    item.valided = false;
                    break;
                }
            }
        } else {
            for (auto it = handles.begin(); it != handles.end(); it++) {
                auto& item = *it;

                if (item.eventId == eventId) {
                    handles.erase(it);
                    break;
                }
            }
        }
    }

protected:
    struct EventHandle {
        EventHandle(EventId eventId, EventCallback func) {
            this->eventId = eventId;
            this->func    = func;
        };

        EventHandle(const EventHandle& rhs) { this->operator=(rhs); }

        EventHandle(EventHandle&& rhs) { this->operator=(rhs); }

        EventHandle& operator=(const EventHandle& rhs) {
            this->eventId = rhs.eventId;
            this->func    = rhs.func;
            this->valided = rhs.valided;
        }

        EventHandle& operator=(EventHandle&& rhs) {
            this->eventId = rhs.eventId;
            this->func    = rhs.func;
            this->valided = rhs.valided;
        }

        EventId eventId;
        EventCallback func;
        bool valided{true};
    };

protected:
    std::unordered_map<EventType, std::vector<EventHandle>> m_events;
    EventId m_eventId{0};
    bool m_dispatching{false};
};