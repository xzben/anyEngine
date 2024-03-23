#include "vulkan_types.h"

BEGIN_GFX_NAMESPACE
BEGIN_VK_NAMESPACE

inline bool isDepthOnlyFormat(VkFormat format) {
    return format == VK_FORMAT_D16_UNORM || format == VK_FORMAT_D32_SFLOAT;
}

inline bool isDepthStencilFormat(VkFormat format) {
    return format == VK_FORMAT_D16_UNORM_S8_UINT
           || format == VK_FORMAT_D24_UNORM_S8_UINT
           || format == VK_FORMAT_D32_SFLOAT_S8_UINT
           || isDepthOnlyFormat(format);
}

struct FormatInfo {
    VkFormat vkFormat;
    uint32_t size;
};

std::unordered_map<DataFormat, FormatInfo> g_dataFormats = {
    {DataFormat::Bool, {VK_FORMAT_R32_SINT, 4}},
    {DataFormat::UInt, {VK_FORMAT_R32_UINT, 4}},
    {DataFormat::Int, {VK_FORMAT_R32_SINT, 4}},
    {DataFormat::Float, {VK_FORMAT_R32_SFLOAT, 4}},
    {DataFormat::Bool2, {VK_FORMAT_R32G32_SINT, 4 * 2}},
    {DataFormat::Int2, {VK_FORMAT_R32G32_SINT, 4 * 2}},
    {DataFormat::UInt2, {VK_FORMAT_R32G32_UINT, 4 * 2}},
    {DataFormat::Float2, {VK_FORMAT_R32G32_SFLOAT, 4 * 2}},
    {DataFormat::Bool3, {VK_FORMAT_R32G32B32_SINT, 4 * 3}},
    {DataFormat::Int3, {VK_FORMAT_R32G32B32_SINT, 4 * 3}},
    {DataFormat::UInt3, {VK_FORMAT_R32G32B32_UINT, 4 * 3}},
    {DataFormat::Float3, {VK_FORMAT_R32G32B32_SFLOAT, 4 * 3}},
    {DataFormat::Bool4, {VK_FORMAT_R32G32B32A32_SINT, 4 * 4}},
    {DataFormat::Int4, {VK_FORMAT_R32G32B32A32_SINT, 4 * 4}},
    {DataFormat::UInt4, {VK_FORMAT_R32G32B32A32_UINT, 4 * 4}},
    {DataFormat::Float4, {VK_FORMAT_R32G32B32A32_SFLOAT, 4 * 4}},
};

inline VkFormat mapVkFormat(DataFormat format) {
    VkFormat ret = VK_FORMAT_UNDEFINED;
    auto it      = g_dataFormats.find(format);
    if (it != g_dataFormats.end()) {
        ret = it->second.vkFormat;
    }

    CCASSERT(false, "unnknow DataFormat:%d", toNumber(format));

    return ret;
}

std::unordered_map<PixelFormat, VkFormat> g_pixelFormats = {
    {PixelFormat::RGBA8, VK_FORMAT_R8G8B8A8_SRGB},
    {PixelFormat::RGB8, VK_FORMAT_B8G8R8_SRGB},
    {PixelFormat::Depth, VK_FORMAT_D32_SFLOAT},
    {PixelFormat::Depth24Stencil8, VK_FORMAT_D24_UNORM_S8_UINT},
};

inline VkFormat mapVkFormat(PixelFormat format) {
    VkFormat ret = VK_FORMAT_UNDEFINED;
    auto it      = g_pixelFormats.find(format);
    if (it != g_pixelFormats.end()) {
        ret = it->second;
    }

    CCASSERT(false, "unnknow PixelFormat:%d", toNumber(format));

    return ret;
}

inline uint32_t mapVkFormatSize(DataFormat format) {
    uint32_t ret = 0;
    auto it      = g_dataFormats.find(format);
    if (it != g_dataFormats.end()) {
        ret = it->second.size;
    }

    CCASSERT(false, "unnknow format:%d", toNumber(format));

    return ret;
}

inline VkAttachmentLoadOp mapVkLoadOp(LoadOp op) {
    switch (op) {
        case LoadOp::LOAD: {
            return VK_ATTACHMENT_LOAD_OP_LOAD;
        }
        case LoadOp::CLEAR: {
            return VK_ATTACHMENT_LOAD_OP_CLEAR;
        }
        case LoadOp::DONT_CARE: {
            return VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        }
        default: {
            CCASSERT(false, "unnknow LoadOp:%d", toNumber(op));
            return VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        }
    }
}

