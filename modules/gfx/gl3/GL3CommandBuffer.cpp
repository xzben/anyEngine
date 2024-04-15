#include "GL3CommandBuffer.h"

BEGIN_GFX_NAMESPACE

GL3CommandBuffer::GL3CommandBuffer(gl3::GLContext& context) {}
GL3CommandBuffer::~GL3CommandBuffer() {}

bool GL3CommandBuffer::reset() { return true; }
void GL3CommandBuffer::begin(CommandBufferUsage usage) {}
void GL3CommandBuffer::beginRendPass(
    RenderPass* renderpass, const std::vector<DrawSurface>& attachments,
    const std::vector<ClearValue>& clearValues) {}
void GL3CommandBuffer::bindPipeline(Pipeline* pipeline) {}
void GL3CommandBuffer::bindInputAssembler(InputAssembler* input) {}
void GL3CommandBuffer::bindTexture(const std::string& name, Texture* tex) {}
void GL3CommandBuffer::bindUniformBuffer(const std::string& name, Buffer* buf) {
}
void GL3CommandBuffer::bindStorageBuffer(const std::string& name, Buffer* buf) {
}
void GL3CommandBuffer::draw() {}
void GL3CommandBuffer::dispatch(uint32_t groupNumX, uint32_t groupNumY,
                                uint32_t groupNumZ) {}
void GL3CommandBuffer::nextSubPass() {}
void GL3CommandBuffer::endRendPass() {}
void GL3CommandBuffer::end() {}

void GL3CommandBuffer::setViewport(uint32_t x, uint32_t y, uint32_t width,
                                   uint32_t height) {}
void GL3CommandBuffer::setScissor(uint32_t x, uint32_t y, uint32_t width,
                                  uint32_t height) {}

void GL3CommandBuffer::updateBuffer(Buffer* buffer, const void* pData,
                                    uint32_t size, uint32_t offset) {}

void GL3CommandBuffer::updateTexture(Texture* texture, const void* pData,
                                     uint32_t width, uint32_t height) {}

void GL3CommandBuffer::copyBuffer(Buffer* src, Buffer* dst, uint32_t size,
                                  uint32_t srcOffset, uint32_t dstOffset) {}
void GL3CommandBuffer::copyTexture(Texture* src, Texture* dst,
                                   const TextureCopyInfo& info) {}
void GL3CommandBuffer::blitTexture(Texture* src, Texture* dst,
                                   const TextureBliteInfo& info) {}
void GL3CommandBuffer::generateMipmaps(Texture* textre, uint32_t mipLevels) {}

END_GFX_NAMESPACE