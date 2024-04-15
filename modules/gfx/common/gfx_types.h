#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>

#include "gfx_defines.h"
#include "gfx_functions.h"

BEGIN_GFX_NAMESPACE

using GFX_HANDLE                        = void *;
static const GFX_HANDLE GFX_NULL_HANDLE = 0;

enum class GFX_API {
    UNKNOWN,
    OPENGL3,
    GLES3,
    VULKAN,
    METAL,
};

enum QueueType {
    Graphics = 0,
    Present,
    Transfer,
    Compute,
    Count,
};

struct SurfaceInfo {
    void *handle{nullptr};
    uint32_t width;
    uint32_t height;
};

struct DeviceInfo {
    GFX_API api{GFX_API::OPENGL3};
    std::string name{""};
    bool present{true};
    SurfaceInfo surface;
    std::unordered_map<QueueType, uint32_t> queues;
    uint32_t requireMemSize{0};
};

enum class GfxObjectType : uint8_t {
    UnKnown = 0,
    SwapChain,
    Buffer,
    Texture,
    RenderPass,
    FrameBuffer,
    Sampler,
    Shader,
    DescriptorSet,
    DescriptorSetPool,
    DescriptorSetLayout,
    PipelineLayout,
    Pipeline,
    InputAssembler,
    CommandBuffer,
    CommandPool,
    Queue,
    Device,
    Fence,
    Semaphore,
    Event,
    Count,
};

enum class ResetMode : uint8_t {
    ResetPool = 0,
    ResetIndividually,
    AlwaysAllocate,
};

enum class CommandBufferLevel : uint8_t {
    PRIMARY = 0,
    SECONDARY,
};

enum class CommandBufferUsage : uint8_t {
    ONE_TIME_SUBMIT      = 1 << 0,
    RENDER_PASS_CONTINUE = 1 << 1,
    SIMULTANEOUS_USE     = 1 << 2,
};
CC_ENUM_BITWISE_OPERATORS(CommandBufferUsage)

enum class DataFormat {
    UNDEFINED = 0,
    Bool,
    Bool2,
    Bool3,
    Bool4,
    Int,
    Int2,
    Int3,
    Int4,
    UInt,
    UInt2,
    UInt3,
    UInt4,
    Float,
    Float2,
    Float3,
    Float4,
};

enum class PixelFormat {
    UNDEFINED = 0,
    RGBA8,
    RGB8,
    Depth,
    Depth24Stencil8,
};

struct Attribute {
    uint32_t location{0};
    std::string name{""};
    DataFormat format{DataFormat::UNDEFINED};
    bool isNormalized{false};
};

enum class BufferType : uint32_t {
    INDEX,
    VERTEX,
    UNIFORM,
    SHADER_STORAGE,
    PIXEL,
};

enum class TextureUsage {
    USAGE_UNDEFINE                     = 0,
    USAGE_TRANSFER_SRC_BIT             = 1 << 0,
    USAGE_TRANSFER_DST_BIT             = 1 << 1,
    USAGE_SAMPLED_BIT                  = 1 << 2,
    USAGE_STORAGE_BIT                  = 1 << 3,
    USAGE_COLOR_ATTACHMENT_BIT         = 1 << 4,
    USAGE_DEPTH_STENCIL_ATTACHMENT_BIT = 1 << 5,
    USAGE_TRANSIENT_ATTACHMENT_BIT     = 1 << 6,
    USAGE_INPUT_ATTACHMENT_BIT         = 1 << 7,
};
CC_ENUM_BITWISE_OPERATORS(TextureUsage)

enum class ShaderStage {
    NONE         = 0x0,
    VERTEX       = 0x1,
    CONTROL      = 0x2,
    EVALUATION   = 0x4,
    GEOMETRY     = 0x8,
    FRAGMENT     = 0x10,
    COMPUTE      = 0x20,
    ALL_GRAPHICS = VERTEX | FRAGMENT,
    ALL          = 0x3f,
};
CC_ENUM_BITWISE_OPERATORS(ShaderStage)

