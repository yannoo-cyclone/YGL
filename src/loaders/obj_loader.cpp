#include "loaders/obj_loader.hpp"
#include "core/mesh.hpp"
#include <fstream>
#include <sstream>
#include <unordered_map>

namespace ygl {
bool OBJLoader::LoadOBJ(const std::string& filepath, Mesh& mesh) {
    std::ifstream file(filepath);
    if (!file.is_open()) return false;

    std::vector<Vec3> temp_positions;
    std::vector<Vec3> temp_normals;
    std::vector<Vec2> temp_texcoords;
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::unordered_map<std::string, unsigned int> vertex_map;

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;
        auto tokens = Loader::SplitLine(line);
        if (tokens.empty()) continue;

        if (tokens[0] == "v" && tokens.size() >= 4) {
            temp_positions.emplace_back(std::stof(tokens[1]), std::stof(tokens[2]), std::stof(tokens[3]));
        } else if (tokens[0] == "vn" && tokens.size() >= 4) {
            temp_normals.emplace_back(std::stof(tokens[1]), std::stof(tokens[2]), std::stof(tokens[3]));
        } else if (tokens[0] == "vt" && tokens.size() >= 2) {
            float u = std::stof(tokens[1]);
            float v = (tokens.size() >= 3) ? std::stof(tokens[2]) : 0.0f;
            temp_texcoords.emplace_back(u, v);
        } else if (tokens[0] == "f" && tokens.size() >= 4) {
            for (size_t i = 1; i < tokens.size(); ++i) {
                const std::string& vertex_token = tokens[i];
                size_t slash1 = vertex_token.find('/');
                size_t slash2 = (slash1 != std::string::npos) ? vertex_token.find('/', slash1 + 1) : std::string::npos;

                unsigned int pos_idx = std::stoul(vertex_token.substr(0, slash1)) - 1;
                unsigned int tex_idx = 0;
                if (slash1 != std::string::npos) {
                    std::string tex_str = (slash2 != std::string::npos) ?
                        vertex_token.substr(slash1 + 1, slash2 - slash1 - 1) :
                        vertex_token.substr(slash1 + 1);
                    if (!tex_str.empty()) tex_idx = std::stoul(tex_str) - 1;
                }
                unsigned int norm_idx = 0;
                if (slash2 != std::string::npos) {
                    std::string norm_str = vertex_token.substr(slash2 + 1);
                    if (!norm_str.empty()) norm_idx = std::stoul(norm_str) - 1;
                }

                std::string vertex_key = std::to_string(pos_idx) + "/" + std::to_string(tex_idx) + "/" + std::to_string(norm_idx);
                if (vertex_map.find(vertex_key) == vertex_map.end()) {
                    Vertex vertex;
                    vertex.position = temp_positions[pos_idx];
                    if (tex_idx < temp_texcoords.size()) vertex.texcoord = temp_texcoords[tex_idx];
                    if (norm_idx < temp_normals.size()) vertex.normal = temp_normals[norm_idx];
                    else vertex.normal = Vec3(0.0f);
                    vertex_map[vertex_key] = static_cast<unsigned int>(vertices.size());
                    vertices.push_back(vertex);
                }
                indices.push_back(vertex_map[vertex_key]);
            }
        }
    }

    if (temp_normals.empty()) {
        for (size_t i = 0; i < indices.size(); i += 3) {
            if (i + 2 >= indices.size()) break;
            Vec3 e1 = vertices[indices[i+1]].position - vertices[indices[i]].position;
            Vec3 e2 = vertices[indices[i+2]].position - vertices[indices[i]].position;
            Vec3 n = normalize(cross(e1, e2));
            vertices[indices[i]].normal += n;
            vertices[indices[i+1]].normal += n;
            vertices[indices[i+2]].normal += n;
        }
        for (auto& v : vertices) v.normal = normalize(v.normal);
    }

    mesh.SetVertices(vertices);
    mesh.SetIndices(indices);
    mesh.ComputeBoundingBox();
    return true;
}

std::vector<std::shared_ptr<Mesh>> OBJLoader::load(const std::string& filename) {
    Mesh mesh;
    return LoadOBJ(filename, mesh) ? std::vector{std::make_shared<Mesh>(mesh)} : std::vector<std::shared_ptr<Mesh>>();
}
std::shared_ptr<Mesh> OBJLoader::loadSingle(const std::string& filename) {
    Mesh mesh;
    return LoadOBJ(filename, mesh) ? std::make_shared<Mesh>(mesh) : nullptr;
}
OBJLoader::OBJLoader() = default;
OBJLoader::~OBJLoader() = default;
bool OBJLoader::parseOBJ(const std::string&, std::vector<Vec3>&, std::vector<Vec3>&, std::vector<Vec2>&, std::vector<OBJFace>&, std::vector<OBJMaterial>&) { return false; }
bool OBJLoader::parseMTL(const std::string&, std::vector<OBJMaterial>&) { return false; }
std::shared_ptr<Mesh> OBJLoader::createMeshFromData(const std::vector<Vec3>&, const std::vector<Vec3>&, const std::vector<Vec2>&, const std::vector<OBJFace>&, const std::string&) { return nullptr; }
} // namespace ygl
