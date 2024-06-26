#include "GL3Device.h"

static const uint32_t g_RenderThreadNum = 2;

BEGIN_GFX_NAMESPACE
GL3Device::GL3Device() {}
GL3Device::~GL3Device() {}

void GL3Device::exit() {
    m_exit = true;
    if (m_pRenderThreads) {
        m_pRenderThreads->exit();
        delete m_pRenderThreads;
    }

    if (m_innerQueue) {
        delete m_innerQueue;
        m_innerQueue = nullptr;
    }

    if (m_innerCommandPool) {
        delete m_innerCommandPool;
        m_innerCommandPool = nullptr;
    }

    for (auto& queue : m_queues) {
        delete queue;
    }
    m_queues.clear();
    m_queueMapInfo.clear();

    for (auto context : m_subContext) {
        delete context;
    }
    m_subContext.clear();
    m_pMainContext->exitCurrent();
    m_pMainContext.reset(nullptr);
}

bool GL3Device::init(const DeviceInfo& info) {
    m_pMainContext = std::make_unique<gl3::GLContext>(*this);
    m_pMainContext->makeCurrent(nullptr);
    initSubRenderThreads(g_RenderThreadNum);

    int count = info.queues.size();
    m_queues.resize(count);
    for (int i = 0; i < count; i++) {
        auto& item  = info.queues[i];
        auto queue  = new GL3Queue(*this, item.type, item.priority);
        m_queues[i] = queue;

        auto it = m_queueMapInfo.find(item.type);
        if (it == m_queueMapInfo.end()) {
            m_queueMapInfo[item.type] = {queue};
        } else {
            it->second.push_back(queue);
        }
    }

    m_innerQueue       = new GL3Queue(*this, QueueType::Transfer, 1.0);
    m_innerCommandPool = new GL3CommandPool(*this, *m_innerQueue, ResetMode::AlwaysAllocate);

    return true;
}

void GL3Device::initSubRenderThreads(uint32_t threadNum) {
    m_subContext.resize(threadNum);
    for (uint32_t i = 0; i < threadNum; i++) {
        m_subContext[i] = m_pMainContext->createSubContext();
    }
    m_pRenderThreads = new RenderThreadPool(
        threadNum,
        [&](WorkTask* task, uint32_t threadIndex) {
            task->execute(m_subContext[threadIndex]);
            task->finish();
        },
        [&](uint32_t threadIndex) {
            m_subContext[threadIndex]->makeCurrent();
            m_subContext[threadIndex]->initContextRes();
        },
        [&](uint32_t threadIndex) {
            m_subContext[threadIndex]->clearContextRes();
            m_subContext[threadIndex]->exitCurrent();
        });
}

GL3Shader* GL3Device::createShader(ShaderModuleInfo* info, uint32_t count) {
    return new GL3Shader(*this, info, count);
}

GL3Queue* GL3Device::getQueue(QueueType type, uint32_t index) {
    auto it = m_queueMapInfo.find(type);
    if (it == m_queueMapInfo.end()) {
        return nullptr;
    }

    uint32_t count = it->second.size();
    if (index >= count) return nullptr;

    return it->second[index];
}

GL3RenderPass* GL3Device::createRenderPass(const std::vector<Attachment>& attachments,
                                           const std::vector<SubPass>& subpass,
                                           const std::vector<SubPassDependency>& dependencies) {
    return new GL3RenderPass(*this, attachments, subpass, dependencies);
}

GL3Pipeline* GL3Device::createPipeline(RenderPass* renderPass, uint32_t subpass, Shader* shader,
                                       const PipelineState& state) {
    return new GL3Pipeline(*this, renderPass, subpass, shader, state);
}

GL3Texture* GL3Device::createTexture(const TextureInfo& info, const void* pData) {
    return new GL3Texture(*this, info, pData);
}

GL3Buffer* GL3Device::createBuffer(BufferType type, uint32_t size, const void* pData) {
    return new GL3Buffer(*this, type, size, pData);
}
GL3Sampler* GL3Device::createSampler(const SamplerInfo& info) {
    return new GL3Sampler(*this, info);
}

GL3InputAssembler* GL3Device::createInputAssembler(PrimitiveType primitiveType,
                                                   const std::vector<Attribute>& attributes,
                                                   const void* pVertexData, uint32_t vertexCount,
                                                   const void* pIndexData, uint32_t indexCount,
                                                   uint32_t indexItemSize) {
    return new GL3InputAssembler(*this, primitiveType, attributes, pVertexData, vertexCount,
                                 pIndexData, indexCount, indexItemSize);
}

GL3InputAssembler* GL3Device::createInputAssembler(PrimitiveType primitiveType,
                                                   const std::vector<Attribute>& attributes,
                                                   const std::vector<Attribute>& InstanceAttributes,
                                                   const void* pVertexData, uint32_t vertexCount,
                                                   const void* pInstanceData,
                                                   uint32_t instanceCount, const void* pIndexData,
                                                   uint32_t indexCount, uint32_t indexItemSize) {
    return new GL3InputAssembler(*this, primitiveType, attributes, InstanceAttributes, pVertexData,
                                 vertexCount, pInstanceData, instanceCount, pIndexData, indexCount,
                                 indexItemSize);
}

GL3SwapChain* GL3Device::createSwapChain(const SurfaceInfo& info, bool needDepthStencil) {
    return m_pMainContext->createSwapChain(info, needDepthStencil);
}
GL3Fence* GL3Device::createFence(bool signaled) { return new GL3Fence(*this, signaled); }

GL3Semaphore* GL3Device::createSemaphore() { return new GL3Semaphore(*this); }

GL3Event* GL3Device::createEvent() { return new GL3Event(*this); }

void GL3Device::waitIdle() {
    m_innerQueue->waitIdle();

    for (auto& queue : m_queues) {
        queue->waitIdle();
    }
}

void GL3Device::withOneTimeCmd(std::function<void(CommandBuffer& cmd)> callback) {
    auto* cmd = m_innerCommandPool->alloc(CommandBufferLevel::PRIMARY);
    callback(*cmd);

    gl3::CustomWorkTask task(m_innerQueue, [&](gl3::GLContext* ctx) { cmd->execute(ctx); });
    addTask(&task);
    task.waitFinish();
    m_innerCommandPool->free(cmd);
}

//-------------------------------------------------------------

void GL3Device::destroyObject(GfxObject* obj) { obj->delRef(); }

END_GFX_NAMESPACE