#include "ygl.hpp"
#include "core/camera.hpp"
#include "core/scene.hpp"
#include "render/deferred_renderer.hpp"
#include "loaders/obj_loader.hpp"
#include "loaders/loader.hpp"
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <filesystem>

using namespace ygl;

const int RENDER_WIDTH = 1920;
const int RENDER_HEIGHT = 1080;
const std::string OUTPUT_DIR = "output/";
std::vector<std::string> model_files;

void LoadModelsFromDirectory(const std::string& directory) {
    for (const auto& entry : std::filesystem::directory_iterator(directory)) {
        if (entry.is_regular_file()) {
            std::string ext = entry.path().extension().string();
            if (ext == ".obj" || ext == ".md2" || ext == ".md3" || ext == ".md5mesh" || ext == ".gltf") {
                model_files.push_back(entry.path().string());
            }
        }
    }
}

void LoadScene() {
    camera = new Camera();
    camera->SetPosition(vec3(0.0f, 0.0f, 10.0f));
    camera->SetTarget(vec3(0.0f, 0.0f, 0.0f));
    camera->SetUp(vec3(0.0f, 1.0f, 0.0f));
    camera->SetAspectRatio(static_cast<float>(RENDER_WIDTH) / RENDER_HEIGHT);
    camera->SetFOV(45.0f);

    scene = new Scene();
    Loader loader;

    for (const auto& model_file : model_files) {
        Mesh* mesh = new Mesh();
        if (loader.LoadMesh(model_file, *mesh)) {
            Material* material = new Material(vec4(
                static_cast<float>(rand()) / RAND_MAX,
                static_cast<float>(rand()) / RAND_MAX,
                static_cast<float>(rand()) / RAND_MAX,
                1.0f
            ));
            material->SetRoughness(0.5f);
            material->SetMetallic(0.0f);

            Object3D* object = new Object3D(mesh, material);
            float angle = static_cast<float>(scene->GetObjects().size()) * 2.0f * PI / 10.0f;
            float radius = 5.0f + static_cast<float>(scene->GetObjects().size()) * 2.0f;
            object->SetPosition(vec3(radius * cos(angle), 0.0f, radius * sin(angle)));
            scene->AddObject(object);
            std::cout << "Loaded: " << model_file << std::endl;
        } else {
            delete mesh;
            std::cerr << "Failed to load: " << model_file << std::endl;
        }
    }

    renderer = new DeferredRenderer(RENDER_WIDTH, RENDER_HEIGHT);
}

void SaveRenderToFile(const std::string& filename) {
    std::vector<unsigned char> pixels(RENDER_WIDTH * RENDER_HEIGHT * 3);
    glReadPixels(0, 0, RENDER_WIDTH, RENDER_HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, pixels.data());

    std::vector<unsigned char> flipped_pixels(pixels.size());
    for (int y = 0; y < RENDER_HEIGHT; ++y) {
        for (int x = 0; x < RENDER_WIDTH; ++x) {
            int src_idx = ((RENDER_HEIGHT - 1 - y) * RENDER_WIDTH + x) * 3;
            int dst_idx = (y * RENDER_WIDTH + x) * 3;
            for (int c = 0; c < 3; ++c) flipped_pixels[dst_idx + c] = pixels[src_idx + c];
        }
    }

    if (!std::filesystem::exists(OUTPUT_DIR)) std::filesystem::create_directory(OUTPUT_DIR);
    stbi_write_png((OUTPUT_DIR + filename).c_str(), RENDER_WIDTH, RENDER_HEIGHT, 3, flipped_pixels.data(), RENDER_WIDTH * 3);
    std::cout << "Saved render to: " << OUTPUT_DIR << filename << std::endl;
}

bool InitOffscreenGLFW() {
    if (!glfwInit()) { std::cerr << "Failed to initialize GLFW" << std::endl; return false; }
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(640, 480, "YGL Batch Render", nullptr, nullptr);
    if (!window) { std::cerr << "Failed to create GLFW window" << std::endl; glfwTerminate(); return false; }

    glfwMakeContextCurrent(window);
    if (glewInit() != GLEW_OK) { std::cerr << "Failed to initialize GLEW" << std::endl; return false; }
    return true;
}

void BatchRender() {
    GLuint fbo, render_texture, depth_buffer;
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    glGenTextures(1, &render_texture);
    glBindTexture(GL_TEXTURE_2D, render_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, RENDER_WIDTH, RENDER_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, render_texture, 0);

    glGenRenderbuffers(1, &depth_buffer);
    glBindRenderbuffer(GL_RENDERBUFFER, depth_buffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, RENDER_WIDTH, RENDER_HEIGHT);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth_buffer);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cerr << "Framebuffer is not complete!" << std::endl;
        return;
    }

    for (size_t i = 0; i < model_files.size(); ++i) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glViewport(0, 0, RENDER_WIDTH, RENDER_HEIGHT);

        float angle = static_cast<float>(i) * 2.0f * PI / model_files.size();
        float radius = 10.0f;
        camera->SetPosition(vec3(radius * cos(angle), radius * 0.5f, radius * sin(angle)));
        camera->SetTarget(vec3(0.0f, 0.0f, 0.0f));
        camera->Update();

        renderer->Render(*scene, *camera);
        std::string filename = std::filesystem::path(model_files[i]).stem().string() + "_render.png";
        SaveRenderToFile(filename);
    }

    glDeleteFramebuffers(1, &fbo);
    glDeleteTextures(1, &render_texture);
    glDeleteRenderbuffers(1, &depth_buffer);
}

void Cleanup() {
    delete renderer;
    delete scene;
    delete camera;
    glfwTerminate();
}

int main() {
    LoadModelsFromDirectory("assets/models/");
    if (model_files.empty()) { std::cerr << "No models found in assets/models/" << std::endl; return -1; }
    if (!InitOffscreenGLFW()) return -1;

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    LoadScene();
    BatchRender();
    Cleanup();
    return 0;
}
