#pragma once

#include "loaders/loader.hpp"
#include "math/vec3.hpp"
#include "math/vec2.hpp"
#include <vector>
#include <string>

namespace ygl {

class MD2Loader : public Loader {
public:
    MD2Loader();
    ~MD2Loader();

    std::vector<std::shared_ptr<Mesh>> load(const std::string& filename) override;
    std::shared_ptr<Mesh> loadSingle(const std::string& filename) override;

    // MD2 specific
    struct MD2Header {
        int magic;
        int version;
        int skinWidth;
        int skinHeight;
        int frameSize;
        int numSkins;
        int numVertices;
        int numTexCoords;
        int numTriangles;
        int numGlCommands;
        int numFrames;
        int offsetSkins;
        int offsetTexCoords;
        int offsetTriangles;
        int offsetFrames;
        int offsetGlCommands;
        int offsetEnd;
    };

    struct MD2Frame {
        Vec3 scale;
        Vec3 translate;
        std::string name;
        std::vector<Vec3> vertices;
    };

    struct MD2Animation {
        std::string name;
        int firstFrame;
        int lastFrame;
        float fps;
    };

    const std::vector<MD2Frame>& getFrames() const { return m_frames; }
    const std::vector<MD2Animation>& getAnimations() const { return m_animations; }
    int getCurrentFrame() const { return m_currentFrame; }
    void setCurrentFrame(int frame);

private:
    std::vector<MD2Frame> m_frames;
    std::vector<MD2Animation> m_animations;
    int m_currentFrame = 0;

    bool parseHeader(FILE* file, MD2Header& header);
    bool parseFrames(FILE* file, const MD2Header& header);
    bool parseAnimations();
};

} // namespace ygl
