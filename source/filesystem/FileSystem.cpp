#include "FileSystem.h"

#include <stdio.h>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>

#include "safe_macro.h"

#if CUR_PLATFORM != PLATFORM_WIN32
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#endif

BEGIN_NS_SCENCE_GRAPH
namespace fs = std::filesystem;

FileSystem::FileSystem() {
#ifdef BUILTIN_RESOURCE_DIR
    addSearchPath(MACRO_XSTR(BUILTIN_RESOURCE_DIR));
#endif  // BUILTIN_RESOURCE_DIR
}

FileSystem::~FileSystem() {}

void FileSystem::addSearchPath(const std::string &path, bool front /*= false */) {
    if (front) {
        m_searchPaths.insert(m_searchPaths.begin(), path);
    } else {
        m_searchPaths.push_back(path);
    }
}
void FileSystem::removeSearchPath(const std::string &path) {
    for (auto it = m_searchPaths.begin(); it != m_searchPaths.end();) {
        if (*it == path) {
            it = m_searchPaths.erase(it);
        } else {
            it++;
        }
    }
}

void FileSystem::clearFileCache() { m_filecaches.clear(); }

std::string FileSystem::getFullPath(const std::string &path) {
    fs::path temppath(path);
    if (temppath.is_absolute()) {
        return path;
    }

    auto it = m_filecaches.find(path);
    if (it != m_filecaches.end()) {
        return it->second;
    }

    for (auto it = m_searchPaths.begin(); it != m_searchPaths.end(); it++) {
        std::string fullpath = *it + "/" + path;
        if (fs::exists(fullpath)) {
            m_filecaches.insert(std::make_pair(path, fullpath));
            return fullpath;
        }
    }

    return std::string("");
}

bool FileSystem::isFileExists(const std::string &path) {
    std::string fullpath = getFullPath(path);
    if (fullpath.empty()) return false;

    return fs::exists(fullpath);
}

bool FileSystem::isDirectory(const std::string &path) {
    std::string fullpath = getFullPath(path);
    if (fullpath == "") return false;

    fs::path temppath(fullpath);
    return fs::is_directory(temppath);
}

bool FileSystem::isDirectoryExists(const std::string &path) { return isDirectory(path); }

static void getDirectoryFiles(std::string &fullpath, std::vector<std::string> &files) {
    fs::path temppath(fullpath);
    fs::directory_entry entry(temppath);
    if (entry.status().type() != fs::file_type::directory) {
        return;
    }

    fs::directory_iterator list(temppath);
    for (auto &it : list) {
        files.push_back(it.path().filename().string());
    }
}

bool FileSystem::getDirectoryContents(const std::string &path, std::vector<std::string> &files,
                                      std::vector<std::string> &directories,
                                      bool recursive /*= false*/) {
    std::string fullpath = getFullPath(path);
    if (fullpath == "") return false;

    fs::path directory(fullpath);
    if (!fs::is_directory(directory)) return false;

    if (recursive) {
        for (auto const &directory_entry : fs::recursive_directory_iterator{directory}) {
            if (directory_entry.is_regular_file()) {
                files.push_back(directory_entry.path().lexically_relative(directory).string());
            } else {
                directories.push_back(
                    directory_entry.path().lexically_relative(directory).string());
            }
        }
    } else {
        fs::directory_iterator list(directory);
        for (auto &it : list) {
            if (it.is_regular_file()) {
                files.push_back(it.path().lexically_relative(directory).string());
            } else {
                directories.push_back(it.path().lexically_relative(directory).string());
            }
        }
    }

    return true;
}

bool FileSystem::getDirectoryFiles(const std::string &path, std::vector<std::string> &files,
                                   bool recursive /*= false*/, bool filterDirectory /*= true*/) {
    std::string fullpath = getFullPath(path);
    if (fullpath == "") return false;

    fs::path directory(fullpath);
    if (!fs::is_directory(directory)) return false;

    if (recursive) {
        for (auto const &directory_entry : fs::recursive_directory_iterator{directory}) {
            if (filterDirectory) {
                if (directory_entry.is_regular_file()) {
                    files.push_back(directory_entry.path().lexically_relative(directory).string());
                }
            } else {
                files.push_back(directory_entry.path().lexically_relative(directory).string());
            }
        }
    } else {
        fs::directory_iterator list(directory);
        for (auto &it : list) {
            if (filterDirectory) {
                if (it.is_regular_file()) {
                    files.push_back(it.path().lexically_relative(directory).string());
                }
            } else {
                files.push_back(it.path().lexically_relative(directory).string());
            }
        }
    }

    return true;
}

bool FileSystem::rename(const std::string &from, const std::string &dest) {
    if (!isFileExists(from)) return false;
    if (isFileExists(dest)) return false;

    fs::rename(from, dest);

    return true;
}

bool FileSystem::remove(const std::string &path) { return fs::remove_all(getFullPath(path)); }

bool FileSystem::createDirectories(const std::string &path) { return fs::create_directories(path); }

std::string FileSystem::getString(const std::string &path) {
    Data data;
    if (!getContentData(path, data)) return "";

    return (char *)data.getData();
}

bool FileSystem::getContentData(const std::string &path, Data &data) {
    if (!isFileExists(path)) false;

    std::string fullpath = getFullPath(path);
    FILE *fp             = fopen(fullpath.c_str(), "rb");
    if (!fp) return false;

#if defined(_MSC_VER)
    int descriptor = _fileno(fp);
#else
    int descriptor = fileno(fp);
#endif

    struct stat statBuf;
    if (fstat(descriptor, &statBuf) == -1) {
        fclose(fp);
        return false;
    }

    size_t size = statBuf.st_size;
    data.reverse(size);
    size_t readsize = fread(data.getData(), 1, size, fp);
    fclose(fp);
    data.writeOffset(readsize);

    return true;
}

std::string FileSystem::getFilename(const std::string &filepath) {
    fs::path temppath(filepath);
    return fs::path(filepath).filename().string();
}

std::string FileSystem::getPath(const std::string &filepath) {
    fs::path temppath(filepath);
    return fs::path(filepath).remove_filename().string();
}

std::string FileSystem::getFileExt(const std::string &filepath) {
    fs::path temppath(filepath);
    return fs::path(filepath).extension().string();
}

bool FileSystem::writeString(const std::string &fullpath, const std::string &content) {
    FILE *fp = fopen(fullpath.c_str(), "w+");
    if (!fp) return false;

    uint32_t count     = content.size();
    const char *buffer = content.c_str();
    uint32_t nw        = 0;

    while (nw < count) {
        nw += fwrite(buffer + nw, 1, count - nw, fp);
    }

    fclose(fp);
    return true;
}

bool FileSystem::writeData(const std::string &fullpath, Data *data) {
    FILE *fp = fopen(fullpath.c_str(), "wb+");
    if (!fp) return false;

    int count    = data->getSize();
    byte *buffer = data->getData();
    int nw       = 0;

    while (nw < count) {
        nw += fwrite(buffer + nw, 1, count - nw, fp);
    }

    fclose(fp);
    return true;
}

END_NS_SCENCE_GRAPH
