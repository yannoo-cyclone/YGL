#pragma once

// ============================================================================
// YGL - Yannoo Graphic Library
// Main header file - Include all YGL components
// ============================================================================

// -----------------------------------------------------------------------------
// Math
// -----------------------------------------------------------------------------
#include "math/vec2.hpp"
#include "math/vec3.hpp"
#include "math/vec4.hpp"
#include "math/mat4.hpp"
#include "math/quat.hpp"
#include "math/ray.hpp"

// -----------------------------------------------------------------------------
// Core
// -----------------------------------------------------------------------------
#include "core/object3d.hpp"
#include "core/mesh.hpp"
#include "core/camera.hpp"
#include "core/scene.hpp"
#include "core/bounding_box.hpp"

// -----------------------------------------------------------------------------
// Render
// -----------------------------------------------------------------------------
#include "render/gbuffer.hpp"
#include "render/deferred_renderer.hpp"
#include "render/bvh.hpp"
#include "render/spherical_renderer.hpp"
#include "render/path_tracing_renderer.hpp"
#include "render/hybrid_renderer.hpp"

// -----------------------------------------------------------------------------
// Loaders
// -----------------------------------------------------------------------------
#include "loaders/loader.hpp"
#include "loaders/obj_loader.hpp"
#include "loaders/gltf_loader.hpp"
#include "loaders/md2_loader.hpp"
#include "loaders/md3_loader.hpp"
#include "loaders/md5_loader.hpp"

// -----------------------------------------------------------------------------
// Scene
// -----------------------------------------------------------------------------
#include "scene/light.hpp"
#include "scene/animation.hpp"

// -----------------------------------------------------------------------------
// Materials
// -----------------------------------------------------------------------------
#include "materials/material.hpp"
#include "materials/texture.hpp"

// -----------------------------------------------------------------------------
// Utils
// -----------------------------------------------------------------------------
#include "utils/image_utils.hpp"
#include "utils/file_utils.hpp"
#include "utils/logger.hpp"
#include "utils/timer.hpp"

// -----------------------------------------------------------------------------
// External
// -----------------------------------------------------------------------------
#include "stb_image.h"

