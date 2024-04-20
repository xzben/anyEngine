#pragma once

#include <atomic>
#include <mutex>

#include "../base/Semaphore.h"
#include "gl_common.h"

BEGIN_GFX_NAMESPACE
class GL3Device;
class GL3Semaphore : public Semaphore {
public:
    GL3Semaphore(GL3Device& device);
    virtual ~GL3Semaphore();

    virtual void wait() override;
    virtual void signal() override;

private:
    std::mutex m_conditionLock;
    std::condition_variable m_condition;
    std::atomic<int> m_count{0};

    GL3Device& m_device;
};

END_GFX_NAMESPACE