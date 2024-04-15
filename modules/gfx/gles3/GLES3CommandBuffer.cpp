#include "GLES3CommandBuffer.h"

BEGIN_GFX_NAMESPACE

GLES3CommandBuffer::GLES3CommandBuffer(gles::GLContext& context) {}
GLES3CommandBuffer::~GLES3CommandBuffer() {}

bool GLES3CommandBuffer::reset() { return true; }
void GLES3CommandBuffer::begin(CommandBufferUsage usage) {}
void GLES3CommandBuffer::beginRendPass(
    RenderPass* renderpass, const std::vector<DrawSurface>& attachments,
    const std::vector<ClearValue>& clearValues) {}
void GLES3CommandBuffer::bindPipeline(Pipeline* pipeline) {}
void GLES3CommandBuffer::bindInputAssembler(InputAssembler* input) {}
void GLES3CommandBuffer::bindTexture(const std::string& name, Texture* tex) {}
void GLES3CommandBuffer::bindUniformBuffer(const std::string& name,
                                           Buffer* buf) {}
void GLES3CommandBuffer::bindStorageBuffer(const std::string& name,
                                           Buffer* buf) {}
void GLES3CommandBuffer::draw() {}
void GLES3CommandBuffer::dispatch(uint32_t groupNumX, uint32_t groupNumY,
                                  uint32_t groupNumZ) {}
void GLES3CommandBuffer::nextSubPass() {}
void GLES3CommandBuffer::endRendPass() {}
void GLES3CommandBuffer::end() {}

void GLES3CommandBuffer::setViewport(uint32_t x, uint32_t y, uint32_t width,
                                     uint32_t height) {}
void GLES3CommandBuffer::setScissor(uint32_t x, uint32_t y, uint32_t width,
                                    uint32_t height) {}

void GLES3CommandBuffer::updateBuffer(Buffer* buffer, const void* pData,
                                      uint32_t size, uint32_t offset) {}

void GLES3CommandBuffer::updateTexture(Texture* texture, const void* pData,
                                       uint32_t width, uint32_t height) {}

void GLES3CommandBuffer::copyBuffer(Buffer* src, Buffer* dst, uint32_t size,
                                    uint32_t srcOffset, uint32_t dstOffset) {}
void GLES3CommandBuffer::copyTexture(Texture* src, Texture* dst,
                                     const TextureCopyInfo& info) {}
void GLES3CommandBuffer::blitTexture(Texture* src, Texture* dst,
                                     const TextureBliteInfo& info) {}
void GLES3CommandBuffer::generateMipmaps(Texture* textre, uint32_t mipLevels) {}

END_GFX_NAMESPACE