enum class PrimitiveType {
    POINT_LIST                    = 0,
    LINE_LIST                     = 1,
    LINE_STRIP                    = 2,
    TRIANGLE_LIST                 = 3,
    TRIANGLE_STRIP                = 4,
    TRIANGLE_FAN                  = 5,
    LINE_LIST_WITH_ADJACENCY      = 6,
    LINE_STRIP_WITH_ADJACENCY     = 7,
    TRIANGLE_LIST_WITH_ADJACENCY  = 8,
    TRIANGLE_STRIP_WITH_ADJACENCY = 9,
    PATCH_LIST                    = 10,
};

enum class PolygonModel {
    FILL  = 0,
    LINE  = 1,
    POINT = 2,
};

enum class CullModel {
    NONE,
    FRONT,
    BACK,
    FRONT_AND_BACK,
};

enum class FrontFace {
    COUNTER_CLOCKWISE,  // 逆时针
    CLOCKWISE,          // 顺时针
};

struct RasterizationState {
    bool operator==(const RasterizationState &rhs) {
        return depthClampEnable == rhs.depthClampEnable
               && rasterizerDiscardEnable == rhs.rasterizerDiscardEnable
               && polygonMode == rhs.polygonMode && lineWidth == rhs.lineWidth
               && cullMode == rhs.cullMode && frontFace == rhs.frontFace
               && depthBiasEnable == rhs.depthBiasEnable
               && depthBiasConstantFactor == rhs.depthBiasConstantFactor
               && depthBiasClamp == rhs.depthBiasClamp
               && depthBiassSlopeFactor == rhs.depthBiassSlopeFactor;
    }

    bool operator!=(const RasterizationState &rhs) { return !operator==(rhs); }
    bool depthClampEnable{false};
    bool rasterizerDiscardEnable{false};
    PolygonModel polygonMode{PolygonModel::FILL};
    float lineWidth{1.0f};
    CullModel cullMode{CullModel::NONE};
    FrontFace frontFace{FrontFace::COUNTER_CLOCKWISE};
    bool depthBiasEnable{false};
    float depthBiasConstantFactor{0.0f};
    float depthBiasClamp{0.0f};
    float depthBiassSlopeFactor{0.0f};
};

enum class SampleCount {
    SAMPLE_COUNT_1_BIT  = 0x00000001,
    SAMPLE_COUNT_2_BIT  = 0x00000002,
    SAMPLE_COUNT_4_BIT  = 0x00000004,
    SAMPLE_COUNT_8_BIT  = 0x00000008,
    SAMPLE_COUNT_16_BIT = 0x00000010,
    SAMPLE_COUNT_32_BIT = 0x00000020,
    SAMPLE_COUNT_64_BIT = 0x00000040,
};

struct MultiSampleState {
    bool sampleShadingEnable{false};
    SampleCount rasterizationSamples{SampleCount::SAMPLE_COUNT_1_BIT};
};

enum class CompareOp {
    EMPTY            = 0,
    NEVER            = 1,
    LESS             = 2,
    EQUAL            = 3,
    LESS_OR_EQUAL    = 4,
    GREATER          = 5,
    NOT_EQUAL        = 6,
    GREATER_OR_EQUAL = 7,
    ALWAYS           = 8,
};

enum class StencilOp {
    KEEP                = 0,
    ZERO                = 1,
    REPLACE             = 2,
    INCREMENT_AND_CLAMP = 3,
    DECREMENT_AND_CLAMP = 4,
    INVERT              = 5,
    INCREMENT_AND_WRAP  = 6,
    DECREMENT_AND_WRAP  = 7,
};

struct StencilOpState {
    bool operator==(const StencilOpState &rhs) {
        return failOp == rhs.failOp && passOp == rhs.passOp
               && depthFailOP == rhs.depthFailOP && compareOp == rhs.compareOp
               && compareMask == rhs.compareMask && writeMask == rhs.writeMask
               && reference == rhs.reference;
    }
    bool operator!=(const StencilOpState &rhs) { return !operator==(rhs); }

    StencilOp failOp{StencilOp::KEEP};
    StencilOp passOp{StencilOp::KEEP};
    StencilOp depthFailOP{StencilOp::KEEP};
    CompareOp compareOp{CompareOp::NEVER};
    uint32_t compareMask{0};
    uint32_t writeMask{0};
    uint32_t reference{0};
};

