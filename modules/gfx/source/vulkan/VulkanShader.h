#pragma once

#include "base/Shader.h"
#include "core/vulkan_core.h"
#include "vulkan_common.h"
BEGIN_GFX_NAMESPACE

class ShaderModule {
public:
    ShaderModule(const vk::LogicDevice& device,
                 const std::vector<uint8_t>& code,
                 std::string entryName = "main");
    ShaderModule(const vk::LogicDevice& device, const uint8_t* code,
                 uint32_t size, std::string entryName = "main");
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

    VkPipelineShaderStageCreateInfo getStageInfo();

protected:
    bool reflect(const uint8_t* code, uint32_t size);

private:
    const vk::LogicDevice& m_logicDevice;
    VkShaderModule m_handle{VK_NULL_HANDLE};
    std::string m_entryName;
    VkShaderStageFlagBits m_stage;
    std::vector<vk::DescriptorSetLayoutInfo> m_setLayouts;
    std::vector<vk::VertexInputBinding> m_inputBindings;
};

class VulkanShader : public Shader {
public:
    VulkanShader(const vk::LogicDevice& device);

    bool addStage(const std::vector<uint8_t>& code, gfx::ShaderStage stage,
                  const std::string& entryName = "main");

    bool addStage(const uint8_t* code, uint32_t size, gfx::ShaderStage stage,
                  const std::string& entryName = "main");

    virtual bool build() override;

    void getDescriptorSetBindings(
        uint32_t set_index,
        std::vector<VkDescriptorSetLayoutBinding>& layout_bindings);

    const std::vector<vk::DescriptorSetLayout>& descriptorSetLayouts() const {
        return m_layoutLists;
    }

    const vk::PipelineLayout& getPipelineLayout() const {
        return *m_pipelineLayout;
    }

    const std::vector<vk::VertexInputBinding>& getVertexInputBingdings() const {
        return m_inputBindings;
    }
    const std::vector<vk::DescriptorSetLayoutInfo>& getDescriptorLayoutSets()
        const {
        return m_setLayouts;
    }

    const std::vector<VkPipelineShaderStageCreateInfo>& getStageInfos() const {
        return m_stageInfos;
    }

    const std::vector<VkVertexInputBindingDescription>&
    getVkVertexBindingDescriptions() const {
        return m_vertexBindings;
    }

    const std::vector<VkVertexInputAttributeDescription>&
    getVkVertexAttributeDescriptions() const {
        return m_vertexAttributes;
    }

protected:
    const std::vector<ShaderModule*>& getModules() { return m_modules; }
    virtual GFX_HANDLE getHandleImp() const { return 0; }

protected:
    const vk::DescriptorSetLayout& createDescriptorSetLayout(
        uint32_t set_index);
    bool buildLayoutSet();

private:
    const vk::LogicDevice& m_logicDevice;
    std::vector<ShaderModule*> m_modules;
    std::vector<vk::DescriptorSetLayoutInfo> m_setLayouts;
    std::unordered_map<uint32_t, vk::DescriptorSetLayout> m_layouts;
    std::vector<vk::DescriptorSetLayout> m_layoutLists;

    std::unique_ptr<vk::PipelineLayout> m_pipelineLayout;

    std::vector<VkPipelineShaderStageCreateInfo> m_stageInfos;
    std::vector<vk::VertexInputBinding> m_inputBindings;
    std::vector<VkVertexInputBindingDescription> m_vertexBindings;
    std::vector<VkVertexInputAttributeDescription> m_vertexAttributes;
};
END_GFX_NAMESPACE