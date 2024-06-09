#include "Texture2D.h"

#include "app/Application.h"
#include "assets/AssetMgr.h"
#include "assets/Image.h"

BEGIN_NS_SCENCE_GRAPH

IMPLEMENT_RUNTIME_CLASS(Texture2D)

Texture2D::Texture2D(const char* filename) {
    m_image = AssetMgr::getInstance()->loadAsset<Image>(filename);

    auto device = Application::getInstance()->getRenderSystem()->getDevice();
    NS_GFX::TextureInfo info;
    info.width  = m_image->width();
    info.height = m_image->height();
    info.format = m_image->pixelFormat();
    m_texture   = device->createTexture(info, m_image->pixels());
}

Texture2D::Texture2D(const Image* image) {}

Texture2D::~Texture2D() {
    auto device = Application::getInstance()->getRenderSystem()->getDevice();
    device->destroyObject(m_texture);

    AssetMgr::getInstance()->freeAsset(m_image);
}

END_NS_SCENCE_GRAPH