#include "Surface.h"

#if CUR_PLATFORM == PLATFORM_WINDOWS
#include <Windows.h>
#include <vulkan/vulkan_win32.h>
#else
static_assert("need to implement this platform");
#endif

BEGIN_GFX_NAMESPACE
BEGIN_VK_NAMESPACE
Surface::Surface(VkInstance instance, const SurfaceInfo& info) : m_instance(instance) {
    update(info);
}

bool Surface::update(const SurfaceInfo& info) {
    if (m_info.handle == info.handle) {
        return false;
    }

    m_info = info;
#if CUR_PLATFORM == PLATFORM_WINDOWS
    VkWin32SurfaceCreateInfoKHR createInfo{};
    createInfo.sType     = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
    createInfo.hwnd      = (HWND)m_info.handle;
    createInfo.hinstance = GetModuleHandle(nullptr);

    if (vkCreateWin32SurfaceKHR(m_instance, &createInfo, nullptr, &m_handle) != VK_SUCCESS) {
        throw std::runtime_error("failed to create window surface!");
    }
#else
    static_assert("need to implement this platform");
#endif
    return true;
}

Surface::~Surface() { vkDestroySurfaceKHR(m_instance, m_handle, nullptr); }
END_VK_NAMESPACE
END_GFX_NAMESPACE