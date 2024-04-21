#pragma once

#include "../base/Texture.h"
#include "core/vulkan_core.h"
#include "vulkan_common.h"
BEGIN_GFX_NAMESPACE

class VulkanTexture : public Texture {
public:
    VulkanTexture(const vk::LogicDevice& device, const TextureInfo& info);
    virtual const TextureInfo& getInfo() override { return m_info; }

protected:
    virtual GFX_HANDLE getHandleImp() const override {
        return (GFX_HANDLE)m_image->getHandle();
    }

private:
    SizeI m_size;
    const vk::LogicDevice& m_logicDevice;

    std::unique_ptr<vk::Image> m_image;
    std::unique_ptr<vk::ImageView> m_imageView;
    TextureInfo m_info;
};
END_GFX_NAMESPACE