#include "SystemInfo.h"

#include <algorithm>

BEGIN_GFX_NAMESPACE
BEGIN_VK_NAMESPACE

SystemInfo::SystemInfo() {
    uint32_t layer_count;
    vkEnumerateInstanceLayerProperties(&layer_count, nullptr);
    m_availableLayers.resize(layer_count);
    vkEnumerateInstanceLayerProperties(&layer_count, m_availableLayers.data());

    uint32_t extension_count;
    vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, nullptr);
    m_availableExtensions.resize(extension_count);
    vkEnumerateInstanceExtensionProperties(nullptr, &extension_count,
                                           m_availableExtensions.data());

    for (const auto& layer : m_availableLayers) {
        uint32_t count;
        vkEnumerateInstanceExtensionProperties(layer.layerName, &count,
                                               nullptr);
        std::vector<VkExtensionProperties> extensions(extension_count);
        vkEnumerateInstanceExtensionProperties(
            layer.layerName, &extension_count, extensions.data());

        m_availableExtensions.insert(m_availableExtensions.end(),
                                     extensions.begin(), extensions.end());
    }

    m_validationLayersAvailable =
        std::find_if(m_availableLayers.begin(), m_availableLayers.end(),
                     [](const auto& layer) {
                         return std::strcmp(layer.layerName,
                                            "VK_LAYER_KHRONOS_validation")
                                == 0;
                     })
        != m_availableLayers.end();

    m_debugUtilsAvailable =
        std::find_if(m_availableExtensions.begin(), m_availableExtensions.end(),
                     [](const auto& ext) {
                         return std::strcmp(ext.extensionName,
                                            VK_EXT_DEBUG_UTILS_EXTENSION_NAME)
                                == 0;
                     })
        != m_availableExtensions.end();
}

bool SystemInfo::layerAvailable(const char* layer_name) const {
    if (!layer_name) {
        return false;
    }

    for (const auto& layer : m_availableLayers) {
        if (strcmp(layer_name, layer.layerName) == 0) {
            return true;
        }
    }

    return false;
}

bool SystemInfo::layerAvailable(std::vector<const char*> layer_names) const {
    for (const char* l : layer_names) {
        if (!layerAvailable(l)) {
            return false;
        }
    }

    return true;
}

bool SystemInfo::extensionAvailable(const char* extension_name) const {
    if (!extension_name) {
        return false;
    }

    for (const auto& extension : m_availableExtensions) {
        if (strcmp(extension_name, extension.extensionName) == 0) {
            return true;
        }
    }

    return false;
}

bool SystemInfo::extensionAvailable(
    std::vector<const char*> extension_names) const {
    for (const char* e : extension_names) {
        if (!extensionAvailable(e)) {
            return false;
        }
    }

    return true;
}

const std::vector<VkLayerProperties> SystemInfo::availableLayers() const {
    return m_availableLayers;
}
const std::vector<VkExtensionProperties> SystemInfo::availableExtensions()
    const {
    return m_availableExtensions;
}

bool SystemInfo::validationAvailable() const {
    return m_validationLayersAvailable;
}
bool SystemInfo::debugUtilsAvailable() const { return m_debugUtilsAvailable; }

END_VK_NAMESPACE
END_GFX_NAMESPACE