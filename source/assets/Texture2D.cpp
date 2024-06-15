#include "Texture2D.h"

#include "app/Application.h"
#include "assets/AssetMgr.h"
#include "assets/Image.h"

BEGIN_NS_SCENCE_GRAPH

IMPLEMENT_RUNTIME_CLASS(Texture2D)
Texture2D::Texture2D() : Asset(AssetType::Texture2D) {}

Texture2D::Texture2D(const char* filename) : Asset(AssetType::Texture2D) { load(filename); }

Texture2D::Texture2D(Image* image) : Asset(AssetType::Texture2D) { initWithImage(image); }

Texture2D::~Texture2D() {
    auto device = Application::getInstance()->getRenderSystem()->getDevice();
    device->destroyObject(m_texture);

    freeImage();
}

bool Texture2D::load(const std::string& path) {
    return initWithImage(AssetMgr::getInstance()->loadAsset<Image>(path));
}

void Texture2D::freeImage() {
    if (m_image == nullptr) return;

    if (m_image->checkFlag(AssetFlags::ASSERT_MGR_LOADED)) {
        AssetMgr::getInstance()->freeAsset(m_image);
    } else {
        m_image->delRef();
    }
    m_image = nullptr;
}

bool Texture2D::initWithImage(Image* img) {
    if (img == nullptr) return false;
    freeImage();

    m_image     = img;
    auto device = Application::getInstance()->getRenderSystem()->getDevice();
    NS_GFX::TextureInfo info;
    info.width  = m_image->width();
    info.height = m_image->height();
    info.format = m_image->pixelFormat();
    m_texture   = device->createTexture(info, m_image->pixels());

    loadedFinish();

    return true;
}

uint32_t Texture2D::width() const {
    if (m_image == nullptr) return 0;

    return m_image->width();
}

uint32_t Texture2D::height() const {
    if (m_image == nullptr) return 0;

    return m_image->height();
}

END_NS_SCENCE_GRAPH