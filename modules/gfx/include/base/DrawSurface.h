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
    DrawTextureSurface(Texture* tex, uint32_t layerindex = 0);
    virtual ~DrawTextureSurface() override;
    virtual DrawSurfaceType getType() override { return DrawSurfaceType::TEXTURE; }
    virtual Texture* getTexture() override { return m_texture; }
    virtual uint32_t getLayerIndex() override { return m_layerIndex; }

protected:
    Texture* m_texture{nullptr};
    uint32_t m_layerIndex{0};
};

enum class SwapChainAttachment {
    COLOR,
    DEPTH_STENCIL,
};

class DrawSwaChainSurface : public DrawSurface {
public:
    DrawSwaChainSurface(SwapChain* swapChain, uint32_t imageIndex, SwapChainAttachment attachment);
    virtual ~DrawSwaChainSurface() override;

    virtual DrawSurfaceType getType() override { return DrawSurfaceType::SWAPCHAIN; }
    virtual Texture* getTexture() override;
    virtual uint32_t getLayerIndex() override { return 0; }

protected:
    SwapChain* m_swapChain{nullptr};
    uint32_t m_imageIndex{0};
    SwapChainAttachment m_attachment{SwapChainAttachment::COLOR};
};
END_GFX_NAMESPACE