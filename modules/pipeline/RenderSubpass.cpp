#include "RenderSubpass.h"

BEGIN_PIPELINE_NAMESPACE

RenderSubpass::RenderSubpass(gfx::Device& device, uint32_t index,
                             gfx::Shader* shader,
                             const gfx::PipelineState& state)
    : m_device(device), m_shader(shader), m_index(index), m_state(state) {
    m_shader->addRef();
}

RenderSubpass::~RenderSubpass() {
    if (m_shader) {
        m_shader->delRef();
        m_shader = nullptr;
    }
}

void RenderSubpass::addInput(uint32_t attachment, gfx::ImageLayout layout) {
    m_subpassInfo.inputAttachments.push_back({attachment, layout});
}

void RenderSubpass::addColor(uint32_t attachment, gfx::ImageLayout layout) {
    m_subpassInfo.colorAttachments.push_back({attachment, layout});
}

void RenderSubpass::addDepthStencil(uint32_t attachment,
                                    gfx::ImageLayout layout) {
    m_subpassInfo.depthStencilAttachments.push_back({attachment, layout});
}

void RenderSubpass::addResolve(uint32_t attachment, gfx::ImageLayout layout) {
    m_subpassInfo.resolveAttachments.push_back({attachment, layout});
}

void RenderSubpass::addPerserve(uint32_t attachment) {
    m_subpassInfo.preserveAttachments.push_back(attachment);
}

bool RenderSubpass::build(gfx::RenderPass* renderPass) {
    assert(m_pipeline = nullptr);
    m_pipeline =
        m_device.createPipeline(renderPass, m_index, m_shader, m_state);

    return m_pipeline != nullptr;
}

END_PIPELINE_NAMESPACE