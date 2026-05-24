#include "loaders/obj_loader.hpp"
#include "loaders/loader.hpp"
#include "core/mesh.hpp"
#include "math/vec2.hpp"
#include "math/vec3.hpp"
#include <fstream>
#include <sstream>
#include <unordered_map>

namespace ygl {

bool OBJLoader::LoadOBJ(const std::string& filepath, Mesh& mesh) {
    std::ifstream file(filepath);
    if (!file.is_open()) return false;

    // --- Stockage temporaire des données brutes OBJ ---
    std::vector<vec3> temp_positions;   // Liste des positions (v)
    std::vector<vec3> temp_normals;      // Liste des normales (vn)
    std::vector<vec2> temp_texcoords;    // Liste des UVs (vt)

    // --- Données finales du maillage ---
    std::vector<Vertex> vertices;        // Sommets finaux (avec position, normale, UV)
    std::vector<unsigned int> indices;   // Indices des triangles

    // --- Map pour éviter les doublons de sommets ---
    // Clé = "pos_idx/tex_idx/norm_idx", Valeur = index dans vertices
    std::unordered_map<std::string, unsigned int> vertex_map;

    // --- Lecture ligne par ligne ---
    std::string line;
    while (std::getline(file, line)) {
        // Ignorer les lignes vides et les commentaires
        if (line.empty() || line[0] == '#') continue;

        auto tokens = Loader::SplitLine(line);
        if (tokens.empty()) continue;

        // --- Vertex Position (v x y z) ---
        if (tokens[0] == "v" && tokens.size() >= 4) {
            temp_positions.emplace_back(
                std::stof(tokens[1]),
                std::stof(tokens[2]),
                std::stof(tokens[3])
            );

        // --- Vertex Normal (vn x y z) ---
        } else if (tokens[0] == "vn" && tokens.size() >= 4) {
            temp_normals.emplace_back(
                std::stof(tokens[1]),
                std::stof(tokens[2]),
                std::stof(tokens[3])
            );

        // --- Texture Coordinate (vt u [v]) ---
        } else if (tokens[0] == "vt" && tokens.size() >= 2) {
            float u = std::stof(tokens[1]);
            float v = (tokens.size() >= 3) ? std::stof(tokens[2]) : 0.0f;
            temp_texcoords.emplace_back(u, v);

        // --- Face (f v1/vt1/vn1 v2/vt2/vn2 v3/vt3/vn3 ...) ---
        } else if (tokens[0] == "f" && tokens.size() >= 4) {
            // Une face OBJ peut avoir 3+ sommets (on suppose des triangles)
            for (size_t i = 1; i < tokens.size(); ++i) {
                const std::string& vertex_token = tokens[i];

                // --- Parsing des indices (format: pos[/tex[/norm]]) ---
                size_t slash1 = vertex_token.find('/');
                size_t slash2 = (slash1 != std::string::npos) ?
                    vertex_token.find('/', slash1 + 1) : std::string::npos;

                // Index position (obligatoire, 1-based dans OBJ)
                unsigned int pos_idx = std::stoul(vertex_token.substr(0, slash1)) - 1;

                // Index texture (optionnel)
                unsigned int tex_idx = 0;
                if (slash1 != std::string::npos) {
                    if (slash2 != std::string::npos) {
                        // Format: pos/tex/norm
                        std::string tex_str = vertex_token.substr(slash1 + 1, slash2 - slash1 - 1);
                        if (!tex_str.empty()) tex_idx = std::stoul(tex_str) - 1;
                    } else {
                        // Format: pos/tex (pas de normale)
                        std::string tex_str = vertex_token.substr(slash1 + 1);
                        if (!tex_str.empty()) tex_idx = std::stoul(tex_str) - 1;
                    }
                }

                // Index normale (optionnel)
                unsigned int norm_idx = 0;
                if (slash2 != std::string::npos) {
                    std::string norm_str = vertex_token.substr(slash2 + 1);
                    if (!norm_str.empty()) norm_idx = std::stoul(norm_str) - 1;
                }

                // --- Création d'une clé unique pour ce sommet ---
                std::string vertex_key =
                    std::to_string(pos_idx) + "/" +
                    std::to_string(tex_idx) + "/" +
                    std::to_string(norm_idx);

                // --- Ajout du sommet s'il n'existe pas déjà ---
                if (vertex_map.find(vertex_key) == vertex_map.end()) {
                    Vertex vertex;
                    vertex.position = temp_positions[pos_idx];

                    // UV si disponible
                    if (tex_idx < temp_texcoords.size()) {
                        vertex.texcoord = temp_texcoords[tex_idx];
                    }

                    // Normale si disponible
                    if (norm_idx < temp_normals.size()) {
                        vertex.normal = temp_normals[norm_idx];
                    } else {
                        vertex.normal = vec3(0.0f); // À calculer plus tard
                    }

                    vertex_map[vertex_key] = static_cast<unsigned int>(vertices.size());
                    vertices.push_back(vertex);
                }

                // --- Ajout de l'index ---
                indices.push_back(vertex_map[vertex_key]);
            }
        }
    }

    // --- Calcul des normales si absentes dans le fichier OBJ ---
    if (temp_normals.empty()) {
        for (size_t i = 0; i < indices.size(); i += 3) {
            if (i + 2 >= indices.size()) break;

            unsigned int i0 = indices[i];
            unsigned int i1 = indices[i+1];
            unsigned int i2 = indices[i+2];

            vec3 edge1 = vertices[i1].position - vertices[i0].position;
            vec3 edge2 = vertices[i2].position - vertices[i0].position;
            vec3 normal = normalize(cross(edge1, edge2));

            vertices[i0].normal += normal;
            vertices[i1].normal += normal;
            vertices[i2].normal += normal;
        }

        // Normalisation finale
        for (auto& vertex : vertices) {
            vertex.normal = normalize(vertex.normal);
        }
    }

    // --- Construction du maillage ---
    mesh.SetVertices(vertices);
    mesh.SetIndices(indices);
    mesh.ComputeBoundingBox();

    return true;
}

} // namespace ygl
