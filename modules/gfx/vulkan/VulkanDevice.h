#pragma once

#include <memory>

#include "../base/Device.h"
#include "VulkanBuffer.h"
#include "VulkanEvent.h"
#include "VulkanFence.h"
#include "VulkanInputAssembler.h"
#include "VulkanPipeline.h"
#include "VulkanQueue.h"
#include "VulkanRenderPass.h"
#include "VulkanSampler.h"
#include "VulkanSemaphore.h"
#include "VulkanShader.h"
#include "VulkanSwapChain.h"
#include "VulkanTexture.h"
#include "core/vulkan_core.h"
#include "vulkan_common.h"

BEGIN_GFX_NAMESPACE

class VulkanDevice : public Device {
public:
    VulkanDevice();
    virtual ~VulkanDevice();

    virtual bool init(const DeviceInfo& info) override;
    virtual VulkanShader* createShader(ShaderModuleInfo* info,
                                       uint32_t count) override;

    virtual VulkanQueue* getQueue(QueueType& type, uint32_t index = 0) override;

    virtual VulkanRenderPass* createRenderPass(
        const std::vector<Attachment>& attachments,
        const std::vector<SubPass>& subpass,
        const std::vector<SubPassDependency>& dependencies) override;

    virtual VulkanPipeline* createPipeline(RenderPass* renderPass,
                                           uint32_t subpass, Shader* shader,
                                           const PipelineState& state) override;

    virtual VulkanTexture* createTexture(const TextureInfo& info,
                                         const void* pData = nullptr) override;
    virtual VulkanBuffer* createBuffer(BufferType type, uint32_t size,
                                       const void* pData = nullptr) override;
    virtual VulkanSampler* createSampler(const SamplerInfo& info) override;

    virtual VulkanInputAssembler* createInputAssembler(
        PrimitiveType primitiveType, const std::vector<Attribute>& attributes,
        const void* pVertexData, uint32_t vertexCount,
        const void* pIndexData = nullptr, uint32_t indexCount = 0,
        uint32_t indexItemSize = sizeof(uint32_t)) override;

    virtual VulkanInputAssembler* createInputAssembler(
        PrimitiveType primitiveType, const std::vector<Attribute>& attributes,
        const std::vector<Attribute>& InstanceAttributes,
        const void* pVertexData, uint32_t vertexCount,
        const void* pInstanceData, uint32_t instanceCount,
        const void* pIndexData = nullptr, uint32_t indexCount = 0,
        uint32_t indexItemSize = sizeof(uint32_t)) override;

    virtual VulkanSwapChain* createSwapChain(
        void* nativeWindow, uint32_t width, uint32_t height, bool singleBuffer,
        bool needDepthStencil = false) override;
    virtual VulkanFence* createFence(bool signaled = false) override;

    virtual VulkanSemaphore* createSemaphore() override;

    virtual VulkanEvent* createEvent() override;

    virtual void waitIdle() override;

    virtual void withOneTimeCmd(
        std::function<void(CommandBuffer& cmd)> callback) override;

public:
    virtual void destroyBuffer(Buffer* buffer) override;
    virtual void destroyEvent(Event* event) override;
    virtual void destroyFence(Fence* fence) override;
    virtual void destroyInputAssembler(InputAssembler* input) override;
    virtual void destroyPipeline(Pipeline* pipeline) override;
    virtual void destroyRenderPasss(RenderPass* renderPass) override;
    virtual void destroySampler(Sampler* sampler) override;
    virtual void destroySemaphore(Semaphore* semaphore) override;
    virtual void destroyShader(Shader* shader) override;
    virtual void destroySwapChain(SwapChain* swapChain) override;
    virtual void destroyTexture(Texture* texture) override;

protected:
    static std::unique_ptr<vk::Instance> getInstance(const DeviceInfo& info);
    static vk::PhysicalDevice selectPhysicDevice(const DeviceInfo& info,
                                                 const vk::Instance& instance,
                                                 vk::Surface& surface);

    static std::unique_ptr<vk::LogicDevice> createLogicDevice(
        const DeviceInfo& info, vk::Instance& instance,
        vk::PhysicalDevice physicalDevice, vk::Surface& surface);

private:
    std::unique_ptr<vk::Instance> m_instance;
    std::unique_ptr<vk::LogicDevice> m_logicDevice;
    std::unique_ptr<vk::Surface> m_surface;
};

END_GFX_NAMESPACE