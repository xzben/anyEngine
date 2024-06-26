#pragma once

#include <atomic>
#include <mutex>

#include "base/Fence.h"
#include "gl_common.h"

BEGIN_GFX_NAMESPACE
class GL3Device;
class GL3Fence : public Fence {
public:
    GL3Fence(GL3Device& device, bool createSignaled = false);
    virtual ~GL3Fence();
    bool reset() override;
    WaitResult wait(uint64_t timeout = 0) override;
    void signal();

protected:
    GFX_HANDLE getHandleImp() const override { return 0; };
    void freeSync();
    WaitResult doSyncWait(uint64_t timeout);

private:
    GL3Device& m_device;

    std::mutex m_conditionLock;
    std::condition_variable m_condition;
    std::atomic<bool> m_signed;

#if ENABLE_FENCE_SYNC
    GLsync m_sync{0};
#endif
};

END_GFX_NAMESPACE