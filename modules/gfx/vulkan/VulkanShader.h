#pragma once

#include "../base/Shader.h"
#include "core/vulkan_core.h"
#include "vulkan_common.h"
BEGIN_GFX_NAMESPACE

class ShaderModule {
public:
    ShaderModule(const vk::LogicDevice& device,
                 const std::vector<uint8_t>& code,
                 std::string entryName = "main");
    virtual ~ShaderModule();

    VkShaderStageFlagBits getStage() { return m_stage; }
    operator bool() const { return m_handle != VK_NULL_HANDLE; }
    operator VkShaderModule() const { return m_handle; }

    const std::vector<vk::VertexInputBinding>& getVertexInputBinding() const {
        return m_inputBindings;
    }

    const std::vector<vk::DescriptorSetLayoutInfo>& getDescriptorSetLayouts()
        const {
        return m_setLayouts;
    }

protected:
    bool reflect(const std::vector<uint8_t>& code);

private:
    const vk::LogicDevice& m_logicDevice;
    VkShaderModule m_handle{VK_NULL_HANDLE};
    std::string m_entryName;
    VkShaderStageFlagBits m_stage;
    std::vector<vk::DescriptorSetLayoutInfo> m_setLayouts;
    std::vector<vk::VertexInputBinding> m_inputBindings;
};

class VulkanShader : public Shader, public GfxObject {
public:
    VulkanShader(const vk::LogicDevice& device);

    bool addStage(const std::vector<uint8_t>& code,
                  std::string entryName = "main");

    const std::vector<ShaderModule*>& getModules() { return m_modules; }

    void descriptorSetBindings(
        uint32_t set_index,
        std::vector<VkDescriptorSetLayoutBinding>& layout_bindings);
    const vk::DescriptorSetLayout& descriptorSetLayout(uint32_t set_index);

    std::vector<vk::DescriptorSetLayout> descriptorSetLayouts();

    const vk::PipelineLayout& pipelineLayout();
    const std::vector<vk::VertexInputBinding>& vertexInputBingdings() const;
    const std::vector<vk::DescriptorSetLayoutInfo>& getDescriptorLayoutSets()
        const {
        return m_setLayouts;
    }

private:
    bool buildLayoutSet();

private:
    const vk::LogicDevice& m_logicDevice;
    std::vector<ShaderModule*> m_modules;
    std::vector<vk::DescriptorSetLayoutInfo> m_setLayouts;
    bool m_setLayoutDirty = false;

    std::unordered_map<uint32_t, vk::DescriptorSetLayout> m_layouts;
    std::unique_ptr<vk::PipelineLayout> m_pipelineLayout;

    std::vector<VkVertexInputBindingDescription> m_vertexBindings;
    std::vector<VkVertexInputAttributeDescription> m_vertexAttributes;
};
END_GFX_NAMESPACE