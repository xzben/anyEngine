#pragma once

#include "vulkan_core_common.h"

BEGIN_GFX_NAMESPACE
BEGIN_VK_NAMESPACE

extern inline VkFormat mapVkFormat(DataFormat format);
extern inline uint32_t mapVkFormatSize(DataFormat format);
extern inline VkFormat mapVkFormat(PixelFormat format);
extern inline uint32_t mapVkFormatSize(VkFormat format);
extern inline VkFilter mapVkFilter(Filter filter);
extern inline VkAttachmentLoadOp mapVkLoadOp(LoadOp op);
extern inline VkAttachmentStoreOp mapVkStoreOp(StoreOp op);
extern inline VkPipelineStageFlags mapVkPipelineStage(PipelineStage stage);
extern inline VkAccessFlags mapVkAccessFlags(AccessFlags access);
extern inline VkDependencyFlags mapVkDependencyFlags(DependencyFlags flags);
extern inline VkImageUsageFlags mapVkImageUsage(TextureUsage usage);
extern inline VkImageViewType mapVkImageViewType(TextureType type);
extern inline VkPrimitiveTopology mapVkPrimitiveType(PrimitiveType type);
extern inline VkSamplerAddressMode mapVkSamplerAddressModel(Address address);
extern inline VkPolygonMode mapVkPolygonModel(PolygonModel model);
extern inline VkCullModeFlags mapVkCullModel(CullModel model);
extern inline VkFrontFace mapVkFrontFace(FrontFace face);
extern inline VkBlendFactor mapVkBlendFactor(BlendFactor factor);
extern inline VkBlendOp mapVkBlendOp(BlendOp op);
extern inline VkLogicOp mapVkBlendLogicOp(BlendLogicOp op);
extern inline VkCompareOp mapVkCompareOp(CompareOp op);
extern inline VkStencilOp mapVkStencilOp(StencilOp op);
extern inline VkImageLayout mapVkImageLayout(ImageLayout layout);
extern inline VkSampleCountFlagBits mapVkSampleCount(SampleCount count);
extern inline VkSamplerMipmapMode mapVkMipmapModel(Filter filter);
extern inline VkBorderColor getBorderColor(SamplerBorderColor color);

extern inline bool isDepthOnlyFormat(VkFormat format);
extern inline bool isDepthStencilFormat(VkFormat format);

END_VK_NAMESPACE END_GFX_NAMESPACE