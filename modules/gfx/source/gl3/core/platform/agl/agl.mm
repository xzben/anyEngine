#include "agl.h"

#import <Cocoa/Cocoa.h>
#import <OpenGL/gl3.h>

BEGIN_GFX_NAMESPACE
BEGIN_GL3_CORE_NAMESPACE

AGLSwapChain::AGLSwapChain(GL3Device& device, const SurfaceInfo& info, bool needDepthStencil,
                 AGLContext* shareContext)
:GL3SwapChain(device, info, needDepthStencil),
m_context(shareContext)
{
    glGenFramebuffers(1, &m_fbo);
    glGenRenderbuffers(1, &m_colorTexture);

    handleUpdateSurfaceInfo(info);
}

AGLSwapChain::~AGLSwapChain()
{
    glDeleteBuffers(1, &m_fbo);
    m_fbo = 0;
    glDeleteBuffers(1, &m_colorTexture);
    m_colorTexture = 0;
}

void AGLSwapChain::handleUpdateSurfaceInfo(const SurfaceInfo& info)
{
    m_info = info;

    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
    glBindRenderbuffer(GL_RENDERBUFFER, m_colorTexture);
    [((__bridge EAGLContext*)m_context->handle) renderbufferStorage:GL_RENDERBUFFER fromDrawable:(CAEAGLLayer*)info.handle];
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, m_colorTexture);
    GLenum error = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    updateAttachment(info.width, info.height);
}

void AGLSwapChain::swapBuffer()
{
    AGLHelper::makeCurrent(m_context);
    glBindRenderbuffer(GL_RENDERBUFFER, m_colorTexture);
    [((__bridge EAGLContext*)m_context->handle)  presentRenderbuffer:GL_RENDERBUFFER];
    AGLHelper::exitCurrent(m_context);
}

static std::mutex g_context_lock;

bool AGLHelper::init()
{
    return true;
}

AGLContext* AGLHelper::createContext(AGLContext* share, bool singleBuffer)
{
    void* handle{nullptr};
    void* shareHandle{nullptr};

    if(share != nullptr){
        shareHandle = share->handle;
    }
    AGLContext* newCtx = new AGLContext;
    newCtx->share = share;

#if CUR_PLATFORM == PLATFORM_MAC
    // 设置 OpenGL 属性
    NSOpenGLPixelFormatAttribute attrs[] = {
        NSOpenGLPFAOpenGLProfile, NSOpenGLProfileVersion4_1Core,
        NSOpenGLPFAColorSize, 24,
        NSOpenGLPFAAlphaSize, 8,
        NSOpenGLPFADepthSize, 24,
        NSOpenGLPFAStencilSize, 8,
        NSOpenGLPFADoubleBuffer,
        NSOpenGLPFAAccelerated,
        0
    };

    NSOpenGLContext* shareContext =  shareHandle == nullptr ? nil : (__bridge NSOpenGLContext*)(shareHandle);
    NSOpenGLPixelFormat *pixelFormat = [[NSOpenGLPixelFormat alloc] initWithAttributes:attrs];
    newCtx->handle = (__bridge_retained void*)([[NSOpenGLContext alloc] initWithFormat:pixelFormat shareContext:shareContext]);
    
#elif CUR_PLATFORM == PLATFORM_IOS
    EAGLContext* shareContext = shareHandle == nullptr ? nil : (__bridge EAGLContext*)(shareHandle);
    newCtx->handle = (__bridge_retained void*)([[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES3 sharegroup: shareContext]);
#endif
    return newCtx;
}

void AGLHelper::deleteContext(AGLContext* context){
    if(context == nullptr) return;

#if CUR_PLATFORM == PLATFORM_MAC
    CFBridgingRelease(context->handle);
#elif CUR_PLATFORM == PLATFORM_IOS
    CFBridgingRelease(context->handle);
#endif
    delete context;
}

AGLSwapChain* AGLHelper::createWindowSurface(GL3Device& device, const SurfaceInfo& info,
                                        bool needDepthStencil, AGLContext* mainContext){
    
    return new AGLSwapChain(device, info, needDepthStencil, mainContext);
}

thread_local WGlContext* s_lastContext = nullptr;
thread_local WGlContext* s_preContext  = nullptr;

void AGLHelper::makeCurrent(AGLContext* context)
{
    if(s_lastContext == context) return;
    s_lastContext = context;

    g_context_lock.lock();
#if CUR_PLATFORM == PLATFORM_MAC
    if(context){
        [(__bridge NSOpenGLContext*)(context->handle) makeCurrentContext];
    }else{

    }
#elif CUR_PLATFORM == PLATFORM_IOS
    if(context){
        [EAGLContext setCurrentContext:nil];
    }else{
        [EAGLContext setCurrentContext:(__bridge EAGLContext*)(context->handle)];
    }
#endif
    g_context_lock.unlock();
}

void AGLHelper::deleteWindowSurface(AGLSwapChain* surface)
{
    delete surface;
}

AGLContext* AGLHelper::getLastContext()
{
    return s_lastContext;
}

void AGLHelper::exitCurrent(AGLContext* context)
{
    if(s_lastContext != context)return;

#if CUR_PLATFORM == PLATFORM_MAC
    [(__bridge NSOpenGLContext*)(context->handle) clearCurrentContext];
    s_lastContext = nullptr;
#elif CUR_PLATFORM == PLATFORM_IOS
    [EAGLContext setCurrentContext:nil];
#endif
}

END_GL3_CORE_NAMESPACE
END_GFX_NAMESPACE
