#include "VulkanInputAssembler.h"

BEGIN_GFX_NAMESPACE

VulkanInputAssembler::VulkanInputAssembler(
    const vk::LogicDevice& device, const std::vector<Attribute>& attributes,
    PrimitiveModel primitive)
    : GfxObject(GfxObjectType::InputAssembler),
      m_logicDevice(device),
      m_attributes(attributes),
      m_primiitiveModel(primitive) {
    uint32_t index  = 0;
    uint32_t offset = 0;
    for (const auto& attr : m_attributes) {
        m_attribute_descriptions[index].binding  = m_location;
        m_attribute_descriptions[index].location = attr.location;
        m_attribute_descriptions[index].format =
            map_gfx_type_to_vk_format(attr.format);
        m_attribute_descriptions[index].offset = offset;
        uint32_t size = get_gfx_format_type_size(attr.format);
        offset += size;
        index++;
    }
}
VulkanInputAssembler::~VulkanInputAssembler() {}

END_GFX_NAMESPACE