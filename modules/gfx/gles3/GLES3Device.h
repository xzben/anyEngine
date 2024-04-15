#pragma once

#include "../base/Device.h"
#include "GLES3Buffer.h"
#include "GLES3Event.h"
#include "GLES3Fence.h"
#include "GLES3InputAssembler.h"
#include "GLES3Pipeline.h"
#include "GLES3Queue.h"
#include "GLES3RenderPass.h"
#include "GLES3Sampler.h"
#include "GLES3Semaphore.h"
#include "GLES3Shader.h"
#include "GLES3SwapChain.h"
#include "GLES3Texture.h"
#include "core/GLContext.h"
#include "gles_common.h"

BEGIN_GFX_NAMESPACE

class GLES3Device : public Device {
public:
    virtual bool init(const DeviceInfo& info) override;
    virtual GLES3Shader* createShader() override;

    virtual GLES3Queue* getQueue(QueueType& type, uint32_t index = 0) override;

    virtual GLES3RenderPass* createRenderPass(
        std::vector<Attachment>& attachments,
        const std::vector<SubPass>& subpass,
        const std::vector<SubPassDependency>& dependencies) override;

    virtual GLES3Pipeline* createPipeline(RenderPass* renderPass,
                                          uint32_t subpass, Shader* shader,
                                          const PipelineState& state) override;

    virtual GLES3Texture* createTexture(const TextureInfo& info,
                                        const void* pData = nullptr) override;
    virtual GLES3Buffer* createBuffer(BufferType type, uint32_t size) override;
    virtual GLES3Sampler* createSampler(const SamplerInfo& info) override;

    virtual GLES3InputAssembler* createInputAssembler(
        const std::vector<Attribute>& attributes, const void* pVertexData,
        uint32_t vertexCount, const void* pIndexData = nullptr,
        uint32_t indexCount    = 0,
        uint32_t indexItemSize = sizeof(uint32_t)) override;

    virtual GLES3InputAssembler* createInputAssembler(
        const std::vector<Attribute>& attributes,
        const std::vector<Attribute>& InstanceAttributes,
        const void* pVertexData, uint32_t vertexCount,
        const void* pInstanceData, uint32_t instanceCount,
        const void* pIndexData = nullptr, uint32_t indexCount = 0,
        uint32_t indexItemSize = sizeof(uint32_t)) override;

    virtual GLES3SwapChain* createSwapChain(void* nativeWindow, uint32_t width,
                                            uint32_t height,
                                            bool singleBuffer) override;
    virtual GLES3Fence* createFence(bool signaled = false) override;

    virtual GLES3Semaphore* createSemaphore(uint32_t count = 0) override;

    virtual GLES3Event* createEvent() override;

    virtual void waitIdle() override;

private:
    std::unique_ptr<gles::GLContext> m_pMainContext;
};

END_GFX_NAMESPACE