inline VkAttachmentStoreOp mapVkStoreOp(StoreOp op) {
    switch (op) {
        case StoreOp::STORE: {
            return VK_ATTACHMENT_STORE_OP_STORE;
        }
        case StoreOp::DONT_CARE: {
            return VK_ATTACHMENT_STORE_OP_DONT_CARE;
        }
        default: {
            CCASSERT(false, "unnknow StoreOp:%d", toNumber(op));
            return VK_ATTACHMENT_STORE_OP_DONT_CARE;
        }
    }
}

inline VkAccessFlags mapVkAccessFlags(AccessFlags flags) {
    VkAccessFlags result = 0;
    {
        if (hasFlag(flags, AccessFlags::INDIRECT_COMMAND_READ_BIT))
            result |= VK_ACCESS_INDIRECT_COMMAND_READ_BIT;
        if (hasFlag(flags, AccessFlags::INDEX_READ_BIT))
            result |= VK_ACCESS_INDEX_READ_BIT;
        if (hasFlag(flags, AccessFlags::VERTEX_ATTRIBUTE_READ_BIT))
            result |= VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT;
        if (hasFlag(flags, AccessFlags::UNIFORM_READ_BIT))
            result |= VK_ACCESS_UNIFORM_READ_BIT;
        if (hasFlag(flags, AccessFlags::INPUT_ATTACHMENT_READ_BIT))
            result |= VK_ACCESS_INPUT_ATTACHMENT_READ_BIT;
        if (hasFlag(flags, AccessFlags::SHADER_READ_BIT))
            result |= VK_ACCESS_SHADER_READ_BIT;
        if (hasFlag(flags, AccessFlags::SHADER_WRITE_BIT))
            result |= VK_ACCESS_SHADER_WRITE_BIT;
        if (hasFlag(flags, AccessFlags::COLOR_ATTACHMENT_READ_BIT))
            result |= VK_ACCESS_COLOR_ATTACHMENT_READ_BIT;
        if (hasFlag(flags, AccessFlags::COLOR_ATTACHMENT_WRITE_BIT))
            result |= VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
        if (hasFlag(flags, AccessFlags::DEPTH_STENCIL_ATTACHMENT_READ_BIT))
            result |= VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT;
        if (hasFlag(flags, AccessFlags::DEPTH_STENCIL_ATTACHMENT_WRITE_BIT))
            result |= VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
        if (hasFlag(flags, AccessFlags::TRANSFER_READ_BIT))
            result |= VK_ACCESS_TRANSFER_READ_BIT;
        if (hasFlag(flags, AccessFlags::TRANSFER_WRITE_BIT))
            result |= VK_ACCESS_TRANSFER_WRITE_BIT;
        if (hasFlag(flags, AccessFlags::HOST_READ_BIT))
            result |= VK_ACCESS_HOST_READ_BIT;
        if (hasFlag(flags, AccessFlags::HOST_WRITE_BIT))
            result |= VK_ACCESS_HOST_WRITE_BIT;
        if (hasFlag(flags, AccessFlags::MEMORY_READ_BIT))
            result |= VK_ACCESS_MEMORY_READ_BIT;
        if (hasFlag(flags, AccessFlags::MEMORY_WRITE_BIT))
            result |= VK_ACCESS_MEMORY_WRITE_BIT;

        if (hasFlag(flags, AccessFlags::TRANSFORM_FEEDBACK_WRITE_BIT_EXT))
            result |= VK_ACCESS_TRANSFORM_FEEDBACK_WRITE_BIT_EXT;
        if (hasFlag(flags,
                    AccessFlags::TRANSFORM_FEEDBACK_COUNTER_READ_BIT_EXT))
            result |= VK_ACCESS_TRANSFORM_FEEDBACK_COUNTER_READ_BIT_EXT;
        if (hasFlag(flags,
                    AccessFlags::TRANSFORM_FEEDBACK_COUNTER_WRITE_BIT_EXT))
            result |= VK_ACCESS_TRANSFORM_FEEDBACK_COUNTER_WRITE_BIT_EXT;
        if (hasFlag(flags, AccessFlags::CONDITIONAL_RENDERING_READ_BIT_EXT))
            result |= VK_ACCESS_CONDITIONAL_RENDERING_READ_BIT_EXT;
        if (hasFlag(flags,
                    AccessFlags::COLOR_ATTACHMENT_READ_NONCOHERENT_BIT_EXT))
            result |= VK_ACCESS_COLOR_ATTACHMENT_READ_NONCOHERENT_BIT_EXT;
        if (hasFlag(flags, AccessFlags::ACCELERATION_STRUCTURE_READ_BIT_KHR))
            result |= VK_ACCESS_ACCELERATION_STRUCTURE_READ_BIT_KHR;
        if (hasFlag(flags, AccessFlags::ACCELERATION_STRUCTURE_WRITE_BIT_KHR))
            result |= VK_ACCESS_ACCELERATION_STRUCTURE_WRITE_BIT_KHR;
        if (hasFlag(flags, AccessFlags::FRAGMENT_DENSITY_MAP_READ_BIT_EXT))
            result |= VK_ACCESS_FRAGMENT_DENSITY_MAP_READ_BIT_EXT;
        if (hasFlag(flags,
                    AccessFlags::FRAGMENT_SHADING_RATE_ATTACHMENT_READ_BIT_KHR))
            result |= VK_ACCESS_FRAGMENT_SHADING_RATE_ATTACHMENT_READ_BIT_KHR;
        if (hasFlag(flags, AccessFlags::COMMAND_PREPROCESS_READ_BIT_NV))
            result |= VK_ACCESS_COMMAND_PREPROCESS_READ_BIT_NV;
        if (hasFlag(flags, AccessFlags::COMMAND_PREPROCESS_WRITE_BIT_NV))
            result |= VK_ACCESS_COMMAND_PREPROCESS_WRITE_BIT_NV;
    }
    return result;
}
inline VkPipelineStageFlags mapVkPipelineStage(PipelineStage flags) {
    VkPipelineStageFlags result = 0;
    {
        if (hasFlag(flags, PipelineStage::TOP_OF_PIPE_BIT))
            result |= VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        if (hasFlag(flags, PipelineStage::DRAW_INDIRECT_BIT))
            result |= VK_PIPELINE_STAGE_DRAW_INDIRECT_BIT;
        if (hasFlag(flags, PipelineStage::VERTEX_INPUT_BIT))
            result |= VK_PIPELINE_STAGE_VERTEX_INPUT_BIT;
        if (hasFlag(flags, PipelineStage::VERTEX_SHADER_BIT))
            result |= VK_PIPELINE_STAGE_VERTEX_SHADER_BIT;
        if (hasFlag(flags, PipelineStage::TESSELLATION_CONTROL_SHADER_BIT))
            result |= VK_PIPELINE_STAGE_TESSELLATION_CONTROL_SHADER_BIT;
        if (hasFlag(flags, PipelineStage::TESSELLATION_EVALUATION_SHADER_BIT))
            result |= VK_PIPELINE_STAGE_TESSELLATION_EVALUATION_SHADER_BIT;
        if (hasFlag(flags, PipelineStage::GEOMETRY_SHADER_BIT))
            result |= VK_PIPELINE_STAGE_GEOMETRY_SHADER_BIT;
        if (hasFlag(flags, PipelineStage::FRAGMENT_SHADER_BIT))
            result |= VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
        if (hasFlag(flags, PipelineStage::EARLY_FRAGMENT_TESTS_BIT))
            result |= VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
        if (hasFlag(flags, PipelineStage::LATE_FRAGMENT_TESTS_BIT))
            result |= VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
        if (hasFlag(flags, PipelineStage::COLOR_ATTACHMENT_OUTPUT_BIT))
            result |= VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        if (hasFlag(flags, PipelineStage::COMPUTE_SHADER_BIT))
            result |= VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT;
        if (hasFlag(flags, PipelineStage::TRANSFER_BIT))
            result |= VK_PIPELINE_STAGE_TRANSFER_BIT;
        if (hasFlag(flags, PipelineStage::BOTTOM_OF_PIPE_BIT))
            result |= VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
        if (hasFlag(flags, PipelineStage::HOST_BIT))
            result |= VK_PIPELINE_STAGE_HOST_BIT;
        if (hasFlag(flags, PipelineStage::ALL_GRAPHICS_BIT))
            result |= VK_PIPELINE_STAGE_ALL_GRAPHICS_BIT;
        if (hasFlag(flags, PipelineStage::ALL_COMMANDS_BIT))
            result |= VK_PIPELINE_STAGE_ALL_COMMANDS_BIT;
        if (hasFlag(flags, PipelineStage::TRANSFORM_FEEDBACK_BIT_EXT))
            result |= VK_PIPELINE_STAGE_TRANSFORM_FEEDBACK_BIT_EXT;
        if (hasFlag(flags, PipelineStage::CONDITIONAL_RENDERING_BIT_EXT))
            result |= VK_PIPELINE_STAGE_CONDITIONAL_RENDERING_BIT_EXT;
        if (hasFlag(flags, PipelineStage::ACCELERATION_STRUCTURE_BUILD_BIT_KHR))
            result |= VK_PIPELINE_STAGE_ACCELERATION_STRUCTURE_BUILD_BIT_KHR;
        if (hasFlag(flags, PipelineStage::RAY_TRACING_SHADER_BIT_KHR))
            result |= VK_PIPELINE_STAGE_RAY_TRACING_SHADER_BIT_KHR;
        if (hasFlag(flags, PipelineStage::FRAGMENT_DENSITY_PROCESS_BIT_EXT))
            result |= VK_PIPELINE_STAGE_FRAGMENT_DENSITY_PROCESS_BIT_EXT;
        if (hasFlag(flags,
                    PipelineStage::FRAGMENT_SHADING_RATE_ATTACHMENT_BIT_KHR))
            result |=
                VK_PIPELINE_STAGE_FRAGMENT_SHADING_RATE_ATTACHMENT_BIT_KHR;
        if (hasFlag(flags, PipelineStage::COMMAND_PREPROCESS_BIT_NV))
            result |= VK_PIPELINE_STAGE_COMMAND_PREPROCESS_BIT_NV;
        if (hasFlag(flags, PipelineStage::TASK_SHADER_BIT_EXT))
            result |= VK_PIPELINE_STAGE_TASK_SHADER_BIT_EXT;
        if (hasFlag(flags, PipelineStage::MESH_SHADER_BIT_EXT))
            result |= VK_PIPELINE_STAGE_MESH_SHADER_BIT_EXT;
    }
    return result;
}

