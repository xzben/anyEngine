#include "Device.h"

#include "gl3/GL3Device.h"
#include "vulkan/VulkanDevice.h"
BEGIN_GFX_NAMESPACE

Device* Device::createDevice(const DeviceInfo& info) {
    Device* device = nullptr;
    switch (info.api) {
        case GFX_API::GL3: {
            device = new GL3Device();
            break;
        }
        case GFX_API::VULKAN: {
            device = new VulkanDevice();
            break;
        }
        // case GFX_API::OPENGL3: {
        //     break;
        // }
        // case GFX_API::METAL: {
        //     break;
        // }
        default: {
            assert(false);
        }
    }
    assert(device != nullptr);
    device->init(info);

    return device;
}

END_GFX_NAMESPACE