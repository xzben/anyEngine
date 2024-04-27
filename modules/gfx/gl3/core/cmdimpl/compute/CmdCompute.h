#pragma once

#include "../CmdBase.h"
#include "gl3/GL3Buffer.h"
#include "gl3/GL3InputAssembler.h"
#include "gl3/GL3Pipeline.h"
#include "gl3/GL3Sampler.h"
#include "gl3/GL3Texture.h"

BEGIN_GFX_NAMESPACE
BEGIN_GL3_CORE_NAMESPACE

class CmdCompute : public CmdBase {
public:
    static const CmdType CUR_CMD_TYPE = CmdType::COMPUTE;

public:
    CmdCompute(GL3CommandBuffer& cmdBuf) : CmdBase(cmdBuf, CUR_CMD_TYPE) {}
    virtual ~CmdCompute() {}

    void init(const ComputeInfo& info) {
        m_uniformBuffers.resize(info.uniformCount);
        for (uint32_t i = 0; i < info.uniformCount; i++) {
            m_uniformBuffers[i] = info.pUniformBindings[i];
            m_uniformBuffers[i].buffer->addRef();
        }

        m_storageBuffers.resize(info.storageCount);
        for (uint32_t i = 0; i < info.storageCount; i++) {
            m_storageBuffers[i] = info.pShaderStorageBindings[i];
            m_storageBuffers[i].buffer->addRef();
        }

        m_textures.resize(info.textureCount);
        for (uint32_t i = 0; i < info.textureCount; i++) {
            m_textures[i] = info.pTextureBindings[i];
            m_textures[i].texture->addRef();

            if (m_textures[i].sampler) {
                m_textures[i].sampler->addRef();
            }
        }

        m_pipeline = dynamic_cast<GL3Pipeline*>(info.pipeline);
        m_pipeline->addRef();
        m_groupNumX = info.groupNumX;
        m_groupNumY = info.groupNumY;
        m_groupNumZ = info.groupNumZ;
    }

    virtual void reset() override {
        m_pipeline->delRef();
        m_pipeline = nullptr;

        m_uniformBuffers.clear();
        m_storageBuffers.clear();
        m_textures.clear();
    }
    virtual void execute(gl3::GLContext* context) override {
        auto shader = m_pipeline->getShader();
        for (auto& item : m_uniformBuffers) {
            shader->setUniformBuffer(item.name, item.buffer, item.offset,
                                     item.size);
        }
        for (auto& item : m_storageBuffers) {
            shader->setShaderStorageBuffer(item.name, item.buffer, item.offset,
                                           item.size);
        }
        for (auto& item : m_textures) {
            shader->setTexture(item.name, item.texture, item.sampler);
        }
        // context->updateState(m_pipeline->getState());
        shader->bind();
        GL_CHECK(glDispatchCompute(m_groupNumX, m_groupNumY, m_groupNumZ));
    }

private:
    std::vector<BufferBindingInfo> m_uniformBuffers;
    std::vector<BufferBindingInfo> m_storageBuffers;
    std::vector<TextureBindingInfo> m_textures;

    uint32_t m_groupNumX{1};
    uint32_t m_groupNumY{1};
    uint32_t m_groupNumZ{1};

    GL3Pipeline* m_pipeline{nullptr};
};
END_GL3_CORE_NAMESPACE
END_GFX_NAMESPACE