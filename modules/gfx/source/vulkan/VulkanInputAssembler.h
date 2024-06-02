#pragma once

#include "VulkanBuffer.h"
#include "base/InputAssembler.h"
#include "core/vulkan_core.h"
#include "vulkan_common.h"
BEGIN_GFX_NAMESPACE

class VulkanInputAssembler : public InputAssembler {
public:
    VulkanInputAssembler(const vk::LogicDevice& device, const std::vector<Attribute>& attribute);
    virtual ~VulkanInputAssembler();

    const std::vector<Attribute>& getAttributes() const override { return m_attributes; }

    virtual const std::vector<Attribute>& getInstanceAttributes() const override {
        return m_instanceAttributes;
    }

    Buffer* getVertexBuffer() const override { return m_vertexBuf.get(); }
    Buffer* getIndexBuffer() const override { return m_indexBuf.get(); }
    uint32_t getVertexCount() const override { return m_vertexCount; }
    uint32_t getIndexCount() const override { return m_indexCount; }
    uint32_t getVertexAttribteStride() const override { return m_vertexItemSize; }

    uint32_t getInstanceAttribteStride() const override { return m_instanceStrip; }
    uint32_t getIndexItemSize() const override { return m_indexItemSize; }
    PrimitiveType getPrimitiveType() override { return m_primitiveType; };

private:
    uint32_t m_location = 0;
    PrimitiveType m_primitiveType;
    std::vector<Attribute> m_attributes;
    std::vector<Attribute> m_instanceAttributes;
    const vk::LogicDevice& m_logicDevice;
    std::shared_ptr<VulkanBuffer> m_vertexBuf;
    std::shared_ptr<VulkanBuffer> m_indexBuf;
    uint32_t m_vertexCount{0};
    uint32_t m_indexCount{0};
    uint32_t m_vertexItemSize{0};
    uint32_t m_indexItemSize{sizeof(uint32_t)};
    uint32_t m_instanceStrip{0};
    VkVertexInputBindingDescription m_binding_description;
    std::vector<VkVertexInputAttributeDescription> m_attribute_descriptions;
};
END_GFX_NAMESPACE