inline VkImageViewType mapVkImageViewType(TextureType type) {
    switch (type) {
        case TextureType::TEX1D:
            return VK_IMAGE_VIEW_TYPE_1D;
        case TextureType::TEX1D_ARRAY:
            return VK_IMAGE_VIEW_TYPE_1D_ARRAY;
        case TextureType::TEX2D:
            return VK_IMAGE_VIEW_TYPE_2D;
        case TextureType::TEX2D_ARRAY:
            return VK_IMAGE_VIEW_TYPE_2D_ARRAY;
        case TextureType::TEX3D:
            return VK_IMAGE_VIEW_TYPE_3D;
        case TextureType::CUBE:
            return VK_IMAGE_VIEW_TYPE_CUBE;
    }

    CCERROR("unsupport texture type:%d", toNumber(type));
    return VK_IMAGE_VIEW_TYPE_2D;
}

inline VkImageUsageFlags mapVkImageUsage(TextureUsage usage) {
    VkImageUsageFlags flags = 0U;
    if (hasFlag(usage, TextureUsage::USAGE_TRANSFER_SRC_BIT))
        flags |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
    if (hasFlag(usage, TextureUsage::USAGE_TRANSFER_DST_BIT))
        flags |= VK_IMAGE_USAGE_TRANSFER_DST_BIT;
    if (hasFlag(usage, TextureUsage::USAGE_SAMPLED_BIT))
        flags |= VK_IMAGE_USAGE_SAMPLED_BIT;
    if (hasFlag(usage, TextureUsage::USAGE_STORAGE_BIT))
        flags |= VK_IMAGE_USAGE_STORAGE_BIT;
    if (hasFlag(usage, TextureUsage::USAGE_COLOR_ATTACHMENT_BIT))
        flags |= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    if (hasFlag(usage, TextureUsage::USAGE_DEPTH_STENCIL_ATTACHMENT_BIT))
        flags |= VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
    if (hasFlag(usage, TextureUsage::USAGE_TRANSIENT_ATTACHMENT_BIT))
        flags |= VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT;
    if (hasFlag(usage, TextureUsage::USAGE_INPUT_ATTACHMENT_BIT))
        flags |= VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT;

    return flags;
}

