#include "Sprite.h"

#include "assets/AssetMgr.h"
#include "assets/Texture2D.h"

BEGIN_NS_SCENCE_GRAPH

IMPLEMENT_RUNTIME_CLASS(Sprite)

Sprite::Sprite() {}

Sprite::~Sprite() {}

void Sprite::initWithFile(const std::string& filename) {
    m_texture = AssetMgr::getInstance()->loadAsset<Texture2D>(filename);

    m_size.width  = (float)m_texture->width();
    m_size.height = (float)m_texture->height();
}

void Sprite::setSize(const Size& size) { m_size = size; }

END_NS_SCENCE_GRAPH