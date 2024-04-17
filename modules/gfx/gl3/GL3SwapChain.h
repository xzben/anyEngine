#pragma once

#include "../base/SwapChain.h"
#include "gl_common.h"

BEGIN_GFX_NAMESPACE
class GL3Device;

class GL3SwapChain : public SwapChain {
public:
    GL3SwapChain(GL3Device& device, uint32_t width, uint32_t height,
                 bool singleBuffer);
    virtual ~GL3SwapChain();

    virtual std::pair<bool, uint32_t> acquireNextImage(Semaphore* semophore,
                                                       Fence* fence = nullptr,
                                                       uint32_t timeout = 0);

protected:
    GL3Device& m_device;
    uint32_t m_width;
    uint32_t m_height;
    bool m_singleBuffer{false};
};

END_GFX_NAMESPACE