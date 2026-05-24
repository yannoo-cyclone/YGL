#pragma once

#include "core/mesh.hpp"
#include <string>
#include <vector>

namespace ygl {

class Loader {
public:
    Loader() = default;  // AJOUTÉ
    virtual ~Loader() = default;

    virtual bool LoadMesh(const std::string& filepath, Mesh& mesh);  // AJOUTÉ
    static std::vector<std::string> SplitLine(const std::string& line);  // AJOUTÉ
};

} // namespace ygl
