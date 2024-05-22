#include "GL3Semaphore.h"

BEGIN_GFX_NAMESPACE

GL3Semaphore::GL3Semaphore(GL3Device& device, bool withOutSync)
    : m_device(device), m_withOutSync(withOutSync) {}
GL3Semaphore::~GL3Semaphore() { freeSync(); }

WaitResult GL3Semaphore::wait(uint64_t timeout) {
    std::unique_lock<std::mutex> locker(m_conditionLock);

    if (--m_count < 0) {
        if (timeout > 0) {
            bool success = m_condition.wait_for(locker, std::chrono::nanoseconds(timeout),
                                                [&]() { return m_count.load() > 0; });

            return success ? WaitResult::SUCCESS : WaitResult::TIMEOUT;
        } else {
            m_condition.wait(locker, [&]() { return m_count.load() > 0; });
        }
    }

    return doSyncWait(timeout);
}

void GL3Semaphore::freeSync() {
#if ENABLE_FENCE_SYNC
    std::unique_lock<std::mutex> locker(m_conditionLock);
    if (m_sync != 0) {
        GL_CHECK(glDeleteSync(m_sync));
        m_sync = 0;
    }
#endif
}

WaitResult GL3Semaphore::doSyncWait(uint64_t timeout) {
#if ENABLE_FENCE_SYNC
    if (m_withOutSync) return WaitResult::SUCCESS;

    if (0 != m_sync) {
        GL_CHECK(glWaitSync(m_sync, 0, GL_TIMEOUT_IGNORED));
    }
#endif
    return WaitResult::SUCCESS;
}

void GL3Semaphore::signal() {
    std::lock_guard<std::mutex> locker(m_conditionLock);

#if ENABLE_FENCE_SYNC
    if (m_sync != 0) {
        GL_CHECK(glDeleteSync(m_sync));
        m_sync = 0;
    }
    GL_CHECK(m_sync = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0));
    GL_CHECK(glFlush());
#endif

    if (++m_count <= 0) {
        m_condition.notify_one();
    }
}

END_GFX_NAMESPACE