inline VkDependencyFlags mapVkDependencyFlags(DependencyFlags flags) {
    VkDependencyFlags result = 0;
    {
        if (hasFlag(flags, DependencyFlags::DEPENDENCY_BY_REGION_BIT))
            result |= VK_DEPENDENCY_BY_REGION_BIT;
        if (hasFlag(flags, DependencyFlags::DEPENDENCY_DEVICE_GROUP_BIT))
            result |= VK_DEPENDENCY_DEVICE_GROUP_BIT;
        if (hasFlag(flags, DependencyFlags::DEPENDENCY_VIEW_LOCAL_BIT))
            result |= VK_DEPENDENCY_VIEW_LOCAL_BIT;
        if (hasFlag(flags, DependencyFlags::DEPENDENCY_FEEDBACK_LOOP_BIT_EXT))
            result |= VK_DEPENDENCY_FEEDBACK_LOOP_BIT_EXT;
    }

    return result;
}
inline uint32_t mapVkFormatSize(VkFormat format) { return 0; }
inline VkPrimitiveTopology mapVkPrimitiveType(PrimitiveType type) {
    switch (type) {
        case PrimitiveType::POINT_LIST: {
            return VK_PRIMITIVE_TOPOLOGY_POINT_LIST;
        }
        case PrimitiveType::LINE_LIST: {
            return VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
        }
        case PrimitiveType::LINE_STRIP: {
            return VK_PRIMITIVE_TOPOLOGY_LINE_STRIP;
        }
        case PrimitiveType::TRIANGLE_LIST: {
            return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        }
        case PrimitiveType::TRIANGLE_STRIP: {
            return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
        }
        case PrimitiveType::TRIANGLE_FAN: {
            return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_FAN;
        }
        case PrimitiveType::LINE_LIST_WITH_ADJACENCY: {
            return VK_PRIMITIVE_TOPOLOGY_LINE_LIST_WITH_ADJACENCY;
        }
        case PrimitiveType::LINE_STRIP_WITH_ADJACENCY: {
            return VK_PRIMITIVE_TOPOLOGY_LINE_STRIP_WITH_ADJACENCY;
        }
        case PrimitiveType::TRIANGLE_LIST_WITH_ADJACENCY: {
            return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST_WITH_ADJACENCY;
        }
        case PrimitiveType::TRIANGLE_STRIP_WITH_ADJACENCY: {
            return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP_WITH_ADJACENCY;
        }
        case PrimitiveType::PATCH_LIST: {
            return VK_PRIMITIVE_TOPOLOGY_PATCH_LIST;
        }
        default: {
            CCERROR("unsupport PrimitiveType :%d", toNumber(type));
            return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        }
    }
}
inline VkFilter mapVkFilter(Filter filter) {
    switch (filter) {
        case Filter::POINT:
            return VK_FILTER_NEAREST;
        case Filter::LINEAR:
            return VK_FILTER_LINEAR;
        default: {
            CCERROR("unsupport Filter :%d", toNumber(filter));
            return VK_FILTER_LINEAR;
        }
    }
}
inline VkSamplerAddressMode mapVkSamplerAddressModel(Address address) {
    switch (address) {
        case Address::WRAP:
            return VK_SAMPLER_ADDRESS_MODE_REPEAT;
        case Address::MIRROR:
            return VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT;
        case Address::CLAMP:
            return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
        case Address::BORDER:
            return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
        default: {
            CCERROR("unsupport Address :%d", toNumber(address));
            return VK_SAMPLER_ADDRESS_MODE_REPEAT;
        }
    }
}

