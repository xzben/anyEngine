#include "GLContext.h"

#include <mutex>

#include "../GL3Device.h"

BEGIN_GFX_NAMESPACE
BEGIN_GL3_CORE_NAMESPACE

std::mutex g_initLock;

void GLContext::init() {
#if CUR_GL_TYPE == OPENGL_WGL
    WGLHelper::init();
#elif CUR_GL_TYPE == OPENGL_AGL

#elif CUR_GL_TYPE == OPENGL_EGL

#else
    static_assert(false);  // unsupport OPEN TYPE
#endif
}
GLContext::GLContext(GL3Device& device) : m_device(device) {
    static bool s_inited = false;
    /*if (!s_inited) {
        g_initLock.lock();
        if (!s_inited) {
            init();
            s_inited = true;
        }
        g_initLock.unlock();
    }*/
    m_context = createContext(nullptr);
}

GLContextType GLContext::createContext(GLContextType share) {
#if CUR_GL_TYPE == OPENGL_WGL
    return WGLHelper::createContext(NULL, share, true);
#elif CUR_GL_TYPE == OPENGL_AGL

#elif CUR_GL_TYPE == OPENGL_EGL

#else
    static_assert(false);  // unsupport OPEN TYPE
#endif
}
GLContext::~GLContext() {
#if CUR_GL_TYPE == OPENGL_WGL
    WGLHelper::deleteContext(m_context);
#elif CUR_GL_TYPE == OPENGL_AGL

#elif CUR_GL_TYPE == OPENGL_EGL

#else
    static_assert(false);  // unsupport OPEN TYPE
#endif
}

GL3SwapChain* GLContext::createSwapChain(void* window, uint32_t width, uint32_t height, bool singleBuffer, bool needDepthStencil) {
#if CUR_GL_TYPE == OPENGL_WGL
    return WGLHelper::createWindowSurface(m_device, (HWND)window, width, height, singleBuffer, needDepthStencil, m_context);
#elif CUR_GL_TYPE == OPENGL_AGL

#elif CUR_GL_TYPE == OPENGL_EGL

#else
    static_assert(false);  // unsupport OPEN TYPE
    return nullptr;
#endif
}
void GLContext::destroySwapChain(GL3SwapChain* swapChain) {
#if CUR_GL_TYPE == OPENGL_WGL
    WGLHelper::deleteWindowSurface(dynamic_cast<WGLSwapChain*>(swapChain));
#elif CUR_GL_TYPE == OPENGL_AGL

#elif CUR_GL_TYPE == OPENGL_EGL

#else
    static_assert(false);  // unsupport OPEN TYPE
#endif
}
void GLContext::swapBuffer(GL3SwapChain* swapChain) {
#if CUR_GL_TYPE == OPENGL_WGL
    dynamic_cast<WGLSwapChain*>(swapChain)->swapBuffer();
#elif CUR_GL_TYPE == OPENGL_AGL

#elif CUR_GL_TYPE == OPENGL_EGL

#else
    static_assert(false);  // unsupport OPEN TYPE
#endif
}

void GLContext::makeCurrent(GL3SwapChain* swapChain) {
#if CUR_GL_TYPE == OPENGL_WGL
    if (swapChain == nullptr) {
        WGLHelper::makeCurrent(m_context);
    } else {
        WGLHelper::makeCurrent(dynamic_cast<WGLSwapChain*>(swapChain)->getContext());
    }
#elif CUR_GL_TYPE == OPENGL_AGL

#elif CUR_GL_TYPE == OPENGL_EGL

#else
    static_assert(false);  // unsupport OPEN TYPE
#endif
}

