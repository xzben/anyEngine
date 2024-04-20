#include "GL3Semaphore.h"

BEGIN_GFX_NAMESPACE

GL3Semaphore::GL3Semaphore(GL3Device& device) : m_device(device) {}
GL3Semaphore::~GL3Semaphore() {}

void GL3Semaphore::wait() {
    std::unique_lock<std::mutex> locker(m_conditionLock);
    m_count--;
    if (m_count < 0) {
        m_condition.wait(locker, [&]() { return m_count > 0; });
    }
}

void GL3Semaphore::signal() {
    std::lock_guard<std::mutex> locker(m_conditionLock);
    m_count++;
    if (m_count <= 0) {
        m_condition.notify_one();
    }
}

END_GFX_NAMESPACE