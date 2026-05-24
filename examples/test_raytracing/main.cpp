#include "ygl.hpp"
#include "core/camera.hpp"
#include "core/scene.hpp"
#include "render/path_tracing_renderer.hpp"
#include "loaders/obj_loader.hpp"
#include "materials/material.hpp"
#include <GLFW/glfw3.h>
#include <iostream>
#include <chrono>

using namespace ygl;

const int WIDTH = 800;
const int HEIGHT = 600;
const int MAX_BOUNCES = 5;
const int SAMPLES_PER_PIXEL = 100;
const bool USE_DENOISING = true;

GLFWwindow* window = nullptr;
Camera* camera = nullptr;
Scene* scene = nullptr;
PathTracingRenderer* renderer = nullptr;
bool running = true;
bool render_in_progress = false;
std::chrono::steady_clock::time_point render_start_time;

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
        switch (key) {
            case GLFW_KEY_ESCAPE: running = false; break;
            case GLFW_KEY_SPACE:
                if (!render_in_progress) {
                    render_in_progress = true;
                    render_start_time = std::chrono::steady_clock::now();
                    renderer->StartRender(*scene, *camera, SAMPLES_PER_PIXEL, MAX_BOUNCES);
                }
                break;
            case GLFW_KEY_R:
                if (!render_in_progress) {
                    render_in_progress = true;
                    render_start_time = std::chrono::steady_clock::now();
                    renderer->RestartRender();
                }
                break;
            case GLFW_KEY_S:
                if (render_in_progress) {
                    renderer->StopRender();
                    render_in_progress = false;
                }
                break;
        }
    }
}

void FramebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    if (camera) camera->SetAspectRatio(static_cast<float>(width) / height);
    if (renderer) renderer->Resize(width, height);
}

