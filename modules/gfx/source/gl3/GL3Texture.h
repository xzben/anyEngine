#pragma once

#include "base/Texture.h"
#include "gl_common.h"

BEGIN_GFX_NAMESPACE
class GL3Device;
class GL3Texture : public Texture {
public:
    static void getPixelFormatInfo(PixelFormat format, GLenum* innerFormat,
                                   GLenum* pixelFomrat, GLenum* dataType,
                                   uint32_t* pixelSize);

public:
    GL3Texture(GL3Device& device, const TextureInfo& info,
               const void* pData = nullptr);
    virtual ~GL3Texture();

    const TextureInfo& getInfo() const override { return m_info; };

public:
    static GLenum getTarget(TextureType type);

protected:
    virtual GFX_HANDLE getHandleImp() const override {
        return (GFX_HANDLE)m_handle;
    }

private:
    GL3Device& m_device;
    TextureInfo m_info;
    OGL_HANDLE m_handle{OGL_NULL_HANDLE};
};

END_GFX_NAMESPACE