#pragma once

#include "loaders/loader.hpp"
#include <string>

namespace ygl {

// Déclaration de la struct Vertex (si elle n'est pas définie ailleurs)
struct Vertex {
    Vec3 position;
    Vec2 texcoord;
    Vec3 normal;
};

class MD2Loader : public Loader {
public:
    bool LoadMD2(const std::string& filepath, Mesh& mesh);
};

} // namespace ygl
