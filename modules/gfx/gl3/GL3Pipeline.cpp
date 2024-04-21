#include "GL3Pipeline.h"

#include "GL3RenderPass.h"
#include "GL3Shader.h"

BEGIN_GFX_NAMESPACE
GL3Pipeline::GL3Pipeline(GL3Device& device, RenderPass* renderPass,
                         uint32_t subpass, Shader* shader,
                         const PipelineState& state)
    : m_device(device),
      m_renderPass(renderPass),
      m_subpassIndex(subpass),
      m_shader(shader),
      m_state(state) {
    m_renderPass->addRef();
    m_shader->addRef();
}

GL3Pipeline::~GL3Pipeline() {
    m_renderPass->delRef();
    m_shader->delRef();
}
END_GFX_NAMESPACE