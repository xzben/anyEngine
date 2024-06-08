#include "AssetMgr.h"

BEGIN_NS_SCENCE_GRAPH

void AssetMgr::freeAsset(Asset* asset) { removeObject(asset); }

END_NS_SCENCE_GRAPH