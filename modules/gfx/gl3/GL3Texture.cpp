#include "GL3Texture.h"

#include "GL3Device.h"

BEGIN_GFX_NAMESPACE

static const GLenum g_type2OGLTextureType[] = {
    GL_TEXTURE_2D,        // TEX2D
    GL_TEXTURE_3D,        // TEX3D
    GL_TEXTURE_CUBE_MAP,  // CUBE
    GL_TEXTURE_2D_ARRAY,  // TEX2D_ARRAY
};

static const GLenum g_interformat[] = {
    GL_RGBA8,               // RGBA8
    GL_RGBA16F,             // RGBA16
    GL_RGBA4,               // RGBA4
    GL_RGB8,                // RGB8
    GL_RGB565,              // RGB565
    GL_DEPTH_COMPONENT32F,  // Depth32
    GL_DEPTH_COMPONENT16,   // Depth16
    GL_DEPTH_COMPONENT24,   // Depth24
    GL_STENCIL_INDEX8,      // Stencil8
    GL_DEPTH32F_STENCIL8,   // Depth32Stencil8
    GL_DEPTH24_STENCIL8,    // Depth24Stencil8
};

static const GLenum g_pixleFormat[] = {
    GL_RGBA,             // RGBA8
    GL_RGBA,             // RGBA16
    GL_RGBA,             // RGBA4
    GL_RGB,              // RGB8
    GL_RGB,              // RGB565
    GL_DEPTH_COMPONENT,  // Depth32
    GL_DEPTH_COMPONENT,  // Depth16
    GL_DEPTH_COMPONENT,  // Depth24
    GL_STENCIL_INDEX,    // Stencil8
    GL_DEPTH_STENCIL,    // Depth32Stencil8
    GL_DEPTH_STENCIL,    // Depth24Stencil8
};

static const GLenum g_dataTypeFormat[] = {
    GL_UNSIGNED_BYTE,                   // RGBA8
    GL_HALF_FLOAT,                      // RGBA16
    GL_UNSIGNED_SHORT_4_4_4_4,          // RGBA4
    GL_UNSIGNED_BYTE,                   // RGB8
    GL_UNSIGNED_SHORT_5_6_5,            // RGB565
    GL_FLOAT,                           // Depth32
    GL_UNSIGNED_SHORT,                  // Depth16
    GL_UNSIGNED_INT,                    // Depth24
    GL_UNSIGNED_BYTE,                   // Stencil8
    GL_FLOAT_32_UNSIGNED_INT_24_8_REV,  // Depth32Stencil8
    GL_UNSIGNED_INT_24_8,               // Depth24Stencil8
};

static const uint32_t g_pixelSize[] = {
    4,  // RGBA8
    8,  // RGBA16
    2,  // RGBA4
    3,  // RGB8
    2,  // RGB565
    4,  // Depth32
    2,  // Depth16
    3,  // Depth24
    1,  // Stencil8
    5,  // Depth32Stencil8
    4,  // Depth24Stencil8
};

void GL3Texture::getPixelFormatInfo(PixelFormat format, GLenum* innerFormat,
                                    GLenum* pixelFomrat, GLenum* dataType,
                                    uint32_t* pixelSize) {
    if (innerFormat != nullptr) *innerFormat = g_interformat[(int)format];
    if (pixelFomrat != nullptr) *pixelFomrat = g_pixleFormat[(int)format];
    if (dataType != nullptr) *dataType = g_dataTypeFormat[(int)format];
    if (pixelSize != nullptr) *pixelSize = g_pixelSize[(int)format];
}

GLenum GL3Texture::getTarget(TextureType type) {
    GLenum target = g_type2OGLTextureType[(int)type];
    return target;
}

GL3Texture::GL3Texture(GL3Device& device, const TextureInfo& info,
                       const void* pData)
    : m_device(device), m_info(info) {
    GLenum target         = g_type2OGLTextureType[(int)info.type];
    GLenum internalFormat = g_interformat[(int)info.format];
    GLenum pixelFomrat    = g_pixleFormat[(int)info.format];
    GLenum dataType       = g_dataTypeFormat[(int)info.format];

    uint32_t pixelSize   = g_pixelSize[int(info.format)];
    uint64_t textureSize = info.width * info.height * pixelSize;

    m_device.callSync([&]() {
        GL_CHECK(glGenTextures(1, &m_handle));
        GL_CHECK(glBindTexture(target, m_handle));
        switch (info.type) {
            case TextureType::TEX2D: {
                GL_CHECK(glTexImage2D(target, 0, internalFormat, info.width,
                                      info.height, 0, pixelFomrat, dataType,
                                      pData));
                break;
            }
            case TextureType::TEX3D: {
                GL_CHECK(glTexImage3D(target, 0, internalFormat, info.width,
                                      info.height, info.depth, 0, pixelFomrat,
                                      dataType, pData));
                break;
            }
            case TextureType::CUBE: {
                for (int i = 0; i < 6; i++) {
                    void* pTexData = nullptr;
                    if (pData) {
                        pTexData = (char*)pData + i * textureSize;
                    }
                    GL_CHECK(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0,
                                          internalFormat, info.width,
                                          info.height, 0, pixelFomrat, dataType,
                                          pTexData));
                }
                break;
            }
            case TextureType::TEX2D_ARRAY: {
                GL_CHECK(glTexImage3D(target, 0, internalFormat, info.width,
                                      info.height, info.arrayCount, 0,
                                      pixelFomrat, dataType, pData));
                break;
            }
        }
        GL_CHECK(glBindTexture(target, 0));
    });
}

GL3Texture::~GL3Texture() {
    if (m_handle) {
        m_device.callSync([&]() { GL_CHECK(glDeleteTextures(1, &m_handle)); });
        m_handle = OGL_NULL_HANDLE;
    }
}

END_GFX_NAMESPACE