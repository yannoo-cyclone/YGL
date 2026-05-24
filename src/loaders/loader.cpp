#include "loaders/loader.hpp"
#include "core/mesh.hpp"
#include "utils/file_utils.hpp"
#include <fstream>
#include <sstream>
#include <vector>

namespace ygl {

Loader::Loader() = default;
Loader::~Loader() = default;

bool Loader::LoadMesh(const std::string& filepath, Mesh& mesh) {
    std::string ext = file_utils::GetExtension(filepath);
    
    if (ext == ".obj") {
        return LoadOBJ(filepath, mesh);
    } else if (ext == ".md2") {
        return LoadMD2(filepath, mesh);
    } else if (ext == ".md3") {
        return LoadMD3(filepath, mesh);
    } else if (ext == ".md5mesh") {
        return LoadMD5(filepath, mesh);
    } else if (ext == ".gltf" || ext == ".glb") {
        return LoadGLTF(filepath, mesh);
    }
    
    return false;
}

std::vector<std::string> Loader::SplitLine(const std::string& line) {
    std::vector<std::string> tokens;
    std::istringstream iss(line);
    std::string token;
    
    while (iss >> token) {
        tokens.push_back(token);
    }
    return tokens;
}

} // namespace ygl
