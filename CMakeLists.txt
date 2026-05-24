cmake_minimum_required(VERSION 3.10)
project(YGL LANGUAGES CXX)

# ======================
# C++ Standard
# ======================
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ======================
# Options de compilation
# ======================
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -O2 -fopenmp -mavx2 -mfma -march=native")

# ======================
# Trouver les dépendances
# ======================
find_package(OpenMP REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)

# Affiche les infos de configuration (optionnel)
message(STATUS "YGL Configuration:")
message(STATUS "  C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "  OpenMP: ${OpenMP_CXX_FLAGS}")
message(STATUS "  GLEW Found: ${GLEW_FOUND}")
message(STATUS "  GLFW3 Found: ${glfw3_FOUND}")
message(STATUS "  OpenGL Found: ${OpenGL_FOUND}")
message(STATUS "  GLEW Include: ${GLEW_INCLUDE_DIRS}")
message(STATUS "  GLFW3 Include: ${glfw3_INCLUDE_DIRS}")
message(STATUS "  OpenGL Include: ${OpenGL_INCLUDE_DIRS}")

# ======================
# Dossiers d'inclusion
# ======================
include_directories(
    include
    ${GLEW_INCLUDE_DIRS}
    ${glfw3_INCLUDE_DIRS}
    ${OpenGL_INCLUDE_DIRS}
)

# ======================
# Fichiers sources (Core)
# ======================
set(CORE_SOURCES
    src/core/bounding_box.cpp
    src/core/camera.cpp
    src/core/mesh.cpp
    src/core/object3d.cpp
    src/core/scene.cpp
)

# ======================
# Fichiers sources (Loaders)
# ======================
set(LOADERS_SOURCES
    src/loaders/gltf_loader.cpp
    src/loaders/loader.cpp
    src/loaders/md2_loader.cpp
    src/loaders/md3_loader.cpp
    src/loaders/md5_loader.cpp
    src/loaders/obj_loader.cpp
)

# ======================
# Fichiers sources (Materials)
# ======================
set(MATERIALS_SOURCES
    src/materials/material.cpp
    src/materials/texture.cpp
)

# ======================
# Fichiers sources (Math)
# ======================
set(MATH_SOURCES
    src/math/mat4.cpp
    src/math/quat.cpp
    src/math/ray.cpp
    src/math/vec2.cpp
    src/math/vec3.cpp
    src/math/vec4.cpp
)

# ======================
# Fichiers sources (Render)
# ======================
set(RENDER_SOURCES
    src/render/bvh.cpp
    src/render/deferred_renderer.cpp
    src/render/gbuffer.cpp
    src/render/hybrid_renderer.cpp
    src/render/path_tracing_renderer.cpp
    src/render/spherical_renderer.cpp
)

# ======================
# Fichiers sources (Scene)
# ======================
set(SCENE_SOURCES
    src/scene/animation.cpp
    src/scene/light.cpp
    src/scene/scene.cpp
)

# ======================
# Fichiers sources (Utils)
# ======================
set(UTILS_SOURCES
    src/utils/file_utils.cpp
    src/utils/image_utils.cpp
)

# ======================
# Bibliothèque YGL
# ======================
add_library(ygl
    ${CORE_SOURCES}
    ${LOADERS_SOURCES}
    ${MATERIALS_SOURCES}
    ${MATH_SOURCES}
    ${RENDER_SOURCES}
    ${SCENE_SOURCES}
    ${UTILS_SOURCES}
)

# Lier les dépendances à la bibliothèque
target_link_libraries(ygl
    ${GLEW_LIBRARIES}
    ${glfw3_LIBRARIES}
    ${OpenGL_LIBRARIES}
    OpenMP::OpenMP_CXX
)

# ======================
# Exécutables (Exemples)
# ======================
# Viewer
add_executable(viewer examples/viewer/main.cpp)
target_link_libraries(viewer ygl)

# Batch Render
add_executable(batch_render examples/batch_render/main.cpp)
target_link_libraries(batch_render ygl)

# Test Raytracing
add_executable(test_raytracing examples/test_raytracing/main.cpp)
target_link_libraries(test_raytracing ygl)
