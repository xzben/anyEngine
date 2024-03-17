#include "VulkanBuffer.h"
BEGIN_GFX_NAMESPACE

VkBufferUsageFlags s_vk_usages[] = {
    // index
    VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
    // vertex
    VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
    // uniform
    VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
    // storage
    VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
    // stage
    VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
};

VmaAllocationCreateFlags s_vk_createFlags[] = {
    // index
    0,
    // vertex
    0,
    // uniform
    VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT
        | VMA_ALLOCATION_CREATE_MAPPED_BIT,
    // storage
    0,
    // stage
    VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT
        | VMA_ALLOCATION_CREATE_MAPPED_BIT,
};

VulkanBuffer::VulkanBuffer(const vk::LogicDevice& logicDevice, BufferType type,
                           uint32_t size,
                           const std::vector<uint32_t>& queue_family_indices)
    : GfxObject(GfxObjectType::Buffer),
      m_type(type),
      m_logicDevice(logicDevice),
      m_capacity(size),
      m_size(0) {
    auto allocFlags = s_vk_createFlags[(int)type];
    auto bufUsage   = s_vk_usages[(int)type];

    m_persistent = (allocFlags & VMA_ALLOCATION_CREATE_MAPPED_BIT) != 0;
    VkBufferCreateInfo buffer_info{VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO};

    buffer_info.usage = bufUsage;
    buffer_info.size  = size;

    if (queue_family_indices.size() >= 2) {
        buffer_info.sharingMode           = VK_SHARING_MODE_CONCURRENT;
        buffer_info.queueFamilyIndexCount = (queue_family_indices.size());
        buffer_info.pQueueFamilyIndices   = queue_family_indices.data();
    }

    VmaAllocationCreateInfo memory_info{};
    memory_info.flags = allocFlags;
    memory_info.usage = VMA_MEMORY_USAGE_AUTO;

    VmaAllocationInfo allocation_info{};
    auto result = vmaCreateBuffer(logicDevice.getMemoryAllocator(),
                                  &buffer_info, &memory_info, &m_handle,
                                  &m_allocation, &allocation_info);

    if (result == VK_SUCCESS && m_persistent) {
        m_mappedData = static_cast<uint8_t*>(allocation_info.pMappedData);
        m_mapped     = true;
    }
}

VulkanBuffer::~VulkanBuffer() {
    if (m_allocation) {
        vmaDestroyBuffer(m_logicDevice.getMemoryAllocator(), m_handle,
                         m_allocation);
    }
}

void VulkanBuffer::update(const void* data, uint32_t size, uint32_t offset) {
    if (m_persistent) {
        std::memcpy((uint8_t*)m_mappedData + offset, data, size);
        flush(size, offset);
    } else {
        map();
        std::memcpy((uint8_t*)m_mappedData + offset, data, size);
        flush(offset, size);
        unmap();
    }

    uint32_t curSize = offset + size;
    if (curSize > m_size) m_size = curSize;
}

void VulkanBuffer::flush(uint32_t size, uint32_t offset) {
    size = size == 0 ? m_size : size;
    vmaFlushAllocation(m_logicDevice.getMemoryAllocator(), m_allocation, offset,
                       size);
}

void* VulkanBuffer::map() {
    if (!m_mapped && m_mappedData == nullptr) {
        m_mapped = true;
        vmaMapMemory(m_logicDevice.getMemoryAllocator(), m_allocation,
                     &m_mappedData);
    }

    return m_mappedData;
}

void VulkanBuffer::unmap() {
    if (m_mapped) {
        vmaUnmapMemory(m_logicDevice.getMemoryAllocator(), m_allocation);
        m_mappedData = nullptr;
        m_mapped     = false;
    }
}

END_GFX_NAMESPACE