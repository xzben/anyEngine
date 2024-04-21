#include "VulkanDevice.h"

#include "VulkanShader.h"
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
        switch (queuItem.type) {
            case QueueType::Graphics: {
                builder.requireGraphicsQueue(queuItem.priority,
                                             queuItem.perferSperate);
                break;
            }
            case QueueType::Present: {
                builder.requirePresentQueue(queuItem.priority,
                                            queuItem.perferSperate);
                break;
            }
            case QueueType::Compute: {
                builder.requireComputeQueue(queuItem.priority,
                                            queuItem.perferSperate);
                break;
            }
            case QueueType::Transfer: {
                builder.requireTransferQueue(queuItem.priority,
                                             queuItem.perferSperate);
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

VulkanShader* VulkanDevice::createShader(ShaderModuleInfo* info,
                                         uint32_t count) {
    auto obj = new VulkanShader(*this->m_logicDevice);

    for (int i = 0; i < count; i++) {
        obj->addStage(info[i].pData, info[i].size, info[i].stage,
                      info[i].entryName);
    }

    return obj;
}

VulkanQueue* VulkanDevice::getQueue(QueueType& type, uint32_t index) {
    return nullptr;
}

VulkanRenderPass* VulkanDevice::createRenderPass(
    std::vector<Attachment>& attachments, const std::vector<SubPass>& subpass,
    const std::vector<SubPassDependency>& dependencies) {
    return nullptr;
}

VulkanPipeline* VulkanDevice::createPipeline(RenderPass* renderPass,
                                             uint32_t subpass, Shader* shader,
                                             const PipelineState& state) {
    return nullptr;
}

VulkanTexture* VulkanDevice::createTexture(const TextureInfo& info,
                                           const void* pData) {
    return nullptr;
}
VulkanBuffer* VulkanDevice::createBuffer(BufferType type, uint32_t size) {
    return nullptr;
}
VulkanSampler* VulkanDevice::createSampler(const SamplerInfo& info) {
    return nullptr;
}

VulkanInputAssembler* VulkanDevice::createInputAssembler(
    const std::vector<Attribute>& attributes, const void* pVertexData,
    uint32_t vertexCount, const void* pIndexData, uint32_t indexCount,
    uint32_t indexItemSize) {
    return nullptr;
}

VulkanInputAssembler* VulkanDevice::createInputAssembler(
    const std::vector<Attribute>& attributes,
    const std::vector<Attribute>& InstanceAttributes, const void* pVertexData,
    uint32_t vertexCount, const void* pInstanceData, uint32_t instanceCount,
    const void* pIndexData, uint32_t indexCount, uint32_t indexItemSize) {
    return nullptr;
}

VulkanSwapChain* VulkanDevice::createSwapChain(void* nativeWindow,
                                               uint32_t width, uint32_t height,
                                               bool singleBuffer) {
    return nullptr;
}
VulkanFence* VulkanDevice::createFence(bool signaled) { return nullptr; }

VulkanSemaphore* VulkanDevice::createSemaphore(uint32_t count) {
    return nullptr;
}

VulkanEvent* VulkanDevice::createEvent() { return nullptr; }

void VulkanDevice::waitIdle() {}

//------------------

//-------------------------------------------------------------
void VulkanDevice::destroyBuffer(Buffer* buffer) { buffer->delRef(); }
void VulkanDevice::destroyEvent(Event* event) { event->delRef(); }
void VulkanDevice::destroyFence(Fence* fence) { fence->delRef(); }
void VulkanDevice::destroyInputAssembler(InputAssembler* input) {
    input->delRef();
}
void VulkanDevice::destroyPipeline(Pipeline* pipeline) { pipeline->delRef(); }
void VulkanDevice::destroyRenderPasss(RenderPass* renderPass) {
    renderPass->delRef();
}
void VulkanDevice::destroySampler(Sampler* sampler) { sampler->delRef(); }
void VulkanDevice::destroySemaphore(Semaphore* semaphore) {
    semaphore->delRef();
}
void VulkanDevice::destroyShader(Shader* shader) { shader->delRef(); }
void VulkanDevice::destroySwapChain(SwapChain* swapChain) {
    swapChain->delRef();
}
void VulkanDevice::destroyTexture(Texture* texture) { texture->delRef(); }
END_GFX_NAMESPACE