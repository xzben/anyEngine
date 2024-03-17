#include "Instance.h"

#include "PhysicalDevice.h"
#include "SystemInfo.h"

BEGIN_GFX_NAMESPACE
BEGIN_VK_NAMESPACE

void DestroyDebugUtilsMessengerEXT(VkInstance instance,
                                   VkDebugUtilsMessengerEXT debugMessenger,
                                   const VkAllocationCallbacks* pAllocator) {
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
        instance, "vkDestroyDebugUtilsMessengerEXT");
    if (func != nullptr) {
        func(instance, debugMessenger, pAllocator);
    }
}

VkResult CreateDebugUtilsMessengerEXT(
    VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
    const VkAllocationCallbacks* pAllocator,
    VkDebugUtilsMessengerEXT* pDebugMessenger) {
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
        instance, "vkCreateDebugUtilsMessengerEXT");
    if (func != nullptr) {
        return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
    } else {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

static VKAPI_ATTR VkBool32 VKAPI_CALL
debug_callback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
               VkDebugUtilsMessageTypeFlagsEXT messageType,
               const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
               void* pUserData) {
    CCERROR("validation layer: %s", pCallbackData->pMessage);
    return VK_FALSE;
}

Instance::Instance(VkInstance handle, uint32_t apiVersion,
                   std::vector<const char*> enabledExtensions,
                   bool validationLayerEnabled,
                   VkDebugUtilsMessengerEXT debugMessenger)
    : m_handle(handle),
      m_apiVersion(apiVersion),
      m_enabledExtensions(enabledExtensions),
      m_validationLayerEnabled(validationLayerEnabled),
      m_debugMessenger(debugMessenger) {
    uint32_t count = 0;
    vkEnumeratePhysicalDevices(m_handle, &count, nullptr);
    std::vector<VkPhysicalDevice> physicalDevices(count);
    vkEnumeratePhysicalDevices(m_handle, &count, physicalDevices.data());

    for (auto& device : physicalDevices) {
        m_physicalDevices.emplace_back(device);
    }
}

Instance::~Instance() {
    for (auto& device : m_physicalDevices) {
        delete device;
    }
    m_physicalDevices.clear();
    if (m_debugMessenger != VK_NULL_HANDLE) {
        DestroyDebugUtilsMessengerEXT(m_handle, m_debugMessenger, nullptr);
    }
    if (m_handle != VK_NULL_HANDLE) {
        vkDestroyInstance(m_handle, nullptr);
    }
}

