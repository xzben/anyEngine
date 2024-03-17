#pragma once

#include "vulkan_core_common.h"

BEGIN_GFX_NAMESPACE
BEGIN_VK_NAMESPACE

extern inline VkFormat mapVkFormat(Format format);

extern inline VkFilter mapVkFilter(Filter filter);
extern inline VkSamplerAddressMode mapVkSamplerAddressModel(Address address);

extern inline VkCompareOp mapVkCompareOp(CompareOpFlag flag);
extern inline VkSamplerMipmapMode mapVkMipmapModel(Filter filter);
extern inline VkBorderColor getBorderColor(SamplerBorderColor color);

END_VK_NAMESPACE END_GFX_NAMESPACE