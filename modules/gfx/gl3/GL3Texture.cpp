#include "GL3Texture.h"

BEGIN_GFX_NAMESPACE

GL3Texture::GL3Texture(GL3Device& device, const TextureInfo& info,
                       const void* pData)
    : m_device(device), m_info(info) {}
GL3Texture::~GL3Texture() {}
END_GFX_NAMESPACE