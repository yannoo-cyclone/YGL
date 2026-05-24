#include "loaders/loader.hpp"
#include <sstream>
#include <algorithm>

namespace ygl {

bool Loader::LoadMesh(const std::string& filepath, Mesh& mesh) {
    // Implémentation par défaut (à surcharger par les classes dérivées)
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
