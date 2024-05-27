#include "VulkanSwapChain.h"
#include <cmath>
#include <stdexcept>
#include <algorithm>

BEGIN_GFX_NAMESPACE
VulkanSwapChain::VulkanSwapChain(const vk::LogicDevice& device,
                                 const SurfaceInfo& info)
    : m_logicDevice(device) {
    m_surface = std::make_unique<vk::Surface>(device.getInstance(), info);
    m_builder = std::make_unique<SwapChainBuilder>(m_logicDevice, *m_surface);
    createSwapChainHandle();

    // get images
    uint32_t count = 0;
    vkGetSwapchainImagesKHR(m_logicDevice, m_handle, &count, nullptr);

    std::vector<VkImage> images;
    vkGetSwapchainImagesKHR(m_logicDevice, m_handle, &count, images.data());
    m_images.resize(count);
    for (auto i = 0; i < count; ++i) {
        m_images.emplace_back(m_logicDevice, images[i],
                              VkExtent3D{m_extent.width, m_extent.height, 1},
                              m_format.format,
                              VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT);
        m_imageViews.emplace_back(m_images[i], VK_IMAGE_VIEW_TYPE_2D);
    }
}

bool VulkanSwapChain::createSwapChainHandle() {
    VkSwapchainKHR oldHandle = m_handle;
    if (oldHandle != VK_NULL_HANDLE) {
        m_builder->setOldSwapChain(oldHandle);
    }

    m_extent.width  = m_surface->width();
    m_extent.height = m_surface->height();

    m_builder->requestFormat(VkSurfaceFormatKHR{
        VK_FORMAT_B8G8R8A8_SRGB, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR});
    m_builder->requestPresentMode(VK_PRESENT_MODE_MAILBOX_KHR);
    m_builder->requestFallbackPresentMode(VK_PRESENT_MODE_FIFO_KHR);
    m_builder->requestExtent(m_extent.width, m_extent.height);

    bool result = m_builder->build(m_handle, m_format, m_presentMode, m_extent);

    if (oldHandle != VK_NULL_HANDLE) {
        vkDestroySwapchainKHR(m_logicDevice, oldHandle, nullptr);
    }

    return result;
}

void VulkanSwapChain::handleUpdateSurfaceInfo(const SurfaceInfo& info) {
    m_surface->update(info);
    m_builder->setSurface(*m_surface);
    createSwapChainHandle();
}

VulkanSwapChain::~VulkanSwapChain() {
    vkDestroySwapchainKHR(m_logicDevice, m_handle, nullptr);
}

std::pair<bool, uint32_t> VulkanSwapChain::acquireNextImage(
    Semaphore* semophore, Fence* fence, uint32_t timeout) {
    uint32_t index;
    VkFence hFence = fence ? fence->getHandle<VkFence>() : VK_NULL_HANDLE;
    VkSemaphore hSemaphore = semophore->getHandle<VkSemaphore>();

    if (timeout == 0) {
        timeout = std::numeric_limits<uint32_t>::max();
    }
    auto result = vkAcquireNextImageKHR(m_logicDevice, m_handle, timeout,
                                        hSemaphore, hFence, &index);

    return std::make_pair(result == VK_SUCCESS, index);
}

//---------------------------------------------------------------
SwapChainBuilder::SwapChainBuilder(const vk::LogicDevice& device,
                                   VkSurfaceKHR surface)
    : m_logicDevice(device), m_surface(surface) {}

bool SwapChainBuilder::build(VkSwapchainKHR& handle, VkSurfaceFormatKHR& format,
                             VkPresentModeKHR& presentMode,
                             VkExtent2D& extent) {
    if (m_width == 0 || m_height == 0) {
        return false;
    }

    // foramt
    auto format_opt = chooseFormat();
    if (!format_opt) {
        return false;
    }
    format = format_opt.value();

    // present mode
    auto present_mode_opt = choosePresentMode();
    if (!present_mode_opt) {
        return false;
    }
    presentMode = present_mode_opt.value();

    VkSurfaceCapabilitiesKHR capabilities;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_logicDevice.getPhysicalDevice(),
                                              m_surface, &capabilities);

    // capabilities
    auto image_count   = chooseImageCount(capabilities);
    extent             = chooseExtent(capabilities);
    auto pre_transform = m_preTransform == VkSurfaceTransformFlagBitsKHR()
                             ? capabilities.currentTransform
                             : m_preTransform;

    VkSwapchainCreateInfoKHR create_info{
        VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR};
    create_info.surface          = m_surface;
    create_info.minImageCount    = image_count;
    create_info.imageFormat      = format.format;
    create_info.imageColorSpace  = format.colorSpace;
    create_info.presentMode      = presentMode;
    create_info.imageExtent      = extent;
    create_info.imageArrayLayers = 1;
    create_info.imageUsage       = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    create_info.preTransform   = pre_transform;
    create_info.compositeAlpha = m_compositeAlpha;

    create_info.clipped      = VK_TRUE;
    create_info.oldSwapchain = m_oldSwapchain;

    auto graphics_queue_index =
        m_logicDevice.getQueueFamilyIndex(QueueType::Graphics);
    auto present_queue_index =
        m_logicDevice.getQueueFamilyIndex(QueueType::Present);
    assert(graphics_queue_index && present_queue_index);

    uint32_t queue_family_indices[] = {graphics_queue_index.value(),
                                       present_queue_index.value()};
    if (graphics_queue_index != present_queue_index) {
        create_info.imageSharingMode      = VK_SHARING_MODE_CONCURRENT;
        create_info.queueFamilyIndexCount = 2;
        create_info.pQueueFamilyIndices   = queue_family_indices;
    } else {
        create_info.imageSharingMode      = VK_SHARING_MODE_EXCLUSIVE;
        create_info.queueFamilyIndexCount = 0;        // Optional
        create_info.pQueueFamilyIndices   = nullptr;  // Optional
    }

    if (vkCreateSwapchainKHR(m_logicDevice, &create_info, nullptr, &handle)
        != VK_SUCCESS) {
        return false;
    }

    return true;
}

