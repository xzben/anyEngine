#pragma once

#include "base/SwapChain.h"
#include "core/vulkan_core.h"
#include "vulkan_common.h"
BEGIN_GFX_NAMESPACE

class SwapChainBuilder;

class VulkanSwapChain : public SwapChain {
public:
    VulkanSwapChain(const vk::LogicDevice& device, const SurfaceInfo& info);
    virtual ~VulkanSwapChain();

    operator bool() const { return m_handle != VK_NULL_HANDLE; }
    operator VkSwapchainKHR() const { return m_handle; }

    VkFormat format() const { return m_format.format; }
    VkColorSpaceKHR color_space() const { return m_format.colorSpace; }

    const VkPresentModeKHR& present_mode() const { return m_presentMode; }

    uint32_t width() const { return m_extent.width; }
    uint32_t height() const { return m_extent.height; }
    const VkExtent2D& extent() const { return m_extent; }

    const std::vector<vk::Image>& images() const { return m_images; }
    const std::vector<vk::ImageView>& image_views() const {
        return m_imageViews;
    }

    std::pair<bool, uint32_t> acquireNextImage(Semaphore* semophore,
                                               Fence* fence     = nullptr,
                                               uint32_t timeout = 0) override;

protected:
    void handleUpdateSurfaceInfo(const SurfaceInfo& info) override;

protected:
    bool createSwapChainHandle();
    std::optional<VkSurfaceFormatKHR> chooseFormat(
        std::vector<VkSurfaceFormatKHR> desired_formats);
    std::optional<VkPresentModeKHR> choosePresentMode(
        std::vector<VkPresentModeKHR> presentModes);
    VkExtent2D chooseExtent(const VkSurfaceCapabilitiesKHR& capabilities,
                            VkExtent2D extent);
    uint32_t chooseImageCount(const VkSurfaceCapabilitiesKHR& capabilities,
                              uint32_t miniCount);

private:
    std::unique_ptr<SwapChainBuilder> m_builder{};
    const vk::LogicDevice& m_logicDevice;
    std::unique_ptr<vk::Surface> m_surface{};
    VkSwapchainKHR m_handle{VK_NULL_HANDLE};

    VkSurfaceFormatKHR m_format;
    VkPresentModeKHR m_presentMode;
    VkExtent2D m_extent;

    std::vector<vk::Image> m_images;
    std::vector<vk::ImageView> m_imageViews;
};

class SwapChainBuilder {
public:
    explicit SwapChainBuilder(const vk::LogicDevice& device,
                              VkSurfaceKHR surface);

    bool build(VkSwapchainKHR& handle, VkSurfaceFormatKHR& format,
               VkPresentModeKHR& presentMode, VkExtent2D& extent);

    SwapChainBuilder& setSurface(VkSurfaceKHR surface);
    SwapChainBuilder& setOldSwapChain(VkSwapchainKHR oldSwapChain);

    SwapChainBuilder& requestExtent(uint32_t width, uint32_t height);

    SwapChainBuilder& requestFormat(VkSurfaceFormatKHR format);
    SwapChainBuilder& requestFallbackFormat(VkSurfaceFormatKHR format);

    SwapChainBuilder& requestPresentMode(VkPresentModeKHR present_mode);
    SwapChainBuilder& requestFallbackPresentMode(VkPresentModeKHR present_mode);

    SwapChainBuilder& requestMinImageCount(uint32_t min_image_count);

    SwapChainBuilder& setPreTransform(
        VkSurfaceTransformFlagBitsKHR pre_transform);
    SwapChainBuilder& setCompositeAlpha(
        VkCompositeAlphaFlagBitsKHR composite_alpha);

private:
    std::optional<VkSurfaceFormatKHR> chooseFormat();
    std::optional<VkPresentModeKHR> choosePresentMode();
    VkExtent2D chooseExtent(const VkSurfaceCapabilitiesKHR& capabilities);
    uint32_t chooseImageCount(const VkSurfaceCapabilitiesKHR& capabilities);

private:
    const vk::LogicDevice& m_logicDevice;

    VkSurfaceKHR m_surface        = VK_NULL_HANDLE;
    VkSwapchainKHR m_oldSwapchain = VK_NULL_HANDLE;
    uint32_t m_width              = 0;
    uint32_t m_height             = 0;
    std::vector<VkSurfaceFormatKHR> m_formats;
    std::vector<VkPresentModeKHR> m_presentModes;
    uint32_t m_minImageCount                     = 0;
    VkSurfaceTransformFlagBitsKHR m_preTransform = {};
    VkCompositeAlphaFlagBitsKHR m_compositeAlpha =
        VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
};

END_GFX_NAMESPACE