void GLContext::exitCurrent(GL3SwapChain* swapChain) {
#if CUR_GL_TYPE == OPENGL_WGL
    if (swapChain == nullptr) {
        WGLHelper::exitCurrent(m_context);
    } else {
        WGLHelper::exitCurrent(dynamic_cast<WGLSwapChain*>(swapChain)->getContext());
    }
#elif CUR_GL_TYPE == OPENGL_AGL

#elif CUR_GL_TYPE == OPENGL_EGL

#else
    static_assert(false);  // unsupport OPEN TYPE
#endif
}

GLContext* GLContext::createSubContext() {
    auto context = createContext(m_context);
    return new GLContext(m_device, context);
}

static const uint32_t defaultFboCount = 10;
static const uint32_t defaultVaoCount = 10;

void GLContext::initContextRes() {
    preAllocCacheObject(
        CacheOGLType::FBO, defaultFboCount, [](uint32_t count, OGL_HANDLE* objs) { GL_CHECK(glGenFramebuffers(count, objs)); },
        [](uint32_t count, OGL_HANDLE* objs) { GL_CHECK(glDeleteFramebuffers(count, objs)); });

    preAllocCacheObject(
        CacheOGLType::VAO, defaultVaoCount, [](uint32_t count, OGL_HANDLE* objs) { GL_CHECK(glGenVertexArrays(count, objs)); },
        [](uint32_t count, OGL_HANDLE* objs) { GL_CHECK(glDeleteVertexArrays(count, objs)); });
}

void GLContext::clearContextRes() {
    for (auto it = m_cacheObjects.begin(); it != m_cacheObjects.end(); it++) {
        auto& object = it->second;
        if (object.objs.size() > 0) {
            object.freeFunc(object.objs.size(), object.objs.data());
        }
    }
}

void GLContext::allocFbo(uint32_t count, OGL_HANDLE* fbos) { allocCacheObj(CacheOGLType::FBO, count, fbos); }

void GLContext::freeFbo(uint32_t count, OGL_HANDLE* fbos) { freeCacheObj(CacheOGLType::FBO, count, fbos); }

void GLContext::allocVao(uint32_t count, OGL_HANDLE* vaos) { allocCacheObj(CacheOGLType::VAO, count, vaos); }

void GLContext::freeVao(uint32_t count, OGL_HANDLE* vaos) { freeCacheObj(CacheOGLType::VAO, count, vaos); }

void GLContext::freeCacheObj(CacheOGLType type, uint32_t count, OGL_HANDLE* objs) {
    auto& vec        = getCacheObject(type).objs;
    uint32_t oldSize = vec.size();
    vec.resize(oldSize + count);
    for (uint32_t i = 0; i < count; i++) {
        vec[oldSize + i] = objs[i];
    }
}

GLContext::CacheObject& GLContext::getCacheObject(CacheOGLType type) {
    auto it = m_cacheObjects.find(type);
    assert(it != m_cacheObjects.end());
    return it->second;
}

void GLContext::preAllocCacheObject(CacheOGLType type, uint32_t defaultCount, ALLOC_FUNC allocFunc, FREE_FUNC freeFunc) {
    assert(m_cacheObjects.find(type) == m_cacheObjects.end());

    std::vector<OGL_HANDLE> defaultObjs;
    defaultObjs.resize(defaultCount);
    allocFunc(defaultCount, defaultObjs.data());

    m_cacheObjects[type] = {
        defaultObjs,
        allocFunc,
        freeFunc,
    };
}
void GLContext::allocCacheObj(CacheOGLType type, uint32_t count, OGL_HANDLE* objs) {
    auto obj = getCacheObject(type);

    uint32_t oldSize    = obj.objs.size();
    uint32_t createSize = count > oldSize ? count - oldSize : 0;

    int setIndex = 0;
    if (oldSize > 0) {
        int readIndex = oldSize - 1;
        int readSize  = 0;
        for (int i = oldSize - 1; i >= 0; i--) {
            objs[setIndex++] = obj.objs[i];
            readSize++;
        }
        obj.objs.resize(oldSize - readSize);
    }

    if (createSize > 0) {
        obj.allocFunc(count, &objs[setIndex]);
    }
}

