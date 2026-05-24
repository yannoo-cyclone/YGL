cmake_minimum_required(VERSION 3.10)

# Project name and version
project(YGL VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Enable OpenMP
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")

# Architecture-specific optimizations
if(CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64" OR CMAKE_SYSTEM_PROCESSOR MATCHES "AMD64")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mavx2 -mfma -march=native")
endif()

# Output directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Add cmake modules directory
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

# Find dependencies
find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)

# Include directories
include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${GLEW_INCLUDE_DIRS}
    ${GLFW3_INCLUDE_DIRS}
    ${OPENGL_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/extern
)

# Source files for YGL library
set(YGL_SOURCES
    # Math
    src/math/vec2.cpp
    src/math/vec3.cpp
    src/math/vec4.cpp
    src/math/mat4.cpp
    src/math/quat.cpp
    src/math/ray.cpp

    # Core
    src/core/object3d.cpp
    src/core/camera.cpp
    src/core/mesh.cpp
    src/core/scene.cpp
    src/core/bounding_box.cpp

    # Render
    src/render/renderer.cpp
    src/render/gbuffer.cpp
    src/render/deferred_renderer.cpp
    src/render/bvh.cpp
    src/render/spherical_gbuffer.cpp
    src/render/path_tracing_renderer.cpp
    src/render/hybrid_renderer.cpp

    # Loaders
    src/loaders/loader.cpp
    src/loaders/obj_loader.cpp
    src/loaders/md2_loader.cpp
    src/loaders/md3_loader.cpp
    src/loaders/md5_loader.cpp
    src/loaders/gltf_loader.cpp

    # Materials
    src/materials/material.cpp
    src/materials/texture.cpp

    # Utils
    src/utils/image_utils.cpp
    src/utils/file_utils.cpp

    # Scene
    src/scene/scene.cpp
    src/scene/light.cpp
    src/scene/animation.cpp
)

# Create YGL library
add_library(ygl ${YGL_SOURCES})

target_link_libraries(ygl
    ${GLEW_LIBRARIES}
    ${GLFW3_LIBRARIES}
    ${OPENGL_LIBRARIES}
    stdc++fs
)

# Set compile options for YGL library
target_compile_options(ygl PRIVATE
    -O3
    -Wall
    -Wextra
    -Wpedantic
)

# Copy assets to build directory
function(copy_assets target_dir)
    # Models
    file(COPY ${CMAKE_SOURCE_DIR}/assets/models/ DESTINATION ${target_dir}/assets/models/)

    # Shaders
    file(COPY ${CMAKE_SOURCE_DIR}/assets/shaders/ DESTINATION ${target_dir}/assets/shaders/)

    # Textures
    file(COPY ${CMAKE_SOURCE_DIR}/assets/textures/ DESTINATION ${target_dir}/assets/textures/)
endfunction()

# --- Examples ---

# Viewer example
add_executable(ygl_viewer examples/viewer/main.cpp)

target_link_libraries(ygl_viewer
    ygl
    ${GLEW_LIBRARIES}
    ${GLFW3_LIBRARIES}
    ${OPENGL_LIBRARIES}
    stdc++fs
)

copy_assets(${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

# Set working directory for viewer
set_target_properties(ygl_viewer PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

# Batch render example
add_executable(ygl_batch_render examples/batch_render/main.cpp)

target_link_libraries(ygl_batch_render
    ygl
    ${GLEW_LIBRARIES}
    ${GLFW3_LIBRARIES}
    ${OPENGL_LIBRARIES}
    stdc++fs
)

copy_assets(${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

set_target_properties(ygl_batch_render PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

# Path tracing test example
add_executable(ygl_test_raytracing examples/test_raytracing/main.cpp)

target_link_libraries(ygl_test_raytracing
    ygl
    ${GLEW_LIBRARIES}
    ${GLFW3_LIBRARIES}
    ${OPENGL_LIBRARIES}
    stdc++fs
)

copy_assets(${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

set_target_properties(ygl_test_raytracing PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

# --- Tests ---
option(BUILD_TESTS "Build test programs" OFF)

if(BUILD_TESTS)
    add_subdirectory(tests)
endif()

# --- Install ---
install(TARGETS ygl
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
)

install(DIRECTORY include/ DESTINATION include)

install(DIRECTORY assets/ DESTINATION share/ygl/assets)

install(TARGETS ygl_viewer ygl_batch_render ygl_test_raytracing
    RUNTIME DESTINATION bin
)

# --- Output ---
message(STATUS "YGL Configuration:")
message(STATUS "  C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "  OpenMP: ${CMAKE_CXX_FLAGS}")
message(STATUS "  GLEW Found: ${GLEW_FOUND}")
message(STATUS "  GLFW3 Found: ${GLFW3_FOUND}")
message(STATUS "  OpenGL Found: ${OPENGL_FOUND}")
message(STATUS "  GLEW Include: ${GLEW_INCLUDE_DIRS}")
message(STATUS "  GLFW3 Include: ${GLFW3_INCLUDE_DIRS}")
message(STATUS "  OpenGL Include: ${OPENGL_INCLUDE_DIRS}")