struct DepthStencilState {
    bool operator==(const DepthStencilState &rhs) {
        return depthTestEnable == rhs.depthTestEnable
               && depthWriteEnable == rhs.depthWriteEnable
               && depthCompareOp == rhs.depthCompareOp
               && depthBoundsTestEnable == rhs.depthBoundsTestEnable
               && minDepthBounds == rhs.minDepthBounds
               && maxDepthBounds == rhs.maxDepthBounds
               && stencilTestEnable == rhs.stencilTestEnable
               && front == rhs.front && back == rhs.back;
    }

    bool operator!=(const DepthStencilState &rhs) { return !operator==(rhs); }

    bool depthTestEnable{true};
    bool depthWriteEnable{true};
    CompareOp depthCompareOp{CompareOp::LESS};
    bool depthBoundsTestEnable{false};
    float minDepthBounds{0.0};
    float maxDepthBounds{1.0f};
    bool stencilTestEnable{false};
    StencilOpState front;
    StencilOpState back;
};

enum class BlendFactor {
    ZERO                     = 0,
    ONE                      = 1,
    SRC_COLOR                = 2,
    ONE_MINUS_SRC_COLOR      = 3,
    DST_COLOR                = 4,
    ONE_MINUS_DST_COLOR      = 5,
    SRC_ALPHA                = 6,
    ONE_MINUS_SRC_ALPHA      = 7,
    DST_ALPHA                = 8,
    ONE_MINUS_DST_ALPHA      = 9,
    CONSTANT_COLOR           = 10,
    ONE_MINUS_CONSTANT_COLOR = 11,
    CONSTANT_ALPHA           = 12,
    ONE_MINUS_CONSTANT_ALPHA = 13,
};

enum class BlendOp {
    ADD              = 0,
    SUBTRACT         = 1,
    REVERSE_SUBTRACT = 2,
};

enum class BlendLogicOp {
    CLEAR         = 0,
    AND           = 1,
    AND_REVERSE   = 2,
    COPY          = 3,
    AND_INVERTED  = 4,
    NO_OP         = 5,
    XOR           = 6,
    OR            = 7,
    NOR           = 8,
    EQUIVALENT    = 9,
    INVERT        = 10,
    OR_REVERSE    = 11,
    COPY_INVERTED = 12,
    OR_INVERTED   = 13,
    NAND          = 14,
    SET           = 15,
};

struct ColorBlendState {
    bool operator==(const ColorBlendState &rhs) {
        return blendEnable == rhs.blendEnable
               && srcColorFactor == rhs.srcColorFactor
               && dstColorFactor == rhs.dstColorFactor
               && colorBlendOp == rhs.colorBlendOp
               && srcAlphaFactor == rhs.srcAlphaFactor
               && dstAlphaFactor == rhs.dstAlphaFactor
               && alphaBlendOp == rhs.alphaBlendOp
               && logicOpEnable == rhs.logicOpEnable && logicOp == rhs.logicOp
               && constansts[0] == rhs.constansts[0]
               && constansts[1] == rhs.constansts[1]
               && constansts[2] == rhs.constansts[2]
               && constansts[3] == rhs.constansts[3];
    }

    bool operator!=(const ColorBlendState &rhs) { return !operator==(rhs); }
    bool blendEnable{false};
    BlendFactor srcColorFactor{BlendFactor::SRC_ALPHA};
    BlendFactor dstColorFactor{BlendFactor::ONE_MINUS_SRC_ALPHA};
    BlendOp colorBlendOp{BlendOp::ADD};
    BlendFactor srcAlphaFactor{BlendFactor::ONE};
    BlendFactor dstAlphaFactor{BlendFactor::ZERO};
    BlendOp alphaBlendOp{BlendOp::ADD};

    bool logicOpEnable{false};
    BlendLogicOp logicOp{BlendLogicOp::COPY};
    float constansts[4]{0.0f, 0.0f, 0.0f, 0.0f};
};

