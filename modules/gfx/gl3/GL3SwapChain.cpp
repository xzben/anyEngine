#include "GL3SwapChain.h"

#include "GL3Fence.h"

BEGIN_GFX_NAMESPACE

GL3SwapChain::GL3SwapChain(GL3Device& device, uint32_t width, uint32_t height,
                           bool singleBuffer, bool needDepthStencil)
    : m_device(device),
      m_width(width),
      m_height(height),
      m_singleBuffer(singleBuffer),
      m_needDepthStencil(needDepthStencil) {
    updateAttachment(width, height);
}

GL3SwapChain::~GL3SwapChain() {}

std::pair<bool, uint32_t> GL3SwapChain::acquireNextImage(Semaphore* semophore,
                                                         Fence* fence,
                                                         uint32_t timeout) {
    if (fence) dynamic_cast<GL3Fence*>(fence)->signal();
    semophore->signal();
    return std::make_pair(true, 0);
}

void GL3SwapChain::updateAttachment(uint32_t width, uint32_t height) {
    if (m_colorTexture != nullptr) {
        auto& info = m_colorTexture->getInfo();
        if (info.width != width || info.height != height) {
            delete m_colorTexture;
            m_colorTexture = nullptr;
        }
    }

    if (m_colorTexture == nullptr) {
        TextureInfo info;
        info.width  = width;
        info.height = height;
        info.format = PixelFormat::RGBA8;
        info.usage  = TextureUsage::USAGE_SAMPLED_BIT
                     | TextureUsage::USAGE_TRANSFER_DST_BIT
                     | TextureUsage::USAGE_TRANSFER_SRC_BIT
                     | TextureUsage::USAGE_COLOR_ATTACHMENT_BIT;

        m_colorTexture = new GL3Texture(m_device, info, nullptr);
    }

    if (m_needDepthStencil) {
        if (m_depthTexture != nullptr) {
            auto& info = m_depthTexture->getInfo();
            if (info.width != width || info.height != height) {
                delete m_depthTexture;
                m_depthTexture = nullptr;
            }
        }

        if (m_depthTexture == nullptr) {
            TextureInfo info;
            info.width  = width;
            info.height = height;
            info.format = PixelFormat::Depth24Stencil8;
            info.usage  = TextureUsage::USAGE_SAMPLED_BIT
                         | TextureUsage::USAGE_TRANSFER_DST_BIT
                         | TextureUsage::USAGE_TRANSFER_SRC_BIT
                         | TextureUsage::USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;

            m_depthTexture = new GL3Texture(m_device, info, nullptr);
        }
    }
}

void GL3SwapChain::present() {
    makeCurrent();
    if (m_imageFbo == OGL_NULL_HANDLE) {
        GL_CHECK(glGenFramebuffers(1, &m_imageFbo));
    }
    auto wfbo = getPresentFbo();

    GL_CHECK(glBindFramebuffer(GL_READ_FRAMEBUFFER, m_imageFbo));
    GL_CHECK(glFramebufferTexture2D(
        GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
        m_colorTexture->getHandle<OGL_HANDLE>(), 0));

    assert(glCheckFramebufferStatus(GL_READ_FRAMEBUFFER)
           == GL_FRAMEBUFFER_COMPLETE);
    GL_CHECK(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, wfbo));
    assert(glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER)
           == GL_FRAMEBUFFER_COMPLETE);

    GL_CHECK(glBlitFramebuffer(0, 0, m_width, m_height, 0, 0, m_width, m_height,
                               GL_COLOR_BUFFER_BIT, GL_LINEAR));

    GL_CHECK(glBindFramebuffer(GL_READ_FRAMEBUFFER, 0));
    GL_CHECK(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0));

    swapBuffer();
    clearCurrent();
}

END_GFX_NAMESPACE