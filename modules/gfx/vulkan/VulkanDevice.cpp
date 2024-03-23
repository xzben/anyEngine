#include "VulkanDevice.h"
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

BEGIN_GFX_NAMESPACE

static VKAPI_ATTR VkBool32 VKAPI_CALL
debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
              VkDebugUtilsMessageTypeFlagsEXT messageType,
              const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
              void* pUserData) {
    switch (messageSeverity) {
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT: {
            CCWARN("validation layer: %s", pCallbackData->pMessage);
            break;
        }
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT: {
            CCERROR("validation layer: %s", pCallbackData->pMessage);
            break;
        }
    }

    return VK_FALSE;
}

std::unique_ptr<vk::Instance> VulkanDevice::getInstance(
    const DeviceInfo& info) {
    vk::InstanceBuilder builder;

#ifndef NDEBUG
    builder.enableValidationLayer();
    builder.setDebugMessengerSeverity(
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT
        | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT
        | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT);
#endif  // NDEBUG
    builder.setAppName(info.name);
    builder.setDebugCallback(debugCallback);

    // glfw window extensions
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions =
        glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
    for (auto i = 0; i < glfwExtensionCount; ++i) {
        builder.enableExtension(glfwExtensions[i]);
    }
    return builder.build();
}

vk::PhysicalDevice VulkanDevice::selectPhysicDevice(
    const DeviceInfo& info, const vk::Instance& instance,
    vk::Surface& surface) {
    vk::PhysicalDeviceSelector selector(instance, surface);
    selector.preferDeviceType(VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU);
    if (info.present) {
        selector.requirePresent();
    }
    selector.requestDeviceMemorySize(info.requireMemSize);
    return std::move(selector.select());
}

std::unique_ptr<vk::LogicDevice> createLogicDevice(
    const DeviceInfo& info, vk::Instance& instance,
    vk::PhysicalDevice physicalDevice, vk::Surface& surface) {
    vk::LogicDeviceBuilder builder(instance, physicalDevice, surface);

    if (info.present) {
        builder.requirePresent();
        builder.requireExtension(
            VK_KHR_MAINTENANCE_1_EXTENSION_NAME);  // for viewport -1*height
    }

    //  device_builder.require_extension(VK_EXT_CUSTOM_BORDER_COLOR_EXTENSION_NAME);
    VkPhysicalDeviceFeatures deviceFeatures{};
    deviceFeatures.samplerAnisotropy        = VK_TRUE;
    deviceFeatures.fragmentStoresAndAtomics = VK_TRUE;
    deviceFeatures.independentBlend         = VK_TRUE;

    bool enable_point_light_shadow = true;
    if (enable_point_light_shadow) {
        deviceFeatures.geometryShader = VK_TRUE;
    }
    builder.requireFeatures(deviceFeatures);

    builder.requireValidationLayer("VK_LAYER_KHRONOS_validation");
    for (auto queuItem : info.queues) {
        switch (queuItem.first) {
            case QueueType::Graphics: {
                builder.requireGraphicsQueue(queuItem.second);
                break;
            }
            case QueueType::Present: {
                builder.requirePresentQueue(queuItem.second);
                break;
            }
            case QueueType::Compute: {
                builder.requireComputeQueue(queuItem.second);
                break;
            }
            case QueueType::Transfer: {
                builder.requireTransferQueue(queuItem.second);
                break;
            };
        }
    }

    auto device = builder.build();
    return device;
}

VulkanDevice::VulkanDevice() {}
VulkanDevice::~VulkanDevice() {}

bool VulkanDevice::init(const DeviceInfo& info) {
    m_instance = getInstance(info);
    if (m_instance == nullptr) {
        CCERROR("create instance failed!");
        return false;
    }

    m_surface = std::make_unique<vk::Surface>(*m_instance, info.surface);

    auto phycialDevice =
        std::move(selectPhysicDevice(info, *m_instance, *m_surface));

    m_logicDevice = createLogicDevice(info, *m_instance,
                                      std::move(phycialDevice), *m_surface);

    return true;
}

END_GFX_NAMESPACE