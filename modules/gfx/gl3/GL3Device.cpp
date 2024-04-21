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

    for (auto context : m_subContext) {
        delete context;
    }
    m_subContext.clear();
    m_pMainContext.reset(nullptr);
}

bool GL3Device::init(const DeviceInfo& info) {
    m_pMainContext = std::make_unique<gl3::GLContext>(*this);
    initSubRenderThreads(g_RenderThreadNum);
    return true;
}

void GL3Device::initSubRenderThreads(uint32_t threadNum) {
    m_subContext.resize(threadNum);
    for (int i = 0; i < threadNum; i++) {
        m_subContext[i] = m_pMainContext->createSubContext();
    }
    m_pRenderThreads = new RenderThreadPool(
        threadNum,
        [&](WorkTask* task, uint32_t threadIndex) {
            task->execute(m_subContext[threadIndex]);
        },
        [&](uint32_t threadIndex) { m_subContext[threadIndex]->makeCurrent(); },
        [&](uint32_t threadIndex) {
            m_subContext[threadIndex]->exitCurrent();
        });
}

void GL3Device::initResourceThread() {
    m_syncWorkThread = new std::thread([&]() {
        m_pMainContext->makeCurrent();

        std::queue<SyncWork*> works;
        while (!m_exit) {
            m_syncWorkQueue.wait(works);

            while (!works.empty()) {
                auto& item = works.front();
                works.pop();
                item->func();
                item->finish = true;
            }
        }
    });
}

GL3Shader* GL3Device::createShader(ShaderModuleInfo* info, uint32_t count) {
    return new GL3Shader(*this, info, count);
}

GL3Queue* GL3Device::getQueue(QueueType& type, uint32_t index) {
    return nullptr;
}

GL3RenderPass* GL3Device::createRenderPass(
    std::vector<Attachment>& attachments, const std::vector<SubPass>& subpass,
    const std::vector<SubPassDependency>& dependencies) {
    return nullptr;
}

GL3Pipeline* GL3Device::createPipeline(RenderPass* renderPass, uint32_t subpass,
                                       Shader* shader,
                                       const PipelineState& state) {
    return nullptr;
}

GL3Texture* GL3Device::createTexture(const TextureInfo& info,
                                     const void* pData) {
    return nullptr;
}
GL3Buffer* GL3Device::createBuffer(BufferType type, uint32_t size) {
    return nullptr;
}
GL3Sampler* GL3Device::createSampler(const SamplerInfo& info) {
    return nullptr;
}

GL3InputAssembler* GL3Device::createInputAssembler(
    const std::vector<Attribute>& attributes, const void* pVertexData,
    uint32_t vertexCount, const void* pIndexData, uint32_t indexCount,
    uint32_t indexItemSize) {
    return nullptr;
}

GL3InputAssembler* GL3Device::createInputAssembler(
    const std::vector<Attribute>& attributes,
    const std::vector<Attribute>& InstanceAttributes, const void* pVertexData,
    uint32_t vertexCount, const void* pInstanceData, uint32_t instanceCount,
    const void* pIndexData, uint32_t indexCount, uint32_t indexItemSize) {
    return nullptr;
}

GL3SwapChain* GL3Device::createSwapChain(void* nativeWindow, uint32_t width,
                                         uint32_t height, bool singleBuffer) {
    return nullptr;
}
GL3Fence* GL3Device::createFence(bool signaled) { return nullptr; }

GL3Semaphore* GL3Device::createSemaphore(uint32_t count) { return nullptr; }

GL3Event* GL3Device::createEvent() { return nullptr; }
void GL3Device::waitIdle() {}

//-------------------------------------------------------------
void GL3Device::destroyBuffer(Buffer* buffer) { buffer->delRef(); }
void GL3Device::destroyEvent(Event* event) { event->delRef(); }
void GL3Device::destroyFence(Fence* fence) { fence->delRef(); }
void GL3Device::destroyInputAssembler(InputAssembler* input) {
    input->delRef();
}
void GL3Device::destroyPipeline(Pipeline* pipeline) { pipeline->delRef(); }
void GL3Device::destroyRenderPasss(RenderPass* renderPass) {
    renderPass->delRef();
}
void GL3Device::destroySampler(Sampler* sampler) { sampler->delRef(); }
void GL3Device::destroySemaphore(Semaphore* semaphore) { semaphore->delRef(); }
void GL3Device::destroyShader(Shader* shader) { shader->delRef(); }
void GL3Device::destroySwapChain(SwapChain* swapChain) { swapChain->delRef(); }
void GL3Device::destroyTexture(Texture* texture) { texture->delRef(); }

END_GFX_NAMESPACE