SwapChainBuilder& SwapChainBuilder::setSurface(VkSurfaceKHR surface) {
    m_surface = surface;

    return *this;
}
SwapChainBuilder& SwapChainBuilder::setOldSwapChain(
    VkSwapchainKHR oldSwapChain) {
    m_oldSwapchain = oldSwapChain;

    return *this;
}

SwapChainBuilder& SwapChainBuilder::requestExtent(uint32_t width,
                                                  uint32_t height) {
    m_width  = width;
    m_height = height;
    return *this;
}

SwapChainBuilder& SwapChainBuilder::requestFormat(VkSurfaceFormatKHR format) {
    m_formats.insert(m_formats.begin(), format);

    return *this;
}
SwapChainBuilder& SwapChainBuilder::requestFallbackFormat(
    VkSurfaceFormatKHR format) {
    m_formats.push_back(format);

    return *this;
}

SwapChainBuilder& SwapChainBuilder::requestPresentMode(
    VkPresentModeKHR present_mode) {
    m_presentModes.insert(m_presentModes.begin(), present_mode);

    return *this;
}

SwapChainBuilder& SwapChainBuilder::requestFallbackPresentMode(
    VkPresentModeKHR present_mode) {
    m_presentModes.insert(m_presentModes.begin(), present_mode);

    return *this;
}

SwapChainBuilder& SwapChainBuilder::requestMinImageCount(
    uint32_t min_image_count) {
    m_minImageCount = min_image_count;
    return *this;
}

SwapChainBuilder& SwapChainBuilder::setPreTransform(
    VkSurfaceTransformFlagBitsKHR pre_transform) {
    m_preTransform = pre_transform;
    return *this;
}
SwapChainBuilder& SwapChainBuilder::setCompositeAlpha(
    VkCompositeAlphaFlagBitsKHR composite_alpha) {
    m_compositeAlpha = composite_alpha;
    return *this;
}

std::optional<VkSurfaceFormatKHR> SwapChainBuilder::chooseFormat() {
    uint32_t count = 0;
    vkGetPhysicalDeviceSurfaceFormatsKHR(m_logicDevice.getPhysicalDevice(),
                                         m_surface, &count, nullptr);
    std::vector<VkSurfaceFormatKHR> formats(count);
    vkGetPhysicalDeviceSurfaceFormatsKHR(m_logicDevice.getPhysicalDevice(),
                                         m_surface, &count, formats.data());

    auto desired_formats = m_formats;
    if (desired_formats.empty()) {
        desired_formats.push_back(
            {VK_FORMAT_B8G8R8A8_SRGB, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR});
    }

    std::optional<VkSurfaceFormatKHR> format;
    for (auto i = 0; i < desired_formats.size(); ++i) {
        if (std::find_if(formats.begin(), formats.end(),
                         [format = desired_formats[i]](const auto& f) {
                             return f.format == format.format
                                    && f.colorSpace == format.colorSpace;
                         })
            != formats.end()) {
            format = desired_formats[i];
            break;
        }
    }

    return format;
}
std::optional<VkPresentModeKHR> SwapChainBuilder::choosePresentMode() {
    uint32_t count = 0;
    vkGetPhysicalDeviceSurfacePresentModesKHR(m_logicDevice.getPhysicalDevice(),
                                              m_surface, &count, nullptr);
    std::vector<VkPresentModeKHR> present_modes(count);
    vkGetPhysicalDeviceSurfacePresentModesKHR(m_logicDevice.getPhysicalDevice(),
                                              m_surface, &count,
                                              present_modes.data());

    auto desired_present_modes = m_presentModes;
    if (desired_present_modes.empty()) {
        desired_present_modes.push_back(VK_PRESENT_MODE_MAILBOX_KHR);
        desired_present_modes.push_back(VK_PRESENT_MODE_FIFO_KHR);
    }

    std::optional<VkPresentModeKHR> present_mode;
    for (auto i = 0; i < desired_present_modes.size(); ++i) {
        if (std::find(present_modes.begin(), present_modes.end(),
                      desired_present_modes[i])
            != present_modes.end()) {
            present_mode = desired_present_modes[i];
            break;
        }
    }

    return present_mode;
}
VkExtent2D SwapChainBuilder::chooseExtent(
    const VkSurfaceCapabilitiesKHR& capabilities) {
    VkExtent2D actual_extent = {static_cast<uint32_t>(m_width),
                                static_cast<uint32_t>(m_height)};
    if (capabilities.currentExtent.width
        != std::numeric_limits<uint32_t>::max()) {
        actual_extent = capabilities.currentExtent;
    } else {
        actual_extent.width =
            std::clamp(actual_extent.width, capabilities.minImageExtent.width,
                       capabilities.maxImageExtent.width);
        actual_extent.height =
            std::clamp(actual_extent.height, capabilities.minImageExtent.height,
                       capabilities.maxImageExtent.height);
    }

    return actual_extent;
}

uint32_t SwapChainBuilder::chooseImageCount(
    const VkSurfaceCapabilitiesKHR& capabilities) {
    auto imageCount =
        m_minImageCount == 0 ? capabilities.minImageCount + 1 : m_minImageCount;
    return std::clamp(imageCount, capabilities.minImageCount,
                      capabilities.maxImageCount);
}
END_GFX_NAMESPACE