#include "GLES3CommandBuffer.h"

BEGIN_GFX_NAMESPACE

void begin(CommandBufferUsage usage) {}
void beginRendPass(RenderPass* renderpass) {}
void bindPipeline(Pipeline* pipeline) {}
void bindInputAssembler(InputAssembler* input) {}
void bindTexture(const std::string& name, Texture* tex);
void bindUniformBuffer(const std::string& name, Buffer* buf);
void draw() {}
void nextSubPass() {}
void endRendPass() {}
void end() {}

void setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {}
void setScissor(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {}
void copyBuffer(Buffer* src, Buffer* dst, uint32_t size, uint32_t srcOffset = 0,
                uint32_t dstOffset = 0) {}
void copyTexture(Texture* src, Texture* dst, const RectI& srcRect,
                 const RectI& dstRect) {}

void generateMipmaps(Texture* textre, uint32_t mipLevels) {}

END_GFX_NAMESPACE