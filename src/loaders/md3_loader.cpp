#include "loaders/md3_loader.hpp"
#include "core/mesh.hpp"
#include "core/vertex.hpp"
#include <fstream>
#include <vector>

namespace ygl {

bool MD3Loader::LoadMD3(const std::string& filepath, Mesh& mesh) {
    std::ifstream file(filepath, std::ios::binary);
    if (!file.is_open()) return false;

    MD3Header header;
    file.read(reinterpret_cast<char*>(&header), sizeof(MD3Header));
    if (std::string(header.magic, 4) != "IDP3" || header.version != 15) return false;

    file.seekg(header.ofsFrames);
    m_frames.resize(header.numFrames);
    file.read(reinterpret_cast<char*>(m_frames.data()), header.numFrames * sizeof(MD3Frame));

    file.seekg(header.ofsSurfaces);
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    for (int s = 0; s < header.numSurfaces; ++s) {
        MD3Surface surface;
        file.read(reinterpret_cast<char*>(&surface), sizeof(MD3Surface));

        file.seekg(surface.ofsTris);
        std::vector<MD3Triangle> tris(surface.numTris);
        file.read(reinterpret_cast<char*>(tris.data()), surface.numTris * sizeof(MD3Triangle));

        file.seekg(surface.ofsSt);
        std::vector<MD3TexCoord> tcs(surface.numVerts);
        file.read(reinterpret_cast<char*>(tcs.data()), surface.numVerts * sizeof(MD3TexCoord));

        file.seekg(surface.ofsXyzNormal);
        std::vector<MD3Vertex> verts(surface.numVerts * surface.numFrames);
        file.read(reinterpret_cast<char*>(verts.data()), surface.numVerts * surface.numFrames * sizeof(MD3Vertex));

        size_t base = vertices.size();
        for (int i = 0; i < surface.numVerts; ++i) {
            Vertex v;
            v.position = Vec3(verts[i].xyz[0] / 64.0f, verts[i].xyz[1] / 64.0f, verts[i].xyz[2] / 64.0f);
            v.texcoord = Vec2(tcs[i].st[0], tcs[i].st[1]);
            v.normal = Vec3(0.0f);
            vertices.push_back(v);
        }
        for (const auto& tri : tris) {
            indices.push_back(base + tri.indexes[0]);
            indices.push_back(base + tri.indexes[1]);
            indices.push_back(base + tri.indexes[2]);
        }
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

    // Extraction des données pour Mesh
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

std::vector<std::shared_ptr<Mesh>> MD3Loader::load(const std::string& filename) {
    auto m = std::make_shared<Mesh>();
    return LoadMD3(filename, *m) ? std::vector{m} : std::vector<std::shared_ptr<Mesh>>();
}
std::shared_ptr<Mesh> MD3Loader::loadSingle(const std::string& filename) {
    auto m = std::make_shared<Mesh>();
    return LoadMD3(filename, *m) ? m : nullptr;
}
MD3Loader::MD3Loader() = default;
MD3Loader::~MD3Loader() = default;
void MD3Loader::setCurrentFrame(int frame) { if (frame >= 0 && frame < static_cast<int>(m_frames.size())) m_currentFrame = frame; }
} // namespace ygl
