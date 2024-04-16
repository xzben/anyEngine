#pragma once

#include "../base/RenderPass.h"
#include "gl_common.h"

BEGIN_GFX_NAMESPACE
class GL3Device;
class GL3RenderPass : public RenderPass {
public:
    GL3RenderPass(GL3Device& device);
    virtual ~GL3RenderPass();

private:
    GL3Device& m_device;
};

END_GFX_NAMESPACE