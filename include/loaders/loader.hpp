#pragma once

#include <string>
#include <vector>
#include <memory>

namespace ygl {

class Mesh;

class Loader {
public:
    virtual ~Loader();

    virtual std::vector<std::shared_ptr<Mesh>> load(const std::string& filename) = 0;
    virtual std::shared_ptr<Mesh> loadSingle(const std::string& filename);
};

} // namespace ygl

