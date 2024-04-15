#include "GLES3Device.h"

BEGIN_GFX_NAMESPACE

bool GLES3Device::init(const DeviceInfo& info) { return true; }
GLES3Shader* GLES3Device::createShader() {
    return new GLES3Shader(*this->m_pMainContext);
}

GLES3Queue* GLES3Device::getQueue(QueueType& type, uint32_t index) {
    return nullptr;
}

GLES3RenderPass* GLES3Device::createRenderPass(
    std::vector<Attachment>& attachments, const std::vector<SubPass>& subpass,
    const std::vector<SubPassDependency>& dependencies) {
    return nullptr;
}

GLES3Pipeline* GLES3Device::createPipeline(RenderPass* renderPass,
                                           uint32_t subpass, Shader* shader,
                                           const PipelineState& state) {
    return nullptr;
}

GLES3Texture* GLES3Device::createTexture(const TextureInfo& info,
                                         const void* pData) {
    return nullptr;
}
GLES3Buffer* GLES3Device::createBuffer(BufferType type, uint32_t size) {
    return nullptr;
}
GLES3Sampler* GLES3Device::createSampler(const SamplerInfo& info) {
    return nullptr;
}

GLES3InputAssembler* GLES3Device::createInputAssembler(
    const std::vector<Attribute>& attributes, const void* pVertexData,
    uint32_t vertexCount, const void* pIndexData, uint32_t indexCount,
    uint32_t indexItemSize) {
    return nullptr;
}

GLES3InputAssembler* GLES3Device::createInputAssembler(
    const std::vector<Attribute>& attributes,
    const std::vector<Attribute>& InstanceAttributes, const void* pVertexData,
    uint32_t vertexCount, const void* pInstanceData, uint32_t instanceCount,
    const void* pIndexData, uint32_t indexCount, uint32_t indexItemSize) {
    return nullptr;
}

GLES3SwapChain* GLES3Device::createSwapChain(void* nativeWindow, uint32_t width,
                                             uint32_t height,
                                             bool singleBuffer) {
    return nullptr;
}
GLES3Fence* GLES3Device::createFence(bool signaled) { return nullptr; }

GLES3Semaphore* GLES3Device::createSemaphore(uint32_t count) { return nullptr; }

GLES3Event* GLES3Device::createEvent() { return nullptr; }

void waitIdle() {}
END_GFX_NAMESPACE