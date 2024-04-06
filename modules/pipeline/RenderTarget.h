#include "pipeline_common.h"

BEGIN_PIPELINE_NAMESPACE

class RenderTarget {
public:
    RenderTarget(const std::vector<gfx::Texture*>& images);
    RenderTarget(gfx::SwapChain* swapchain);
};
END_PIPELINE_NAMESPACE