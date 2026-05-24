#pragma once

#include "loaders/loader.hpp"
#include "math/vec3.hpp"
#include "math/vec2.hpp"
#include <vector>
#include <string>

namespace ygl {

class MD3Loader : public Loader {
public:
    MD3Loader();
    ~MD3Loader();

    std::vector<std::shared_ptr<Mesh>> load(const std::string& filename) override;
    std::shared_ptr<Mesh> loadSingle(const std::string& filename) override;

    // MD3 specific
    struct MD3Header {
        char magic[4];
        int version;
        char name[64];
        int flags;
        int numFrames;
        int numTags;
        int numSurfaces;
        int numSkins;
        int ofsFrames;
        int ofsTags;
        int ofsSurfaces;
        int ofsEnd;
    };

    struct MD3Frame {
        float minBounds[3];
        float maxBounds[3];
        float localOrigin[3];
        float radius;
        std::string name;
    };

    struct MD3Tag {
        std::string name;
        Vec3 origin;
        Mat4 rotationMatrix;
    };

    struct MD3Surface {
        char magic[4];
        std::string name;
        int flags;
        int numFrames;
        int numShaders;
        int numVerts;
        int numTris;
        int ofsTris;
        int ofsShaders;
        int ofsSt;
        int ofsXyzNormal;
        int ofsEnd;
    };

    const std::vector<MD3Frame>& getFrames() const { return m_frames; }
    const std::vector<MD3Tag>& getTags() const { return m_tags; }
    const std::vector<MD3Surface>& getSurfaces() const { return m_surfaces; }
    int getCurrentFrame() const { return m_currentFrame; }
    void setCurrentFrame(int frame);

private:
    std::vector<MD3Frame> m_frames;
    std::vector<MD3Tag> m_tags;
    std::vector<MD3Surface> m_surfaces;
    int m_currentFrame = 0;

    bool parseHeader(FILE* file, MD3Header& header);
    bool parseFrames(FILE* file, const MD3Header& header);
    bool parseTags(FILE* file, const MD3Header& header);
    bool parseSurfaces(FILE* file, const MD3Header& header);
};

} // namespace ygl
