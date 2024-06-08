#include "AssetMgr.h"

BEGIN_NS_SCENCE_GRAPH

void AssetMgr::freeAsset(Asset* asset) {
    auto& fullpath = asset->getResPath();
    removeObject(fullpath);
}

END_NS_SCENCE_GRAPH