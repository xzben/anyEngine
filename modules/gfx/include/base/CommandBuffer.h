#pragma once

#include "../common/gfx_common.h"

BEGIN_GFX_NAMESPACE

class Texture;
class Buffer;
class InputAssembler;
class Pipeline;
class Sampler;
class DrawSurface;
class RenderPass;

struct BufferBindingInfo {
    std::string name;
    Buffer* buffer{nullptr};
    uint32_t offset{0};
    uint32_t size{0};
};

struct TextureBindingInfo {
    std::string name;
    Texture* texture{nullptr};
    Sampler* sampler{nullptr};
};

struct MeshInfo {
    InputAssembler* input{nullptr};
    uint32_t offset{0};
    uint32_t count{0};
    uint32_t instanceCount{1};
};

struct DrawMeshInfo {
    Pipeline* pipeline{nullptr};

    uint32_t uniformCount{0};
    BufferBindingInfo* pUniformBindings{nullptr};

    uint32_t storageCount{0};
    BufferBindingInfo* pShaderStorageBinding{nullptr};

    uint32_t textureCount{0};
    TextureBindingInfo* pTextureBindings{nullptr};

    uint32_t meshCount{0};
    MeshInfo* pMeshs{nullptr};
};

struct ComputeInfo {
    Pipeline* pipeline{nullptr};

    uint32_t uniformCount{0};
    BufferBindingInfo* pUniformBindings{nullptr};

    uint32_t storageCount{0};
    BufferBindingInfo* pShaderStorageBindings{nullptr};

    uint32_t textureCount{0};
    TextureBindingInfo* pTextureBindings{nullptr};

    uint32_t groupNumX{1};
    uint32_t groupNumY{1};
    uint32_t groupNumZ{1};
};

enum class RenderState {
    SCISSOR = 1 << 0,
};

struct BeginRenderPassInfo {
    RenderPass* renderPass{nullptr};
    uint32_t surfaceCount{0};
    DrawSurface** pSurfaces{nullptr};
    ClearValue* pClearValues{nullptr};
    Viewport viewport{0, 0, 0, 0};
};

struct UpdateAssemblerInfo {
    const void* pVertexData{nullptr};
    uint32_t vertexDataSize{0};
    uint32_t vertexCount{0};

    const void* pIndexData{nullptr};
    uint32_t indexDataSize{0};
    uint32_t indexCount{0};

    const void* pInstanceData{nullptr};
    uint32_t instanceDataSize{0};
    uint32_t instanceCount{0};
    bool staticData{false};
};

class CommandBuffer : public GfxObject {
public:
    virtual bool reset()                                                               = 0;
    virtual void begin(CommandBufferUsage usage = CommandBufferUsage::ONE_TIME_SUBMIT) = 0;
    virtual void beginRendPass(const BeginRenderPassInfo& info)                        = 0;
    virtual void draw(const DrawMeshInfo& info)                                        = 0;
    virtual void compute(const ComputeInfo& info)                                      = 0;
    virtual void nextSubPass()                                                         = 0;
    virtual void endRendPass()                                                         = 0;
    virtual void end()                                                                 = 0;
    virtual void enable(RenderState state)                                             = 0;
    virtual void disable(RenderState state)                                            = 0;
    virtual void setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)  = 0;
    virtual void setScissor(uint32_t x, uint32_t y, uint32_t width, uint32_t height)   = 0;
    virtual void updateBuffer(Buffer* buffer, const void* pData, uint32_t size, uint32_t offset = 0,
                              bool staticData = false)                                 = 0;

    virtual void updateTexture(Texture* texture, const TextureSubInfo& info, const void* pData,
                               uint32_t size, bool staticData = false) = 0;

    virtual void copyBuffer(Buffer* src, Buffer* dst, uint32_t size, uint32_t srcOffset = 0,
                            uint32_t dstOffset = 0)                                      = 0;
    virtual void copyTexture(Texture* src, Texture* dst, const TextureCopyInfo& info)    = 0;
    virtual void blitTexture(Texture* src, Texture* dst, const TextureBliteInfo& info)   = 0;
    virtual void generateMipmaps(Texture* textre, uint32_t mipLevels)                    = 0;
    virtual void clearTexture(Texture* src, TextureSubInfo& subInfo, const Color& color) = 0;

    virtual void updateInputAssembler(InputAssembler* input, const UpdateAssemblerInfo& info) = 0;

protected:
    CommandBuffer() : GfxObject(GfxObjectType::CommandBuffer) {}
    virtual ~CommandBuffer() = default;
};

END_GFX_NAMESPACE