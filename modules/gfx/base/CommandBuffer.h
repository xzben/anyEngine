#pragma once

#include "../common/gfx_common.h"

BEGIN_GFX_NAMESPACE

class Texture;
class Buffer;
class InputAssembler;
class Pipeline;
class RenderPass;

class CommandBuffer : public GfxObject {
public:
    virtual bool reset()                                                   = 0;
    virtual void begin(CommandBufferUsage usage)                           = 0;
    virtual void beginRendPass(RenderPass* renderpass,
                               const std::vector<DrawSurface>& attachments,
                               const std::vector<ClearValue>& clearValues) = 0;
    virtual void bindPipeline(Pipeline* pipeline)                          = 0;
    virtual void bindInputAssembler(InputAssembler* input)                 = 0;
    virtual void bindTexture(const std::string& name, Texture* tex);
    virtual void bindUniformBuffer(const std::string& name, Buffer* buf);
    virtual void bindStorageBuffer(const std::string& name, Buffer* buf);
    virtual void draw()                       = 0;
    virtual void dispatch(uint32_t groupNumX, uint32_t groupNumY,
                          uint32_t groupNumZ) = 0;
    virtual void nextSubPass()                = 0;
    virtual void endRendPass()                = 0;
    virtual void end()                        = 0;

    virtual void setViewport(uint32_t x, uint32_t y, uint32_t width,
                             uint32_t height) = 0;
    virtual void setScissor(uint32_t x, uint32_t y, uint32_t width,
                            uint32_t height)  = 0;

    virtual void updateBuffer(Buffer* buffer, const void* pData, uint32_t size,
                              uint32_t offset = 0) = 0;

    virtual void updateTexture(Texture* texture, const void* pData,
                               uint32_t width, uint32_t height) = 0;

    virtual void copyBuffer(Buffer* src, Buffer* dst, uint32_t size,
                            uint32_t srcOffset = 0, uint32_t dstOffset = 0) = 0;
    virtual void copyTexture(Texture* src, Texture* dst,
                             const TextureCopyInfo& info)                   = 0;
    virtual void blitTexture(Texture* src, Texture* dst,
                             const TextureBliteInfo& info);
    virtual void generateMipmaps(Texture* textre, uint32_t mipLevels) = 0;

protected:
    CommandBuffer() : GfxObject(GfxObjectType::CommandBuffer) {}
    virtual ~CommandBuffer() = default;
};

END_GFX_NAMESPACE