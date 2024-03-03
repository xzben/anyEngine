#pragma once

#include "core_common.h"

BEGIN_GFX_NAMESPACE
BEGIN_VK_NAMESPACE

class PhysicalDevice;

class Instance {
public:
    Instance(VkInstance instance, uint32_t apiVersion,
             std::vector<const char*> enabledExtensions,
             bool validationLayerEnabled,
             VkDebugUtilsMessengerEXT debugMessenger);
    ~Instance();

public:
    operator bool() const { return m_handle != VK_NULL_HANDLE; }
    operator VkInstance() const { return m_handle; }

    const std::vector<const char*>& enabled_extensions() const {
        return m_enabledExtensions;
    }
    bool validation_layer_enabled() const { return m_validationLayerEnabled; }

    uint32_t api_version() const { return m_apiVersion; }

    const std::vector<PhysicalDevice*>& getPhysicalDevices() const {
        return m_physicalDevices;
    }

private:
    std::vector<PhysicalDevice*> m_physicalDevices;
    VkInstance m_handle   = VK_NULL_HANDLE;
    uint32_t m_apiVersion = VK_API_VERSION_1_0;

    VkDebugUtilsMessengerEXT m_debugMessenger;
    std::vector<const char*> m_enabledExtensions;
    bool m_validationLayerEnabled = false;
};

class InstanceBuilder {
public:
    explicit InstanceBuilder()                         = default;
    InstanceBuilder(const InstanceBuilder&)            = delete;
    InstanceBuilder(InstanceBuilder&&)                 = delete;
    InstanceBuilder& operator=(const InstanceBuilder&) = delete;
    InstanceBuilder& operator=(InstanceBuilder&&)      = delete;

    Instance* build();

    InstanceBuilder& setAppName(const std::string& name);
    InstanceBuilder& setAppVersion(uint32_t appVersion);
    InstanceBuilder& setEngineName(const std::string& name);
    InstanceBuilder& setEngineVersion(uint32_t engineVersion);

    InstanceBuilder& enableLayer(const char* layerName);
    InstanceBuilder& enableExtension(const char* extensionName);
    InstanceBuilder& requestApiVersion(uint32_t version);

    InstanceBuilder& enableValidationLayer();
    InstanceBuilder& setDebugMessengerSeverity(
        VkDebugUtilsMessageSeverityFlagsEXT severity);
    InstanceBuilder& addDebugMessengerSeverity(
        VkDebugUtilsMessageSeverityFlagsEXT severity);
    InstanceBuilder& setDebugMessengerType(
        VkDebugUtilsMessageTypeFlagsEXT type);
    InstanceBuilder& addDebugMessengerType(
        VkDebugUtilsMessageTypeFlagsEXT type);
    InstanceBuilder& setDebugCallback(
        PFN_vkDebugUtilsMessengerCallbackEXT callback);
    InstanceBuilder& setDebugCallbackUserdata(void* userdata);

private:
    std::string m_appName;
    uint32_t m_appVersion = 0;

    std::string m_engineName;
    uint32_t m_engineVersion = 0;

    std::vector<const char*> m_layers;
    std::vector<const char*> m_extensions;

    uint32_t m_apiVersion{VK_API_VERSION_1_0};

    bool m_enableInvalidationLayer{false};

    PFN_vkDebugUtilsMessengerCallbackEXT m_debugCallback = nullptr;
    void* m_debugUserdata                                = nullptr;
    VkDebugUtilsMessageSeverityFlagsEXT m_debugMessageSeverity =
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT
        | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    VkDebugUtilsMessageTypeFlagsEXT m_debugMessageType =
        VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT
        | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT
        | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
};

END_VK_NAMESPACE
END_GFX_NAMESPACE