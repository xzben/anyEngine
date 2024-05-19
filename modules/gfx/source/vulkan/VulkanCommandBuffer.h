#pragma once

#include "base/CommandBuffer.h"
#include "core/vulkan_core.h"
#include "vulkan_common.h"
BEGIN_GFX_NAMESPACE

class VulkanCommandPool;

class VulkanCommandBuffer : public CommandBuffer {
public:
    VulkanCommandBuffer(VulkanCommandPool& pool,
                        CommandBufferLevel level = CommandBufferLevel::PRIMARY);

    virtual ~VulkanCommandBuffer();
    virtual bool reset() override;
    virtual void begin(CommandBufferUsage usage = CommandBufferUsage::ONE_TIME_SUBMIT) override;
    virtual void beginRendPass(RenderPass* renderpass, const std::vector<DrawSurface*>& attachments,
                               const std::vector<ClearValue>& clearValues) override;
    void bindPipeline(Pipeline* pipeline);
    void bindInputAssembler(InputAssembler* input);
    void bindTexture(const std::string& name, Texture* tex);
    void bindUniformBuffer(const std::string& name, Buffer* buf);
    void bindStorageBuffer(const std::string& name, Buffer* buf);
    void draw();
    void dispatch(uint32_t groupNumX, uint32_t groupNumY, uint32_t groupNumZ);
    virtual void enable(RenderState state) override;
    virtual void disable(RenderState state) override;
    virtual void draw(const DrawMeshInfo& info) override;
    virtual void compute(const ComputeInfo& info) override;

    virtual void nextSubPass() override;
    virtual void endRendPass() override;
    virtual void end() override;

    virtual void setViewport(float x, float y, float width, float height) override;
    virtual void setScissor(float x, float y, float width, float height) override;

    virtual void updateBuffer(Buffer* buffer, const void* pData, uint32_t size, uint32_t offset = 0,
                              bool staticData = false) override;

    virtual void updateTexture(Texture* texture, const TextureUpdateInfo& info, const void* pData,
                               uint32_t size, bool staticData = false) override;

    virtual void copyBuffer(Buffer* src, Buffer* dst, uint32_t size, uint32_t srcOffset = 0,
                            uint32_t dstOffset = 0) override;
    virtual void copyTexture(Texture* src, Texture* dst, const TextureCopyInfo& info) override;
    virtual void blitTexture(Texture* src, Texture* dst, const TextureBliteInfo& info);
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