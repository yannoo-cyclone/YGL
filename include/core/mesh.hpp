#pragma once

#include "core/object3d.hpp"
#include "math/vec3.hpp"
#include "math/vec2.hpp"
#include <vector>
#include <memory>

namespace ygl {

class Material;
class BoundingBox;

class Mesh : public Object3D {
public:
    enum class PrimitiveType {
        POINTS,
        LINES,
        LINE_LOOP,
        LINE_STRIP,
        TRIANGLES,
        TRIANGLE_STRIP,
        TRIANGLE_FAN
    };

    Mesh(const std::string& name = "Mesh");
    virtual ~Mesh();

    const std::vector<Vec3>& getPositions() const { return m_positions; }
    void setPositions(const std::vector<Vec3>& positions);
    void addPosition(const Vec3& position);

    const std::vector<Vec3>& getNormals() const { return m_normals; }
    void setNormals(const std::vector<Vec3>& normals);
    void addNormal(const Vec3& normal);

    const std::vector<Vec2>& getTexCoords() const { return m_texCoords; }
    void setTexCoords(const std::vector<Vec2>& texCoords);
    void addTexCoord(const Vec2& texCoord);

    const std::vector<Vec3>& getColors() const { return m_colors; }
    void setColors(const std::vector<Vec3>& colors);
    void addColor(const Vec3& color);

    const std::vector<unsigned int>& getIndices() const { return m_indices; }
    void setIndices(const std::vector<unsigned int>& indices);
    void addIndex(unsigned int index);
    void addTriangle(unsigned int v0, unsigned int v1, unsigned int v2);

    void clear();

    void setMaterial(std::shared_ptr<Material> material);
    std::shared_ptr<Material> getMaterial() const { return m_material; }

    void setPrimitiveType(PrimitiveType type);
    PrimitiveType getPrimitiveType() const { return m_primitiveType; }

    const BoundingBox& getBoundingBox() const;
    void updateBoundingBox();

    size_t getVertexCount() const { return m_positions.size(); }
    size_t getFaceCount() const { return m_indices.size() / 3; }

    virtual void render();
    virtual void update(float deltaTime) override;

protected:
    std::vector<Vec3> m_positions;
    std::vector<Vec3> m_normals;
    std::vector<Vec2> m_texCoords;
    std::vector<Vec3> m_colors;
    std::vector<unsigned int> m_indices;

    std::shared_ptr<Material> m_material;
    PrimitiveType m_primitiveType = PrimitiveType::TRIANGLES;

    mutable BoundingBox m_boundingBox;
    mutable bool m_boundingBoxDirty = true;

    unsigned int m_vao = 0;
    unsigned int m_vbo = 0;
    unsigned int m_ebo = 0;

    void createBuffers();
    void updateBuffers();
    void deleteBuffers();
};

} // namespace ygl

