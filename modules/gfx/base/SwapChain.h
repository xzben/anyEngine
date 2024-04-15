#pragma once

#include "../common/gfx_common.h"
#include "Fence.h"
#include "Semaphore.h"

BEGIN_GFX_NAMESPACE

class SwapChain: public GfxObject {
public:
    virtual std::pair<bool, uint32_t> acquireNextImage(
        const Fence* fence, const Semaphore* semophore,
        uint32_t timeout = std::numeric_limits<uint32_t>::max()) = 0;

    virtual void handleUpdateSurfaceInfo(SurfaceInfo info) = 0;

protected:
    SwapChain() : GfxObject(GfxObjectType::SwapChain) {}
    virtual ~SwapChain() = default;
};

END_GFX_NAMESPACE