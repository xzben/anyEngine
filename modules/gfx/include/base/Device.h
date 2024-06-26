#pragma once

#include "../common/gfx_common.h"
#include "Buffer.h"
#include "CommandBuffer.h"
#include "CommandPool.h"
#include "DrawSurface.h"
#include "Event.h"
#include "Fence.h"
#include "InputAssembler.h"
#include "Pipeline.h"
#include "Queue.h"
#include "RenderPass.h"
#include "Sampler.h"
#include "Semaphore.h"
#include "Shader.h"
#include "SwapChain.h"
#include "Texture.h"

BEGIN_GFX_NAMESPACE
class Device {
public:
    static Device* createDevice(const DeviceInfo& info);

public:
    /*
     *  获取 命令队列，按照创建 Device 时 DeviceInfo 申请的 QueueType 和 count
     *  去获取
     *  @param type 需要的队列类型
     *  @param index 申请的队列类型数量对应的下标  0 <= index < count
     */
    virtual Queue* getQueue(QueueType type, uint32_t index = 0) = 0;

    /*
     *  创建 渲染 RenderPass 对象，
     *  此对象主要用于描述一个复杂的渲染过程，对应我们常用的材质对象，
     *   一个RenderPass 描述了材质的一个渲染pass的构成和 pass 之间的依赖关系
     * @param attachments 代表渲染过程中需要多少个附件
     * @param subpass 渲染过程中的 subpass 信息
     * @param dependencies 渲染过程中 subpass 之间的依赖关系
     */
    virtual RenderPass* createRenderPass(const std::vector<Attachment>& attachments,
                                         const std::vector<SubPass>& subpass,
                                         const std::vector<SubPassDependency>& dependencies) = 0;

    DrawTextureSurface* createDrawSurface(Texture* texture, uint32_t layerindex) {
        return new DrawTextureSurface(texture, layerindex);
    }

    DrawSwaChainSurface* createDrawSurface(SwapChain* swapchain, uint32_t imageIndex,
                                           SwapChainAttachment colorAttachment) {
        return new DrawSwaChainSurface(swapchain, imageIndex, colorAttachment);
    }

    void destroyDrawSurface(DrawSurface* surface) { delete surface; }
    /*
     *  创建一个渲染管线
     * @param renderPass 管线所属的 renderpass
     * @param subpass 管线在 renderpass 中的 subpass index
     * @param shader 管线的着色器对象
     * @param  state 管线的状态信息
     * @param primitiveType 管线
     */
    virtual Pipeline* createPipeline(RenderPass* renderPass, uint32_t subpass, Shader* shader,
                                     const PipelineState& state) = 0;
    /*
     *  创建一个着色器对象
     *  info shaderModule 信息数组指针
     *  count info 数组的size
     */
    virtual Shader* createShader(ShaderModuleInfo* info, uint32_t count) = 0;

    // resource interface
    virtual Texture* createTexture(const TextureInfo& info, const void* pData = nullptr)      = 0;
    virtual Buffer* createBuffer(BufferType type, uint32_t size, const void* pData = nullptr) = 0;
    virtual Sampler* createSampler(const SamplerInfo& info)                                   = 0;

    /*
     *  创建渲染几何物体对象，包含了物体的顶点属性描述、顶点buffer、索引buffer
     *  @param primitiveType 物体的图形构成形状
     *  @param attribute 几何物体的顶点属性描述
     *  @param pVertexData、vertexCount 顶点数据 data，和顶点数量
     *  @param pIndexData、indexCount、indexItemSize 索引数据
     *  data，索引数量，索引数据类型的size
     */
    virtual InputAssembler* createInputAssembler(PrimitiveType primitiveType,
                                                 const std::vector<Attribute>& attributes,
                                                 const void* pVertexData, uint32_t vertexCount,
                                                 const void* pIndexData = nullptr,
                                                 uint32_t indexCount    = 0,
                                                 uint32_t indexItemSize = sizeof(uint32_t)) = 0;
    /*
     *  创建Instance渲染的几何物体对象，包含了物体的顶点属性描述、顶点buffer、索引buffer、Instance
     *  绘制buffer 和信息
     *  @param primitiveType 物体的图形构成形状
     *  @param attribute 几何物体的顶点属性描述
     *  @param pVertexData、vertexCount 顶点数据 data，和顶点数量
     *  @param pInstanceData、instanceCount instance绘制的data, 和 instance 数量
     *  @param pIndexData、indexCount、indexItemSize 索引数据
     *  data，索引数量，索引数据类型的size
     */
    virtual InputAssembler* createInputAssembler(PrimitiveType primitiveType,
                                                 const std::vector<Attribute>& attributes,
                                                 const std::vector<Attribute>& InstanceAttributes,
                                                 const void* pVertexData, uint32_t vertexCount,
                                                 const void* pInstanceData, uint32_t instanceCount,
                                                 const void* pIndexData = nullptr,
                                                 uint32_t indexCount    = 0,
                                                 uint32_t indexItemSize = sizeof(uint32_t)) = 0;

    /*
     *   创建渲染的窗口表面对象
     *   @param nativeWindow 各个平台的渲染创建对象指针
     *   @param width、height 窗口尺寸 宽 高
     *   @param singleBuffer 窗口是单缓冲还是双缓冲
     *   @param needDepthStencil 是否需要深度模版缓冲
     */
    virtual SwapChain* createSwapChain(const SurfaceInfo& info, bool needDepthStencil = false) = 0;
    // 同步对象
    /*
     *  创建用于 CPU 和 GPU 同步用的同步对象
     *  @param signaled 创建好是是否直接处于 信号准备好状态
     */
    virtual Fence* createFence(bool signaled = false) = 0;
    /*
     * 创建用于 GPU 同步使用的信号量对象
     */
    virtual Semaphore* createSemaphore() = 0;
    /*
     *  创建用于 commandBuffer 内部同步使用的时间对象
     */
    virtual Event* createEvent() = 0;

    /*
     *  等待当前device 空闲
     */
    virtual void waitIdle() = 0;

    /*
     * 执行一次一次性的命令，并且立即提交执行等待执行完成
     */
    virtual void withOneTimeCmd(std::function<void(CommandBuffer& cmd)> callback) = 0;

    virtual void destroyObject(GfxObject* obj) = 0;

protected:
    Device()          = default;
    virtual ~Device() = default;

    virtual bool init(const DeviceInfo& info) = 0;
};

END_GFX_NAMESPACE