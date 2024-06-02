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
    virtual void beginRendPass(const BeginRenderPassInfo& info) override;
    void bindPipeline(Pipeline* pipeline);
    void bindInputAssembler(InputAssembler* input);
    void bindTexture(const std::string& name, Texture* tex);
    void bindUniformBuffer(const std::string& name, Buffer* buf);
    void bindStorageBuffer(const std::string& name, Buffer* buf);
    void draw();
    void dispatch(uint32_t groupNumX, uint32_t groupNumY, uint32_t groupNumZ);
    void enable(RenderState state) override;
    void disable(RenderState state) override;
    void draw(const DrawMeshInfo& info) override;
    void compute(const ComputeInfo& info) override;

    void nextSubPass() override;
    void endRendPass() override;
    void end() override;

    void setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;
    void setScissor(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;

    void updateBuffer(Buffer* buffer, const void* pData, uint32_t size, uint32_t offset = 0,
                      bool staticData = false) override;

    void updateTexture(Texture* texture, const TextureSubInfo& info, const void* pData,
                       uint32_t size, bool staticData = false) override;

    void copyBuffer(Buffer* src, Buffer* dst, uint32_t size, uint32_t srcOffset = 0,
                    uint32_t dstOffset = 0) override;
    void copyTexture(Texture* src, Texture* dst, const TextureCopyInfo& info) override;
    void blitTexture(Texture* src, Texture* dst, const TextureBliteInfo& info) override;
    void generateMipmaps(Texture* textre, uint32_t mipLevels) override;
    void clearTexture(Texture* src, TextureSubInfo& subInfo, const Color& color) override;
    void updateInputAssembler(InputAssembler* input, const UpdateAssemblerInfo& info) override;

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