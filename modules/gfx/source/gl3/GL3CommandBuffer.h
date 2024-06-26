﻿#pragma once

#include <type_traits>

#include "base/CommandBuffer.h"
#include "core/cmdimpl/CmdBase.h"
#include "gl_common.h"

BEGIN_GFX_NAMESPACE

BEGIN_GL3_CORE_NAMESPACE
class CmdBeginRenderPass;
class GLContext;
END_GL3_CORE_NAMESPACE

class GL3Device;
class GL3CommandPool;
class GL3CommandBuffer : public CommandBuffer {
    friend class RenderWorkTask;

public:
    GL3CommandBuffer(GL3Device& device, GL3CommandPool& pool, uint32_t index);
    virtual ~GL3CommandBuffer();

    virtual bool reset() override;
    virtual void begin(CommandBufferUsage usage = CommandBufferUsage::ONE_TIME_SUBMIT) override;
    virtual void beginRendPass(const BeginRenderPassInfo& info) override;
    virtual void enable(RenderState state) override;
    virtual void disable(RenderState state) override;
    virtual void draw(const DrawMeshInfo& info) override;
    virtual void compute(const ComputeInfo& info) override;
    virtual void nextSubPass() override;
    virtual void endRendPass() override;
    virtual void end() override;

    virtual void setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;

    virtual void setScissor(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;

    virtual void updateBuffer(Buffer* buffer, const void* pData, uint32_t size, uint32_t offset = 0,
                              bool staticData = false) override;

    virtual void updateTexture(Texture* texture, const TextureSubInfo& info, const void* pData,
                               uint32_t size, bool staticData = false) override;

    virtual void copyBuffer(Buffer* src, Buffer* dst, uint32_t size, uint32_t srcOffset = 0,
                            uint32_t dstOffset = 0) override;
    virtual void copyTexture(Texture* src, Texture* dst, const TextureCopyInfo& info) override;
    virtual void blitTexture(Texture* src, Texture* dst, const TextureBliteInfo& info) override;
    virtual void generateMipmaps(Texture* textre, uint32_t mipLevels) override;
    virtual void clearTexture(Texture* src, TextureSubInfo& subInfo, const Color& color) override;
    virtual void updateInputAssembler(InputAssembler* input,
                                      const UpdateAssemblerInfo& info) override;

public:
    enum class Status {
        Initial,
        Recording,
        Executable,
        Pending,
    };
    void execute(gl3::GLContext* context);
    const Status getCurStatus() const { return m_curStatus; }
    void changeStatus(Status status);

protected:
    template <class CmdClass, typename... Params>
    CmdClass* allocCmd(Params... params) {
        gl3::CmdType type = CmdClass::CUR_CMD_TYPE;

        auto it       = m_freeCmds.find(type);
        CmdClass* cmd = nullptr;
        if (it == m_freeCmds.end() || it->second.empty()) {
            cmd = new CmdClass(*this);
        } else {
            cmd = dynamic_cast<CmdClass*>(it->second.back());
            it->second.pop_back();
        }
        assert(cmd->getType() == type);
        cmd->init(params...);
        return cmd;
    }

    void freeCmd(gl3::CmdBase* cmd) {
        auto type = cmd->getType();
        auto it   = m_freeCmds.find(type);
        cmd->reset();
        if (it == m_freeCmds.end()) {
            m_freeCmds[type] = {cmd};
        } else {
            it->second.push_back(cmd);
        }
    }

    void clearCmds();

private:
    std::vector<gl3::CmdBase*> m_cmds;
    std::unordered_map<gl3::CmdType, std::vector<gl3::CmdBase*>> m_freeCmds;
    GL3Device* m_device{nullptr};
    GL3CommandPool* m_pool{nullptr};
    uint32_t m_index;

    gl3::CmdBeginRenderPass* m_lastBeginRenderPass{nullptr};
    Status m_curStatus{Status::Initial};
};

END_GFX_NAMESPACE