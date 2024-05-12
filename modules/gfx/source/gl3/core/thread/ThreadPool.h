#pragma once

#include <atomic>
#include <functional>
#include <thread>

#include "ThreadPriorityQueue.h"
#include "ThreadQueue.h"
#include "gl3/core/gl3_core_common.h"

BEGIN_GFX_NAMESPACE
BEGIN_GL3_CORE_NAMESPACE

template <typename WorkTaskType>
class ThreadPool {
protected:
    void threadFunc(uint32_t index) {
        m_threaPrepareFunc(index);
        while (!m_exit) {
            WorkTaskType task;
            if (m_queue.wait(task)) {
                m_taskExcuteFunc(task, index);
            }
        }
        m_threadExitFunc(index);
    }

public:
    using TASK_EXECUTE_FUNC =
        std::function<void(WorkTaskType& item, uint32_t threadIndex)>;

    using THREAD_PREPARE_FUNC = std::function<void(uint32_t threadIndex)>;
    using THREAD_EXIT_FUNC    = std::function<void(uint32_t threadIndex)>;

public:
    ThreadPool(
        uint32_t threadNum, TASK_EXECUTE_FUNC taskExecuteFunc,
        THREAD_PREPARE_FUNC prepareFunc = [](uint32_t) {},
        THREAD_EXIT_FUNC exitFunc       = [](uint32_t) {})
        : m_taskExcuteFunc(std::move(taskExecuteFunc)),
          m_threaPrepareFunc(std::move(prepareFunc)),
          m_threadExitFunc(std::move(exitFunc)) {
        for (int i = 0; i < threadNum; i++) {
            m_threads[i] = new std::thread(&ThreadPool::threadFunc, this, i);
        }
    }
    ~ThreadPool() {
        for (auto thd : m_threads) {
            thd->join();
            delete thd;
        }
        m_threads.clear();
    }

    void exit() {
        m_exit.store(true);
        m_queue.exit();
        for (auto thd : m_threads) {
            thd->join();
            delete thd;
        }
        m_threads.clear();
    }

    bool addTask(const WorkTaskType& task) {
        if (m_exit.load()) return false;
        m_queue.addItem(task);

        return true;
    }

    bool addTask(const WorkTaskType&& task) {
        if (m_exit.load()) return false;
        m_queue.addItem(task);

        return true;
    }

private:
    std::vector<std::thread*> m_threads;
    ThreadQueue<WorkTaskType> m_queue;
    std::atomic<bool> m_exit{false};
    THREAD_PREPARE_FUNC m_threaPrepareFunc;
    THREAD_EXIT_FUNC m_threadExitFunc;
    TASK_EXECUTE_FUNC m_taskExcuteFunc;
};

END_GL3_CORE_NAMESPACE
END_GFX_NAMESPACE