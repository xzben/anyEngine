#pragma once

#include "../base/Texture.h"
#include "core/vulkan_core.h"
#include "vulkan_common.h"
BEGIN_GFX_NAMESPACE

class VulkanTexture : public Texture, public GfxObject {
public:
protected:
    virtual GFX_HANDLE getHandleImp() override {
        return (GFX_HANDLE)m_imgHandle;
    }

    virtual const SizeI& getSize() override { return m_size; }

private:
    SizeI m_size;
    const vk::LogicDevice& m_logicDevice;
    VkImage m_imgHandle{VK_NULL_HANDLE};
    VkImageView m_imgView{VK_NULL_HANDLE};
};
END_GFX_NAMESPACE