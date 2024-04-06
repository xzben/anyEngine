#include "Device.h"

#include "../gles3/GLES3Device.h"
#include "../vulkan/VulkanDevice.h"
BEGIN_GFX_NAMESPACE

Device* Device::createDevice(const DeviceInfo& info) {
    Device* device = nullptr;
    switch (info.api) {
        case GFX_API::GLES3: {
            device = new GLES3Device();
        }
        case GFX_API::VULKAN: {
            device = new VulkanDevice();
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