#include "GL3Fence.h"

BEGIN_GFX_NAMESPACE

GL3Fence::GL3Fence(GL3Device& device, bool createSignaled) : m_device(device) {
    m_signed.store(createSignaled);
}

GL3Fence::~GL3Fence() {}

bool GL3Fence::reset() {
    m_signed.store(false);
    return true;
}

WaitResult GL3Fence::wait(uint64_t timeout) {
    if (!m_signed.load()) {
        std::unique_lock<std::mutex> locker(m_conditionLock);
        if (timeout > 0) {
            bool success =
                m_condition.wait_for(locker, std::chrono::nanoseconds(timeout),
                                     [&]() { return m_signed.load(); });

            return success ? WaitResult::SUCCESS : WaitResult::TIMEOUT;
        } else {
            m_condition.wait(locker, [&]() { return m_signed.load(); });
        }
    }

    return WaitResult::SUCCESS;
}

void GL3Fence::signal() {
    std::lock_guard<std::mutex> locker(m_conditionLock);
    m_signed.store(true);
    m_condition.notify_all();
}

END_GFX_NAMESPACE