#pragma once

#include "../common/gfx_common.h"
#include "Handle.h"
#include "SwapChain.h"
#include "Texture.h"

BEGIN_GFX_NAMESPACE

class Texture;
class SwapChain;

enum DrawSurfaceType {
    TEXTURE,
    SWAPCHAIN,
};

class DrawSurface : public GfxObject {
public:
    DrawSurface() : GfxObject(GfxObjectType::DrawSurface){};
    virtual ~DrawSurface() = default;

    virtual DrawSurfaceType getType() = 0;
    // 当渲染的target 纹理
    virtual Texture* getTexture() = 0;
    // 可能 target texture 是 array texture 这里可以获取 layerindex
    virtual uint32_t getLayerIndex() = 0;

    bool isTexture() { return getType() == DrawSurfaceType::TEXTURE; }
    bool isSwapChain() { return getType() == DrawSurfaceType::SWAPCHAIN; }
};

class DrawTextureSurface : public DrawSurface {
public:
    DrawTextureSurface(Texture* tex, uint32_t layerindex = 0)
        : m_texture(tex), m_layerIndex(layerindex) {
        m_texture->addRef();
    }

    virtual ~DrawTextureSurface() {
        if (m_texture) {
            m_texture->delRef();
            m_texture = nullptr;
        }
    }

    virtual DrawSurfaceType getType() override { return DrawSurfaceType::TEXTURE; }

    virtual Texture* getTexture() override { return m_texture; }
    virtual uint32_t getLayerIndex() override { return m_layerIndex; }

private:
    Texture* m_texture{nullptr};
    uint32_t m_layerIndex{0};
};

enum class SwapChainAttachment {
    COLOR,
    DEPTH_STENCIL,
};

class DrawSwaChainSurface : public DrawSurface {
public:
    DrawSwaChainSurface(SwapChain* swapChain, uint32_t imageIndex, SwapChainAttachment attachment)
        : m_swapChain(swapChain), m_attachment(attachment), m_imageIndex(imageIndex) {}
    virtual ~DrawSwaChainSurface() {
        m_swapChain->delRef();
        m_swapChain = nullptr;
    }

    virtual DrawSurfaceType getType() override { return DrawSurfaceType::SWAPCHAIN; }

    virtual Texture* getTexture() override {
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
    virtual uint32_t getLayerIndex() override { return 0; }

    SwapChain* m_swapChain{nullptr};
    uint32_t m_imageIndex{0};
    SwapChainAttachment m_attachment{SwapChainAttachment::COLOR};
};
END_GFX_NAMESPACE