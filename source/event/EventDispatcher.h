#pragma once

#include <cassert>
#include <unordered_map>
#include <vector>

#include "Event.h"
#include "sceneGraphDefine.h"

BEGIN_NS_SCENCE_GRAPH
class EventDispatcher {
public:
    using EventCallback = std::function<void(const Event& event)>;

    EventId addEventListener(EventType eventType, EventCallback func) {
        auto& handles = m_events[eventType];
        m_eventId++;
        handles.emplace_back(m_eventId, func);

        return m_eventId;
    }

    void dispatchEvent(const Event& event) { dispatchEvent(event.type, event); }

    void dispatchEvent(EventType eventType, const Event& event) {
        assert(event.type == eventType);

        auto it = m_events.find(eventType);
        if (it == m_events.end()) {
            return;
        }
        m_dispatching = true;
        auto& handles = m_events[eventType];

        for (auto& item : handles) {
            item.func(event);
        }
    }

    void removeEventHandle(EventType eventType, EventId eventId) {
        auto it = m_events.find(eventType);
        if (it == m_events.end()) {
            return;
        }
        auto& handles = m_events[eventType];

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

            return *this;
        }

        EventHandle& operator=(EventHandle&& rhs) {
            this->eventId = rhs.eventId;
            this->func    = rhs.func;
            this->valided = rhs.valided;
            return *this;
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

END_NS_SCENCE_GRAPH