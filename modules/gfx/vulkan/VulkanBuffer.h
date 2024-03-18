#pragma once

#include "../base/Buffer.h"
#include "core/vulkan_core.h"
#include "vulkan_common.h"
BEGIN_GFX_NAMESPACE

class VulkanBuffer : public Buffer, public GfxObject {
public:
    VulkanBuffer(const vk::LogicDevice& logicDevice, BufferType type,
                 uint32_t size,
                 const std::vector<uint32_t>& queue_family_indices);
    virtual ~VulkanBuffer();
    virtual void update(const void* data, uint32_t size,
                        uint32_t offset = 0) override;
    virtual uint32_t size() override { return m_size; }
    virtual uint32_t capacity() override { return m_capacity; }

    void flush(uint32_t size = 0, uint32_t offset = 0);
    void* map();
    void unmap();

    operator bool() const { return m_handle != VK_NULL_HANDLE; }
    operator VkBuffer() const { return m_handle; }

protected:
    virtual GFX_HANDLE getHandleImp() const override {
        return (GFX_HANDLE)m_handle;
    }

private:
    const vk::LogicDevice& m_logicDevice;
    VkBuffer m_handle{VK_NULL_HANDLE};
    VmaAllocation m_allocation = VK_NULL_HANDLE;
    uint32_t m_size{0};
    uint32_t m_capacity{0};
    BufferType m_type;

    void* m_mappedData = nullptr;
    bool m_persistent  = false;
    bool m_mapped      = false;
};
END_GFX_NAMESPACE