void GLContext::updateState(const PipelineState& newPipelineState) {
    setRasterizationState(newPipelineState.rasterize);
    setColorBlendState(newPipelineState.blend);
    setDepthStencilState(newPipelineState.depthStencil);
}

void GLContext::setEnable(GLenum statue, bool enable) {
    auto it = m_statusEnables.find(statue);

    if (it != m_statusEnables.end() && it->second == enable) return;

    if (enable) {
        GL_CHECK(glEnable(statue));
    } else {
        GL_CHECK(glDisable(statue));
    }

    m_statusEnables[statue] = enable;
}

static const GLenum g_glPologonModels[] = {
    GL_FILL,   // PolygonModel::FILL
    GL_LINE,   // PolygonModel::LINE
    GL_POINT,  // PolygonModel::POINT
};

static const GLenum g_cullModels[] = {
    GL_FRONT,           // CullModel::FRONT
    GL_BACK,            // CullModel::BACK
    GL_FRONT_AND_BACK,  // CullModel::FRONT_AND_BACK
};

static const GLenum g_faceFlag[] = {
    GL_CCW,  // FrontFace::COUNTER_CLOCKWISE 逆时针
    GL_CW,   // FrontFace::CLOCKWISE 顺时针
};

void GLContext::setRasterizationState(const RasterizationState& state) {
    auto& rasterizationState = m_pipelineState.rasterize;

    if (rasterizationState.polygonMode != state.polygonMode) {
        GLenum mode = g_glPologonModels[(int)state.polygonMode];
        GL_CHECK(glPolygonMode(GL_FRONT_AND_BACK, mode));
        rasterizationState.polygonMode = state.polygonMode;
    }

    if (rasterizationState.lineWidth != state.lineWidth) {
        GL_CHECK(glLineWidth(state.lineWidth));
        rasterizationState.lineWidth = state.lineWidth;
    }

    if (rasterizationState.cullMode != state.cullMode) {
        bool enable = state.cullMode != CullModel::NONE;
        setEnable(GL_CULL_FACE, enable);
        if (enable) {
            GLenum mode = g_cullModels[(int)state.cullMode];
            GL_CHECK(glCullFace(mode));
        }
        rasterizationState.cullMode = state.cullMode;
    }

    if (rasterizationState.frontFace != state.frontFace) {
        GLenum face = g_faceFlag[int(state.frontFace)];
        GL_CHECK(glFrontFace(face));
        rasterizationState.frontFace = state.frontFace;
    }
}

static const GLenum g_blendFuncs[] = {
    GL_FUNC_ADD,               // BlendOp::ADD
    GL_FUNC_SUBTRACT,          // BlendOp::SUBTRACT
    GL_FUNC_REVERSE_SUBTRACT,  // BlendOp::REVERSE_SUBTRACT
    GL_MIN,                    // BlendOp::MIN
    GL_MAX,                    // BlendOp::MAX
};

static const GLenum g_blendFactors[] = {
    GL_ZERO,                      // BlendFactor::ZERO
    GL_ONE,                       // BlendFactor::ONE
    GL_SRC_COLOR,                 // BlendFactor::SRC_COLOR
    GL_ONE_MINUS_SRC_COLOR,       // BlendFactor::ONE_MINUS_SRC_COLOR
    GL_DST_COLOR,                 // BlendFactor::DST_COLOR
    GL_ONE_MINUS_DST_COLOR,       // BlendFactor::ONE_MINUS_DST_COLOR
    GL_SRC_ALPHA,                 // BlendFactor::SRC_ALPHA
    GL_ONE_MINUS_SRC_ALPHA,       // BlendFactor::ONE_MINUS_SRC_ALPHA
    GL_DST_ALPHA,                 // BlendFactor::DST_ALPHA
    GL_ONE_MINUS_DST_ALPHA,       // BlendFactor::ONE_MINUS_DST_ALPHA
    GL_CONSTANT_COLOR,            // BlendFactor::CONSTANT_COLOR
    GL_ONE_MINUS_CONSTANT_COLOR,  // BlendFactor::ONE_MINUS_CONSTANT_COLOR
    GL_CONSTANT_ALPHA,            // BlendFactor::CONSTANT_ALPHA
    GL_ONE_MINUS_CONSTANT_ALPHA,  // BlendFactor::ONE_MINUS_CONSTANT_ALPHA
    GL_SRC_ALPHA_SATURATE,        // BlendFactor::SRC_ALPHA_SATURATE
};

