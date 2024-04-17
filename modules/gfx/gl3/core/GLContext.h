#pragma once

#include <cassert>
#include <type_traits>

#include "NoCopy.h"
#include "gl3_core_common.h"
#include "glselect.h"

BEGIN_GFX_NAMESPACE
class GL3SwapChain;
class GL3Device;

BEGIN_GL3_CORE_NAMESPACE

class GLContext : public NoCopy {
public:
    GLContext(GL3Device& device);
    virtual ~GLContext();

    void makeCurrent(GL3SwapChain* swapChain = nullptr);
    GL3SwapChain* createSwapChain(void* winow, uint32_t width, uint32_t height,
                                  bool singleBuffer);
    void destroySwapChain(GL3SwapChain* swapChain);
    void swapBuffer(GL3SwapChain* swapChain);

private:
    GLContextType m_context;
    GL3Device& m_device;
};

END_GL3_CORE_NAMESPACE
END_GFX_NAMESPACE