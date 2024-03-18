#pragma once

#include "../base/Fence.h"
#include "core/vulkan_core.h"
#include "vulkan_common.h"

BEGIN_GFX_NAMESPACE

class VulkanFence : public Fence, public GfxObject {
public:
    VulkanFence(const vk::LogicDevice& device, bool createSignaled = false);
    virtual ~VulkanFence();

    bool wait(uint32_t timeout = std::numeric_limits<uint32_t>::max()) override;
    bool reset() override;

    operator bool() const { return m_handle != VK_NULL_HANDLE; }
    operator VkFence() const { return m_handle; }

public:
    static bool wait(const vk::LogicDevice& device, std::vector<VkFence> fences,
                     uint32_t timeout = std::numeric_limits<uint32_t>::max());
    static bool reset(const vk::LogicDevice& device,
                      std::vector<VkFence> fences);

protected:
    virtual GFX_HANDLE getHandleImp() const override {
        return (GFX_HANDLE)m_handle;
    }

private:
    const vk::LogicDevice& m_logicDevice;
    VkFence m_handle{VK_NULL_HANDLE};
};
END_GFX_NAMESPACE