void InitPathTracingScene() {
    camera = new Camera();
    camera->SetPosition(vec3(0.0f, 0.0f, 5.0f));
    camera->SetTarget(vec3(0.0f, 0.0f, 0.0f));
    camera->SetUp(vec3(0.0f, 1.0f, 0.0f));
    camera->SetAspectRatio(static_cast<float>(WIDTH) / HEIGHT);
    camera->SetFOV(60.0f);

    scene = new Scene();

    // Floor
    Mesh* floor_mesh = new Mesh();
    std::vector<Vertex> floor_vertices = {
        {{-5.0f, -1.0f, -5.0f}, {0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
        {{ 5.0f, -1.0f, -5.0f}, {1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
        {{ 5.0f, -1.0f,  5.0f}, {1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}},
        {{-5.0f, -1.0f,  5.0f}, {0.0f, 1.0f}, {0.0f, 1.0f, 0.0f}},
    };
    std::vector<unsigned int> floor_indices = {0, 1, 2, 0, 2, 3};
    floor_mesh->SetVertices(floor_vertices);
    floor_mesh->SetIndices(floor_indices);

    Material* floor_material = new Material(vec4(0.7f, 0.7f, 0.7f, 1.0f));
    floor_material->SetRoughness(0.8f);
    scene->AddObject(new Object3D(floor_mesh, floor_material));

    // Ceiling
    Mesh* ceiling_mesh = new Mesh();
    std::vector<Vertex> ceiling_vertices = {
        {{-5.0f, 5.0f, -5.0f}, {0.0f, 0.0f}, {0.0f, -1.0f, 0.0f}},
        {{ 5.0f, 5.0f, -5.0f}, {1.0f, 0.0f}, {0.0f, -1.0f, 0.0f}},
        {{ 5.0f, 5.0f,  5.0f}, {1.0f, 1.0f}, {0.0f, -1.0f, 0.0f}},
        {{-5.0f, 5.0f,  5.0f}, {0.0f, 1.0f}, {0.0f, -1.0f, 0.0f}},
    };
    ceiling_mesh->SetVertices(ceiling_vertices);
    ceiling_mesh->SetIndices(floor_indices);

    Material* ceiling_material = new Material(vec4(0.7f, 0.7f, 0.7f, 1.0f));
    ceiling_material->SetRoughness(0.8f);
    scene->AddObject(new Object3D(ceiling_mesh, ceiling_material));

    // Back wall (red)
    Mesh* back_wall_mesh = new Mesh();
    std::vector<Vertex> back_wall_vertices = {
        {{-5.0f, -1.0f, -5.0f}, {0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}},
        {{ 5.0f, -1.0f, -5.0f}, {1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}},
        {{ 5.0f,  5.0f, -5.0f}, {1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}},
        {{-5.0f,  5.0f, -5.0f}, {0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}},
    };
    back_wall_mesh->SetVertices(back_wall_vertices);
    back_wall_mesh->SetIndices(floor_indices);

    Material* red_material = new Material(vec4(0.8f, 0.1f, 0.1f, 1.0f));
    red_material->SetRoughness(0.2f);
    scene->AddObject(new Object3D(back_wall_mesh, red_material));

    // Front wall (green)
    Mesh* front_wall_mesh = new Mesh();
    std::vector<Vertex> front_wall_vertices = {
        {{-5.0f, -1.0f, 5.0f}, {0.0f, 0.0f}, {0.0f, 0.0f, -1.0f}},
        {{ 5.0f, -1.0f, 5.0f}, {1.0f, 0.0f}, {0.0f, 0.0f, -1.0f}},
        {{ 5.0f,  5.0f, 5.0f}, {1.0f, 1.0f}, {0.0f, 0.0f, -1.0f}},
        {{-5.0f,  5.0f, 5.0f}, {0.0f, 1.0f}, {0.0f, 0.0f, -1.0f}},
    };
    front_wall_mesh->SetVertices(front_wall_vertices);
    front_wall_mesh->SetIndices(floor_indices);

    Material* green_material = new Material(vec4(0.1f, 0.8f, 0.1f, 1.0f));
    green_material->SetRoughness(0.2f);
    scene->AddObject(new Object3D(front_wall_mesh, green_material));

    // Left wall (blue)
    Mesh* left_wall_mesh = new Mesh();
    std::vector<Vertex> left_wall_vertices = {
        {{-5.0f, -1.0f, -5.0f}, {0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}},
        {{-5.0f, -1.0f,  5.0f}, {1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}},
        {{-5.0f,  5.0f,  5.0f}, {1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}},
        {{-5.0f,  5.0f, -5.0f}, {0.0f, 1.0f}, {1.0f, 0.0f, 0.0f}},
    };
    left_wall_mesh->SetVertices(left_wall_vertices);
    left_wall_mesh->SetIndices(floor_indices);

    Material* blue_material = new Material(vec4(0.1f, 0.1f, 0.8f, 1.0f));
    blue_material->SetRoughness(0.2f);
    scene->AddObject(new Object3D(left_wall_mesh, blue_material));

    // Right wall (yellow)
    Mesh* right_wall_mesh = new Mesh();
    std::vector<Vertex> right_wall_vertices = {
        {{5.0f, -1.0f, -5.0f}, {0.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}},
        {{5.0f, -1.0f,  5.0f}, {1.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}},
        {{5.0f,  5.0f,  5.0f}, {1.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}},
        {{5.0f,  5.0f, -5.0f}, {0.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}},
    };
    right_wall_mesh->SetVertices(right_wall_vertices);
    right_wall_mesh->SetIndices(floor_indices);

    Material* yellow_material = new Material(vec4(0.8f, 0.8f, 0.1f, 1.0f));
    yellow_material->SetRoughness(0.2f);
    scene->AddObject(new Object3D(right_wall_mesh, yellow_material));

    // Central sphere
    const float golden_ratio = (1.0f + std::sqrt(5.0f)) / 2.0f;
    std::vector<vec3> sphere_vertices_vec3 = {
        {-1.0f, golden_ratio, 0.0f}, {1.0f, golden_ratio, 0.0f},
        {-1.0f, -golden_ratio, 0.0f}, {1.0f, -golden_ratio, 0.0f},
        {0.0f, -1.0f, golden_ratio}, {0.0f, 1.0f, golden_ratio},
        {0.0f, -1.0f, -golden_ratio}, {0.0f, 1.0f, -golden_ratio},
        {golden_ratio, 0.0f, -1.0f}, {golden_ratio, 0.0f, 1.0f},
        {-golden_ratio, 0.0f, -1.0f}, {-golden_ratio, 0.0f, 1.0f}
    };

    std::vector<Vertex> sphere_vertices;
    for (const auto& v : sphere_vertices_vec3) {
        vec3 n = normalize(v);
        sphere_vertices.push_back({n, vec2(0.0f), n});
    }

    std::vector<unsigned int> sphere_indices = {
        0, 11, 5, 0, 5, 1, 0, 1, 7, 0, 7, 10, 0, 10, 11,
        1, 5, 9, 5, 11, 4, 11, 10, 2, 10, 7, 6, 7, 1, 8,
        3, 9, 4, 3, 4, 2, 3, 2, 6, 3, 6, 8, 3, 8, 9,
        4, 9, 5, 2, 4, 11, 6, 2, 10, 8, 6, 7, 9, 8, 1
    };

    Mesh* sphere_mesh = new Mesh();
    sphere_mesh->SetVertices(sphere_vertices);
    sphere_mesh->SetIndices(sphere_indices);
    for (auto& v : sphere_mesh->GetVertices()) v.position *= 1.5f;

    Material* sphere_material = new Material(vec4(0.9f, 0.9f, 0.9f, 1.0f));
    sphere_material->SetRoughness(0.1f);
    sphere_material->SetMetallic(0.8f);
    Object3D* sphere_obj = new Object3D(sphere_mesh, sphere_material);
    sphere_obj->SetPosition(vec3(0.0f, 0.0f, 0.0f));
    scene->AddObject(sphere_obj);

    // Light source
    Mesh* light_mesh = new Mesh();
    std::vector<Vertex> light_vertices;
    for (const auto& v : sphere_vertices_vec3) {
        vec3 n = normalize(v);
        light_vertices.push_back({n * 0.2f, vec2(0.0f), n});
    }
    light_mesh->SetVertices(light_vertices);
    light_mesh->SetIndices(sphere_indices);

    Material* light_material = new Material(vec4(1.0f, 1.0f, 1.0f, 1.0f));
    light_material->SetEmission(vec3(10.0f, 10.0f, 10.0f), 1.0f);
    light_material->SetRoughness(1.0f);
    Object3D* light_obj = new Object3D(light_mesh, light_material);
    light_obj->SetPosition(vec3(2.0f, 3.0f, 2.0f));
    scene->AddObject(light_obj);

    renderer = new PathTracingRenderer(WIDTH, HEIGHT);
    renderer->SetMaxBounces(MAX_BOUNCES);
    renderer->SetUseDenoising(USE_DENOISING);
}

bool InitGLFW() {
    if (!glfwInit()) { std::cerr << "Failed to initialize GLFW" << std::endl; return false; }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    window = glfwCreateWindow(WIDTH, HEIGHT, "YGL Path Tracing Test", nullptr, nullptr);
    if (!window) { std::cerr << "Failed to create GLFW window" << std::endl; glfwTerminate(); return false; }
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, KeyCallback);
    glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);
    glfwSwapInterval(1);
    return true;
}

void Cleanup() {
    delete renderer;
    delete scene;
    delete camera;
    glfwTerminate();
}

void MainLoop() {
    double last_time = glfwGetTime();
    while (running && !glfwWindowShouldClose(window)) {
        double current_time = glfwGetTime();
        double delta_time = current_time - last_time;
        last_time = current_time;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        camera->Update();

        if (render_in_progress) {
            bool finished = renderer->UpdateRender(delta_time);
            if (finished) {
                auto end_time = std::chrono::steady_clock::now();
                auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - render_start_time);
                std::cout << "Path tracing render completed in " << duration.count() << " ms" << std::endl;
                render_in_progress = false;
            }
            renderer->DisplayRender();
        } else {
            renderer->DisplayRender();
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

int main() {
    if (!InitGLFW()) return -1;
    if (glewInit() != GLEW_OK) { std::cerr << "Failed to initialize GLEW" << std::endl; return -1; }
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    InitPathTracingScene();
    MainLoop();
    Cleanup();
    return 0;
}
