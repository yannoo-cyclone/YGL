#pragma once

#include <cstdint>

namespace ygl {

#pragma pack(push, 1)
// Structs MD2 (format binaire)
struct MD2Header {
    int32_t magic;          // 0x32504449 ("IDP2")
    int32_t version;       // 8
    int32_t skinwidth;     // Largeur des textures
    int32_t skinheight;    // Hauteur des textures
    int32_t framesize;     // Taille d'une frame
    int32_t num_skins;     // Nombre de skins
    int32_t num_vertices;  // Nombre de sommets par frame
    int32_t num_st;        // Nombre de coordonnées de texture
    int32_t num_tris;      // Nombre de triangles
    int32_t num_glcmds;    // Nombre de commandes OpenGL
    int32_t num_frames;    // Nombre de frames
    int32_t offset_skins;   // Offset des skins
    int32_t offset_st;      // Offset des coordonnées de texture
    int32_t offset_tris;    // Offset des triangles
    int32_t offset_frames;  // Offset des frames
    int32_t offset_glcmds;  // Offset des commandes OpenGL
    int32_t offset_end;     // Offset de fin
};

struct MD2Vertex {
    uint8_t v[3];         // Coordonnées (x, y, z)
    uint8_t lightnormal;   // Index de la normale
};

struct MD2TexCoord {
    int16_t s, t;         // Coordonnées de texture
};

struct MD2Triangle {
    uint16_t vertex[3];   // Indices des sommets
    uint16_t st[3];       // Indices des coordonnées de texture
};

struct MD2Frame {
    float scale[3];       // Échelle
    float translate[3];  // Translation
    char name[16];       // Nom de la frame
    MD2Vertex vertices[1]; // Sommets (taille variable)
};
#pragma pack(pop)

} // namespace ygl
