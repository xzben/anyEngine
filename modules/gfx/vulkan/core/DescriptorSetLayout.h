#pragma once

#include "LogicDevice.h"
#include "vulkan_core_common.h"

BEGIN_GFX_NAMESPACE
BEGIN_VK_NAMESPACE

struct DescriptorBindingVarialble {
    std::string name;
    std::string type_name;
    uint32_t binding;
    uint32_t offset;
    uint32_t absolute_offset;
    uint32_t size;
    uint32_t padded_size;
    std::vector<DescriptorBindingVarialble> members;
};

struct DescriptorBindingTexture {
    std::string name;
};

struct DescriptorBinding  // block
{
    uint32_t binding;
    VkDescriptorType descriptorType;
    uint32_t descriptorCount;
    VkShaderStageFlags stageFlags;

    DescriptorBindingVarialble block;
    DescriptorBindingTexture textures;
};

struct VertexInputBinding {
    std::string name;
    uint32_t location;
    VkFormat format;
};

struct DescriptorSetLayoutInfo {
    uint32_t index;
    std::vector<DescriptorBinding> bindings;
};

class DescriptorSetLayout {
public:
    DescriptorSetLayout(const LogicDevice& device,
                        std::vector<VkDescriptorSetLayoutBinding> bindings);
    ~DescriptorSetLayout();

    DescriptorSetLayout(const DescriptorSetLayout& rhs)            = default;
    DescriptorSetLayout& operator=(const DescriptorSetLayout& rhs) = default;

    DescriptorSetLayout(DescriptorSetLayout&& rhs);
    DescriptorSetLayout& operator=(DescriptorSetLayout&& rhs);

    const std::vector<VkDescriptorSetLayoutBinding>& bindings() const {
        return m_bindings;
    }

    const std::vector<VkDescriptorBindingFlagsEXT>& binding_flags() const {
        return m_bindingFlags;
    }

    operator bool() const { return m_handle != VK_NULL_HANDLE; }
    operator VkDescriptorSetLayout() const { return m_handle; }

private:
    const LogicDevice* m_logicDevice;
    std::vector<VkDescriptorSetLayoutBinding> m_bindings;
    std::vector<VkDescriptorBindingFlagsEXT> m_bindingFlags;
    VkDescriptorSetLayout m_handle{VK_NULL_HANDLE};
};

END_VK_NAMESPACE
END_GFX_NAMESPACE