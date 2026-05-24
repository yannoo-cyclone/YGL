#include "loaders/md5_loader.hpp"
#include "core/mesh.hpp"
#include "core/vertex.hpp"
#include <fstream>

namespace ygl {
bool MD5Loader::LoadMD5(const std::string& filepath, Mesh& mesh) {
    std::ifstream file(filepath);
    if (!file.is_open()) return false;

    std::vector<MD5Joint> joints;
    std::vector<MD5Mesh> meshes;
    MD5Mesh current_mesh;
    std::string line;
    bool in_mesh = false;

    while (std::getline(file, line)) {
        if (line.empty()) continue;
        auto tokens = Loader::SplitLine(line);
        if (tokens.empty()) continue;

        if (tokens[0] == "joints" && tokens.size() > 1) {
            int num = std::stoi(tokens[1]);
            for (int i = 0; i < num; ++i) {
                std::getline(file, line);
                auto jt = Loader::SplitLine(line);
                if (jt.size() < 10) continue;
                MD5Joint j;
                j.name = jt[0];
                j.parent = std::stoi(jt[1]);
                j.position = Vec3(std::stof(jt[3]), std::stof(jt[4]), std::stof(jt[5]));
                j.orientation = Quat(std::stof(jt[7]), std::stof(jt[8]), std::stof(jt[9]), std::stof(jt[6]));
                joints.push_back(j);
            }
        } else if (tokens[0] == "mesh" && tokens.size() > 1) {
            in_mesh = true;
            current_mesh = MD5Mesh();
            current_mesh.shader = tokens[1].substr(1, tokens[1].size() - 2);
        } else if (in_mesh && tokens[0] == "}") {
            in_mesh = false;
            meshes.push_back(current_mesh);
        } else if (in_mesh) {
            if (tokens[0] == "numverts" && tokens.size() > 1) {
                int num = std::stoi(tokens[1]);
                for (int i = 0; i < num; ++i) {
                    std::getline(file, line);
                    auto vt = Loader::SplitLine(line);
                    if (vt.size() < 5) continue;
                    MD5Vertex v;
                    v.index = std::stoi(vt[0]);
                    v.u = std::stof(vt[1]);
                    v.v = std::stof(vt[2]);
                    v.startWeight = std::stoi(vt[3]);
                    v.countWeight = std::stoi(vt[4]);
                    current_mesh.vertices.push_back(v);
                }
            } else if (tokens[0] == "numtris" && tokens.size() > 1) {
                int num = std::stoi(tokens[1]);
                for (int i = 0; i < num; ++i) {
                    std::getline(file, line);
                    auto tt = Loader::SplitLine(line);
                    if (tt.size() < 3) continue;
                    MD5Triangle t;
                    t.vertexIndices[0] = std::stoul(tt[0]);
                    t.vertexIndices[1] = std::stoul(tt[1]);
                    t.vertexIndices[2] = std::stoul(tt[2]);
                    current_mesh.triangles.push_back(t);
                }
            } else if (tokens[0] == "numweights" && tokens.size() > 1) {
                int num = std::stoi(tokens[1]);
                for (int i = 0; i < num; ++i) {
                    std::getline(file, line);
                    auto wt = Loader::SplitLine(line);
                    if (wt.size() < 7) continue;
                    MD5Weight w;
                    w.joint = std::stoi(wt[0]);
                    w.bias = std::stof(wt[1]);
                    w.position = Vec3(std::stof(wt[2]), std::stof(wt[3]), std::stof(wt[4]));
                    current_mesh.weights.push_back(w);
                }
            }
        }
    }

    if (meshes.empty()) return false;
    const auto& m = meshes[0];
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    for (const auto& tri : m.triangles) {
        indices.push_back(tri.vertexIndices[0]);
        indices.push_back(tri.vertexIndices[1]);
        indices.push_back(tri.vertexIndices[2]);
    }

    for (const auto& vert : m.vertices) {
        Vec3 pos(0.0f);
        for (int j = 0; j < vert.countWeight; ++j) {
            const auto& w = m.weights[vert.startWeight + j];
            const auto& jt = joints[w.joint];
            Vec3 rotated = jt.orientation * w.position;
            pos += (rotated + jt.position) * w.bias;
        }
        vertices.push_back({pos, Vec2(vert.u, vert.v), Vec3(0.0f)});
    }

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

    std::vector<Vec3> positions, normals;
    std::vector<Vec2> texcoords;
    for (const auto& v : vertices) {
        positions.push_back(v.position);
        normals.push_back(v.normal);
        texcoords.push_back(v.texcoord);
    }
    mesh.setPositions(positions);
    mesh.setNormals(normals);
    mesh.setTexCoords(texcoords);
    mesh.setIndices(indices);
    mesh.updateBoundingBox();
    return true;
}

std::vector<std::shared_ptr<Mesh>> MD5Loader::load(const std::string& filename) {
    auto m = std::make_shared<Mesh>();
    return LoadMD5(filename, *m) ? std::vector{m} : std::vector<std::shared_ptr<Mesh>>();
}
std::shared_ptr<Mesh> MD5Loader::loadSingle(const std::string& filename) {
    auto m = std::make_shared<Mesh>();
    return LoadMD5(filename, *m) ? m : nullptr;
}
MD5Loader::MD5Loader() = default;
MD5Loader::~MD5Loader() = default;
void MD5Loader::addAnimation(const MD5Animation& animation) { m_animations[animation.name] = animation; }
bool MD5Loader::parseMD5Mesh(FILE* /*file*/) { return false; }
bool MD5Loader::parseMD5Animation(FILE* /*file*/) { return false; }
std::shared_ptr<Mesh> MD5Loader::createMeshFromMD5Mesh(const MD5Mesh& /*md5Mesh*/) { return nullptr; }
void MD5Loader::applySkeleton(std::shared_ptr<Mesh> /*mesh*/, const std::vector<MD5Joint>& /*joints*/) {}
void MD5Loader::applyAnimation(std::shared_ptr<Mesh> /*mesh*/, const MD5Animation& /*animation*/, int /*frame*/) {}
} // namespace ygl
