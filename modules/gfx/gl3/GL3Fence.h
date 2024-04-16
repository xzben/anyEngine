#pragma once

#include "../base/Fence.h"
#include "gl_common.h"

BEGIN_GFX_NAMESPACE
class GL3Device;
class GL3Fence : public Fence {
public:
    GL3Fence(GL3Device& device);
    virtual ~GL3Fence();

private:
    GL3Device& m_device;
};

END_GFX_NAMESPACE