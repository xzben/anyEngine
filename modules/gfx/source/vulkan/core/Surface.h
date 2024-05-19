#pragma once

#include "vulkan_core_common.h"

BEGIN_GFX_NAMESPACE
BEGIN_VK_NAMESPACE
class Instance;

class Surface {
public:
    Surface(VkInstance instance, const SurfaceInfo& info);
    virtual ~Surface();

    operator bool() const { return m_handle != VK_NULL_HANDLE; }
    operator VkSurfaceKHR() const { return m_handle; }

    uint32_t width() const { return m_info.width; }
    uint32_t height() const { return m_info.height; }

    bool update(const SurfaceInfo& info);
    const SurfaceInfo& getInfo() const { return m_info; }

private:
    SurfaceInfo m_info;
    VkSurfaceKHR m_handle{VK_NULL_HANDLE};
    VkInstance m_instance{VK_NULL_HANDLE};
};

END_VK_NAMESPACE
END_GFX_NAMESPACE