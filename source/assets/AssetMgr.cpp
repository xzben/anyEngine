#include "AssetMgr.h"

BEGIN_NS_SCENCE_GRAPH

void AssetMgr::freeAsset(Asset* asset) {
    if (asset->delRef() <= 1) {
        removeObject(asset);
    }
}

END_NS_SCENCE_GRAPH