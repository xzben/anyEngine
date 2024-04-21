#include "GL3Event.h"

BEGIN_GFX_NAMESPACE
GL3Event::GL3Event(GL3Device& device) : m_device(device) {}
GL3Event::~GL3Event() {}

void GL3Event::wait() {
    std::unique_lock<std::mutex> locker(m_conditionLock);
    m_count--;
    if (m_count < 0) {
        m_condition.wait(locker, [&]() { return m_count > 0; });
    }
}

void GL3Event::signal() {
    std::lock_guard<std::mutex> locker(m_conditionLock);
    m_count++;
    if (m_count <= 0) {
        m_condition.notify_one();
    }
}
END_GFX_NAMESPACE