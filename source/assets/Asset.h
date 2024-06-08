#pragma once

#include "common/Object.h"
#include "filesystem/Data.h"
#include "sceneGraphDefine.h"
BEGIN_NS_SCENCE_GRAPH

enum class AssetType {
    Image,
};

class Asset : public Object {
    friend class AssetMgr;

protected:
    Asset(AssetType type) : m_type(type) {}
    virtual ~Asset() = default;

    bool load(const std::string& path);
    const std::string& getResPath() { return m_fullpath; }
    void setResPath(const std::string& path) { m_fullpath = path; }

protected:
    virtual bool onLoaded(Data& data) = 0;

protected:
    std::string m_fullpath;
    AssetType m_type;
    bool m_valid{false};
};
END_NS_SCENCE_GRAPH