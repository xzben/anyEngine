#pragma once

#include "../common/gfx_common.h"
#include "Fence.h"
#include "Semaphore.h"

BEGIN_GFX_NAMESPACE
class Texture;

class SwapChain : public GfxObject {
public:
    virtual std::pair<bool, uint32_t> acquireNextImage(Semaphore* semophore, Fence* fence = nullptr,
                                                       uint32_t timeout = 0) = 0;

    virtual void handleUpdateSurfaceInfo(const SurfaceInfo& info) = 0;

    virtual const SurfaceInfo& getInfo() const                  = 0;
    virtual void setActiveImageIndex(uint32_t imageIndex)       = 0;
    virtual uint32_t getCurImageIndex() const                   = 0;
    virtual Texture* getColorTexture(uint32_t imageIndex) const = 0;
    virtual Texture* getDepthTexture(uint32_t imageIndex) const = 0;
    virtual uint32_t getImageCount() const                      = 0;

protected:
    SwapChain() : GfxObject(GfxObjectType::SwapChain) {}
    virtual ~SwapChain() = default;
};

END_GFX_NAMESPACE