struct PipelineState {
    RasterizationState rasterize;
    DepthStencilState depthStencil;
    ColorBlendState blend;
    MultiSampleState multiSample;
};

enum class Filter {
    POINT,
    LINEAR,
};

enum class Address {
    WRAP,
    MIRROR,
    CLAMP,
    BORDER,
};

enum class SamplerBorderColor {
    TRANSPARENT_BLACK,
    OPAQUA_BLACK,
    OPQUA_WHITE,
};

class SamplerInfo {
public:
    static size_t compute_hash(const SamplerInfo &info) {
        size_t hash = (size_t)info.minFilter;

        hash_combine(hash, info.magFilter);
        hash_combine(hash, info.mipFilter);
        hash_combine(hash, info.addressU);
        hash_combine(hash, info.addressV);
        hash_combine(hash, info.addressW);
        hash_combine(hash, info.maxAnisotropy);
        hash_combine(hash, info.cmpFunc);
        hash_combine(hash, info.bordercolor);

        return hash;
    }

public:
    Filter minFilter{Filter::LINEAR};     // vlaue 0 - 3
    Filter magFilter{Filter::LINEAR};     // vlaue 0 - 3
    Filter mipFilter{Filter::LINEAR};     // vlaue 0 - 3
    Address addressU{Address::WRAP};      // vlaue 0 - 3
    Address addressV{Address::WRAP};      // vlaue 0 - 3
    Address addressW{Address::WRAP};      // vlaue 0 - 3
    uint32_t maxAnisotropy{0};            // vlaue 0 - 15
    CompareOp cmpFunc{CompareOp::EMPTY};  // value 0 - 8
    SamplerBorderColor bordercolor{SamplerBorderColor::TRANSPARENT_BLACK};
};

enum class GfxError : uint32_t {
    SUCCESS = 0,
    UNKNOWN_ERROR,
    NEED_RECREATE_SWAPCHAIN,
    SUBMIT_COMMAND_FAILED,
};

enum class LoadOp {
    LOAD,
    CLEAR,
    DONT_CARE,
};

enum class StoreOp {
    STORE,
    DONT_CARE,
};

enum class ImageLayout {
    UNDEFINED = 0,
    GENERAL,
    COLOR_ATTACHMENT_OPTIMAL,
    DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
    DEPTH_STENCIL_READ_ONLY_OPTIMAL,
    SHADER_READ_ONLY_OPTIMAL,
    TRANSFER_SRC_OPTIMAL,
    TRANSFER_DST_OPTIMAL,
    PREINITIALIZED,
    DEPTH_READ_ONLY_STENCIL_ATTACHMENT_OPTIMAL,
    DEPTH_ATTACHMENT_STENCIL_READ_ONLY_OPTIMAL,
    DEPTH_ATTACHMENT_OPTIMAL,
    DEPTH_READ_ONLY_OPTIMAL,
    STENCIL_ATTACHMENT_OPTIMAL,
    STENCIL_READ_ONLY_OPTIMAL,
    PRESENT_SRC_KHR,
    COUNT,
};

enum class TextureType {
    TEX1D,
    TEX2D,
    TEX3D,
    CUBE,
    TEX1D_ARRAY,
    TEX2D_ARRAY,
};

struct TextureInfo {
    TextureType type{TextureType::TEX2D};
    uint32_t width{0};
    uint32_t height{0};
    uint32_t miplevels{1};
    SampleCount sampleCount{SampleCount::SAMPLE_COUNT_1_BIT};
    PixelFormat format{PixelFormat::RGBA8};
    TextureUsage usage{TextureUsage::USAGE_SAMPLED_BIT
                       | TextureUsage::USAGE_TRANSFER_DST_BIT
                       | TextureUsage::USAGE_TRANSFER_SRC_BIT};
};

