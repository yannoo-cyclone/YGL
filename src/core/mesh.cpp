#include "core/mesh.hpp"
#include "core/bounding_box.hpp"
#include "materials/material.hpp"
#include <GL/glew.h>

namespace ygl {

Mesh::Mesh(const std::string& name) : Object3D(name) {
    createBuffers();
}

Mesh::~Mesh() {
    deleteBuffers();
}

// Vertex data
void Mesh::setPositions(const std::vector<Vec3>& positions) {
    m_positions = positions; m_boundingBoxDirty = true; updateBuffers();
}
void Mesh::addPosition(const Vec3& position) {
    m_positions.push_back(position); m_boundingBoxDirty = true;
}

void Mesh::setNormals(const std::vector<Vec3>& normals) {
    m_normals = normals; updateBuffers();
}
void Mesh::addNormal(const Vec3& normal) { m_normals.push_back(normal); }

void Mesh::setTexCoords(const std::vector<Vec2>& texCoords) {
    m_texCoords = texCoords; updateBuffers();
}
void Mesh::addTexCoord(const Vec2& texCoord) { m_texCoords.push_back(texCoord); }

void Mesh::setColors(const std::vector<Vec3>& colors) {
    m_colors = colors; updateBuffers();
}
void Mesh::addColor(const Vec3& color) { m_colors.push_back(color); }

void Mesh::setIndices(const std::vector<unsigned int>& indices) {
    m_indices = indices; updateBuffers();
}
void Mesh::addIndex(unsigned int index) { m_indices.push_back(index); }
void Mesh::addTriangle(unsigned int v0, unsigned int v1, unsigned int v2) {
    m_indices.push_back(v0); m_indices.push_back(v1); m_indices.push_back(v2);
}

void Mesh::clear() {
    m_positions.clear(); m_normals.clear(); m_texCoords.clear();
    m_colors.clear(); m_indices.clear(); m_boundingBoxDirty = true;
    updateBuffers();
}

// Material
void Mesh::setMaterial(std::shared_ptr<Material> material) { m_material = material; }
std::shared_ptr<Material> Mesh::getMaterial() const { return m_material; }

void Mesh::setPrimitiveType(PrimitiveType type) { m_primitiveType = type; }
Mesh::PrimitiveType Mesh::getPrimitiveType() const { return m_primitiveType; }

// Bounding box
const BoundingBox& Mesh::getBoundingBox() const {
    if (m_boundingBoxDirty) updateBoundingBox();
    return m_boundingBox;
}

void Mesh::updateBoundingBox() const {
    m_boundingBox.reset();
    for (const auto& pos : m_positions) m_boundingBox.expand(pos);
    m_boundingBoxDirty = false;
}

// Buffers
void Mesh::createBuffers() {
    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);
    glGenBuffers(1, &m_ebo);
}

void Mesh::updateBuffers() {
    if (!m_vao || !m_vbo) return;

    glBindVertexArray(m_vao);

    // Calculate stride and create interleaved data
    size_t stride = 3 * sizeof(float);
    bool hasNormals = !m_normals.empty() && m_normals.size() == m_positions.size();
    bool hasTexCoords = !m_texCoords.empty() && m_texCoords.size() == m_positions.size();
    bool hasColors = !m_colors.empty() && m_colors.size() == m_positions.size();

    if (hasNormals) stride += 3 * sizeof(float);
    if (hasTexCoords) stride += 2 * sizeof(float);
    if (hasColors) stride += 3 * sizeof(float);

    std::vector<float> vertexData;
    vertexData.reserve(m_positions.size() * (3 + (hasNormals?3:0) + (hasTexCoords?2:0) + (hasColors?3:0)));

    for (size_t i = 0; i < m_positions.size(); ++i) {
        vertexData.push_back(m_positions[i].x);
        vertexData.push_back(m_positions[i].y);
        vertexData.push_back(m_positions[i].z);

        if (hasNormals && i < m_normals.size()) {
            vertexData.push_back(m_normals[i].x);
            vertexData.push_back(m_normals[i].y);
            vertexData.push_back(m_normals[i].z);
        }
        if (hasTexCoords && i < m_texCoords.size()) {
            vertexData.push_back(m_texCoords[i].x);
            vertexData.push_back(m_texCoords[i].y);
        }
        if (hasColors && i < m_colors.size()) {
            vertexData.push_back(m_colors[i].x);
            vertexData.push_back(m_colors[i].y);
            vertexData.push_back(m_colors[i].z);
        }
    }

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(float), vertexData.data(), GL_STATIC_DRAW);

    if (!m_indices.empty()) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), m_indices.data(), GL_STATIC_DRAW);
    }

    // Set attributes
    size_t offset = 0;
    int attributeIndex = 0;

    glEnableVertexAttribArray(attributeIndex);
    glVertexAttribPointer(attributeIndex, 3, GL_FLOAT, GL_FALSE, stride, (void*)offset);
    offset += 3 * sizeof(float); attributeIndex++;

    if (hasNormals) {
        glEnableVertexAttribArray(attributeIndex);
        glVertexAttribPointer(attributeIndex, 3, GL_FLOAT, GL_FALSE, stride, (void*)offset);
        offset += 3 * sizeof(float); attributeIndex++;
    }
    if (hasTexCoords) {
        glEnableVertexAttribArray(attributeIndex);
        glVertexAttribPointer(attributeIndex, 2, GL_FLOAT, GL_FALSE, stride, (void*)offset);
        offset += 2 * sizeof(float); attributeIndex++;
    }
    if (hasColors) {
        glEnableVertexAttribArray(attributeIndex);
        glVertexAttribPointer(attributeIndex, 3, GL_FLOAT, GL_FALSE, stride, (void*)offset);
    }

    glBindVertexArray(0);
}

void Mesh::deleteBuffers() {
    if (m_vao) glDeleteVertexArrays(1, &m_vao);
    if (m_vbo) glDeleteBuffers(1, &m_vbo);
    if (m_ebo) glDeleteBuffers(1, &m_ebo);
    m_vao = m_vbo = m_ebo = 0;
}

// Render
void Mesh::render() {
    if (!m_vao || m_positions.empty()) return;

    if (m_material) m_material->bind();
    glBindVertexArray(m_vao);

    switch (m_primitiveType) {
        case PrimitiveType::POINTS: glDrawArrays(GL_POINTS, 0, static_cast<GLsizei>(m_positions.size())); break;
        case PrimitiveType::LINES: glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(m_positions.size())); break;
        case PrimitiveType::LINE_LOOP: glDrawArrays(GL_LINE_LOOP, 0, static_cast<GLsizei>(m_positions.size())); break;
        case PrimitiveType::LINE_STRIP: glDrawArrays(GL_LINE_STRIP, 0, static_cast<GLsizei>(m_positions.size())); break;
        case PrimitiveType::TRIANGLES:
            if (!m_indices.empty()) glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_indices.size()), GL_UNSIGNED_INT, 0);
            else glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(m_positions.size()));
            break;
        case PrimitiveType::TRIANGLE_STRIP: glDrawArrays(GL_TRIANGLE_STRIP, 0, static_cast<GLsizei>(m_positions.size())); break;
        case PrimitiveType::TRIANGLE_FAN: glDrawArrays(GL_TRIANGLE_FAN, 0, static_cast<GLsizei>(m_positions.size())); break;
    }

    glBindVertexArray(0);
    if (m_material) m_material->unbind();
}

// Update
void Mesh::update(float deltaTime) {
    Object3D::update(deltaTime);
}

} // namespace ygl
