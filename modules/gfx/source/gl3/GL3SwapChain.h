#pragma once

#include <mutex>

#include "GL3Texture.h"
#include "base/SwapChain.h"
#include "gl_common.h"
BEGIN_GFX_NAMESPACE

BEGIN_GL3_CORE_NAMESPACE
class GLContext;
END_GL3_CORE_NAMESPACE

class GL3Device;

class GL3SwapChain : public SwapChain {
public:
    GL3SwapChain(GL3Device& device, const SurfaceInfo& info, bool needDepthStencil);
    virtual ~GL3SwapChain();

    virtual std::pair<bool, uint32_t> acquireNextImage(Semaphore* semophore, Fence* fence = nullptr,
                                                       uint32_t timeout = 0);
    virtual void swapBuffer() = 0;
    virtual uint32_t getImageCount() const override { return 1; }

    virtual OGL_HANDLE getPresentFbo() { return 0; }
    virtual void setActiveImageIndex(uint32_t imageIndex) override {}
    virtual uint32_t getCurImageIndex() const override { return 0; }
    virtual Texture* getColorTexture(uint32_t imageIndex) const override;
    virtual Texture* getDepthTexture(uint32_t imageIndex) const override;

    void updateAttachment(uint32_t width, uint32_t height);

    void present(gl3::GLContext* context);
    virtual const SurfaceInfo& getInfo() const override { return m_info; };

protected:
    GL3Device& m_device;
    SurfaceInfo m_info;
    bool m_needDepthStencil{false};
    GL3Texture* m_colorTexture{nullptr};
    GL3Texture* m_depthTexture{nullptr};
    mutable std::mutex m_lock;
    OGL_HANDLE m_imageFbo{OGL_NULL_HANDLE};
};

END_GFX_NAMESPACE