inline VkCompareOp mapVkCompareOp(CompareOp flag) {
    switch (flag) {
        case CompareOp::NEVER:
            return VK_COMPARE_OP_NEVER;
        case CompareOp::LESS:
            return VK_COMPARE_OP_LESS;
        case CompareOp::EQUAL:
            return VK_COMPARE_OP_EQUAL;
        case CompareOp::LESS_OR_EQUAL:
            return VK_COMPARE_OP_LESS_OR_EQUAL;
        case CompareOp::GREATER:
            return VK_COMPARE_OP_GREATER;
        case CompareOp::NOT_EQUAL:
            return VK_COMPARE_OP_NOT_EQUAL;
        case CompareOp::GREATER_OR_EQUAL:
            return VK_COMPARE_OP_GREATER_OR_EQUAL;
        case CompareOp::ALWAYS:
            return VK_COMPARE_OP_ALWAYS;
        default: {
            CCERROR("unsupport compareop:%d", toNumber(flag));
            return VK_COMPARE_OP_NEVER;
        }
    }

    return VK_COMPARE_OP_NEVER;
}

inline VkImageLayout mapVkImageLayout(ImageLayout layout) {
    switch (layout) {
        case ImageLayout::UNDEFINED: {
            return VK_IMAGE_LAYOUT_UNDEFINED;
        }
        case ImageLayout::GENERAL: {
            return VK_IMAGE_LAYOUT_GENERAL;
        }
        case ImageLayout::COLOR_ATTACHMENT_OPTIMAL: {
            return VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        }
        case ImageLayout::DEPTH_STENCIL_ATTACHMENT_OPTIMAL: {
            return VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
        }
        case ImageLayout::DEPTH_STENCIL_READ_ONLY_OPTIMAL: {
            return VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;
        }
        case ImageLayout::SHADER_READ_ONLY_OPTIMAL: {
            return VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        }
        case ImageLayout::TRANSFER_SRC_OPTIMAL: {
            return VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
        }
        case ImageLayout::TRANSFER_DST_OPTIMAL: {
            return VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
        }
        case ImageLayout::PREINITIALIZED: {
            return VK_IMAGE_LAYOUT_PREINITIALIZED;
        }
        case ImageLayout::DEPTH_READ_ONLY_STENCIL_ATTACHMENT_OPTIMAL: {
            return VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_STENCIL_ATTACHMENT_OPTIMAL;
        }
        case ImageLayout::DEPTH_ATTACHMENT_STENCIL_READ_ONLY_OPTIMAL: {
            return VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_STENCIL_READ_ONLY_OPTIMAL;
        }
        case ImageLayout::DEPTH_ATTACHMENT_OPTIMAL: {
            return VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL;
        }
        case ImageLayout::DEPTH_READ_ONLY_OPTIMAL: {
            return VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_OPTIMAL;
        }
        case ImageLayout::STENCIL_ATTACHMENT_OPTIMAL: {
            return VK_IMAGE_LAYOUT_STENCIL_ATTACHMENT_OPTIMAL;
        }
        case ImageLayout::STENCIL_READ_ONLY_OPTIMAL: {
            return VK_IMAGE_LAYOUT_STENCIL_READ_ONLY_OPTIMAL;
        }
        case ImageLayout::PRESENT_SRC_KHR: {
            return VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
        }
        default: {
            CCERROR("unsupport ImageLayout:%d", toNumber(layout));
            return VK_IMAGE_LAYOUT_UNDEFINED;
        }
    }
}

