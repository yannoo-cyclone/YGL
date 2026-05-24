#include "loaders/md5_loader.hpp"
#include "loaders/loader.hpp"
#include "core/mesh.hpp"
#include "math/vec3.hpp"
#include "math/quat.hpp"
#include <fstream>
#include <vector>
#include <string>

namespace ygl {

struct MD5Vertex { int index; vec2 texcoord; int start_weight, count_weight; };
struct MD5Weight { int joint; float bias; vec3 position; };
struct MD5Joint { std::string name; int parent; vec3 position; quat orientation; };
struct MD5Mesh { std::string shader; std::vector<MD5Vertex> vertices; std::vector<unsigned int> triangles; std::vector<MD5Weight> weights; };

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
                j.position = vec3(std::stof(jt[3]), std::stof(jt[4]), std::stof(jt[5]));
                j.orientation = quat(std::stof(jt[7]), std::stof(jt[8]), std::stof(jt[9]), std::stof(jt[6]));
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
                    v.texcoord = vec2(std::stof(vt[1]), std::stof(vt[2]));
                    v.start_weight = std::stoi(vt[3]);
                    v.count_weight = std::stoi(vt[4]);
                    current_mesh.vertices.push_back(v);
                }
            } else if (tokens[0] == "numtris" && tokens.size() > 1) {
                int num = std::stoi(tokens[1]);
                for (int i = 0; i < num; ++i) {
                    std::getline(file, line);
                    auto tt = Loader::SplitLine(line);
                    if (tt.size() < 3) continue;
                    current_mesh.triangles.push_back(std::stoul(tt[0]));
                    current_mesh.triangles.push_back(std::stoul(tt[1]));
                    current_mesh.triangles.push_back(std::stoul(tt[2]));
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
                    w.position = vec3(std::stof(wt[2]), std::stof(wt[3]), std::stof(wt[4]));
                    current_mesh.weights.push_back(w);
                }
            }
        }
    }

    if (meshes.empty()) return false;
    const auto& m = meshes[0];
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices = m.triangles;

    for (const auto& vert : m.vertices) {
        vec3 pos(0.0f);
        for (int j = 0; j < vert.count_weight; ++j) {
            const auto& w = m.weights[vert.start_weight + j];
            const auto& jt = joints[w.joint];
            vec3 rotated = jt.orientation * w.position;
            pos += (rotated + jt.position) * w.bias;
        }
        vertices.push_back({pos, vert.texcoord, vec3(0.0f)});
    }

    for (size_t i = 0; i < indices.size(); i += 3) {
        if (i + 2 >= indices.size()) break;
        vec3 e1 = vertices[indices[i+1]].position - vertices[indices[i]].position;
        vec3 e2 = vertices[indices[i+2]].position - vertices[indices[i]].position;
        vec3 n = normalize(cross(e1, e2));
        vertices[indices[i]].normal += n;
        vertices[indices[i+1]].normal += n;
        vertices[indices[i+2]].normal += n;
    }
    for (auto& v : vertices) v.normal = normalize(v.normal);

    mesh.SetVertices(vertices);
    mesh.SetIndices(indices);
    mesh.ComputeBoundingBox();
    return true;
}

} // namespace ygl
