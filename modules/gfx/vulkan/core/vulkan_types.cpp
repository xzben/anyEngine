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

std::unordered_map<Format, FormatInfo> g_formats = {
    {Format::Bool, {VK_FORMAT_R32_SINT, 4}},
    {Format::UInt, {VK_FORMAT_R32_UINT, 4}},
    {Format::Int, {VK_FORMAT_R32_SINT, 4}},
    {Format::Float, {VK_FORMAT_R32_SFLOAT, 4}},

    {Format::Bool2, {VK_FORMAT_R32G32_SINT, 4 * 2}},
    {Format::Int2, {VK_FORMAT_R32G32_SINT, 4 * 2}},
    {Format::UInt2, {VK_FORMAT_R32G32_UINT, 4 * 2}},
    {Format::Float2, {VK_FORMAT_R32G32_SFLOAT, 4 * 2}},

    {Format::Bool3, {VK_FORMAT_R32G32B32_SINT, 4 * 3}},
    {Format::Int3, {VK_FORMAT_R32G32B32_SINT, 4 * 3}},
    {Format::UInt3, {VK_FORMAT_R32G32B32_UINT, 4 * 3}},
    {Format::Float3, {VK_FORMAT_R32G32B32_SFLOAT, 4 * 3}},

    {Format::Bool4, {VK_FORMAT_R32G32B32A32_SINT, 4 * 4}},
    {Format::Int4, {VK_FORMAT_R32G32B32A32_SINT, 4 * 4}},
    {Format::UInt4, {VK_FORMAT_R32G32B32A32_UINT, 4 * 4}},
    {Format::Float4, {VK_FORMAT_R32G32B32A32_SFLOAT, 4 * 4}},
};

inline VkFormat mapVkFormat(Format format) {
    VkFormat ret = VK_FORMAT_UNDEFINED;
    auto it      = g_formats.find(format);
    if (it != g_formats.end()) {
        ret = it->second.vkFormat;
    }

    CCASSERT(false, "unnknow format:%d", format);

    return ret;
}

inline uint32_t mapVkFormatSize(Format format) {
    uint32_t ret = 0;
    auto it      = g_formats.find(format);
    if (it != g_formats.end()) {
        ret = it->second.size;
    }

    CCASSERT(false, "unnknow format:%d", format);

    return ret;
}

inline VkFilter mapVkFilter(Filter filter) {
    switch (filter) {
        case Filter::POINT:
            return VK_FILTER_NEAREST;
        case Filter::LINEAR:
            return VK_FILTER_LINEAR;
        default: {
            CCERROR("unsupport filter :%d", filter);
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
            CCERROR("unsupport address mode :%d", address);
            return VK_SAMPLER_ADDRESS_MODE_REPEAT;
        }
    }
}

inline VkCompareOp mapVkCompareOp(CompareOpFlag flag) {
    switch (flag) {
        case CompareOpFlag::NEVER:
            return VK_COMPARE_OP_NEVER;
        case CompareOpFlag::LESS:
            return VK_COMPARE_OP_LESS;
        case CompareOpFlag::EQUAL:
            return VK_COMPARE_OP_EQUAL;
        case CompareOpFlag::LESS_OR_EQUAL:
            return VK_COMPARE_OP_LESS_OR_EQUAL;
        case CompareOpFlag::GREATER:
            return VK_COMPARE_OP_GREATER;
        case CompareOpFlag::NOT_EQUAL:
            return VK_COMPARE_OP_NOT_EQUAL;
        case CompareOpFlag::GREATER_OR_EQUAL:
            return VK_COMPARE_OP_GREATER_OR_EQUAL;
        case CompareOpFlag::ALWAYS:
            return VK_COMPARE_OP_ALWAYS;
        default: {
            CCERROR("unsupport compareop:%d", toNumber(flag));
            return VK_COMPARE_OP_NEVER;
        }
    }

    return VK_COMPARE_OP_NEVER;
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