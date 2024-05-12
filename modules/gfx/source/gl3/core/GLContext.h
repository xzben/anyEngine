#pragma once

#include <array>
#include <cassert>
#include <type_traits>

#include "NoCopy.h"
#include "gl3_core_common.h"
#include "glselect.h"

BEGIN_GFX_NAMESPACE
class GL3SwapChain;
class GL3Device;

BEGIN_GL3_CORE_NAMESPACE

class GLContext : public NoCopy {
private:
    enum class CacheOGLType {
        FBO = 0,
        VAO,
    };

public:
    GLContext(GL3Device& device);
    virtual ~GLContext();

    void initContextRes();
    void clearContextRes();

    void makeCurrent(GL3SwapChain* swapChain = nullptr);
    void exitCurrent(GL3SwapChain* swapChain = nullptr);
    GL3SwapChain* createSwapChain(void* winow, uint32_t width, uint32_t height,
                                  bool singleBuffer, bool needDepthStencil);
    void destroySwapChain(GL3SwapChain* swapChain);
    void swapBuffer(GL3SwapChain* swapChain);

    GLContext* createSubContext();

    void allocFbo(uint32_t count, OGL_HANDLE* fbos);
    void freeFbo(uint32_t count, OGL_HANDLE* fbos);

    void allocVao(uint32_t count, OGL_HANDLE* vaos);
    void freeVao(uint32_t count, OGL_HANDLE* vaos);

    void updateState(const PipelineState& state);
    void setEnable(GLenum statue, bool enable);
    
protected:
    void setRasterizationState(const RasterizationState& state);
    void setColorBlendState(const ColorBlendState& state);
    void setDepthStencilState(const DepthStencilState& state);

protected:
    GLContextType createContext(GLContextType share);
    GLContext(GL3Device& device, GLContextType context)
        : m_device(device), m_context(context) {}

    using ALLOC_FUNC = std::function<void(uint32_t count, OGL_HANDLE* objs)>;
    using FREE_FUNC  = std::function<void(uint32_t count, OGL_HANDLE* objs)>;
    struct CacheObject {
        std::vector<OGL_HANDLE> objs;
        ALLOC_FUNC allocFunc;
        FREE_FUNC freeFunc;
    };
    void freeCacheObj(CacheOGLType type, uint32_t count, OGL_HANDLE* objs);
    void allocCacheObj(CacheOGLType type, uint32_t count, OGL_HANDLE* objs);
    CacheObject& getCacheObject(CacheOGLType type);

    void preAllocCacheObject(CacheOGLType type, uint32_t defaultCount,
                             ALLOC_FUNC allocFunc, FREE_FUNC freeFunc);

private:
    std::unordered_map<CacheOGLType, CacheObject> m_cacheObjects;
    GLContextType m_context;
    GL3Device& m_device;
    PipelineState m_pipelineState;
    std::unordered_map<GLenum, bool> m_statusEnables;
};

END_GL3_CORE_NAMESPACE
END_GFX_NAMESPACE