#pragma once

#include "../base/CommandBuffer.h"
#include "gles_common.h"

BEGIN_GFX_NAMESPACE

class GLES3CommandBuffer : public CommandBuffer {
public:
    GLES3CommandBuffer(gles::GLContext& context);
    virtual ~GLES3CommandBuffer();

    virtual bool reset() override;
    virtual void begin(CommandBufferUsage usage) override;
    virtual void beginRendPass(
        RenderPass* renderpass, const std::vector<DrawSurface>& attachments,
        const std::vector<ClearValue>& clearValues) override;
    virtual void bindPipeline(Pipeline* pipeline) override;
    virtual void bindInputAssembler(InputAssembler* input) override;
    virtual void bindTexture(const std::string& name, Texture* tex);
    virtual void bindUniformBuffer(const std::string& name, Buffer* buf);
    virtual void bindStorageBuffer(const std::string& name, Buffer* buf);
    virtual void draw() override;
    virtual void dispatch(uint32_t groupNumX, uint32_t groupNumY,
                          uint32_t groupNumZ) override;
    virtual void nextSubPass() override;
    virtual void endRendPass() override;
    virtual void end() override;

    virtual void setViewport(uint32_t x, uint32_t y, uint32_t width,
                             uint32_t height) override;
    virtual void setScissor(uint32_t x, uint32_t y, uint32_t width,
                            uint32_t height) override;

    virtual void updateBuffer(Buffer* buffer, const void* pData, uint32_t size,
                              uint32_t offset = 0) override;

    virtual void updateTexture(Texture* texture, const void* pData,
                               uint32_t width, uint32_t height) override;

    virtual void copyBuffer(Buffer* src, Buffer* dst, uint32_t size,
                            uint32_t srcOffset = 0,
                            uint32_t dstOffset = 0) override;
    virtual void copyTexture(Texture* src, Texture* dst,
                             const TextureCopyInfo& info) override;
    virtual void blitTexture(Texture* src, Texture* dst,
                             const TextureBliteInfo& info);
    virtual void generateMipmaps(Texture* textre, uint32_t mipLevels) override;

private:
};

END_GFX_NAMESPACE