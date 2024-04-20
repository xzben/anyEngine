#pragma once

#include <atomic>
#include <mutex>

#include "../base/Fence.h"
#include "gl_common.h"

BEGIN_GFX_NAMESPACE
class GL3Device;
class GL3Fence : public Fence {
public:
    GL3Fence(GL3Device& device, bool createSignaled = false);
    virtual ~GL3Fence();
    virtual bool reset() override;
    virtual bool wait(uint64_t timeout = 0) override;
    void signal();

private:
    GL3Device& m_device;

    std::mutex m_conditionLock;
    std::condition_variable m_condition;
    std::atomic<int> m_count{0};
};

END_GFX_NAMESPACE