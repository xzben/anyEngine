#pragma once

#include "../base/Texture.h"
#include "core/vulkan_core.h"
#include "vulkan_common.h"
BEGIN_GFX_NAMESPACE

class VulkanTexture : public Texture, public GfxObject {
public:
    VulkanTexture(const vk::LogicDevice& device, const TextureInfo& info);

protected:
    virtual GFX_HANDLE getHandleImp() override {
        return (GFX_HANDLE)m_image->getHandle();
    }

    virtual const SizeI& getSize() override { return m_size; }

private:
    SizeI m_size;
    const vk::LogicDevice& m_logicDevice;

    std::unique_ptr<vk::Image> m_image;
    std::unique_ptr<vk::ImageView> m_imageView;
};
END_GFX_NAMESPACE