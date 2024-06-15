#pragma once

#include <unordered_map>

#include "Asset.h"
#include "Image.h"
#include "common/ObjectContainor.h"
#include "filesystem/FileSystem.h"
BEGIN_NS_SCENCE_GRAPH

class AssetMgr : protected ObjectContainor<Asset>, public InstanceObject<AssetMgr> {
public:
    AssetMgr();
    virtual ~AssetMgr();

    void freeAsset(Asset* asset);

    template <class AssetCls>
    AssetCls* loadAsset(const std::string& path) {
        if (!FileSystem::getInstance()->isFileExists(path)) return nullptr;

        std::string fullpath = FileSystem::getInstance()->getFullPath(path);

        AssetCls* obj = getObject<AssetCls>(fullpath);
        if (obj == nullptr) {
            obj = addObject<AssetCls>(fullpath);
            obj->load(fullpath);
        }

        return obj;
    }

protected:
    void handleAddObject(Asset* obj) override;
    void handleRemoveObject(Asset* obj) override;

protected:
    uint32_t removeRecordAsset(Asset* asset);
    uint32_t addRecordAsset(Asset* asset);

protected:
    const std::string& getObjectName(Asset* asset) override { return asset->getResPath(); }
    void setObjectName(Asset* asset, const std::string& name) override { asset->setResPath(name); }

private:
    std::unordered_map<Asset*, int> m_assetInfos;
};
END_NS_SCENCE_GRAPH