void GLContext::setColorBlendState(const ColorBlendState& state) {
    auto& blend = m_pipelineState.blend;
    setEnable(GL_BLEND, state.blendEnable);
    if (!state.blendEnable) return;

    if (blend.constansts[0] != state.constansts[0] || blend.constansts[1] != state.constansts[1] || blend.constansts[2] != state.constansts[2]
        || blend.constansts[3] != state.constansts[3]) {
        GL_CHECK(glBlendColor(state.constansts[0], state.constansts[1], state.constansts[2], state.constansts[3]));

        blend.constansts[0] = state.constansts[0];
        blend.constansts[1] = state.constansts[1];
        blend.constansts[2] = state.constansts[2];
        blend.constansts[3] = state.constansts[3];
    }

    if (blend.colorBlendOp != state.colorBlendOp || blend.alphaBlendOp != state.alphaBlendOp) {
        GLenum colorFunc = g_blendFuncs[int(state.colorBlendOp)];
        GLenum alphaFunc = g_blendFuncs[int(state.colorBlendOp)];
        GL_CHECK(glBlendEquationSeparate(colorFunc, alphaFunc));

        blend.colorBlendOp = state.colorBlendOp;
        blend.alphaBlendOp = state.alphaBlendOp;
    }

    if (blend.srcColorFactor != state.srcColorFactor || blend.dstColorFactor != state.dstColorFactor || blend.srcAlphaFactor != state.srcAlphaFactor
        || blend.dstAlphaFactor != state.dstAlphaFactor) {
        GL_CHECK(glBlendFuncSeparate(g_blendFactors[(int)state.srcColorFactor], g_blendFactors[(int)state.dstColorFactor],
                                     g_blendFactors[(int)state.srcAlphaFactor], g_blendFactors[(int)state.dstAlphaFactor]));

        blend.srcColorFactor = state.srcColorFactor;
        blend.dstColorFactor = state.dstColorFactor;
        blend.srcAlphaFactor = state.srcAlphaFactor;
        blend.dstAlphaFactor = state.dstAlphaFactor;
    }
}

static const GLenum g_CompareFuncs[] = {
    GL_NEVER,     // CompareOp::NEVER
    GL_LESS,      // CompareOp::LESS
    GL_EQUAL,     // CompareOp::EQUAL
    GL_LEQUAL,    // CompareOp::LESS_OR_EQUAL
    GL_GREATER,   // CompareOp::GREATER
    GL_NOTEQUAL,  // CompareOp::NOT_EQUAL
    GL_GEQUAL,    // CompareOp::GREATER_OR_EQUAL
    GL_ALWAYS,    // CompareOp::ALWAYS
};

static const GLenum g_stencilOps[] = {
    GL_KEEP,
    GL_ZERO,       //
    GL_REPLACE,    //
    GL_INCR,       //
    GL_DECR,       //
    GL_INVERT,     //
    GL_INCR_WRAP,  //
    GL_DECR_WRAP,  //
};

