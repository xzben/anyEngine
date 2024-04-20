#pragma once

#include <atomic>
#include <condition_variable>
#include <mutex>
#include <queue>
#include <thread>

#include "gl3/core/gl3_core_common.h"

BEGIN_GFX_NAMESPACE
BEGIN_GL3_CORE_NAMESPACE

template <typename ItemType>
class ThreadQueue {
public:
    ThreadQueue() {}
    ~ThreadQueue() {}

    void addItem(const ItemType& item) {
        m_itemLock.lock();
        m_items.push(item);
        m_count++;
        m_itemLock.unlock();
        m_condition.notify_one();
    }

    void addItem(const ItemType&& item) {
        m_itemLock.lock();
        m_items.push(item);
        m_count++;
        m_itemLock.unlock();
    }

    void wait(std::queue<ItemType>& queue) {
        assert(queue.empty());

        if (m_count <= 0) {
            std::unique_lock<std::mutex> locker(m_lock);
            m_condition.wait(locker, [&]() { return m_count > 0; });
        }

        m_itemLock.lock();
        m_items.swap(queue);
        m_itemLock.unlock();
    }

    ItemType wait() {
        if (m_count <= 0) {
            std::unique_lock<std::mutex> locker(m_lock);
            m_condition.wait(locker, [&]() { return m_count > 0; });
        }

        m_itemLock.lock();
        ItemType item = m_items.front();
        m_items.pop();
        m_itemLock.unlock();

        return std::move(item);
    }

private:
    std::mutex m_lock;
    std::atomic_uint32_t m_count{0};
    std::condition_variable m_condition;

    std::mutex m_itemLock;
    std::queue<ItemType> m_items;
};

END_GL3_CORE_NAMESPACE
END_GFX_NAMESPACE