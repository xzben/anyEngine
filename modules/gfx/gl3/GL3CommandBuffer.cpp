#include "GL3CommandBuffer.h"

#include "GL3CommandPool.h"
#include "core/cmdimpl/CmdImpls.h"

BEGIN_GFX_NAMESPACE

GL3CommandBuffer::GL3CommandBuffer(GL3Device& device, GL3CommandPool& pool,
                                   uint32_t index)
    : m_device(&device), m_pool(&pool), m_index(index) {}
GL3CommandBuffer::~GL3CommandBuffer() { clearCmds(); }

bool GL3CommandBuffer::reset() {
    for (auto& cmd : m_cmds) {
        freeCmd(cmd);
    }
    m_lastBeginRenderPass = nullptr;
    return true;
}
void GL3CommandBuffer::begin(CommandBufferUsage usage) {}
void GL3CommandBuffer::beginRendPass(
    RenderPass* renderpass, const std::vector<DrawSurface>& attachments,
    const std::vector<ClearValue>& clearValues) {
    auto cmd =
        allocCmd<gl3::CmdBeginRenderPass>(renderpass, attachments, clearValues);
    m_lastBeginRenderPass = cmd;
    m_cmds.push_back(cmd);
}

void GL3CommandBuffer::enable(RenderState state) {
    auto cmd = allocCmd<gl3::CmdEnable>(state);
    m_cmds.push_back(cmd);
}
void GL3CommandBuffer::disable(RenderState state) {
    auto cmd = allocCmd<gl3::CmdDisable>(state);
    m_cmds.push_back(cmd);
}
void GL3CommandBuffer::draw(const DrawMeshInfo& info) {
    auto cmd = allocCmd<gl3::CmdDraw>(info);
    m_cmds.push_back(cmd);
}
void GL3CommandBuffer::compute(const ComputeInfo& info) {
    auto cmd = allocCmd<gl3::CmdCompute>(info);
    m_cmds.push_back(cmd);
}

void GL3CommandBuffer::nextSubPass() {
    auto cmd = allocCmd<gl3::CmdNextSubPass>(m_lastBeginRenderPass);
    m_cmds.push_back(cmd);
}

void GL3CommandBuffer::endRendPass() {
    auto cmd = allocCmd<gl3::CmdEndRenderPass>(m_lastBeginRenderPass);
    m_cmds.push_back(cmd);
}
void GL3CommandBuffer::end() {}

void GL3CommandBuffer::setViewport(float x, float y, float width,
                                   float height) {
    auto cmd = allocCmd<gl3::CmdSetViewport>(x, y, width, height);
    m_cmds.push_back(cmd);
}
void GL3CommandBuffer::setScissor(float x, float y, float width, float height) {
    auto cmd = allocCmd<gl3::CmdSetScissor>(x, y, width, height);
    m_cmds.push_back(cmd);
}

void GL3CommandBuffer::updateBuffer(Buffer* buffer, const void* pData,
                                    uint32_t size, uint32_t offset) {
    auto cmd = allocCmd<gl3::CmdUpdateBuffer>(buffer, pData, size, offset);
    m_cmds.push_back(cmd);
}

void GL3CommandBuffer::updateTexture(Texture* texture, const void* pData,
                                     uint32_t size, uint32_t width,
                                     uint32_t height) {
    auto cmd =
        allocCmd<gl3::CmdUpdateTexture>(texture, pData, size, width, height);
    m_cmds.push_back(cmd);
}

void GL3CommandBuffer::copyBuffer(Buffer* src, Buffer* dst, uint32_t size,
                                  uint32_t srcOffset, uint32_t dstOffset) {
    auto cmd =
        allocCmd<gl3::CmdCopyBuffer>(src, dst, size, srcOffset, dstOffset);
    m_cmds.push_back(cmd);
}
void GL3CommandBuffer::copyTexture(Texture* src, Texture* dst,
                                   const TextureCopyInfo& info) {
    auto cmd = allocCmd<gl3::CmdCopyTexture>(src, dst, info);
    m_cmds.push_back(cmd);
}
void GL3CommandBuffer::blitTexture(Texture* src, Texture* dst,
                                   const TextureBliteInfo& info) {
    auto cmd = allocCmd<gl3::CmdBlitTexture>(src, dst, info);
    m_cmds.push_back(cmd);
}
void GL3CommandBuffer::generateMipmaps(Texture* textre, uint32_t mipLevels) {
    auto cmd = allocCmd<gl3::CmdGenerateMipmap>(textre, mipLevels);
    m_cmds.push_back(cmd);
}

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

void GL3CommandBuffer::execute() {
    for (auto& cmd : m_cmds) {
        cmd->execute();
    }
}
END_GFX_NAMESPACE