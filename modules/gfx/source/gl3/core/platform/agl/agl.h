#pragma once

#include "../../glconfig.h"

#if CUR_GL_TYPE == OPENGL_AGL
#include "GL/glew.h"
#include "gl3/GL3SwapChain.h"

BEGIN_GFX_NAMESPACE
BEGIN_GL3_CORE_NAMESPACE

struct AGLContext {
    AGLContext* share{nullptr};
    void* handle{nullptr};
};

class AGLSwapChain : public GL3SwapChain {
public:
    // 基于一个mainContext 创建
    AGLSwapChain(GL3Device& device, const SurfaceInfo& info, bool needDepthStencil,
                 AGLContext* shareContext);
    virtual ~AGLSwapChain();
    virtual void handleUpdateSurfaceInfo(const SurfaceInfo& info) override;
    void swapBuffer() override;

    AGLContext* getContext(){ return m_context;}
private:
    uint32_t m_fbo;
    uint32_t m_colorTexture;

    AGLContext* m_context;
};

class AGLHelper {
public:
    static bool init();
    static AGLContext* createContext(AGLContext* share, bool singleBuffer);
    static void deleteContext(AGLContext* context);
    static AGLSwapChain* createWindowSurface(GL3Device& device, const SurfaceInfo& info,
                                             bool needDepthStencil, AGLContext* mainContext);

    static void makeCurrent(AGLContext* context);
    static void deleteWindowSurface(AGLSwapChain* surface);
    static AGLContext* getLastContext();
    static void exitCurrent(AGLContext* context);
};

using GLContextType = AGLContext*;
END_GL3_CORE_NAMESPACE
END_GFX_NAMESPACE

#endif  // CUR_GL_TYPE == OPENGL_AGL
