#include "utils/file_utils.h"
#include <fstream>
#include <sstream>
#include <filesystem>

namespace ygl {
namespace file_utils {

std::string ReadFile(const std::string& path) {
    std::ifstream file(path, std::ios::binary);
    if (!file.is_open()) return "";
    std::ostringstream ss; ss << file.rdbuf();
    return ss.str();
}

bool WriteFile(const std::string& path, const std::string& content) {
    std::ofstream file(path);
    if (!file.is_open()) return false;
    file << content;
    return file.good();
}

bool Exists(const std::string& path) { return std::filesystem::exists(path); }
bool IsDirectory(const std::string& path) { return std::filesystem::is_directory(path); }
bool CreateDirectory(const std::string& path) { return std::filesystem::create_directories(path); }
std::string GetDirectory(const std::string& path) { return std::filesystem::path(path).parent_path().string(); }
std::string GetFilename(const std::string& path) { return std::filesystem::path(path).filename().string(); }
std::string GetFilenameWithoutExtension(const std::string& path) { return std::filesystem::path(path).stem().string(); }
std::string GetExtension(const std::string& path) { return std::filesystem::path(path).extension().string(); }

std::vector<std::string> GetFilesInDirectory(const std::string& dir, bool recursive) {
    std::vector<std::string> files;
    if (recursive) {
        for (const auto& e : std::filesystem::recursive_directory_iterator(dir))
            if (e.is_regular_file()) files.push_back(e.path().string());
    } else {
        for (const auto& e : std::filesystem::directory_iterator(dir))
            if (e.is_regular_file()) files.push_back(e.path().string());
    }
    return files;
}

std::vector<std::string> GetDirectoriesInDirectory(const std::string& dir, bool recursive) {
    std::vector<std::string> dirs;
    if (recursive) {
        for (const auto& e : std::filesystem::recursive_directory_iterator(dir))
            if (e.is_directory()) dirs.push_back(e.path().string());
    } else {
        for (const auto& e : std::filesystem::directory_iterator(dir))
            if (e.is_directory()) dirs.push_back(e.path().string());
    }
    return dirs;
}

std::string JoinPaths(const std::vector<std::string>& paths) {
    std::filesystem::path p;
    for (const auto& path : paths) p /= path;
    return p.string();
}

std::string NormalizePath(const std::string& path) { return std::filesystem::weakly_canonical(path).string(); }
std::string GetAbsolutePath(const std::string& path) { return std::filesystem::absolute(path).string(); }
std::string GetRelativePath(const std::string& path, const std::string& base) { return std::filesystem::relative(path, base).string(); }
uint64_t GetFileSize(const std::string& path) { return std::filesystem::file_size(path); }

std::string GetFileModificationTime(const std::string& path) {
    return std::to_string(std::filesystem::last_write_time(path).time_since_epoch().count());
}

bool CopyFile(const std::string& src, const std::string& dst) {
    try { std::filesystem::copy_file(src, dst, std::filesystem::copy_options::overwrite_existing); return true; }
    catch (...) { return false; }
}

bool MoveFile(const std::string& src, const std::string& dst) {
    try { std::filesystem::rename(src, dst); return true; }
    catch (...) { return false; }
}

bool DeleteFile(const std::string& path) {
    try { std::filesystem::remove(path); return true; }
    catch (...) { return false; }
}

bool DeleteDirectory(const std::string& path, bool recursive) {
    try { if (recursive) std::filesystem::remove_all(path); else std::filesystem::remove(path); return true; }
    catch (...) { return false; }
}

} // namespace file_utils
} // namespace ygl
