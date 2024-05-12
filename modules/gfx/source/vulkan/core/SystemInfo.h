#pragma once

#include <vector>

#include "vulkan_core_common.h"

BEGIN_GFX_NAMESPACE
BEGIN_VK_NAMESPACE

class SystemInfo {
public:
    SystemInfo();

    bool layerAvailable(const char* layer_name) const;
    bool layerAvailable(std::vector<const char*> layer_names) const;
    bool extensionAvailable(const char* extension_name) const;
    bool extensionAvailable(std::vector<const char*> extension_names) const;

    const std::vector<VkLayerProperties> availableLayers() const;
    const std::vector<VkExtensionProperties> availableExtensions() const;

    bool validationAvailable() const;
    bool debugUtilsAvailable() const;

private:
    std::vector<VkLayerProperties> m_availableLayers;
    std::vector<VkExtensionProperties> m_availableExtensions;
    bool m_validationLayersAvailable = false;
    bool m_debugUtilsAvailable       = false;
};
END_VK_NAMESPACE
END_GFX_NAMESPACE