inline VkStencilOp mapVkStencilOp(StencilOp op) {
    switch (op) {
        case StencilOp::KEEP: {
            return VK_STENCIL_OP_KEEP;
        }
        case StencilOp::ZERO: {
            return VK_STENCIL_OP_ZERO;
        }
        case StencilOp::REPLACE: {
            return VK_STENCIL_OP_REPLACE;
        }
        case StencilOp::INCREMENT_AND_CLAMP: {
            return VK_STENCIL_OP_INCREMENT_AND_CLAMP;
        }
        case StencilOp::DECREMENT_AND_CLAMP: {
            return VK_STENCIL_OP_DECREMENT_AND_CLAMP;
        }
        case StencilOp::INVERT: {
            return VK_STENCIL_OP_INVERT;
        }
        case StencilOp::INCREMENT_AND_WRAP: {
            return VK_STENCIL_OP_INCREMENT_AND_WRAP;
        }
        case StencilOp::DECREMENT_AND_WRAP: {
            return VK_STENCIL_OP_DECREMENT_AND_WRAP;
        }
        default: {
            CCERROR("unsupport StencilOp:%d", toNumber(op));
            return VK_STENCIL_OP_KEEP;
        }
    }
}

inline VkPolygonMode mapVkPolygonModel(PolygonModel model) {
    switch (model) {
        case PolygonModel::FILL: {
            return VK_POLYGON_MODE_FILL;
        }
        case PolygonModel::LINE: {
            return VK_POLYGON_MODE_LINE;
        }
        case PolygonModel::POINT: {
            return VK_POLYGON_MODE_POINT;
        }
        default: {
            CCERROR("unsupport PolygonModel:%d", toNumber(model));
            return VK_POLYGON_MODE_LINE;
        }
    }
}

