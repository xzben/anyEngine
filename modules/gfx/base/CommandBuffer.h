#pragma once

#include "../common/gfx_common.h"

BEGIN_GFX_NAMESPACE

class Texture;
class Buffer;
class InputAssembler;
class Pipeline;
class RenderPass;

class CommandBuffer {
public:
    CommandBuffer()          = default;
    virtual ~CommandBuffer() = default;

    virtual void begin(CommandBufferUsage usage)           = 0;
    virtual void beginRendPass(RenderPass* renderpass)     = 0;
    virtual void bindPipeline(Pipeline* pipeline)          = 0;
    virtual void bindInputAssembler(InputAssembler* input) = 0;
    virtual void bindTexture(const std::string& name, Texture* tex);
    virtual void bindUniformBuffer(const std::string& name, Buffer* buf);
    virtual void draw()        = 0;
    virtual void nextSubPass() = 0;
    virtual void endRendPass() = 0;
    virtual void end()         = 0;

    virtual void setViewport(uint32_t x, uint32_t y, uint32_t width,
                             uint32_t height)                               = 0;
    virtual void setScissor(uint32_t x, uint32_t y, uint32_t width,
                            uint32_t height)                                = 0;
    virtual void copyBuffer(Buffer* src, Buffer* dst, uint32_t size,
                            uint32_t srcOffset = 0, uint32_t dstOffset = 0) = 0;
    virtual void copyTexture(Texture* src, Texture* dst, const RectI& srcRect,
                             const RectI& dstRect)                          = 0;

    virtual void generateMipmaps(Texture* textre, uint32_t mipLevels) = 0;
};

END_GFX_NAMESPACE