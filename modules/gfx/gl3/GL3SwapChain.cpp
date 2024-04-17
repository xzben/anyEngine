#include "GL3SwapChain.h"

#include "GL3Fence.h"

BEGIN_GFX_NAMESPACE

GL3SwapChain::GL3SwapChain(GL3Device& device, uint32_t width, uint32_t height,
                           bool singleBuffer)
    : m_device(device),
      m_width(width),
      m_height(height),
      m_singleBuffer(singleBuffer) {}
GL3SwapChain::~GL3SwapChain() {}

std::pair<bool, uint32_t> GL3SwapChain::acquireNextImage(Semaphore* semophore,
                                                         Fence* fence,
                                                         uint32_t timeout) {
    if (fence) dynamic_cast<GL3Fence*>(fence)->signal();
    semophore->signal();
    return std::make_pair(true, 0);
}

END_GFX_NAMESPACE