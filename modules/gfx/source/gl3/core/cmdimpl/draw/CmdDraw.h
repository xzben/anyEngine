#pragma once

#include "../CmdBase.h"
#include "gl3/GL3Buffer.h"
#include "gl3/GL3InputAssembler.h"
#include "gl3/GL3Pipeline.h"
#include "gl3/GL3Sampler.h"
#include "gl3/GL3Texture.h"
BEGIN_GFX_NAMESPACE
BEGIN_GL3_CORE_NAMESPACE

class CmdDraw : public CmdBase {
public:
    static const CmdType CUR_CMD_TYPE = CmdType::DRAW;

public:
    CmdDraw(GL3CommandBuffer& cmdBuf) : CmdBase(cmdBuf, CUR_CMD_TYPE) {}
    virtual ~CmdDraw() {}

    void init(const DrawMeshInfo& info) {
        m_uniformBuffers.resize(info.uniformCount);
        for (uint32_t i = 0; i < info.uniformCount; i++) {
            m_uniformBuffers[i] = info.pUniformBindings[i];
            m_uniformBuffers[i].buffer->addRef();
        }

        m_storageBuffers.resize(info.storageCount);
        for (uint32_t i = 0; i < info.storageCount; i++) {
            m_storageBuffers[i] = info.pShaderStorageBinding[i];
            m_storageBuffers[i].buffer->addRef();
        }

        m_textures.resize(info.textureCount);
        for (uint32_t i = 0; i < info.uniformCount; i++) {
            m_textures[i] = info.pTextureBindings[i];
            m_textures[i].texture->addRef();

            if (m_textures[i].sampler) {
                m_textures[i].sampler->addRef();
            }
        }

        m_meshs.resize(info.meshCount);
        for (uint32_t i = 0; i < info.meshCount; i++) {
            m_meshs[i] = info.pMeshs[i];
            m_meshs[i].input->addRef();
        }

        m_pipeline = dynamic_cast<GL3Pipeline*>(info.pipeline);
        m_pipeline->addRef();
    }

    virtual void reset() override {
        for (auto& item : m_uniformBuffers) {
            item.buffer->delRef();
        }
        m_uniformBuffers.clear();

        for (auto& item : m_storageBuffers) {
            item.buffer->delRef();
        }
        m_storageBuffers.clear();

        for (auto& item : m_textures) {
            item.texture->delRef();
            if (item.sampler) {
                item.sampler->delRef();
            }
        }
        m_textures.clear();

        for (auto& item : m_meshs) {
            item.input->delRef();
        }
        m_meshs.clear();

        m_pipeline->delRef();
        m_pipeline = nullptr;
    }
    virtual void execute(gl3::GLContext* context) override;

private:
    std::vector<BufferBindingInfo> m_uniformBuffers;
    std::vector<BufferBindingInfo> m_storageBuffers;
    std::vector<TextureBindingInfo> m_textures;
    std::vector<MeshInfo> m_meshs;
    std::vector<OGL_HANDLE> m_vaos;

    GL3Pipeline* m_pipeline{nullptr};
};
END_GL3_CORE_NAMESPACE
END_GFX_NAMESPACE