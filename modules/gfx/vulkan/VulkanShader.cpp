#include "VulkanShader.h"

#include "spirv_reflect.h"

BEGIN_GFX_NAMESPACE

static void fillDescriptorBindingBlock(vk::DescriptorBindingVarialble& block,
                                       const SpvReflectBlockVariable& rblock) {
    block.name            = rblock.name == nullptr ? "" : rblock.name;
    block.type_name       = rblock.type_description->type_name == nullptr
                                ? ""
                                : rblock.type_description->type_name;
    block.size            = rblock.size;
    block.offset          = rblock.offset;
    block.absolute_offset = rblock.absolute_offset;
    block.padded_size     = rblock.padded_size;

    if (rblock.member_count > 0) {
        block.members.resize(rblock.member_count);
        for (auto i = 0; i < rblock.member_count; i++) {
            fillDescriptorBindingBlock(block.members[i], rblock.members[i]);
        }
    } else {
        block.members.clear();
    }
}

static void getVertexInputBingdings(
    const SpvReflectShaderModule& spvmodule,
    std::vector<vk::VertexInputBinding>& bindings) {
    if (spvmodule.shader_stage != VK_SHADER_STAGE_VERTEX_BIT) {
        return;
    }

    uint32_t count = 0;
    auto result = spvReflectEnumerateInputVariables(&spvmodule, &count, NULL);
    assert(result == SPV_REFLECT_RESULT_SUCCESS);

    std::vector<SpvReflectInterfaceVariable*> input_vars(count);
    result = spvReflectEnumerateInputVariables(&spvmodule, &count,
                                               input_vars.data());
    assert(result == SPV_REFLECT_RESULT_SUCCESS);

    bindings.resize(count);

    for (auto i = 0; i < count; i++) {
        auto p_var           = input_vars[i];
        bindings[i].location = p_var->location;
        bindings[i].name     = p_var->name == nullptr ? "" : p_var->name;
        bindings[i].format   = static_cast<VkFormat>(p_var->format);
    }

    std::sort(
        bindings.begin(), bindings.end(),
        [](const auto& l, const auto& r) { return l.location <= r.location; });
}

static void getDescriptorSets(
    const SpvReflectShaderModule& spvmodule,
    std::vector<vk::DescriptorSetLayoutInfo>& layout_sets) {
    uint32_t count = 0;
    auto result = spvReflectEnumerateDescriptorSets(&spvmodule, &count, NULL);
    assert(result == SPV_REFLECT_RESULT_SUCCESS);

    std::vector<SpvReflectDescriptorSet*> sets(count);
    result = spvReflectEnumerateDescriptorSets(&spvmodule, &count, sets.data());
    assert(result == SPV_REFLECT_RESULT_SUCCESS);

    auto transfrom_binding = [](VkShaderStageFlagBits stage,
                                vk::DescriptorBinding& layout_binding,
                                const SpvReflectDescriptorBinding* binding) {
        ;

        layout_binding.binding = binding->binding;
        layout_binding.descriptorType =
            static_cast<VkDescriptorType>(binding->descriptor_type);

        layout_binding.descriptorCount = 1;
        for (uint32_t i_dim = 0; i_dim < binding->array.dims_count; ++i_dim) {
            layout_binding.descriptorCount *= binding->array.dims[i_dim];
        }

        switch (layout_binding.descriptorType) {
            case VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE:
            case VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER: {
                layout_binding.textures.name =
                    binding->name == nullptr ? "" : binding->name;
                break;
            }

            case VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER: {
                fillDescriptorBindingBlock(layout_binding.block,
                                           binding->block);
                break;
            }
        }
        layout_binding.stageFlags = stage;
    };

    auto transfrom_set = [&transfrom_binding](
                             VkShaderStageFlagBits stage,
                             vk::DescriptorSetLayoutInfo& layout_set,
                             const SpvReflectDescriptorSet* set) {
        layout_set.bindings.resize(set->binding_count);
        for (auto i = 0; i < set->binding_count; ++i) {
            transfrom_binding(stage, layout_set.bindings[i], set->bindings[i]);
        }

        layout_set.index = set->set;
        std::sort(layout_set.bindings.begin(), layout_set.bindings.end(),
                  [](const auto& l, const auto& r) {
                      return l.binding <= r.binding;
                  });
    };
    auto stage = static_cast<VkShaderStageFlagBits>(spvmodule.shader_stage);
    layout_sets.resize(sets.size());
    for (auto i = 0; i < sets.size(); i++) {
        transfrom_set(stage, layout_sets[i], sets[i]);
    }
}

ShaderModule::ShaderModule(const vk::LogicDevice& device,
                           const std::vector<uint8_t>& code,
                           std::string entryName)
    : m_logicDevice(device), m_entryName(std::move(entryName)) {
    VkShaderModuleCreateInfo info{VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO};
    info.codeSize = code.size();
    info.pCode    = reinterpret_cast<const uint32_t*>(code.data());

    if (vkCreateShaderModule(device, &info, nullptr, &m_handle) != VK_SUCCESS) {
        CCASSERT(false, "create shader module failed");
        m_handle = VK_NULL_HANDLE;
    }

    reflect(code);
}

ShaderModule::~ShaderModule() {
    vkDestroyShaderModule(m_logicDevice, m_handle, nullptr);
}

