// STB Image - v2.27 - public domain - https://nothings.org/stb_image.h
// DO NOT MODIFY THIS FILE
#pragma once
#define STB_IMAGE_IMPLEMENTATION
#include <stddef.h>

typedef unsigned char stbi_uc;

extern "C" {
int stbi_load(char const* filename, int* x, int* y, int* channels_in_file, int desired_channels);
void stbi_image_free(void* retval_from_stbi_load);
void stbi_set_flip_vertically_on_load(int flag);
}
