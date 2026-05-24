#include "loaders/md2_loader.hpp"
#include "loaders/md2_format.hpp"  // AJOUTÉ
#include <fstream>
#include <sstream>

namespace ygl {

bool MD2Loader::LoadMD2(const std::string& filepath, Mesh& mesh) {
    std::ifstream file(filepath, std::ios::binary);
    if (!file.is_open()) return false;

    MD2Header header;
    file.read(reinterpret_cast<char*>(&header), sizeof(MD2Header));
    if (header.magic != 0x32504449 || header.version != 8) return false;

    std::vector<Vec3> positions;
    std::vector<Vec2> texcoords;
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
            v.normal = Vec3(0.0f);
            vertices.push_back(v);
            indices.push_back(indices.size());
        }
    }

    // Compute normals
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

    // Extraire les données pour Mesh
    std::vector<Vec3> finalPositions;
    std::vector<Vec2> finalTexCoords;
    std::vector<Vec3> finalNormals;
    for (const auto& v : vertices) {
        finalPositions.push_back(v.position);
        finalTexCoords.push_back(v.texcoord);
        finalNormals.push_back(v.normal);
    }
    mesh.setPositions(finalPositions);
    mesh.setTexCoords(finalTexCoords);
    mesh.setNormals(finalNormals);
    mesh.setIndices(indices);
    mesh.updateBoundingBox();
    return true;
}

} // namespace ygl
