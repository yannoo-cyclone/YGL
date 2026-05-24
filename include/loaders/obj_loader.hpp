#pragma once
#include "loaders/loader.hpp"
#include "math/vec3.hpp"
#include "math/vec2.hpp"
#include <string>
#include <vector>

namespace ygl {
class OBJLoader : public Loader {
public:
    OBJLoader();
    ~OBJLoader();
    std::vector<std::shared_ptr<Mesh>> load(const std::string& filename) override;
    std::shared_ptr<Mesh> loadSingle(const std::string& filename) override;
    bool LoadOBJ(const std::string& filepath, Mesh& mesh);

private:
    struct OBJMaterial { std::string name; Vec3 ambient; Vec3 diffuse; Vec3 specular; float shininess; float opacity; std::string diffuseMap; std::string normalMap; };
    struct OBJFace { std::vector<int> positionIndices; std::vector<int> normalIndices; std::vector<int> texCoordIndices; std::string materialName; };
    bool parseOBJ(const std::string& filename, std::vector<Vec3>& positions, std::vector<Vec3>& normals, std::vector<Vec2>& texCoords, std::vector<OBJFace>& faces, std::vector<OBJMaterial>& materials);
    bool parseMTL(const std::string& filename, std::vector<OBJMaterial>& materials);
    std::shared_ptr<Mesh> createMeshFromData(const std::vector<Vec3>& positions, const std::vector<Vec3>& normals, const std::vector<Vec2>& texCoords, const std::vector<OBJFace>& faces, const std::string& name = "");
};
} // namespace ygl
