#pragma once

#include "common/Object.h"
#include "filesystem/Data.h"
#include "sceneGraphDefine.h"
BEGIN_NS_SCENCE_GRAPH

enum class AssetType {
    Image,
    Texture2D,
};
enum AssetFlags : uint32_t {
    ASSERT_MGR_LOADED = 1 << 0,  // 是否是AssetMgr 加载进来的资源
};
class Asset : public Object {
    DECLARE_RUNTIME_CLASS(Asset)
public:
    friend class AssetMgr;

    uint32_t getFlags() { return m_flags; }
    uint32_t addFlag(AssetFlags flag);
    uint32_t removeFlag(AssetFlags flag);
    bool checkFlag(AssetFlags flags);
    const std::string& getResPath() { return m_fullpath; }

protected:
    Asset(AssetType type) : m_type(type) {}
    virtual ~Asset() = default;

    virtual bool load(const std::string& path) = 0;

    void setResPath(const std::string& path) { m_fullpath = path; }

protected:
    bool getResData(const std::string& path, Data& data);
    void loadedFinish();

protected:
    std::string m_fullpath;
    AssetType m_type;
    bool m_valid{false};

    uint32_t m_flags{0};
};
END_NS_SCENCE_GRAPH