#include "GL3Device.h"

BEGIN_GFX_NAMESPACE

bool GL3Device::init(const DeviceInfo& info) {
    m_pMainContext = std::make_unique<gl3::GLContext>(*this);
    return true;
}

GL3Shader* GL3Device::createShader() { return new GL3Shader(*this); }

GL3Queue* GL3Device::getQueue(QueueType& type, uint32_t index) {
    return nullptr;
}

GL3RenderPass* GL3Device::createRenderPass(
    std::vector<Attachment>& attachments, const std::vector<SubPass>& subpass,
    const std::vector<SubPassDependency>& dependencies) {
    return nullptr;
}

GL3Pipeline* GL3Device::createPipeline(RenderPass* renderPass, uint32_t subpass,
                                       Shader* shader,
                                       const PipelineState& state) {
    return nullptr;
}

GL3Texture* GL3Device::createTexture(const TextureInfo& info,
                                     const void* pData) {
    return nullptr;
}
GL3Buffer* GL3Device::createBuffer(BufferType type, uint32_t size) {
    return nullptr;
}
GL3Sampler* GL3Device::createSampler(const SamplerInfo& info) {
    return nullptr;
}

GL3InputAssembler* GL3Device::createInputAssembler(
    const std::vector<Attribute>& attributes, const void* pVertexData,
    uint32_t vertexCount, const void* pIndexData, uint32_t indexCount,
    uint32_t indexItemSize) {
    return nullptr;
}

GL3InputAssembler* GL3Device::createInputAssembler(
    const std::vector<Attribute>& attributes,
    const std::vector<Attribute>& InstanceAttributes, const void* pVertexData,
    uint32_t vertexCount, const void* pInstanceData, uint32_t instanceCount,
    const void* pIndexData, uint32_t indexCount, uint32_t indexItemSize) {
    return nullptr;
}

GL3SwapChain* GL3Device::createSwapChain(void* nativeWindow, uint32_t width,
                                         uint32_t height, bool singleBuffer) {
    return nullptr;
}
GL3Fence* GL3Device::createFence(bool signaled) { return nullptr; }

GL3Semaphore* GL3Device::createSemaphore(uint32_t count) { return nullptr; }

GL3Event* GL3Device::createEvent() { return nullptr; }
void GL3Device::waitIdle() {}

//-------------------------------------------------------------
void GL3Device::destroyBuffer(Buffer* buffer) { buffer->delRef(); }
void GL3Device::destroyEvent(Event* event) { event->delRef(); }
void GL3Device::destroyFence(Fence* fence) { fence->delRef(); }
void GL3Device::destroyInputAssembler(InputAssembler* input) {
    input->delRef();
}
void GL3Device::destroyPipeline(Pipeline* pipeline) { pipeline->delRef(); }
void GL3Device::destroyRenderPasss(RenderPass* renderPass) {
    renderPass->delRef();
}
void GL3Device::destroySampler(Sampler* sampler) { sampler->delRef(); }
void GL3Device::destroySemaphore(Semaphore* semaphore) { semaphore->delRef(); }
void GL3Device::destroyShader(Shader* shader) { shader->delRef(); }
void GL3Device::destroySwapChain(SwapChain* swapChain) { swapChain->delRef(); }
void GL3Device::destroyTexture(Texture* texture) { texture->delRef(); }

END_GFX_NAMESPACE