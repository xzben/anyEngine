#include "GL3Semaphore.h"

BEGIN_GFX_NAMESPACE

GL3Semaphore::GL3Semaphore(GL3Device& device) : m_device(device) {}
GL3Semaphore::~GL3Semaphore() {}

WaitResult GL3Semaphore::wait(uint64_t timeout) {
    std::unique_lock<std::mutex> locker(m_conditionLock);
    if (--m_count < 0) {
        if (timeout > 0) {
            bool success =
                m_condition.wait_for(locker, std::chrono::nanoseconds(timeout),
                                     [&]() { return m_count.load() > 0; });

            return success ? WaitResult::SUCCESS : WaitResult::TIMEOUT;
        } else {
            m_condition.wait(locker, [&]() { return m_count.load() > 0; });
        }
    }

    return WaitResult::SUCCESS;
}

void GL3Semaphore::signal() {
    std::lock_guard<std::mutex> locker(m_conditionLock);

    if (++m_count <= 0) {
        m_condition.notify_one();
    }
}

END_GFX_NAMESPACE