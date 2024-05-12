#pragma once

#include <queue>
#include <thread>
#include <vector>

#include "gl3/core/gl3_core_common.h"

BEGIN_GFX_NAMESPACE
BEGIN_GL3_CORE_NAMESPACE

template <typename ItemType>
class ThreadPriorityQueue {
public:
    ThreadPriorityQueue() {}
    ~ThreadPriorityQueue() {}

    void addItem(const ItemType& item, float priority = 1.0) {
        m_itemLock.lock();
        m_items.emplace(priority, item);
        m_count++;
        m_itemLock.unlock();
        m_condition.notify_one();
    }

    void addItem(const ItemType&& item, float priority = 1.0) {
        m_itemLock.lock();
        m_items.emplace(priority, std::forward(item));
        m_count++;
        m_itemLock.unlock();
    }

    bool wait(ItemType& out) {
        if (m_count.load() <= 0) {
            std::unique_lock<std::mutex> locker(m_lock);
            m_condition.wait(
                locker, [&]() { return m_count.load() > 0 || m_exit.load(); });
        }

        if (m_exit.load()) {
            return false;
        }

        m_itemLock.lock();
        auto queuItem = m_items.top();
        m_items.pop();
        m_count--;
        m_itemLock.unlock();

        out = std::move(queuItem.data);

        return true;
    }

    void exit() {
        m_exit.store(true);
        m_condition.notify_all();
    }

protected:
    struct QueueItem {
        QueueItem(float p, const ItemType& d)
            : priority(p), data(std::move(d)) {}
        QueueItem(float p, const ItemType&& d)
            : priority(p), data(std::forward(d)) {}

        float priority;
        ItemType data;
    };

    struct QueueTypeLess {
        bool operator()(QueueItem& lhs, QueueItem& rhs) const {
            return lhs.priority < rhs.priority;
        }
    };

private:
    std::mutex m_lock;
    std::atomic<uint32_t> m_count{0};
    std::condition_variable m_condition;

    std::mutex m_itemLock;
    std::priority_queue<QueueItem, std::vector<QueueItem>, QueueTypeLess>
        m_items;
    std::atomic<bool> m_exit{false};
};

END_GL3_CORE_NAMESPACE
END_GFX_NAMESPACE