inline VkCullModeFlags mapVkCullModel(CullModel model) {
    switch (model) {
        case CullModel::NONE: {
            return VK_CULL_MODE_NONE;
        }
        case CullModel::FRONT: {
            return VK_CULL_MODE_FRONT_BIT;
        }
        case CullModel::BACK: {
            return VK_CULL_MODE_BACK_BIT;
        }
        case CullModel::FRONT_AND_BACK: {
            return VK_CULL_MODE_FRONT_AND_BACK;
        }
        default: {
            CCERROR("unsupport CullModel:%d", toNumber(model));
            return VK_CULL_MODE_NONE;
        }
    }
}
inline VkFrontFace mapVkFrontFace(FrontFace face) {
    switch (face) {
        case FrontFace::COUNTER_CLOCKWISE: {
            return VK_FRONT_FACE_COUNTER_CLOCKWISE;
        }
        case FrontFace::CLOCKWISE: {
            return VK_FRONT_FACE_CLOCKWISE;
        }
        default: {
            CCERROR("unsupport FrontFace:%d", toNumber(face));
            return VK_FRONT_FACE_COUNTER_CLOCKWISE;
        }
    }
}

inline VkSampleCountFlagBits mapVkSampleCount(SampleCount count) {
    switch (count) {
        case SampleCount::SAMPLE_COUNT_1_BIT: {
            return VK_SAMPLE_COUNT_1_BIT;
        }
        case SampleCount::SAMPLE_COUNT_2_BIT: {
            return VK_SAMPLE_COUNT_2_BIT;
        }
        case SampleCount::SAMPLE_COUNT_4_BIT: {
            return VK_SAMPLE_COUNT_4_BIT;
        }
        case SampleCount::SAMPLE_COUNT_8_BIT: {
            return VK_SAMPLE_COUNT_8_BIT;
        }
        case SampleCount::SAMPLE_COUNT_16_BIT: {
            return VK_SAMPLE_COUNT_16_BIT;
        }
        case SampleCount::SAMPLE_COUNT_32_BIT: {
            return VK_SAMPLE_COUNT_32_BIT;
        }
        case SampleCount::SAMPLE_COUNT_64_BIT: {
            return VK_SAMPLE_COUNT_64_BIT;
        }
        default: {
            CCERROR("unsupport SampleCount:%d", toNumber(count));
            return VK_SAMPLE_COUNT_1_BIT;
        }
    }
}

