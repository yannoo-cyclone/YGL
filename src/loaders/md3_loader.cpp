#include "loaders/md3_loader.hpp"
#include "loaders/loader.hpp"
#include "core/mesh.hpp"
#include "math/vec3.hpp"
#include <fstream>
#include <vector>

namespace ygl {

#pragma pack(push, 1)
struct MD3Header { char fileID[4]; int fileVersion; char fileName[64]; int numFrames, numTags, numSurfaces, numSkins, ofsFrames, ofsTags, ofsSurfaces, ofsEnd; };
struct MD3Frame { float minBounds[3], maxBounds[3], localOrigin[3], radius; char name[16]; };
struct MD3Surface { char surfaceID[4], surfaceName[64]; int flags, numFrames, numShaders, numVerts, numTris, ofsTris, ofsShaders, ofsSt, ofsXyzNormal, ofsEnd; };
struct MD3Vertex { short xyz[3]; unsigned char normal[2]; };
struct MD3TexCoord { float st[2]; };
struct MD3Triangle { int indexes[3]; };
#pragma pack(pop)

bool MD3Loader::LoadMD3(const std::string& filepath, Mesh& mesh) {
    std::ifstream file(filepath, std::ios::binary);
    if (!file.is_open()) return false;

    MD3Header header;
    file.read(reinterpret_cast<char*>(&header), sizeof(MD3Header));
    if (std::string(header.fileID, 4) != "IDP3" || header.fileVersion != 15) return false;

    file.seekg(header.ofsFrames);
    std::vector<MD3Frame> frames(header.numFrames);
    file.read(reinterpret_cast<char*>(frames.data()), header.numFrames * sizeof(MD3Frame));

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
            v.position = vec3(verts[i].xyz[0] / 64.0f, verts[i].xyz[1] / 64.0f, verts[i].xyz[2] / 64.0f);
            v.texcoord = vec2(tcs[i].st[0], tcs[i].st[1]);
            v.normal = vec3(0.0f);
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
