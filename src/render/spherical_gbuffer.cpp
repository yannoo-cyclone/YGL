#include "render/spherical_gbuffer.hpp"
#include "core/camera.hpp"
#include <GL/glew.h>
#include <iostream>

namespace ygl {

Vec3 SphericalGBuffer::sphericalToCartesian(float theta, float phi) {
    return Vec3(std::sin(phi) * std::cos(theta),
                std::sin(phi) * std::sin(theta),
                std::cos(phi));
}

void SphericalGBuffer::cartesianToSpherical(const Vec3& dir, float& theta, float& phi) {
    Vec3 normalized = normalize(dir);
    theta = std::atan2(normalized.y, normalized.x);
    phi = std::acos(normalized.z);
}

SphericalGBuffer::SphericalGBuffer(int resolution) : m_resolution(resolution) {}

SphericalGBuffer::~SphericalGBuffer() {
    cleanup();
}

bool SphericalGBuffer::initialize() {
    if (!createCubeMap(m_emitter)) return false;
    if (!createCubeMap(m_receiver)) return false;
    if (!createDepthTexture(m_emitter)) return false;
    if (!createDepthTexture(m_receiver)) return false;
    if (!createFBO(m_emitter)) return false;
    if (!createFBO(m_receiver)) return false;

    std::cout << "SphericalGBuffer initialized (resolution: " << m_resolution << ")" << std::endl;
    return true;
}

void SphericalGBuffer::cleanup() {
    deleteBuffer(m_emitter);
    deleteBuffer(m_receiver);
}

void SphericalGBuffer::beginPass(BufferType type) {
    m_currentType = type;
    glBindFramebuffer(GL_FRAMEBUFFER, getFBO(type));
    glViewport(0, 0, m_resolution, m_resolution);
}

void SphericalGBuffer::endPass() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void SphericalGBuffer::swapBuffers() {
    std::swap(m_emitter, m_receiver);
    m_currentType = (m_currentType == BufferType::EMITTER) ?
                    BufferType::RECEIVER : BufferType::EMITTER;
}

void SphericalGBuffer::bindForWriting(BufferType type, int faceIndex) {
    glBindFramebuffer(GL_FRAMEBUFFER, getFBO(type));
    if (faceIndex >= 0 && faceIndex < 6) {
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                              GL_TEXTURE_CUBE_MAP_POSITIVE_X + faceIndex,
                              getCubeMap(type), 0);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                              GL_TEXTURE_CUBE_MAP_POSITIVE_X + faceIndex,
                              getDepthTexture(type), 0);
    }
    glViewport(0, 0, m_resolution, m_resolution);
}

void SphericalGBuffer::bindForReading(BufferType type) {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, getCubeMap(type));
}

void SphericalGBuffer::bindDepthForReading(BufferType type) {
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_CUBE_MAP, getDepthTexture(type));
}

void SphericalGBuffer::unbind() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    glActiveTexture(GL_TEXTURE0);
}

unsigned int SphericalGBuffer::getTexture(BufferType type, TextureType texType) const {
    const auto& buffer = (type == BufferType::EMITTER) ? m_emitter : m_receiver;
    return buffer.textures[static_cast<int>(texType)];
}

unsigned int SphericalGBuffer::getCubeMap(BufferType type) const {
    const auto& buffer = (type == BufferType::EMITTER) ? m_emitter : m_receiver;
    return buffer.cubeMap;
}

unsigned int SphericalGBuffer::getDepthTexture(BufferType type) const {
    const auto& buffer = (type == BufferType::EMITTER) ? m_emitter : m_receiver;
    return buffer.depthTexture;
}

unsigned int SphericalGBuffer::getFBO(BufferType type) const {
    const auto& buffer = (type == BufferType::EMITTER) ? m_emitter : m_receiver;
    return buffer.fbo;
}

void SphericalGBuffer::propagate(const BVH& bvh, int samples) {
    GLint currentFBO; glGetIntegerv(GL_FRAMEBUFFER_BINDING, &currentFBO);
    GLint currentProgram; glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);

    bindForReading(BufferType::EMITTER);
    bindDepthForReading(BufferType::EMITTER);

    for (int face = 0; face < 6; face++) {
        bindForWriting(BufferType::RECEIVER, face);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        renderFullscreenQuad();
    }

    swapBuffers();
    glUseProgram(currentProgram);
    glBindFramebuffer(GL_FRAMEBUFFER, currentFBO);
    unbind();
}

