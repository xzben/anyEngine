#pragma once
#include <string>
#include <unordered_map>
#include <vector>

#include "Data.h"
#include "common/InstanceObject.h"
#include "sceneGraphDefine.h"
BEGIN_NS_SCENCE_GRAPH
class FileSystem : public InstanceObject<FileSystem> {
public:
    FileSystem();
    ~FileSystem();
    std::string getFullPath(const std::string &path);
    bool isFileExists(const std::string &path);
    bool isDirectory(const std::string &path);
    bool isDirectoryExists(const std::string &path);
    std::string getString(const std::string &path);
    bool getContentData(const std::string &path, Data &data);

    void addSearchPath(const std::string &path, bool front = false);
    void removeSearchPath(const std::string &path);
    const std::vector<std::string> &getSearchPaths() { return m_searchPaths; }
    void clearFileCache();

    bool rename(const std::string &from, const std::string &dest);
    bool remove(const std::string &path);
    bool createDirectories(const std::string &path);

    bool getDirectoryContents(const std::string &path, std::vector<std::string> &files,
                              std::vector<std::string> &directories, bool recursive = false);
    bool getDirectoryFiles(const std::string &path, std::vector<std::string> &files,
                           bool recursive = false, bool filterDirectory = true);

    std::string getFilename(const std::string &filepath);

    std::string getPath(const std::string &filepath);

    std::string getFileExt(const std::string &filepath);

    bool writeString(const std::string &fullpath, const std::string &content);
    bool writeData(const std::string &fullpath, Data *data);

private:
    std::vector<std::string> m_searchPaths;
    std::unordered_map<std::string, std::string> m_filecaches;
};

END_NS_SCENCE_GRAPH