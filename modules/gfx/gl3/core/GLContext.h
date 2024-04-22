#pragma once

#include <array>
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

    void initContextRes();
    void clearContextRes();

    void makeCurrent(GL3SwapChain* swapChain = nullptr);
    void exitCurrent(GL3SwapChain* swapChain = nullptr);
    GL3SwapChain* createSwapChain(void* winow, uint32_t width, uint32_t height,
                                  bool singleBuffer);
    void destroySwapChain(GL3SwapChain* swapChain);
    void swapBuffer(GL3SwapChain* swapChain);

    GLContext* createSubContext();

    OGL_HANDLE getFbo(uint32_t index) { return m_fbos[index]; }
    OGL_HANDLE getVao(uint32_t index) { return m_vaos[index]; }

protected:
    GLContextType createContext(GLContextType share);
    GLContext(GL3Device& device, GLContextType context)
        : m_device(device), m_context(context) {}

private:
    std::array<OGL_HANDLE, 2> m_fbos;
    std::array<OGL_HANDLE, 1> m_vaos;
    GLContextType m_context;
    GL3Device& m_device;
};

END_GL3_CORE_NAMESPACE
END_GFX_NAMESPACE