void SphericalGBuffer::setResolution(int resolution) {
    if (m_resolution != resolution) {
        m_resolution = resolution;
        cleanup();
        initialize();
    }
}

bool SphericalGBuffer::createCubeMap(BufferTextures& buffer) {
    glGenTextures(1, &buffer.cubeMap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, buffer.cubeMap);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    for (unsigned int i = 0; i < 6; ++i) {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA32F,
                    m_resolution, m_resolution, 0, GL_RGBA, GL_FLOAT, nullptr);
    }

    for (int i = 0; i < static_cast<int>(TextureType::COUNT); ++i) {
        buffer.textures[i] = buffer.cubeMap;
    }

    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    return true;
}

bool SphericalGBuffer::createDepthTexture(BufferTextures& buffer) {
    glGenTextures(1, &buffer.depthTexture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, buffer.depthTexture);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    for (unsigned int i = 0; i < 6; ++i) {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT32F,
                    m_resolution, m_resolution, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    }

    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    return true;
}

bool SphericalGBuffer::createFBO(BufferTextures& buffer) {
    glGenFramebuffers(1, &buffer.fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, buffer.fbo);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, buffer.cubeMap, 0);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, buffer.depthTexture, 0);

    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE) {
        std::cerr << "SphericalGBuffer FBO incomplete: " << status << std::endl;
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        return false;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return true;
}

void SphericalGBuffer::deleteBuffer(BufferTextures& buffer) {
    if (buffer.cubeMap) { glDeleteTextures(1, &buffer.cubeMap); buffer.cubeMap = 0; }
    if (buffer.depthTexture) { glDeleteTextures(1, &buffer.depthTexture); buffer.depthTexture = 0; }
    if (buffer.fbo) { glDeleteFramebuffers(1, &buffer.fbo); buffer.fbo = 0; }
    for (auto& tex : buffer.textures) tex = 0;
}

unsigned int SphericalGBuffer::loadShader(const std::string& vertPath, const std::string& fragPath) {
    FILE* vertFile = fopen(vertPath.c_str(), "r");
    FILE* fragFile = fopen(fragPath.c_str(), "r");
    if (!vertFile || !fragFile) {
        if (vertFile) fclose(vertFile);
        if (fragFile) fclose(fragFile);
        return 0;
    }

    fseek(vertFile, 0, SEEK_END); long vertSize = ftell(vertFile);
    fseek(vertFile, 0, SEEK_SET); char* vertSource = new char[vertSize + 1];
    fread(vertSource, 1, vertSize, vertFile); vertSource[vertSize] = '\0';

    fseek(fragFile, 0, SEEK_END); long fragSize = ftell(fragFile);
    fseek(fragFile, 0, SEEK_SET); char* fragSource = new char[fragSize + 1];
    fread(fragSource, 1, fragSize, fragFile); fragSource[fragSize] = '\0';

    fclose(vertFile); fclose(fragFile);

    unsigned int vertShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertShader, 1, &vertSource, nullptr); glCompileShader(vertShader);
    unsigned int fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragShader, 1, &fragSource, nullptr); glCompileShader(fragShader);

    delete[] vertSource; delete[] fragSource;

    int success; char infoLog[512];
    glGetShaderiv(vertShader, GL_COMPILE_STATUS, &success);
    if (!success) { glGetShaderInfoLog(vertShader, 512, nullptr, infoLog); std::cerr << infoLog << std::endl; return 0; }
    glGetShaderiv(fragShader, GL_COMPILE_STATUS, &success);
    if (!success) { glGetShaderInfoLog(fragShader, 512, nullptr, infoLog); std::cerr << infoLog << std::endl; return 0; }

    unsigned int program = glCreateProgram();
    glAttachShader(program, vertShader); glAttachShader(program, fragShader); glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) { glGetProgramInfoLog(program, 512, nullptr, infoLog); std::cerr << infoLog << std::endl; return 0; }

    glDeleteShader(vertShader); glDeleteShader(fragShader);
    return program;
}

void SphericalGBuffer::renderFullscreenQuad() {
    static unsigned int vao = 0, vbo = 0, ebo = 0;
    if (vao == 0) {
        float vertices[] = {-1.0f, -1.0f, 0.0f, 1.0f, -1.0f, 0.0f, -1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f};
        unsigned int indices[] = {0, 1, 2, 2, 1, 3};
        glGenVertexArrays(1, &vao); glGenBuffers(1, &vbo); glGenBuffers(1, &ebo);
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo); glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo); glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); glEnableVertexAttribArray(0);
        glBindVertexArray(0);
    }
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

} // namespace ygl
