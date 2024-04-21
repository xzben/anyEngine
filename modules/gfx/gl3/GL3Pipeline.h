#pragma once

#include "../base/Pipeline.h"
#include "gl_common.h"

BEGIN_GFX_NAMESPACE
class GL3Device;
class RenderPass;

class GL3Pipeline : public Pipeline {
public:
    GL3Pipeline(GL3Device& device, RenderPass* renderPass, uint32_t subpass,
                Shader* shader, const PipelineState& state);
    virtual ~GL3Pipeline();

    virtual Shader* getShader() override { return m_shader; }

    virtual const PipelineState& getState() const { return m_state; };

protected:
    virtual GFX_HANDLE getHandleImp() const { return 0; };

private:
    GL3Device& m_device;
    RenderPass* m_renderPass{nullptr};
    uint32_t m_subpassIndex;
    Shader* m_shader{nullptr};
    PipelineState m_state;
};

END_GFX_NAMESPACE