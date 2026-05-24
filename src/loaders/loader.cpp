#include "loaders/loader.hpp"
#include <sstream>
#include <algorithm>

namespace ygl {
bool Loader::LoadMesh(const std::string& /*filepath*/, Mesh& /*mesh*/) { return false; }
std::vector<std::shared_ptr<Mesh>> Loader::load(const std::string& /*filename*/) { return {}; }
std::shared_ptr<Mesh> Loader::loadSingle(const std::string& /*filename*/) { return nullptr; }

std::vector<std::string> Loader::SplitLine(const std::string& line) {
    std::vector<std::string> tokens;
    std::istringstream iss(line);
    std::string token;
    while (iss >> token) tokens.push_back(token);
    return tokens;
}
} // namespace ygl
