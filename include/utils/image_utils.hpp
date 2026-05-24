#pragma once

#include <string>
#include <vector>

namespace ygl {

class ImageUtils {
public:
    // Charger une image depuis un fichier
    static std::vector<unsigned char> loadImage(const std::string& filename,
                                               int& width,
                                               int& height,
                                               int& channels,
                                               int desiredChannels = 4);

    // Charger une image HDR (16-bit float)
    static std::vector<float> loadHDRImage(const std::string& filename,
                                          int& width,
                                          int& height,
                                          int& channels);

    // Sauvegarder une image
    static bool saveImage(const std::string& filename,
                         int width,
                         int height,
                         int channels,
                         const unsigned char* data);

    // Sauvegarder une image HDR
    static bool saveHDRImage(const std::string& filename,
                           int width,
                           int height,
                           int channels,
                           const float* data);

    // Redimensionner une image
    static std::vector<unsigned char> resizeImage(const unsigned char* data,
                                                int srcWidth, int srcHeight, int srcChannels,
                                                int dstWidth, int dstHeight);

    // Convertir entre formats
    static std::vector<unsigned char> convertRGBToRGBA(const unsigned char* rgbData,
                                                    int width, int height);
    static std::vector<unsigned char> convertRGBAToRGB(const unsigned char* rgbaData,
                                                    int width, int height);

    // Inverser une image verticalement
    static void flipImageVertically(std::vector<unsigned char>& data,
                                  int width, int height, int channels);

    // Inverser une image horizontalement
    static void flipImageHorizontally(std::vector<unsigned char>& data,
                                    int width, int height, int channels);

    // Générer une image vide (noir, blanc, couleur)
    static std::vector<unsigned char> createEmptyImage(int width, int height, int channels,
                                                     const unsigned char* color = nullptr);

    // Calculer l'histogramme d'une image
    static std::vector<int> computeHistogram(const unsigned char* data,
                                           int width, int height, int channels);

    // Normaliser une image (0-1)
    static std::vector<float> normalizeImage(const unsigned char* data,
                                           int width, int height, int channels);

    // Vérifier si un fichier est une image HDR
    static bool isHDRImage(const std::string& filename);

    // Obtenir les informations d'une image sans la charger
    static bool getImageInfo(const std::string& filename,
                           int& width, int& height, int& channels);
};

} // namespace ygl
