#include "GL3CommandBuffer.h"

#include <unordered_map>
#include <unordered_set>

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
    changeStatus(Status::Initial);
    return true;
}

void GL3CommandBuffer::begin(CommandBufferUsage usage) { changeStatus(Status::Recording); }
void GL3CommandBuffer::beginRendPass(const BeginRenderPassInfo& info) {
    auto cmd              = allocCmd<gl3::CmdBeginRenderPass>(info);
    m_lastBeginRenderPass = cmd;
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
void GL3CommandBuffer::compute(const ComputeInfo& info) {}

using CmdStatus = GL3CommandBuffer::Status;
static std::unordered_map<CmdStatus, std::unordered_set<CmdStatus>> g_statusChangeConfig = {
    {CmdStatus::Initial,
     {
         CmdStatus::Initial,     //
         CmdStatus::Recording,   //
         CmdStatus::Executable,  //
         CmdStatus::Pending,     //
     }},                         //
    {CmdStatus::Recording,
     {
         CmdStatus::Initial,     //
         CmdStatus::Executable,  //
     }},
    {CmdStatus::Executable,
     {
         CmdStatus::Recording,  //
         CmdStatus::Pending,    //
     }},
    {CmdStatus::Pending,
     {
         CmdStatus::Executable,  //
     }},
};

void GL3CommandBuffer::changeStatus(CmdStatus status) {
    auto itStatus = g_statusChangeConfig.find(status);
    CCASSERT(itStatus != g_statusChangeConfig.end(), "can't find status:%d config", status);
    const auto& config = itStatus->second;
    CCASSERT(config.find(m_curStatus) != config.end(),
             "gl3 command buffer can't change status from:%d to %d", m_curStatus, status);
    m_curStatus = status;
}

void GL3CommandBuffer::end() { changeStatus(Status::Executable); }

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