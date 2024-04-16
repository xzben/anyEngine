#pragma once

#include "../base/SwapChain.h"
#include "gl_common.h"

BEGIN_GFX_NAMESPACE
class GL3Device;
class GL3SwapChain : public SwapChain {
public:
    GL3SwapChain(GL3Device& device);
    virtual ~GL3SwapChain();

private:
    GL3Device& m_device;
};

END_GFX_NAMESPACE