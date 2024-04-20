#include "GL3Fence.h"

BEGIN_GFX_NAMESPACE

GL3Fence::GL3Fence(GL3Device& device, bool createSignaled) : m_device(device) {
    m_count = createSignaled ? 1 : 0;
}

GL3Fence::~GL3Fence() {}

bool GL3Fence::reset() {
    m_count = 0;
    return true;
}

bool GL3Fence::wait(uint64_t timeout) {
    std::unique_lock<std::mutex> locker(m_conditionLock);
    if (m_count <= 0) {
        if (timeout > 0) {
            m_condition.wait_for(locker, std::chrono::nanoseconds(timeout),
                                 [&]() { return m_count > 0; });
        } else {
            m_condition.wait(locker, [&]() { return m_count > 0; });
        }
    }
    return true;
}

void GL3Fence::signal() {
    std::lock_guard<std::mutex> locker(m_conditionLock);
    m_count++;
    if (m_count <= 0) {
        m_condition.notify_all();
    }
}

END_GFX_NAMESPACE