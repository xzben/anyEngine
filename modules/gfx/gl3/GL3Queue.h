#pragma once

#include "../base/Queue.h"
#include "gl_common.h"

BEGIN_GFX_NAMESPACE
class GL3Device;
class GL3Queue : public Queue {
public:
    GL3Queue(GL3Device& device);
    virtual ~GL3Queue();

private:
    GL3Device& m_device;
};

END_GFX_NAMESPACE