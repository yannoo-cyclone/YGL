# YGL - Yannoo Graphic Library

**YGL** est une bibliothèque C++ moderne pour le rendu 3D haute performance avec support pour le **path tracing** et la **radiosité** via des **G-Buffers sphériques**.

---

## 🎯 Architecture


YGL/
├── include/          # Headers
│   ├── ygl.hpp      # Header principal
│   ├── math/        # Mathématiques (vec2, vec3, mat4, quat...)
│   ├── core/        # Noyau (object3d, mesh, camera, scene)
│   ├── render/      # Rendering (gbuffer, deferred, bvh, spherical_gbuffer...)
│   ├── loaders/     # Chargeurs de fichiers (obj, gltf, md2...)
│   ├── scene/       # Scène (light, animation)
│   ├── materials/   # Matériaux et textures
│   └── utils/       # Utilitaires
├── src/             # Implémentations
├── assets/          # Ressources (modèles, shaders, textures)
├── examples/        # Exemples (viewer, batch_render...)
├── extern/          # Dépendances externes
├── tests/           # Tests
└── cmake/           # Modules CMake


## Model Formats
### Supported Formats
- **OBJ**: Wavefront OBJ (positions, normals, UVs, faces)
- **MD2**: Quake 2 (animated models)
- **MD3**: Quake 3 (surfaces, tags)
- **MD5**: Doom 3 (skeletal animation)
- **GLTF**: GL Transmission Format (modern, JSON-based)

### Adding Models
1. Place model files in `assets/models/`
2. Use any of the supported formats
3. Models are automatically loaded by the loader system


## Shaders
### Pipeline Overview

YGL uses a **hybrid OpenGL + path tracing** pipeline with **spherical G-Buffer radiosity


1) Geometry Pass → G-Buffer (position, normal, albedo, etc.)
2) Spherical Propagation Pass → O(n) radiosity via cube maps
3) Composite Pass → Combine with direct lighting + post-processing

### Shader Details

#### `spherical_propagate.vert/frag`
- **Purpose**: Propagate radiosity in spherical coordinates
- **Technique**: Uses cube maps for emitter/receiver buffers
- **Complexity**: O(n) instead of traditional O(n²)
- **Key Features**:
  - Spherical coordinate system
  - Cube map sampling for 360° coverage
  - Energy attenuation based on distance

#### `composite.vert/frag`
- **Purpose**: Final image composition
- **Features**:
  - Combines spherical G-Buffer with direct lighting
  - Tone mapping (Reinhard)
  - Gamma correction
  - Debug visualizations (normals, lighting components)

---

## Textures

### Current Textures
- **white.png**: 1x1 white texture (PPM format, compatible with stb_image)

### Adding Textures
1. Place texture files in `assets/textures/`
2. Supported formats: PNG, JPG, TGA (via stb_image)



### Dépendances
- OpenGL 3.3+
- GLFW 3.3+
- GLEW
- C++17

### Compilation (Linux)
mkdir build && cd build
cmake ..
cmake --build .








