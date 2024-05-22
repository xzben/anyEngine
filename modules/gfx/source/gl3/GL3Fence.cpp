#include "GL3Fence.h"

BEGIN_GFX_NAMESPACE

GL3Fence::GL3Fence(GL3Device& device, bool createSignaled) : m_device(device) {
    m_signed = createSignaled;
}

GL3Fence::~GL3Fence() { freeSync(); }

bool GL3Fence::reset() {
    m_signed = false;
    freeSync();

    return true;
}

void GL3Fence::freeSync() {
#if ENABLE_FENCE_SYNC
    std::unique_lock<std::mutex> locker(m_conditionLock);
    if (m_sync != 0) {
        GL_CHECK(glDeleteSync(m_sync));
        m_sync = 0;
    }
#endif
}

WaitResult GL3Fence::doSyncWait(uint64_t timeout) {
#if ENABLE_FENCE_SYNC
    if (0 != m_sync) {
        GLenum result = GL_WAIT_FAILED;
        GL_CHECK(result = glClientWaitSync(m_sync, GL_SYNC_FLUSH_COMMANDS_BIT, timeout));

        switch (result) {
            case GL_ALREADY_SIGNALED:
            case GL_CONDITION_SATISFIED: {
                return WaitResult::SUCCESS;
            }
            case GL_TIMEOUT_EXPIRED: {
                return WaitResult::TIMEOUT;
            }
            default: {
                CCASSERT(false, "error wait result:%d", result);
                return WaitResult::FAILED;
            }
        }
    }
#endif
    return WaitResult::SUCCESS;
}

WaitResult GL3Fence::wait(uint64_t timeout) {
    std::unique_lock<std::mutex> locker(m_conditionLock);
    WaitResult result = WaitResult::SUCCESS;
    if (!m_signed) {
        if (timeout > 0) {
            bool success = m_condition.wait_for(locker, std::chrono::nanoseconds(timeout),
                                                [&]() { return m_signed.load(); });

            result = success ? WaitResult::SUCCESS : WaitResult::TIMEOUT;
        } else {
            m_condition.wait(locker, [&]() { return m_signed.load(); });
        }
    }

    if (result == WaitResult::SUCCESS) {
        return doSyncWait(timeout);
    }

    return result;
}

void GL3Fence::signal() {
    std::lock_guard<std::mutex> locker(m_conditionLock);
    m_signed = true;
#if ENABLE_FENCE_SYNC
    if (m_sync != 0) {
        GL_CHECK(glDeleteSync(m_sync));
        m_sync = 0;
    }
    GL_CHECK(m_sync = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0));
    GL_CHECK(glFlush());
#endif
    m_condition.notify_all();
}

END_GFX_NAMESPACE