inline VkBlendFactor mapVkBlendFactor(BlendFactor factor) {
    switch (factor) {
        case BlendFactor::ZERO: {
            return VK_BLEND_FACTOR_ZERO;
        }
        case BlendFactor::ONE: {
            return VK_BLEND_FACTOR_ONE;
        }
        case BlendFactor::SRC_COLOR: {
            return VK_BLEND_FACTOR_SRC1_COLOR;
        }
        case BlendFactor::ONE_MINUS_SRC_COLOR: {
            return VK_BLEND_FACTOR_ONE_MINUS_SRC_COLOR;
        }
        case BlendFactor::DST_COLOR: {
            return VK_BLEND_FACTOR_DST_COLOR;
        }
        case BlendFactor::ONE_MINUS_DST_COLOR: {
            return VK_BLEND_FACTOR_ONE_MINUS_DST_COLOR;
        }
        case BlendFactor::SRC_ALPHA: {
            return VK_BLEND_FACTOR_SRC_ALPHA;
        }
        case BlendFactor::ONE_MINUS_SRC_ALPHA: {
            return VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
        }
        case BlendFactor::DST_ALPHA: {
            return VK_BLEND_FACTOR_DST_ALPHA;
        }
        case BlendFactor::ONE_MINUS_DST_ALPHA: {
            return VK_BLEND_FACTOR_ONE_MINUS_DST_ALPHA;
        }
        case BlendFactor::CONSTANT_COLOR: {
            return VK_BLEND_FACTOR_CONSTANT_COLOR;
        }
        case BlendFactor::ONE_MINUS_CONSTANT_COLOR: {
            return VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_COLOR;
        }
        case BlendFactor::CONSTANT_ALPHA: {
            return VK_BLEND_FACTOR_CONSTANT_ALPHA;
        }
        case BlendFactor::ONE_MINUS_CONSTANT_ALPHA: {
            return VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_ALPHA;
        }
        default: {
            CCERROR("unsupport BlendFactor:%d", toNumber(factor));
            return VK_BLEND_FACTOR_ONE_MINUS_SRC_COLOR;
        }
    }
}
inline VkBlendOp mapVkBlendOp(BlendOp op) {
    switch (op) {
        case BlendOp::ADD: {
            return VK_BLEND_OP_ADD;
        }
        case BlendOp::SUBTRACT: {
            return VK_BLEND_OP_SUBTRACT;
        }
        case BlendOp::REVERSE_SUBTRACT: {
            return VK_BLEND_OP_REVERSE_SUBTRACT;
        }
        default: {
            CCERROR("unsupport BlendOp:%d", toNumber(op));
            return VK_BLEND_OP_ADD;
        }
    }
}
inline VkLogicOp mapVkBlendLogicOp(BlendLogicOp op) {
    switch (op) {
        case BlendLogicOp::CLEAR: {
            return VK_LOGIC_OP_CLEAR;
        }
        case BlendLogicOp::AND: {
            return VK_LOGIC_OP_AND;
        }
        case BlendLogicOp::AND_REVERSE: {
            return VK_LOGIC_OP_AND_REVERSE;
        }
        case BlendLogicOp::COPY: {
            return VK_LOGIC_OP_COPY;
        }
        case BlendLogicOp::AND_INVERTED: {
            return VK_LOGIC_OP_AND_INVERTED;
        }
        case BlendLogicOp::NO_OP: {
            return VK_LOGIC_OP_NO_OP;
        }
        case BlendLogicOp::XOR: {
            return VK_LOGIC_OP_XOR;
        }
        case BlendLogicOp::OR: {
            return VK_LOGIC_OP_OR;
        }
        case BlendLogicOp::NOR: {
            return VK_LOGIC_OP_NOR;
        }
        case BlendLogicOp::EQUIVALENT: {
            return VK_LOGIC_OP_EQUIVALENT;
        }
        case BlendLogicOp::INVERT: {
            return VK_LOGIC_OP_INVERT;
        }
        case BlendLogicOp::OR_REVERSE: {
            return VK_LOGIC_OP_OR_REVERSE;
        }
        case BlendLogicOp::COPY_INVERTED: {
            return VK_LOGIC_OP_COPY_INVERTED;
        }
        case BlendLogicOp::OR_INVERTED: {
            return VK_LOGIC_OP_OR_INVERTED;
        }
        case BlendLogicOp::NAND: {
            return VK_LOGIC_OP_NAND;
        }
        case BlendLogicOp::SET: {
            return VK_LOGIC_OP_SET;
        }
        default: {
            CCERROR("unsupport BlendOp:%d", toNumber(op));
            return VK_LOGIC_OP_COPY;
        }
    }
}

inline VkSamplerMipmapMode mapVkMipmapModel(Filter filter) {
    switch (filter) {
        case Filter::POINT:
            return VK_SAMPLER_MIPMAP_MODE_NEAREST;
        case Filter::LINEAR:
            return VK_SAMPLER_MIPMAP_MODE_LINEAR;
        default: {
            CCERROR("unsupport mipMap Model :%d", filter);
            return VK_SAMPLER_MIPMAP_MODE_NEAREST;
        }
    }
}

inline VkBorderColor getBorderColor(SamplerBorderColor color) {
    switch (color) {
        case SamplerBorderColor::OPAQUA_BLACK: {
            return VkBorderColor::VK_BORDER_COLOR_FLOAT_OPAQUE_BLACK;
        }
        case SamplerBorderColor::OPQUA_WHITE: {
            return VkBorderColor::VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;
        }

        case SamplerBorderColor::TRANSPARENT_BLACK: {
            return VkBorderColor::VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK;
        }
    }

    return VkBorderColor::VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK;
}

END_VK_NAMESPACE END_GFX_NAMESPACE