void GLContext::setDepthStencilState(const DepthStencilState& state) {
    auto& depthStencil = m_pipelineState.depthStencil;

    bool depthTestEnable = state.depthTestEnable;

    if (state.depthCompareOp == CompareOp::EMPTY) {
        CCASSERT(depthTestEnable == false, "depth test enable compare func must not been empty");
        depthTestEnable = false;
    }
    setEnable(GL_DEPTH_TEST, depthTestEnable);
    if (depthTestEnable) {
        if (depthStencil.depthCompareOp != state.depthCompareOp) {
            GL_CHECK(glDepthFunc(g_CompareFuncs[(int)state.depthCompareOp]));
            depthStencil.depthCompareOp = state.depthCompareOp;
        }
        if (depthStencil.depthWriteEnable != state.depthWriteEnable) {
            GL_CHECK(glDepthMask(state.depthWriteEnable ? GL_TRUE : GL_FALSE));
            depthStencil.depthWriteEnable = state.depthWriteEnable;
        }
    }

    bool stencilTestEnable = state.stencilTestEnable;
    setEnable(GL_STENCIL_TEST, stencilTestEnable);

    if (stencilTestEnable) {
        // front
        {
            const auto& stencilState = state.front;
            auto& cacheState         = depthStencil.front;

            if (stencilState.writeMask != cacheState.writeMask) {
                GL_CHECK(glStencilMaskSeparate(GL_FRONT, stencilState.writeMask));
                cacheState.writeMask = stencilState.writeMask;
            }

            if (stencilState.compareOp != cacheState.compareOp || stencilState.reference != cacheState.reference
                || stencilState.compareMask != cacheState.compareMask) {
                GL_CHECK(
                    glStencilFuncSeparate(GL_FRONT, g_CompareFuncs[(int)stencilState.compareOp], stencilState.reference, stencilState.compareMask));
                cacheState.compareOp   = stencilState.compareOp;
                cacheState.reference   = stencilState.reference;
                cacheState.compareMask = stencilState.compareMask;
            }

            if (stencilState.failOp != cacheState.failOp || stencilState.depthFailOP != cacheState.depthFailOP
                || stencilState.passOp != cacheState.passOp) {
                GL_CHECK(glStencilOpSeparate(GL_FRONT, g_stencilOps[(int)stencilState.failOp], g_stencilOps[(int)stencilState.depthFailOP],
                                             g_stencilOps[(int)stencilState.passOp]));
                cacheState.failOp      = stencilState.failOp;
                cacheState.depthFailOP = stencilState.depthFailOP;
                cacheState.passOp      = stencilState.passOp;
            }
        }

        // back
        {
            const auto& stencilState = state.back;
            auto& cacheState         = depthStencil.back;

            if (stencilState.writeMask != cacheState.writeMask) {
                GL_CHECK(glStencilMaskSeparate(GL_BACK, stencilState.writeMask));
                cacheState.writeMask = stencilState.writeMask;
            }

            if (stencilState.compareOp != cacheState.compareOp || stencilState.reference != cacheState.reference
                || stencilState.compareMask != cacheState.compareMask) {
                GL_CHECK(
                    glStencilFuncSeparate(GL_BACK, g_CompareFuncs[(int)stencilState.compareOp], stencilState.reference, stencilState.compareMask));
                cacheState.compareOp   = stencilState.compareOp;
                cacheState.reference   = stencilState.reference;
                cacheState.compareMask = stencilState.compareMask;
            }

            if (stencilState.failOp != cacheState.failOp || stencilState.depthFailOP != cacheState.depthFailOP
                || stencilState.passOp != cacheState.passOp) {
                GL_CHECK(glStencilOpSeparate(GL_BACK, g_stencilOps[(int)stencilState.failOp], g_stencilOps[(int)stencilState.depthFailOP],
                                             g_stencilOps[(int)stencilState.passOp]));
                cacheState.failOp      = stencilState.failOp;
                cacheState.depthFailOP = stencilState.depthFailOP;
                cacheState.passOp      = stencilState.passOp;
            }
        }
    }
}

END_GL3_CORE_NAMESPACE
END_GFX_NAMESPACE