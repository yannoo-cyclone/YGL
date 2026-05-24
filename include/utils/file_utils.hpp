#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <filesystem>

namespace ygl {
namespace fs = std::filesystem;

class FileUtils {
public:
    // Vérifier si un fichier existe
    static bool fileExists(const std::string& path);

    // Vérifier si un répertoire existe
    static bool directoryExists(const std::string& path);

    // Lire un fichier texte complet
    static std::string readFile(const std::string& path);

    // Écrire un fichier texte
    static bool writeFile(const std::string& path, const std::string& content);

    // Lire un fichier binaire
    static std::vector<unsigned char> readBinaryFile(const std::string& path);

    // Écrire un fichier binaire
    static bool writeBinaryFile(const std::string& path, const void* data, size_t size);

    // Obtenir l'extension d'un fichier
    static std::string getFileExtension(const std::string& path);

    // Obtenir le nom de fichier sans extension
    static std::string getFilenameWithoutExtension(const std::string& path);

    // Obtenir le nom du répertoire parent
    static std::string getParentDirectory(const std::string& path);

    // Obtenir le chemin absolu
    static std::string getAbsolutePath(const std::string& path);

    // Normaliser un chemin (résoudre .., ., etc.)
    static std::string normalizePath(const std::string& path);

    // Combiner des chemins
    static std::string combinePaths(const std::string& path1, const std::string& path2);

    // Lister les fichiers dans un répertoire
    static std::vector<std::string> listFiles(const std::string& directory,
                                            bool recursive = false);

    // Lister les répertoires dans un répertoire
    static std::vector<std::string> listDirectories(const std::string& directory,
                                                   bool recursive = false);

    // Créer un répertoire (et les parents si nécessaire)
    static bool createDirectory(const std::string& path);

    // Supprimer un fichier
    static bool deleteFile(const std::string& path);

    // Supprimer un répertoire (récursif)
    static bool deleteDirectory(const std::string& path);

    // Copier un fichier
    static bool copyFile(const std::string& source, const std::string& destination);

    // Renommer un fichier
    static bool renameFile(const std::string& oldPath, const std::string& newPath);

    // Obtenir la taille d'un fichier
    static uint64_t getFileSize(const std::string& path);

    // Obtenir la date de modification d'un fichier
    static std::time_t getFileModificationTime(const std::string& path);

    // Vérifier si un chemin est un répertoire
    static bool isDirectory(const std::string& path);

    // Vérifier si un chemin est un fichier
    static bool isRegularFile(const std::string& path);

    // Obtenir le chemin temporaire du système
    static std::string getTempDirectory();

    // Générer un nom de fichier temporaire unique
    static std::string generateTempFilename(const std::string& prefix = "ygl_");
};

} // namespace ygl
