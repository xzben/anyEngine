#pragma once

#include "../base/CommandBuffer.h"
#include "core/vulkan_core.h"
#include "vulkan_common.h"
BEGIN_GFX_NAMESPACE

class VulkanCommandPool;

class VulkanCommandBuffer : public CommandBuffer, public GfxObject {
public:
    VulkanCommandBuffer(VulkanCommandPool& pool,
                        CommandBufferLevel level = CommandBufferLevel::PRIMARY);

    virtual ~VulkanCommandBuffer();
    virtual void begin(CommandBufferUsage usage) override;
    virtual void beginRendPass(RenderPass* renderpass) override;
    virtual void bindPipeline(Pipeline* pipeline) override;
    virtual void bindInputAssembler(InputAssembler* input) override;
    virtual void bindTexture(const std::string& name, Texture* tex) override;
    virtual void bindUniformBuffer(const std::string& name,
                                   Buffer* buf) override;
    virtual void draw() override;
    virtual void nextSubPass() override;
    virtual void endRendPass() override;
    virtual void end() override;

    virtual void setViewport(uint32_t x, uint32_t y, uint32_t width,
                             uint32_t height) override;
    virtual void setScissor(uint32_t x, uint32_t y, uint32_t width,
                            uint32_t height) override;
    virtual void copyBuffer(Buffer* src, Buffer* dst, uint32_t size,
                            uint32_t srcOffset = 0,
                            uint32_t dstOffset = 0) override;
    virtual void copyTexture(Texture* src, Texture* dst, const RectI& srcRect,
                             const RectI& dstRect) override;

    virtual void generateMipmaps(Texture* textre, uint32_t mipLevels) override;

    operator bool() const { return m_handle != VK_NULL_HANDLE; }
    operator VkCommandBuffer() const { return m_handle; }

private:
    VkCommandBuffer m_handle{VK_NULL_HANDLE};
    CommandBufferLevel m_level{CommandBufferLevel::PRIMARY};
    VulkanCommandPool& m_pool;

    Pipeline* m_curPipeline{nullptr};
    InputAssembler* m_curInput{nullptr};
};

END_GFX_NAMESPACE