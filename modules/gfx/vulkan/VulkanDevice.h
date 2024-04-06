#pragma once

#include <memory>

#include "../base/Device.h"
#include "VulkanShader.h"
#include "core/vulkan_core.h"
#include "vulkan_common.h"

BEGIN_GFX_NAMESPACE

class VulkanDevice : public Device {
public:
    VulkanDevice();
    virtual ~VulkanDevice();

    virtual bool init(const DeviceInfo& info) override;
    virtual VulkanShader* createShader() override;

protected:
    static std::unique_ptr<vk::Instance> getInstance(const DeviceInfo& info);
    static vk::PhysicalDevice selectPhysicDevice(const DeviceInfo& info,
                                                 const vk::Instance& instance,
                                                 vk::Surface& surface);

    static std::unique_ptr<vk::LogicDevice> createLogicDevice(
        const DeviceInfo& info, vk::Instance& instance,
        vk::PhysicalDevice physicalDevice, vk::Surface& surface);

private:
    std::unique_ptr<vk::Instance> m_instance;
    std::unique_ptr<vk::LogicDevice> m_logicDevice;
    std::unique_ptr<vk::Surface> m_surface;
};

END_GFX_NAMESPACE