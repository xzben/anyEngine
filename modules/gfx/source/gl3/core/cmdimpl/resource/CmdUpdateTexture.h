#pragma once

#include "../CmdBase.h"
#include "gl3/GL3Texture.h"

BEGIN_GFX_NAMESPACE
BEGIN_GL3_CORE_NAMESPACE

class CmdUpdateTexture : public CmdBase {
public:
    static const CmdType CUR_CMD_TYPE = CmdType::UPDATE_TEXUTRE;

public:
    CmdUpdateTexture(GL3CommandBuffer& cmdBuf) : CmdBase(cmdBuf, CUR_CMD_TYPE) {}
    virtual ~CmdUpdateTexture() {}

    void init(Texture* texture, const TextureSubInfo& info, const void* pData, uint32_t size,
              bool staticData) {
        m_texture = texture;
        m_texture->addRef();

        if (staticData) {
            m_pSrcData = pData;
        } else {
            m_data.resize(size);
            memcpy(m_data.data(), pData, size);
        }

        m_info = info;
        m_size = size;
    }

    virtual void reset() override {
        m_texture->delRef();
        m_texture  = nullptr;
        m_pSrcData = nullptr;
        m_size     = 0;
        m_data.swap(std::vector<uint8_t>());
    }

    virtual void execute(gl3::GLContext* context) override {
        auto& info       = m_texture->getInfo();
        const void* data = m_pSrcData;
        if (data == nullptr) {
            data = m_data.data();
        }

        OGL_HANDLE handle = m_texture->getHandle<OGL_HANDLE>();
        GLenum internalFormat;
        GLenum pixelFomrat;
        GLenum dataType;
        uint32_t pixelSize;

        GL3Texture::getPixelFormatInfo(info.format, &internalFormat, &pixelFomrat, &dataType,
                                       &pixelSize);

        uint32_t textureSize = info.width * info.height * pixelSize;
        switch (info.type) {
            case TextureType::TEX2D: {
                CCASSERT(m_size == textureSize, "update Texture size error");
                GL_CHECK(glBindTexture(GL_TEXTURE_2D, handle));
                GL_CHECK(glTexSubImage2D(GL_TEXTURE_2D, 0, m_info.offset.x, m_info.offset.y,
                                         m_info.range.x, m_info.range.y, pixelFomrat, dataType,
                                         data));
                GL_CHECK(glBindTexture(GL_TEXTURE_2D, 0));
                break;
            }
            case TextureType::TEX2D_ARRAY: {
                CCASSERT(m_size == textureSize * m_info.range.z, "update Texture size error");
                GL_CHECK(glBindTexture(GL_TEXTURE_2D_ARRAY, handle));
                GL_CHECK(glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, m_info.offset.x, m_info.offset.y,
                                         m_info.offset.z, m_info.range.x, m_info.range.y,
                                         m_info.range.z, pixelFomrat, dataType, data));
                GL_CHECK(glBindTexture(GL_TEXTURE_2D_ARRAY, 0));
                break;
            }
            case TextureType::TEX3D: {
                CCASSERT(m_size == textureSize * m_info.range.z, "update Texture size error");
                GL_CHECK(glBindTexture(GL_TEXTURE_3D, handle));
                GL_CHECK(glTexSubImage3D(GL_TEXTURE_3D, 0, m_info.offset.x, m_info.offset.y,
                                         m_info.offset.z, m_info.range.x, m_info.range.y,
                                         m_info.range.z, pixelFomrat, dataType, data));
                GL_CHECK(glBindTexture(GL_TEXTURE_3D, 0));
                break;
            }
            case TextureType::CUBE: {
                CCASSERT(m_size == textureSize * 6, "update Texture size error");
                GL_CHECK(glBindTexture(GL_TEXTURE_CUBE_MAP, handle));

                uint32_t offset = 0;
                for (int i = 0; i < 6; i++) {
                    const void* pData = ((uint8_t*)data + offset);
                    GL_CHECK(glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, 0, 0,
                                             m_info.range.x, m_info.range.y, pixelFomrat, dataType,
                                             pData));
                    offset += textureSize;
                }
                GL_CHECK(glBindTexture(GL_TEXTURE_CUBE_MAP, 0));
                break;
            }
        }
    }

private:
    Texture* m_texture{nullptr};
    std::vector<uint8_t> m_data;
    const void* m_pSrcData{nullptr};
    uint32_t m_size;
    TextureSubInfo m_info;
};
END_GL3_CORE_NAMESPACE
END_GFX_NAMESPACE