enum class PipelineStage : uint32_t {
    TOP_OF_PIPE_BIT                          = 1 << 0,
    DRAW_INDIRECT_BIT                        = 1 << 1,
    VERTEX_INPUT_BIT                         = 1 << 2,
    VERTEX_SHADER_BIT                        = 1 << 3,
    TESSELLATION_CONTROL_SHADER_BIT          = 1 << 4,
    TESSELLATION_EVALUATION_SHADER_BIT       = 1 << 5,
    GEOMETRY_SHADER_BIT                      = 1 << 6,
    FRAGMENT_SHADER_BIT                      = 1 << 7,
    EARLY_FRAGMENT_TESTS_BIT                 = 1 << 8,
    LATE_FRAGMENT_TESTS_BIT                  = 1 << 9,
    COLOR_ATTACHMENT_OUTPUT_BIT              = 1 << 10,
    COMPUTE_SHADER_BIT                       = 1 << 11,
    TRANSFER_BIT                             = 1 << 12,
    BOTTOM_OF_PIPE_BIT                       = 1 << 13,
    HOST_BIT                                 = 1 << 14,
    ALL_GRAPHICS_BIT                         = 1 << 15,
    ALL_COMMANDS_BIT                         = 1 << 16,
    NONE                                     = 0,
    TRANSFORM_FEEDBACK_BIT_EXT               = 1 << 17,
    CONDITIONAL_RENDERING_BIT_EXT            = 1 << 18,
    ACCELERATION_STRUCTURE_BUILD_BIT_KHR     = 1 << 19,
    RAY_TRACING_SHADER_BIT_KHR               = 1 << 20,
    FRAGMENT_DENSITY_PROCESS_BIT_EXT         = 1 << 21,
    FRAGMENT_SHADING_RATE_ATTACHMENT_BIT_KHR = 1 << 22,
    COMMAND_PREPROCESS_BIT_NV                = 1 << 23,
    TASK_SHADER_BIT_EXT                      = 1 << 24,
    MESH_SHADER_BIT_EXT                      = 1 << 25,
};
CC_ENUM_BITWISE_OPERATORS(PipelineStage);

enum class AccessFlags : uint32_t {
    INDIRECT_COMMAND_READ_BIT                     = 1 << 0,
    INDEX_READ_BIT                                = 1 << 1,
    VERTEX_ATTRIBUTE_READ_BIT                     = 1 << 2,
    UNIFORM_READ_BIT                              = 1 << 3,
    INPUT_ATTACHMENT_READ_BIT                     = 1 << 4,
    SHADER_READ_BIT                               = 1 << 5,
    SHADER_WRITE_BIT                              = 1 << 6,
    COLOR_ATTACHMENT_READ_BIT                     = 1 << 7,
    COLOR_ATTACHMENT_WRITE_BIT                    = 1 << 8,
    DEPTH_STENCIL_ATTACHMENT_READ_BIT             = 1 << 9,
    DEPTH_STENCIL_ATTACHMENT_WRITE_BIT            = 1 << 10,
    TRANSFER_READ_BIT                             = 1 << 11,
    TRANSFER_WRITE_BIT                            = 1 << 12,
    HOST_READ_BIT                                 = 1 << 13,
    HOST_WRITE_BIT                                = 1 << 14,
    MEMORY_READ_BIT                               = 1 << 15,
    MEMORY_WRITE_BIT                              = 1 << 16,
    NONE                                          = 1 << 17,
    TRANSFORM_FEEDBACK_WRITE_BIT_EXT              = 1 << 18,
    TRANSFORM_FEEDBACK_COUNTER_READ_BIT_EXT       = 1 << 19,
    TRANSFORM_FEEDBACK_COUNTER_WRITE_BIT_EXT      = 1 << 20,
    CONDITIONAL_RENDERING_READ_BIT_EXT            = 1 << 21,
    COLOR_ATTACHMENT_READ_NONCOHERENT_BIT_EXT     = 1 << 22,
    ACCELERATION_STRUCTURE_READ_BIT_KHR           = 1 << 23,
    ACCELERATION_STRUCTURE_WRITE_BIT_KHR          = 1 << 24,
    FRAGMENT_DENSITY_MAP_READ_BIT_EXT             = 1 << 25,
    FRAGMENT_SHADING_RATE_ATTACHMENT_READ_BIT_KHR = 1 << 26,
    COMMAND_PREPROCESS_READ_BIT_NV                = 1 << 27,
    COMMAND_PREPROCESS_WRITE_BIT_NV               = 1 << 28,
};
CC_ENUM_BITWISE_OPERATORS(AccessFlags);