bool ShaderModule::reflect(const std::vector<uint8_t>& code) {
    SpvReflectShaderModule spvmodule;

    auto result =
        spvReflectCreateShaderModule(code.size(), code.data(), &spvmodule);
    assert(result == SPV_REFLECT_RESULT_SUCCESS);

    m_stage     = static_cast<VkShaderStageFlagBits>(spvmodule.shader_stage);
    m_entryName = spvmodule.entry_point_name == nullptr
                      ? "main"
                      : spvmodule.entry_point_name;

    getVertexInputBingdings(spvmodule, m_inputBindings);
    getDescriptorSets(spvmodule, m_setLayouts);
    std::sort(m_setLayouts.begin(), m_setLayouts.end(),
              [](const auto& l, const auto& r) { return l.index <= r.index; });

    spvReflectDestroyShaderModule(&spvmodule);
    return true;
}

VulkanShader::VulkanShader(const vk::LogicDevice& device)
    : GfxObject(GfxObjectType::Shader), m_logicDevice(device) {}

bool VulkanShader::addStage(const std::vector<uint8_t>& code,
                            std::string entryName) {
    ShaderModule* shaderModule =
        new ShaderModule(m_logicDevice, code, entryName);

    if ((VkShaderModule)shaderModule == VK_NULL_HANDLE) {
        delete shaderModule;
        return false;
    }

    m_setLayoutDirty = true;
    m_modules.push_back(shaderModule);

    return true;
}

void VulkanShader::descriptorSetBindings(
    uint32_t set_index,
    std::vector<VkDescriptorSetLayoutBinding>& layout_bindings) {}
const vk::DescriptorSetLayout& VulkanShader::descriptorSetLayout(
    uint32_t set_index) {
    buildLayoutSet();

    if (auto it = m_layouts.find(set_index); it != m_layouts.end()) {
        return it->second;
    }

    std::vector<VkDescriptorSetLayoutBinding> bindings;
    descriptorSetBindings(set_index, bindings);

    auto it = m_layouts.emplace(std::piecewise_construct,
                                std::forward_as_tuple(set_index),
                                std::forward_as_tuple(m_logicDevice, bindings));
    return it.first->second;
}

std::vector<VkDescriptorSetLayout> VulkanShader::descriptorSetLayouts() {}

const vk::PipelineLayout& VulkanShader::pipelineLayout() {
    buildLayoutSet();

    if (!m_pipelineLayout) {
        m_pipelineLayout =
            std::make_unique<vk::PipelineLayout>(m_logicDevice, m_setLayouts);
    }

    return *m_pipelineLayout;
}

const std::vector<vk::VertexInputBinding>& VulkanShader::vertexInputBingdings()
    const {
    for (auto& stagemodule : m_modules) {
        if (stagemodule->getStage() == VK_SHADER_STAGE_VERTEX_BIT) {
            return stagemodule->getVertexInputBinding();
        }
    }

    assert(false);
    static const std::vector<vk::VertexInputBinding> empty;
    return empty;
}

bool VulkanShader::buildLayoutSet() {
    if (!m_setLayoutDirty) return false;
    m_setLayoutDirty = false;
    m_setLayouts.clear();

    auto merge = [](auto& left, auto& right, auto cmp, auto domerge) {
        if (left.empty()) {
            left = right;
            return;
        }

        if (right.empty()) {
            return;
        }

        auto l = left.begin();
        auto r = right.begin();
        while (l < left.end() && r < right.end()) {
            auto cmp_result = cmp(*l, *r);
            if (cmp_result < 0) {
                ++l;
            } else if (cmp_result > 0) {
                l = left.insert(l, *r);
                ++l;
                ++r;
            } else {
                domerge(*l, *r);
                ++l;
                ++r;
            }

            if (l == left.end()) {
                while (r < right.end()) {
                    left.push_back(*r);
                    ++r;
                }
                break;
            }
        }
    };

    for (auto& mod : m_modules) {
        merge(
            m_setLayouts, mod->getDescriptorSetLayouts(),
            [](const vk::DescriptorSetLayoutInfo& l,
               const vk::DescriptorSetLayoutInfo& r) {
                if (l.index > r.index)
                    return 1;
                else if (l.index < r.index)
                    return -1;
                else
                    return 0;
            },
            [merge](const vk::DescriptorSetLayoutInfo& left,
                    const vk::DescriptorSetLayoutInfo& right) {
                merge(
                    left.bindings, right.bindings,
                    [](vk::DescriptorBinding& l, vk::DescriptorBinding& r) {
                        if (l.binding > r.binding) {
                            return 1;
                        } else if (l.binding < r.binding) {
                            return -1;
                        } else {
                            return 0;
                        }
                    },
                    [](vk::DescriptorBinding& l, vk::DescriptorBinding& r) {
                        assert(l.descriptorType == r.descriptorType);
                        assert(l.descriptorCount == r.descriptorCount);
                        l.stageFlags |= r.stageFlags;
                    });
            });
    }

    std::sort(m_setLayouts.begin(), m_setLayouts.end(),
              [](const auto& l, const auto& r) { return l.index <= r.index; });

    m_layouts.clear();
    m_pipelineLayout.reset(nullptr);

    return true;
}
END_GFX_NAMESPACE