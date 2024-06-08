#include "Asset.h"

#include "filesystem/FileSystem.h"

BEGIN_NS_SCENCE_GRAPH

bool Asset::load(const std::string& path) {
    if (!FileSystem::getInstance()->isFileExists(path)) return false;

    m_fullpath = FileSystem::getInstance()->getFullPath(path);
    Data data;
    FileSystem::getInstance()->getContentData(m_fullpath, data);

    if (!onLoaded(data)) {
        return false;
    }

    m_valid = true;
    return true;
}

END_NS_SCENCE_GRAPH