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

    bool wait(std::queue<ItemType>& queue) {
        assert(queue.empty());

        if (m_count.load() <= 0) {
            std::unique_lock<std::mutex> locker(m_lock);
            m_condition.wait(
                locker, [&]() { return m_count.load() > 0 || m_exit.load(); });
        }

        if (m_exit.load()) {
            return false;
        }

        m_itemLock.lock();
        m_items.swap(queue);
        m_count = 0;
        m_itemLock.unlock();

        return true;
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
        out = m_items.front();
        m_items.pop();
        m_count--;
        m_itemLock.unlock();

        return true;
    }

    void exit() {
        m_exit.store(true);
        m_condition.notify_all();
    }

private:
    std::mutex m_lock;
    std::atomic<uint32_t> m_count{0};
    std::condition_variable m_condition;

    std::atomic<bool> m_exit{false};
    std::mutex m_itemLock;
    std::queue<ItemType> m_items;
};

END_GL3_CORE_NAMESPACE
END_GFX_NAMESPACE