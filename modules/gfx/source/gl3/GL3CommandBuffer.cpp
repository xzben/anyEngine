#include "GL3CommandBuffer.h"

#include "GL3CommandPool.h"
#include "core/cmdimpl/CmdImpls.h"

BEGIN_GFX_NAMESPACE

GL3CommandBuffer::GL3CommandBuffer(GL3Device& device, GL3CommandPool& pool, uint32_t index)
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
void GL3CommandBuffer::beginRendPass(const BeginRenderPassInfo& info) {
    auto cmd              = allocCmd<gl3::CmdBeginRenderPass>(info);
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

void GL3CommandBuffer::setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
    auto cmd = allocCmd<gl3::CmdSetViewport>(x, y, width, height);
    m_cmds.push_back(cmd);
}
void GL3CommandBuffer::setScissor(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
    auto cmd = allocCmd<gl3::CmdSetScissor>(x, y, width, height);
    m_cmds.push_back(cmd);
}

void GL3CommandBuffer::clearTexture(Texture* src, TextureSubInfo& subInfo, const Color& color) {
    auto cmd = allocCmd<gl3::CmdClearTexture>(src, subInfo, color);
    m_cmds.push_back(cmd);
}
void GL3CommandBuffer::updateInputAssembler(InputAssembler* input,
                                            const UpdateAssemblerInfo& info) {
    auto cmd = allocCmd<gl3::CmdUpdateInputAssembler>(input, info);
    m_cmds.push_back(cmd);
}

void GL3CommandBuffer::updateBuffer(Buffer* buffer, const void* pData, uint32_t size,
                                    uint32_t offset, bool staticData) {
    auto cmd = allocCmd<gl3::CmdUpdateBuffer>(buffer, pData, size, offset, staticData);
    m_cmds.push_back(cmd);
}

void GL3CommandBuffer::updateTexture(Texture* texture, const TextureSubInfo& info,
                                     const void* pData, uint32_t size, bool staticData) {
    auto cmd = allocCmd<gl3::CmdUpdateTexture>(texture, info, pData, size, staticData);
    m_cmds.push_back(cmd);
}

void GL3CommandBuffer::copyBuffer(Buffer* src, Buffer* dst, uint32_t size, uint32_t srcOffset,
                                  uint32_t dstOffset) {
    auto cmd = allocCmd<gl3::CmdCopyBuffer>(src, dst, size, srcOffset, dstOffset);
    m_cmds.push_back(cmd);
}
void GL3CommandBuffer::copyTexture(Texture* src, Texture* dst, const TextureCopyInfo& info) {
    auto cmd = allocCmd<gl3::CmdCopyTexture>(src, dst, info);
    m_cmds.push_back(cmd);
}
void GL3CommandBuffer::blitTexture(Texture* src, Texture* dst, const TextureBliteInfo& info) {
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

void GL3CommandBuffer::execute(gl3::GLContext* context) {
    for (auto& cmd : m_cmds) {
        cmd->execute(context);
    }
}
END_GFX_NAMESPACE