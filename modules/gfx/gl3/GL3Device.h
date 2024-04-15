#pragma once

#include "../base/Device.h"
#include "GL3Buffer.h"
#include "GL3Event.h"
#include "GL3Fence.h"
#include "GL3InputAssembler.h"
#include "GL3Pipeline.h"
#include "GL3Queue.h"
#include "GL3RenderPass.h"
#include "GL3Sampler.h"
#include "GL3Semaphore.h"
#include "GL3Shader.h"
#include "GL3SwapChain.h"
#include "GL3Texture.h"
#include "core/GLContext.h"
#include "gl_common.h"

BEGIN_GFX_NAMESPACE

class GL3Device : public Device {
public:
    virtual bool init(const DeviceInfo& info) override;
    virtual GL3Shader* createShader() override;

    virtual GL3Queue* getQueue(QueueType& type, uint32_t index = 0) override;

    virtual GL3RenderPass* createRenderPass(
        std::vector<Attachment>& attachments,
        const std::vector<SubPass>& subpass,
        const std::vector<SubPassDependency>& dependencies) override;

    virtual GL3Pipeline* createPipeline(RenderPass* renderPass,
                                        uint32_t subpass, Shader* shader,
                                        const PipelineState& state) override;

    virtual GL3Texture* createTexture(const TextureInfo& info,
                                      const void* pData = nullptr) override;
    virtual GL3Buffer* createBuffer(BufferType type, uint32_t size) override;
    virtual GL3Sampler* createSampler(const SamplerInfo& info) override;

    virtual GL3InputAssembler* createInputAssembler(
        const std::vector<Attribute>& attributes, const void* pVertexData,
        uint32_t vertexCount, const void* pIndexData = nullptr,
        uint32_t indexCount    = 0,
        uint32_t indexItemSize = sizeof(uint32_t)) override;

    virtual GL3InputAssembler* createInputAssembler(
        const std::vector<Attribute>& attributes,
        const std::vector<Attribute>& InstanceAttributes,
        const void* pVertexData, uint32_t vertexCount,
        const void* pInstanceData, uint32_t instanceCount,
        const void* pIndexData = nullptr, uint32_t indexCount = 0,
        uint32_t indexItemSize = sizeof(uint32_t)) override;

    virtual GL3SwapChain* createSwapChain(void* nativeWindow, uint32_t width,
                                          uint32_t height,
                                          bool singleBuffer) override;
    virtual GL3Fence* createFence(bool signaled = false) override;

    virtual GL3Semaphore* createSemaphore(uint32_t count = 0) override;

    virtual GL3Event* createEvent() override;

    virtual void waitIdle() override;

private:
    std::unique_ptr<gl3::GLContext> m_pMainContext;
};

END_GFX_NAMESPACE