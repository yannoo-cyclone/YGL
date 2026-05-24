#include "loaders/md2_loader.hpp"
#include "loaders/loader.hpp"
#include "core/mesh.hpp"
#include "math/vec3.hpp"
#include <fstream>
#include <vector>

namespace ygl {

#pragma pack(push, 1)
struct MD2Header { int magic, version, skinwidth, skinheight, framesize, num_skins, num_vertices, num_st, num_tris, num_glcmds, num_frames, offset_skins, offset_st, offset_tris, offset_frames, offset_glcmds, offset_end; };
struct MD2Vertex { unsigned char v[3], lightnormalindex; };
struct MD2TexCoord { short s, t; };
struct MD2Triangle { unsigned short vertex[3], st[3]; };
struct MD2Frame { float scale[3], translate[3]; char name[16]; MD2Vertex vertices[1]; };
#pragma pack(pop)

bool MD2Loader::LoadMD2(const std::string& filepath, Mesh& mesh) {
    std::ifstream file(filepath, std::ios::binary);
    if (!file.is_open()) return false;

    MD2Header header;
    file.read(reinterpret_cast<char*>(&header), sizeof(MD2Header));
    if (header.magic != 0x32504449 || header.version != 8) return false;

    std::vector<vec3> positions;
    std::vector<vec2> texcoords;
    file.seekg(header.offset_frames);
    MD2Frame frame;
    file.read(reinterpret_cast<char*>(&frame), sizeof(MD2Frame) - sizeof(MD2Vertex) + header.num_vertices * sizeof(MD2Vertex));

    for (int i = 0; i < header.num_vertices; ++i) {
        auto& v = frame.vertices[i];
        positions.emplace_back(v.v[0] * frame.scale[0] + frame.translate[0],
                               v.v[1] * frame.scale[1] + frame.translate[1],
                               v.v[2] * frame.scale[2] + frame.translate[2]);
    }

    file.seekg(header.offset_st);
    for (int i = 0; i < header.num_st; ++i) {
        MD2TexCoord tc;
        file.read(reinterpret_cast<char*>(&tc), sizeof(MD2TexCoord));
        texcoords.emplace_back(tc.s / (float)header.skinwidth, tc.t / (float)header.skinheight);
    }

    file.seekg(header.offset_tris);
    std::vector<MD2Triangle> tris(header.num_tris);
    file.read(reinterpret_cast<char*>(tris.data()), header.num_tris * sizeof(MD2Triangle));

    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    for (const auto& tri : tris) {
        for (int j = 0; j < 3; ++j) {
            Vertex v;
            v.position = positions[tri.vertex[j]];
            v.texcoord = texcoords[tri.st[j]];
            v.normal = vec3(0.0f);
            vertices.push_back(v);
            indices.push_back(indices.size());
        }
    }

    // Compute normals
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
