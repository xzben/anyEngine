#include "AssetMgr.h"

BEGIN_NS_SCENCE_GRAPH

uint32_t AssetMgr::removeRecordAsset(Asset* asset) {
    auto it              = m_assetInfos.find(asset);
    uint32_t curRefCount = 0;
    if (it != m_assetInfos.end()) {
        it->second--;
        curRefCount = it->second;

        if (curRefCount <= 0) {
            m_assetInfos.erase(it);
        }
    }

    return curRefCount;
}

void AssetMgr::handleAddObject(Asset* obj) {
    addRecordAsset(obj);
    obj->addFlag(AssetFlags::ASSERT_MGR_LOADED);
}

void AssetMgr::handleRemoveObject(Asset* obj) {}

uint32_t AssetMgr::addRecordAsset(Asset* asset) {
    auto it              = m_assetInfos.find(asset);
    uint32_t curRefCount = 0;
    if (it == m_assetInfos.end()) {
        m_assetInfos[asset] = 1;
        curRefCount         = 1;
    } else {
        it->second++;
        curRefCount = it->second;
    }

    return curRefCount;
}

void AssetMgr::freeAsset(Asset* asset) {
    if (!asset->checkFlag(AssetFlags::ASSERT_MGR_LOADED)) {
        CCWARN("free asset must been loaded from asset mgr");
        return;
    }

    if (removeRecordAsset(asset) <= 0) {
        removeObject(asset);
    }
}

END_NS_SCENCE_GRAPH