std::unique_ptr<Instance> InstanceBuilder::build() {
    std::unique_ptr<Instance> instance;
    SystemInfo sysInfo;

    auto extensions = m_extensions;

    if (m_enableInvalidationLayer && sysInfo.debugUtilsAvailable()) {
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

    if (sysInfo.extensionAvailable(
            VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME)) {
        extensions.push_back(
            VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME);
    }

#if defined(VK_KHR_portability_enumeration)
    bool portability_enumeration_support = sysInfo.extensionAvailable(
        VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
    if (portability_enumeration_support) {
        extensions.push_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
    }
#else
    bool portability_enumeration_support = false;
#endif

    if (!sysInfo.extensionAvailable(extensions)) {
        return nullptr;
    }

    auto layers = m_layers;
    if (m_enableInvalidationLayer && sysInfo.validationAvailable()) {
        layers.push_back("VK_LAYER_KHRONOS_validation");
    }
    if (!sysInfo.layerAvailable(layers)) {
        return nullptr;
    }

    VkApplicationInfo app_info{VK_STRUCTURE_TYPE_APPLICATION_INFO};
    app_info.pApplicationName   = m_appName.c_str();
    app_info.applicationVersion = m_appVersion;
    app_info.pEngineName        = m_engineName.c_str();
    app_info.engineVersion      = m_engineVersion;
    app_info.apiVersion         = m_apiVersion;

    VkInstanceCreateInfo instance_info{VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO};
    instance_info.pApplicationInfo        = &app_info;
    instance_info.enabledExtensionCount   = (uint32_t)(extensions.size());
    instance_info.ppEnabledExtensionNames = extensions.data();
    instance_info.enabledLayerCount       = (uint32_t)(layers.size());
    instance_info.ppEnabledLayerNames     = layers.data();

    if (portability_enumeration_support) {
        instance_info.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
    }

    VkDebugUtilsMessengerCreateInfoEXT debug_info{
        VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT};
    debug_info.messageSeverity = m_debugMessageSeverity;
    debug_info.messageType     = m_debugMessageType;
    debug_info.pfnUserCallback = debug_callback;
    debug_info.pUserData       = nullptr;

    if (m_debugCallback) {
        debug_info.pfnUserCallback = m_debugCallback;
        debug_info.pUserData       = m_debugUserdata;
    }
    if (m_enableInvalidationLayer && sysInfo.debugUtilsAvailable()) {
        instance_info.pNext = &debug_info;
    }

    VkInstance instanceHandle;
    if (vkCreateInstance(&instance_info, nullptr, &instanceHandle)
        != VK_SUCCESS) {
        return nullptr;
    }

    VkDebugUtilsMessengerEXT debug_messenger = VK_NULL_HANDLE;
    if (m_enableInvalidationLayer) {
        if (VK_SUCCESS
            != CreateDebugUtilsMessengerEXT(instanceHandle, &debug_info,
                                            nullptr, &debug_messenger)) {
            return nullptr;
        }
    }

    return std::make_unique<Instance>(
        instanceHandle, m_apiVersion, std::move(extensions),
        m_enableInvalidationLayer, debug_messenger);
}

InstanceBuilder& InstanceBuilder::setAppName(const std::string& name) {
    m_appName = name;
    return *this;
}

InstanceBuilder& InstanceBuilder::setAppVersion(uint32_t appVersion) {
    m_appVersion = appVersion;
    return *this;
}

InstanceBuilder& InstanceBuilder::setEngineName(const std::string& name) {
    m_engineName = name;
    return *this;
}

InstanceBuilder& InstanceBuilder::setEngineVersion(uint32_t engineVersion) {
    m_engineVersion = engineVersion;
    return *this;
}

InstanceBuilder& InstanceBuilder::enableLayer(const char* layerName) {
    if (layerName) {
        m_layers.push_back(layerName);
    }

    return *this;
}

InstanceBuilder& InstanceBuilder::enableExtension(const char* extensionName) {
    if (extensionName) {
        m_extensions.push_back(extensionName);
    }

    return *this;
}

InstanceBuilder& InstanceBuilder::requestApiVersion(uint32_t version) {
    m_apiVersion = version;

    return *this;
}

InstanceBuilder& InstanceBuilder::enableValidationLayer() {
    m_enableInvalidationLayer = true;
    return *this;
}

InstanceBuilder& InstanceBuilder::setDebugMessengerSeverity(
    VkDebugUtilsMessageSeverityFlagsEXT severity) {
    m_debugMessageSeverity = severity;
    return *this;
}

InstanceBuilder& InstanceBuilder::addDebugMessengerSeverity(
    VkDebugUtilsMessageSeverityFlagsEXT severity) {
    m_debugMessageSeverity |= severity;
    return *this;
}

InstanceBuilder& InstanceBuilder::setDebugMessengerType(
    VkDebugUtilsMessageTypeFlagsEXT type) {
    m_debugMessageType = type;
    return *this;
}

InstanceBuilder& InstanceBuilder::addDebugMessengerType(
    VkDebugUtilsMessageTypeFlagsEXT type) {
    m_debugMessageType |= type;
    return *this;
}

InstanceBuilder& InstanceBuilder::setDebugCallback(
    PFN_vkDebugUtilsMessengerCallbackEXT callback) {
    m_debugCallback = callback;
    return *this;
}

InstanceBuilder& InstanceBuilder::setDebugCallbackUserdata(void* userdata) {
    m_debugUserdata = userdata;
    return *this;
}

END_VK_NAMESPACE
END_GFX_NAMESPACE