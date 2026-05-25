#pragma once

#include "loaders/loader.hpp"
#include "math/Vec3.hpp"
#include "math/Vec2.hpp"
#include "math/Mat4.hpp"
#include <vector>
#include <string>
#include <memory>
#include <map>

namespace ygl {

class Texture;
class Material;

class GLTFLoader : public Loader {
public:
    GLTFLoader();
    ~GLTFLoader();

    std::vector<std::shared_ptr<Mesh>> load(const std::string& filename) override;
    std::shared_ptr<Mesh> loadSingle(const std::string& filename) override;

    // glTF specific
    struct GLTFNode {
        std::string name;
        int meshIndex = -1;
        int cameraIndex = -1;
        Vec3 translation;
        Quat rotation;
        Vec3 scale;
        Mat4 matrix;
        std::vector<int> children;
    };

    struct GLTFMesh {
        std::string name;
        std::vector<int> primitives;
    };

    struct GLTFPrimitive {
        int indices;
        int material;
        int mode;
        std::map<std::string, int> attributes; // "POSITION", "NORMAL", "TEXCOORD_0", etc.
    };

    struct GLTFAccessor {
        int bufferView;
        int componentType;
        int count;
        std::string type;
        bool normalized;
        std::vector<float> min;
        std::vector<float> max;
    };

    struct GLTFBufferView {
        int buffer;
        size_t byteOffset;
        size_t byteLength;
        size_t byteStride;
    };

    struct GLTFBuffer {
        std::string uri;
        size_t byteLength;
        std::vector<unsigned char> data;
    };

    struct GLTFMaterial {
        std::string name;
        Vec4 baseColorFactor;
        std::string baseColorTexture;
        float metallicFactor;
        float roughnessFactor;
        std::string metallicRoughnessTexture;
        Vec3 emissiveFactor;
        std::string emissiveTexture;
        std::string normalTexture;
        std::string occlusionTexture;
        float alphaCutoff;
        bool doubleSided;
    };

    struct GLTFImage {
        std::string uri;
        std::string mimeType;
        int bufferView;
    };

    struct GLTFTexture {
        int source;
        int sampler;
    };

    const std::vector<GLTFNode>& getNodes() const { return m_nodes; }
    const std::vector<GLTFMesh>& getMeshes() const { return m_meshes; }
    const std::vector<GLTFMaterial>& getMaterials() const { return m_materials; }
    const std::vector<std::shared_ptr<Texture>>& getTextures() const { return m_textures; }
    const std::vector<std::shared_ptr<Material>>& getYGLMaterials() const { return m_yglMaterials; }

    bool loadFromGLB(const std::string& filename);
    bool loadFromGLTF(const std::string& filename);

private:
    std::vector<GLTFNode> m_nodes;
    std::vector<GLTFMesh> m_meshes;
    std::vector<GLTFPrimitive> m_primitives;
    std::vector<GLTFAccessor> m_accessors;
    std::vector<GLTFBufferView> m_bufferViews;
    std::vector<GLTFBuffer> m_buffers;
    std::vector<GLTFMaterial> m_materials;
    std::vector<GLTFImage> m_images;
    std::vector<GLTFTexture> m_textures;

    std::vector<std::shared_ptr<Texture>> m_yglTextures;
    std::vector<std::shared_ptr<Material>> m_yglMaterials;

    bool parseJSON(const std::string& jsonString);
    bool loadBuffers(const std::string& basePath);
    std::shared_ptr<Mesh> createMeshFromGLTFMesh(const GLTFMesh& gltfMesh);
    std::shared_ptr<Material> createMaterialFromGLTFMaterial(const GLTFMaterial& gltfMaterial);
    std::shared_ptr<Texture> loadTexture(const GLTFImage& image, const std::string& basePath);
};

} // namespace ygl
