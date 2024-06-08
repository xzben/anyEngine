#include "Texture2D.h"

#include "assets/AssetMgr.h"
BEGIN_NS_SCENCE_GRAPH

Texture2D::Texture2D(const char* filename) {
    m_image = AssetMgr::getInstance()->loadAsset<Image>(filename);
}

Texture2D::Texture2D(const Image* image) {}

Texture2D::~Texture2D() {}

END_NS_SCENCE_GRAPH