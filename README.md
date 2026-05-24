# 🚀 YGL - YANNOO Graphic Library

**YGL** est une librairie graphique C++ moderne conçue pour le rendu 3D, le *ray tracing*, et le traitement d'images sphériques. Développée par **YANNOO**, elle offre des outils optimisés pour la création d'applications graphiques haute performance.

---

## ✨ **Fonctionnalités**
- **Rendu 3D** : Prise en charge des meshes, caméras, et scènes complexes.
- **Ray Tracing** : Moteur de *path tracing* et rendu hybride (déferred + ray tracing).
- **Shaders** : Gestion avancée des shaders (GLSL) pour les effets personnalisés.
- **Chargement de modèles** : Support des formats `.obj`, `.gltf`, `.md2`, `.md3`, `.md5`.
- **Textures et matériaux** : Gestion des textures et propriétés des matériaux.
- **Mathématiques** : Vecteurs (vec2/3/4), matrices (mat4), quaternions, et rayons.

---

## 📦 **Dépendances**
   Dépendance       | Version       | Installation (Ubuntu/Debian)          | Lien                          |
 |------------------|---------------|---------------------------------------|-------------------------------|
 | **C++17**        | Requise       | `sudo apt install g++`                | -                             |
 | **OpenGL**       | 4.6+          | `sudo apt install libgl1-mesa-dev`    | -                             |
 | **GLFW**         | 3.3+          | `sudo apt install libglfw3-dev`       | [glfw.org](https://www.glfw.org) |
 | **GLEW**         | 2.2+          | `sudo apt install libglew-dev`        | [glew.sourceforge.net](http://glew.sourceforge.net) |
 | **CMake**        | 3.10+         | `sudo apt install cmake`              | [cmake.org](https://cmake.org) |
 | **stb_image**    | Intégré       | Inclus dans le projet                  | -                             |

---

## 🛠️ **Compilation**


#!/bin/sh

sudo apt-get install -y libglfw3-dev  # Installe GLFW si manquant

cd build
rm -rf *
cmake ..
cmake --build .
cd ..




