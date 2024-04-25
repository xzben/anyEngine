#pragma once

#include "../base/Device.h"
#include "GL3Buffer.h"
#include "GL3Event.h"
#include "GL3Fence.h"
#include "GL3InputAssembler.h"
#include "GL3Pipeline.h"
#include "GL3Queue.h"
#include "GL3RenderPass.h"
#include "GL3Sampler.h"
#include "GL3Semaphore.h"
#include "GL3Shader.h"
#include "GL3SwapChain.h"
#include "GL3Texture.h"
#include "core/GLContext.h"
#include "core/render/WorkTask.h"
#include "core/thread/ThreadPool.h"
#include "core/thread/ThreadQueue.h"
#include "gl_common.h"

BEGIN_GFX_NAMESPACE

class GL3Device : public Device {
public:
    friend class GL3Queue;

public:
    GL3Device();
    virtual ~GL3Device();

    virtual bool init(const DeviceInfo& info) override;
    virtual GL3Shader* createShader(ShaderModuleInfo* info,
                                    uint32_t count) override;

    virtual GL3Queue* getQueue(QueueType& type, uint32_t index = 0) override;

    virtual GL3RenderPass* createRenderPass(
        const std::vector<Attachment>& attachments,
        const std::vector<SubPass>& subpass,
        const std::vector<SubPassDependency>& dependencies) override;

    virtual GL3Pipeline* createPipeline(RenderPass* renderPass,
                                        uint32_t subpass, Shader* shader,
                                        const PipelineState& state) override;

    virtual GL3Texture* createTexture(const TextureInfo& info,
                                      const void* pData = nullptr) override;
    virtual GL3Buffer* createBuffer(BufferType type, uint32_t size,
                                    const void* pData = nullptr) override;
    virtual GL3Sampler* createSampler(const SamplerInfo& info) override;

    virtual GL3InputAssembler* createInputAssembler(
        const std::vector<Attribute>& attributes, const void* pVertexData,
        uint32_t vertexCount, const void* pIndexData = nullptr,
        uint32_t indexCount    = 0,
        uint32_t indexItemSize = sizeof(uint32_t)) override;

    virtual GL3InputAssembler* createInputAssembler(
        const std::vector<Attribute>& attributes,
        const std::vector<Attribute>& InstanceAttributes,
        const void* pVertexData, uint32_t vertexCount,
        const void* pInstanceData, uint32_t instanceCount,
        const void* pIndexData = nullptr, uint32_t indexCount = 0,
        uint32_t indexItemSize = sizeof(uint32_t)) override;

    virtual GL3SwapChain* createSwapChain(void* nativeWindow, uint32_t width,
                                          uint32_t height,
                                          bool singleBuffer) override;
    virtual GL3Fence* createFence(bool signaled = false) override;

    virtual GL3Semaphore* createSemaphore() override;

    virtual GL3Event* createEvent() override;

    virtual void waitIdle() override;

protected:
    using WorkTask         = gl3::WorkTask;
    using RenderThreadPool = gl3::ThreadPool<WorkTask*>;
    RenderThreadPool* m_pRenderThreads;
    void initSubRenderThreads(uint32_t threadNum);
    bool addTask(WorkTask* task, float priority = 1.0) {
        return m_pRenderThreads->addTask(task, priority);
    }

    using SyncFunc = std::function<void()>;

    struct SyncWork {
        SyncFunc func;
        std::atomic<bool> finish;
    };
    gl3::ThreadQueue<SyncWork*> m_syncWorkQueue;
    std::thread* m_syncWorkThread;
    void initResourceThread();

public:
    void callSync(SyncFunc func) {
        SyncWork item;
        item.func   = func;
        item.finish = false;

        m_syncWorkQueue.addItem(&item);
        while (!item.finish)
            ;
    }

    void exit();

public:
    virtual void destroyBuffer(Buffer* buffer) override;
    virtual void destroyEvent(Event* event) override;
    virtual void destroyFence(Fence* fence) override;
    virtual void destroyInputAssembler(InputAssembler* input) override;
    virtual void destroyPipeline(Pipeline* pipeline) override;
    virtual void destroyRenderPasss(RenderPass* renderPass) override;
    virtual void destroySampler(Sampler* sampler) override;
    virtual void destroySemaphore(Semaphore* semaphore) override;
    virtual void destroyShader(Shader* shader) override;
    virtual void destroySwapChain(SwapChain* swapChain) override;
    virtual void destroyTexture(Texture* texture) override;

private:
    std::vector<GL3Queue*> m_queues;
    std::unordered_map<QueueType, std::vector<GL3Queue*>> m_queueMapInfo;
    std::unique_ptr<gl3::GLContext> m_pMainContext;
    std::vector<gl3::GLContext*> m_subContext;
    bool m_exit{false};

protected:
};

END_GFX_NAMESPACE