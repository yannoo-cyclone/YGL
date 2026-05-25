#pragma once
#include "loaders/loader.hpp"
#include "math/Vec3.hpp"
#include "math/Vec2.hpp"
#include "math/Quat.hpp"
#include <vector>
#include <string>
#include <map>

namespace ygl {
class MD5Loader : public Loader {
public:
    MD5Loader();
    ~MD5Loader();
    std::vector<std::shared_ptr<Mesh>> load(const std::string& filename) override;
    std::shared_ptr<Mesh> loadSingle(const std::string& filename) override;
    bool LoadMD5(const std::string& filepath, Mesh& mesh);

    struct MD5Joint { std::string name; int parent; Vec3 position; Quat orientation; };
    struct MD5Vertex { int index; float u, v; int startWeight; int countWeight; };
    struct MD5Weight { int joint; float bias; Vec3 position; };
    struct MD5Triangle { int vertexIndices[3]; };
    struct MD5Mesh { std::string shader; std::vector<MD5Vertex> vertices; std::vector<MD5Weight> weights; std::vector<MD5Triangle> triangles; };
    struct MD5AnimationFrame { float time; std::vector<Vec3> positions; std::vector<Quat> orientations; };
    struct MD5Animation { std::string name; int frameRate; int numFrames; std::vector<MD5AnimationFrame> frames; std::vector<std::string> hierarchy; std::vector<int> baseFrame; };

    const std::vector<MD5Joint>& getJoints() const { return m_joints; }
    const std::vector<MD5Mesh>& getMeshes() const { return m_meshes; }
    const std::map<std::string, MD5Animation>& getAnimations() const { return m_animations; }
    void addAnimation(const MD5Animation& animation);

private:
    std::vector<MD5Joint> m_joints;
    std::vector<MD5Mesh> m_meshes;
    std::map<std::string, MD5Animation> m_animations;
    bool parseMD5Mesh(FILE* file);
    bool parseMD5Animation(FILE* file);
    std::shared_ptr<Mesh> createMeshFromMD5Mesh(const MD5Mesh& md5Mesh);
    void applySkeleton(std::shared_ptr<Mesh> mesh, const std::vector<MD5Joint>& joints);
    void applyAnimation(std::shared_ptr<Mesh> mesh, const MD5Animation& animation, int frame);
};
} // namespace ygl
