#include "GL3CommandBuffer.h"

#include "core/cmdimpl/CmdImpls.h"

BEGIN_GFX_NAMESPACE

GL3CommandBuffer::GL3CommandBuffer(GL3Device& device) : m_device(device) {}
GL3CommandBuffer::~GL3CommandBuffer() { clearCmds(); }

bool GL3CommandBuffer::reset() {
    for (auto& cmd : m_cmds) {
        freeCmd(cmd);
    }
    return true;
}
void GL3CommandBuffer::begin(CommandBufferUsage usage) {}
void GL3CommandBuffer::beginRendPass(
    RenderPass* renderpass, const std::vector<DrawSurface>& attachments,
    const std::vector<ClearValue>& clearValues) {
    auto cmd =
        allocCmd<gl3::CmdBeginRenderPass>(renderpass, attachments, clearValues);
    m_cmds.push_back(cmd);
}

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

void GL3CommandBuffer::clearCmds() {
    for (auto& cmd : m_cmds) {
        delete cmd;
    }
    m_cmds.clear();

    for (auto it = m_freeCmds.begin(); it != m_freeCmds.end(); it++) {
        for (auto& cmd : it->second) {
            delete cmd;
        }
    }
    m_freeCmds.clear();
}
END_GFX_NAMESPACE