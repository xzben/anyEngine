#pragma once

#include <atomic>
#include <mutex>

#include "../base/Event.h"
#include "gl_common.h"

BEGIN_GFX_NAMESPACE
class GL3Device;
class GL3Event : public Event {
public:
    GL3Event(GL3Device& device);
    virtual ~GL3Event();

    void wait();
    void signal();

private:
    GL3Device& m_device;

    std::mutex m_conditionLock;
    std::condition_variable m_condition;
    std::atomic<int> m_count{0};
};

END_GFX_NAMESPACE