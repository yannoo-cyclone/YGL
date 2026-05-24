#pragma once

#include "core/mesh.hpp"
#include <string>
#include <vector>

namespace ygl {

class Loader {
public:
    Loader() = default;
    virtual ~Loader() = default;

    virtual bool LoadMesh(const std::string& filepath, Mesh& mesh);
    static std::vector<std::string> SplitLine(const std::string& line);
};

} // namespace ygl
