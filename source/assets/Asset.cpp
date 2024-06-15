#include "Asset.h"

#include "filesystem/FileSystem.h"

BEGIN_NS_SCENCE_GRAPH

bool Asset::getResData(const std::string& path, Data& data) {
    if (!FileSystem::getInstance()->isFileExists(path)) return false;

    m_fullpath = FileSystem::getInstance()->getFullPath(path);

    return FileSystem::getInstance()->getContentData(m_fullpath, data);
}

void Asset::loadedFinish() { m_valid = true; }

uint32_t Asset::addFlag(AssetFlags flag) {
    m_flags |= (uint32_t)flag;
    return m_flags;
}

uint32_t Asset::removeFlag(AssetFlags flag) {
    m_flags &= (~uint32_t(flag));

    return m_flags;
}

bool Asset::checkFlag(AssetFlags flags) { return (m_flags & flags) == flags; }

END_NS_SCENCE_GRAPH