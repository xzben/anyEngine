#include "GL3Sampler.h"

#include "GL3Device.h"

BEGIN_GFX_NAMESPACE

static const GLenum g_filterType[] = {
    GL_NEAREST,                 // Filter::NEAREST
    GL_LINEAR,                  // Filter::LINEAR
    GL_NEAREST_MIPMAP_NEAREST,  // Filter::NEAREST_MIPMAP_NEAREST
    GL_LINEAR_MIPMAP_NEAREST,   // Filter::LINEAR_MIPMAP_NEAREST
    GL_NEAREST_MIPMAP_LINEAR,   // Filter::NEAREST_MIPMAP_LINEAR
    GL_LINEAR_MIPMAP_LINEAR,    // Filter::LINEAR_MIPMAP_LINEAR
};

static const GLenum g_addressType[] = {
    GL_REPEAT,           // Address::WRAP
    GL_MIRRORED_REPEAT,  // Address::MIRROR
    GL_CLAMP_TO_EDGE,    // Address::CLAMP
    GL_CLAMP_TO_BORDER,  // Address::BORDER
};

static const GLenum g_textureCmpFunc[] = {
    GL_NEVER,     // CompareOp::NEVER
    GL_LESS,      // CompareOp::LESS
    GL_EQUAL,     // CompareOp::EQUAL
    GL_LEQUAL,    // CompareOp::LESS_OR_EQUAL
    GL_GREATER,   // CompareOp::GREATER
    GL_NOTEQUAL,  // CompareOp::NOT_EQUAL
    GL_GEQUAL,    // CompareOp::GREATER_OR_EQUAL
    GL_ALWAYS,    // CompareOp::ALWAYS
};

static const float g_borderColors[][4] = {
    {0.f, 0.f, 0.f, 0.f},  // SamplerBorderColor::TRANSPARENT_BLACK
    {0.f, 0.f, 0.f, 1.f},  // SamplerBorderColor::OPAQUA_BLACK
    {1.f, 1.f, 1.f, 1.f},  // SamplerBorderColor::OPQUA_WHITE
};

GL3Sampler::GL3Sampler(GL3Device& device, const SamplerInfo& info)
    : m_device(device), m_info(info) {
    m_device.callSync([&](gl3::GLContext* ctx) {
        GL_CHECK(glGenSamplers(1, &m_handle));
        GL_CHECK(glSamplerParameteri(m_handle, GL_TEXTURE_WRAP_S,
                                     g_addressType[(int)info.addressU]));
        GL_CHECK(glSamplerParameteri(m_handle, GL_TEXTURE_WRAP_T,
                                     g_addressType[(int)info.addressV]));
        GL_CHECK(glSamplerParameteri(m_handle, GL_TEXTURE_WRAP_R,
                                     g_addressType[(int)info.addressW]));

        GLint minFilter = g_addressType[(int)info.minFilter]
                          | g_addressType[(int)info.mipFilter];

        GL_CHECK(
            glSamplerParameteri(m_handle, GL_TEXTURE_MIN_FILTER, minFilter));

        GL_CHECK(glSamplerParameteri(m_handle, GL_TEXTURE_MAG_FILTER,
                                     g_filterType[(int)info.magFilter]));

        if (info.cmpFunc != CompareOp::EMPTY) {
            GL_CHECK(glSamplerParameteri(m_handle, GL_TEXTURE_COMPARE_MODE,
                                         GL_COMPARE_REF_TO_TEXTURE));
            GL_CHECK(glSamplerParameteri(m_handle, GL_TEXTURE_COMPARE_FUNC,
                                         g_filterType[(int)info.cmpFunc]));
        }

        const float* borderColor = g_borderColors[(int)info.bordercolor];
        GL_CHECK(glSamplerParameterfv(m_handle, GL_TEXTURE_BORDER_COLOR,
                                      borderColor));
    });
}

GL3Sampler::~GL3Sampler() {
    if (m_handle) {
        m_device.callSync([&](gl3::GLContext* ctx) {
            GL_CHECK(glDeleteSamplers(1, &m_handle));
        });
        m_handle = OGL_NULL_HANDLE;
    }
}

END_GFX_NAMESPACE