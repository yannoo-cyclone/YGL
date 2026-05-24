#pragma once

#include "core/scene.hpp"
#include "core/camera.hpp"

namespace ygl {

class Renderer {
public:
    Renderer(int width, int height);
    virtual ~Renderer();

    virtual bool initialize(const Scene& scene) = 0;
    virtual void cleanup();
    virtual void render(const Scene& scene, const Camera& camera) = 0;

    int getWidth() const { return m_width; }
    int getHeight() const { return m_height; }

    void resize(int width, int height);

protected:
    int m_width;
    int m_height;
};

} // namespace ygl

