#include "RenderPipeline.h"

BEGIN_PIPELINE_NAMESPACE

RenderPipeline::RenderPipeline(gfx::Device& device, gfx::RenderPass* renderPass,
                               const std::vector<RenderSubpass*>& subpasses)
    : m_device(device), m_renderPass(renderPass), m_subpasses(subpasses) {}
RenderPipeline::~RenderPipeline() {}

void RenderPipeline::setDrawTarget(RenderTarget* target) {}

void RenderPipeline::begin() {}
void RenderPipeline::setUniform(const BufferBinding& binding) {}
void RenderPipeline::setTexture(const TextureBinding& binding) {}
void RenderPipeline::draw(const DrawInfo& info) {}
void RenderPipeline::drawIntances(const DrawInstanceInfo& info) {}
void RenderPipeline::end() {}
END_PIPELINE_NAMESPACE