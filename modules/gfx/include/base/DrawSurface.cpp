#include "DrawSurface.h"

BEGIN_GFX_NAMESPACE
//---------- DrawTextureSurface -----------------
DrawTextureSurface::DrawTextureSurface(Texture* tex, uint32_t layerindex)
    : m_texture(tex), m_layerIndex(layerindex) {
    m_texture->addRef();
}

DrawTextureSurface::~DrawTextureSurface() {
    if (m_texture) {
        m_texture->delRef();
        m_texture = nullptr;
    }
}
//---------- DrawSwaChainSurface -----------------
DrawSwaChainSurface::DrawSwaChainSurface(SwapChain* swapChain, uint32_t imageIndex,
                                         SwapChainAttachment attachment)
    : m_swapChain(swapChain), m_attachment(attachment), m_imageIndex(imageIndex) {}
DrawSwaChainSurface::~DrawSwaChainSurface() {
    m_swapChain->delRef();
    m_swapChain = nullptr;
}
Texture* DrawSwaChainSurface::getTexture() {
    switch (m_attachment) {
        case SwapChainAttachment::COLOR: {
            return m_swapChain->getColorTexture(m_imageIndex);
        }
        case SwapChainAttachment::DEPTH_STENCIL: {
            return m_swapChain->getDepthTexture(m_imageIndex);
        }
    }
    CCASSERT(false, "can't find attachment type[%d] texture", (int)m_attachment);
    return nullptr;
}
END_GFX_NAMESPACE