#include "utils/image_utils.h"
#include <stb_image.h>
#include <stb_image_write.h>
#include <vector>
#include <algorithm>

namespace ygl {
namespace image_utils {

std::vector<unsigned char> LoadImage(const std::string& path, int& w, int& h, int& c) {
    int width, height, channels;
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
    if (!data) { w = h = c = 0; return {}; }
    w = width; h = height; c = channels;
    size_t size = w * h * c;
    std::vector<unsigned char> result(data, data + size);
    stbi_image_free(data);
    return result;
}

std::vector<float> LoadImageFloat(const std::string& path, int& w, int& h, int& c) {
    int width, height, channels;
    float* data = stbi_loadf(path.c_str(), &width, &height, &channels, 0);
    if (!data) { w = h = c = 0; return {}; }
    w = width; h = height; c = channels;
    size_t size = w * h * c;
    std::vector<float> result(data, data + size);
    stbi_image_free(data);
    return result;
}

bool SaveImage(const std::string& path, int w, int h, int c, const unsigned char* data) {
    return stbi_write_png(path.c_str(), w, h, c, data, w * c) != 0;
}

bool SaveImageFloat(const std::string& path, int w, int h, int c, const float* data) {
    std::vector<unsigned char> bytes(w * h * c);
    for (size_t i = 0; i < bytes.size(); ++i) bytes[i] = (unsigned char)(std::clamp(data[i], 0.0f, 1.0f) * 255.0f);
    return SaveImage(path, w, h, c, bytes.data());
}

std::vector<unsigned char> ResizeImage(const std::vector<unsigned char>& img, int ow, int oh, int oc, int nw, int nh) {
    if (nw <= 0 || nh <= 0) return {};
    std::vector<unsigned char> result(nw * nh * oc);
    float xr = (float)ow / nw, yr = (float)oh / nh;
    for (int y = 0; y < nh; ++y) {
        for (int x = 0; x < nw; ++x) {
            int ox = (int)(x * xr), oy = (int)(y * yr);
            ox = std::min(ox, ow - 1); oy = std::min(oy, oh - 1);
            for (int ch = 0; ch < oc; ++ch)
                result[(y * nw + x) * oc + ch] = img[(oy * ow + ox) * oc + ch];
        }
    }
    return result;
}

std::vector<unsigned char> FlipImageVertically(const std::vector<unsigned char>& img, int w, int h, int c) {
    std::vector<unsigned char> result(img.size());
    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            int dy = h - 1 - y;
            for (int ch = 0; ch < c; ++ch)
                result[(dy * w + x) * c + ch] = img[(y * w + x) * c + ch];
        }
    }
    return result;
}

std::vector<unsigned char> ConvertToRGBA(const std::vector<unsigned char>& img, int w, int h, int oc) {
    if (oc == 4) return img;
    std::vector<unsigned char> result(w * h * 4);
    for (size_t i = 0; i < w * h; ++i) {
        for (int ch = 0; ch < 3; ++ch) result[i * 4 + ch] = (ch < oc) ? img[i * oc + ch] : 0;
        result[i * 4 + 3] = (oc == 3) ? 255 : (oc > 3 ? img[i * oc + 3] : 255);
    }
    return result;
}

std::vector<unsigned char> ConvertToRGB(const std::vector<unsigned char>& img, int w, int h, int oc) {
    if (oc == 3) return img;
    std::vector<unsigned char> result(w * h * 3);
    for (size_t i = 0; i < w * h; ++i)
        for (int ch = 0; ch < 3; ++ch)
            result[i * 3 + ch] = (ch < oc) ? img[i * oc + ch] : 0;
    return result;
}

} // namespace image_utils
} // namespace ygl
