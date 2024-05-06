#pragma once

#include "../base/SwapChain.h"
#include "GL3Texture.h"
#include "gl_common.h"

BEGIN_GFX_NAMESPACE

BEGIN_GL3_CORE_NAMESPACE
class GLContext;
END_GL3_CORE_NAMESPACE

class GL3Device;

class GL3SwapChain : public SwapChain {
public:
    GL3SwapChain(GL3Device& device, uint32_t width, uint32_t height,
                 bool singleBuffer, bool needDepthStencil);
    virtual ~GL3SwapChain();

    virtual std::pair<bool, uint32_t> acquireNextImage(Semaphore* semophore,
                                                       Fence* fence = nullptr,
                                                       uint32_t timeout = 0);
    virtual void swapBuffer() = 0;

    virtual OGL_HANDLE getPresentFbo() { return 0; }
    virtual void setActiveImageIndex(uint32_t imageIndex) override {}
    virtual uint32_t getCurImageIndex() override { return 0; }
    virtual Texture* getColorTexture(uint32_t imageIndex) override {
        return m_colorTexture;
    }
    virtual Texture* getDepthTexture(uint32_t imageIndex) override {
        return m_depthTexture;
    }

    void updateAttachment(uint32_t width, uint32_t height);

    void present(gl3::GLContext* context);

protected:
    GL3Device& m_device;
    uint32_t m_width;
    uint32_t m_height;
    bool m_singleBuffer{false};
    bool m_needDepthStencil{false};
    GL3Texture* m_colorTexture{nullptr};
    GL3Texture* m_depthTexture{nullptr};

    OGL_HANDLE m_imageFbo{OGL_NULL_HANDLE};
};

END_GFX_NAMESPACE