enum class DependencyFlags : uint8_t {
    DEPENDENCY_BY_REGION_BIT         = 1 << 0,
    DEPENDENCY_DEVICE_GROUP_BIT      = 1 << 1,
    DEPENDENCY_VIEW_LOCAL_BIT        = 1 << 2,
    DEPENDENCY_FEEDBACK_LOOP_BIT_EXT = 1 << 3,
};
CC_ENUM_BITWISE_OPERATORS(DependencyFlags);

struct SubPassDependence {
    uint32_t subpass;
    PipelineStage stage_mask;
    AccessFlags access_flags;
};

struct SubPassDependency {
    SubPassDependence src;
    SubPassDependence dst;
    DependencyFlags flags;
};

enum class MemoryAccess {
    NONE                             = 0,
    READ_ONLY                        = 1 << 0,
    WRITE_ONLY                       = 1 << 1,
    READ_WRITE                       = READ_ONLY | WRITE_ONLY,
    CREATE_MAPPED_BIT                = 1 << 2,
    HOST_ACCESS_SEQUENTIAL_WRITE_BIT = 1 << 3,
};
CC_ENUM_BITWISE_OPERATORS(MemoryAccess)

struct Attachment {
    PixelFormat format = PixelFormat::UNDEFINED;
    SampleCount samples{SampleCount::SAMPLE_COUNT_1_BIT};
    LoadOp load_op{LoadOp::DONT_CARE};
    StoreOp store_op{StoreOp::DONT_CARE};
    LoadOp stencil_load_op{LoadOp::DONT_CARE};
    StoreOp stencil_store_op{StoreOp::DONT_CARE};
    ImageLayout initial_layout{ImageLayout::UNDEFINED};
    ImageLayout final_layout{ImageLayout::UNDEFINED};
};

struct AttachmentReference {
    uint32_t attachment;
    ImageLayout layout;
};

struct SubPass {
    std::vector<AttachmentReference> colorAttachments;
    std::vector<AttachmentReference> depthStencilAttachments;
    std::vector<AttachmentReference> resolveAttachments;
    std::vector<uint32_t> preserveAttachments;
};

template <typename T>
struct RectImp {
    T left;
    T bottom;
    T width;
    T height;
};

template <typename T>
struct SizeImp {
    T width;
    T height;
};

struct Viewport {
    float x{0.f};
    float y{0.f};
    float width{0.f};
    float height{0.f};
};

using RectI = RectImp<int32_t>;
using RectF = RectImp<float>;

using SizeI = SizeImp<int32_t>;
using SizeF = SizeImp<float>;

struct BufferCopyRange {};

struct Offset3D {
    float x{0.f};
    float y{0.f};
    float z{0.f};
};

struct Range3D {
    float x{0.f};
    float y{0.f};
    float z{0.f};
};

struct TextureCopyInfo {
    Offset3D srcOffset{0.f, 0.f, 0.f};
    Offset3D dstOffset{0.f, 0.f, 0.f};
    Range3D range{0.f, 0.f, 0.f};
    uint32_t srcLayerIndex{0};
    uint32_t dstLayerIndex{0};
};

struct TextureBliteInfo {
    Offset3D srcOffset{0.f, 0.f, 0.f};
    Offset3D dstOffset{0.f, 0.f, 0.f};
    Range3D srcRange{0.f, 0.f, 0.f};
    Range3D dstRange{0.f, 0.f, 0.f};
    uint32_t srcLayerIndex{0};
    uint32_t dstLayerIndex{0};
};

struct Color {
    union {
        float values[4] = {0.f, 0.f, 0.f, 0.f};
        struct {
            float r;
            float g;
            float b;
            float a;
        };
    };
};

struct ClearDepthStencilValue {
    float depth{1.f};
    int32_t stencil{0};
};

struct ClearValue {
    union {
        Color color;
        ClearDepthStencilValue depthStencil;
    };
};

class Texture;

struct DrawSurface {
    Texture *texture{nullptr};
    uint32_t layerIndex{0};
};

END_GFX_NAMESPACE