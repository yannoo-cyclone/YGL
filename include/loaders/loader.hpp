#pragma once
#include "core/mesh.hpp"
#include <string>
#include <vector>
#include <memory>

namespace ygl {
class Loader {
public:
    Loader() = default;
    virtual ~Loader() = default;
    virtual bool LoadMesh(const std::string& filepath, Mesh& mesh);
    virtual std::vector<std::shared_ptr<Mesh>> load(const std::string& filename);
    virtual std::shared_ptr<Mesh> loadSingle(const std::string& filename);
    static std::vector<std::string> SplitLine(const std::string& line);
};
} // namespace ygl
