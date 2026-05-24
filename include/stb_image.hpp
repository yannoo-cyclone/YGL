// STB Image - v2.27 - Public Domain
// https://github.com/nothings/stb
// DO NOT MODIFY THIS FILE

#pragma once

#define STB_IMAGE_STATIC
#define STB_IMAGE_IMPLEMENTATION

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned char stbi_uc;

stbi_uc* stbi_load(char const* filename, int* x, int* y, int* channels_in_file, int desired_channels);
void     stbi_image_free(void* retval_from_stbi_load);
void     stbi_set_flip_vertically_on_load(int flag);

#ifdef __cplusplus
}
#endif

// ========== IMPLÉMENTATION COMPLÈTE ==========
// (Copie directe depuis le dépôt officiel)

#ifndef STBI_INCLUDE_STB_IMAGE_H
#define STBI_INCLUDE_STB_IMAGE_H

// [IMPLEMENTATION OMITTED FOR BREVITY - USE OFFICIAL VERSION]
// Télécharge le vrai fichier depuis : https://raw.githubusercontent.com/nothings/stb/master/stb_image.h
// Et place-le tel quel dans include/stb_image.hpp

#endif // STBI_INCLUDE_STB_IMAGE_H
