#pragma once

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

        if (obj != nullptr) {
            return obj;
        }

        obj = addObject<AssetCls>(fullpath);
        obj->load(fullpath);
        return obj;
    }

protected:
    const std::string& getObjectName(Asset* asset) override { return asset->getResPath(); }
    void setObjectName(Asset* asset, const std::string& name) override { asset->setResPath(name); }
};
END_NS_SCENCE_GRAPH