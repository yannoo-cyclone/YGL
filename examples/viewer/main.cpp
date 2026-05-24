#include "ygl.hpp"
#include "core/camera.hpp"
#include "core/scene.hpp"
#include "render/hybrid_renderer.hpp"
#include "loaders/obj_loader.hpp"
#include <GLFW/glfw3.h>
#include <iostream>

using namespace ygl;

// --- Global variables ---
GLFWwindow* window = nullptr;
Camera* camera = nullptr;
Scene* scene = nullptr;
HybridRenderer* renderer = nullptr;
int window_width = 1280;
int window_height = 720;
bool wireframe_mode = false;
bool running = true;

// --- Input handling ---
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
        switch (key) {
            case GLFW_KEY_ESCAPE:
                running = false;
                break;
            case GLFW_KEY_W:
                camera->MoveForward(0.1f);
                break;
            case GLFW_KEY_S:
                camera->MoveForward(-0.1f);
                break;
            case GLFW_KEY_A:
                camera->MoveRight(-0.1f);
                break;
            case GLFW_KEY_D:
                camera->MoveRight(0.1f);
                break;
            case GLFW_KEY_Q:
                camera->MoveUp(-0.1f);
                break;
            case GLFW_KEY_E:
                camera->MoveUp(0.1f);
                break;
            case GLFW_KEY_F:
                wireframe_mode = !wireframe_mode;
                glPolygonMode(GL_FRONT_AND_BACK, wireframe_mode ? GL_LINE : GL_FILL);
                std::cout << "Wireframe mode: " << (wireframe_mode ? "ON" : "OFF") << std::endl;
                break;
        }
    }
}

void MouseCallback(GLFWwindow* window, double xpos, double ypos) {
    static double last_x = window_width / 2.0;
    static double last_y = window_height / 2.0;
    static bool first_mouse = true;

    if (first_mouse) {
        last_x = xpos;
        last_y = ypos;
        first_mouse = false;
    }

    float xoffset = static_cast<float>(xpos - last_x);
    float yoffset = static_cast<float>(last_y - ypos);
    last_x = xpos;
    last_y = ypos;

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        camera->Rotate(xoffset * 0.005f, yoffset * 0.005f);
    }
}

void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
}

void FramebufferSizeCallback(GLFWwindow* window, int width, int height) {
    window_width = width;
    window_height = height;
    glViewport(0, 0, width, height);
    if (camera) camera->SetAspectRatio(static_cast<float>(width) / height);
    if (renderer) renderer->Resize(width, height);
}

// --- Initialization ---
bool InitGLFW() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    window = glfwCreateWindow(window_width, window_height, "YGL Viewer", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, KeyCallback);
    glfwSetCursorPosCallback(window, MouseCallback);
    glfwSetMouseButtonCallback(window, MouseButtonCallback);
    glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);
    glfwSwapInterval(1);
    return true;
}

bool InitGLEW() {
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return false;
    }
    return true;
}

void InitOpenGL() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
}

void LoadTestScene() {
    camera = new Camera();
    camera->SetPosition(vec3(0.0f, 0.0f, 5.0f));
    camera->SetTarget(vec3(0.0f, 0.0f, 0.0f));
    camera->SetUp(vec3(0.0f, 1.0f, 0.0f));
    camera->SetAspectRatio(static_cast<float>(window_width) / window_height);
    camera->SetFOV(45.0f);

    scene = new Scene();
    OBJLoader loader;
    Mesh* mesh = new Mesh();

    if (loader.LoadOBJ("assets/models/cube.obj", *mesh)) {
        std::cout << "Loaded cube.obj" << std::endl;
    } else {
        std::vector<Vertex> vertices = {
            {{-0.5f, -0.5f,  0.5f}, {0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}},
            {{ 0.5f, -0.5f,  0.5f}, {1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}},
            {{ 0.5f,  0.5f,  0.5f}, {1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}},
            {{-0.5f,  0.5f,  0.5f}, {0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}},
            {{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f}, {0.0f, 0.0f, -1.0f}},
            {{ 0.5f, -0.5f, -0.5f}, {1.0f, 0.0f}, {0.0f, 0.0f, -1.0f}},
            {{ 0.5f,  0.5f, -0.5f}, {1.0f, 1.0f}, {0.0f, 0.0f, -1.0f}},
            {{-0.5f,  0.5f, -0.5f}, {0.0f, 1.0f}, {0.0f, 0.0f, -1.0f}},
        };

        std::vector<unsigned int> indices = {
            0, 1, 2, 0, 2, 3,
            4, 6, 5, 4, 7, 6,
            4, 0, 3, 4, 3, 7,
            1, 5, 6, 1, 6, 2,
            3, 2, 6, 3, 6, 7,
            4, 5, 1, 4, 1, 0
        };

        mesh->SetVertices(vertices);
        mesh->SetIndices(indices);
    }

    Material* material = new Material(vec4(0.8f, 0.2f, 0.2f, 1.0f));
    material->SetRoughness(0.5f);
    material->SetMetallic(0.0f);

    scene->AddObject(new Object3D(mesh, material));
    renderer = new HybridRenderer(window_width, window_height);
}

void Cleanup() {
    delete renderer;
    delete scene;
    delete camera;
    glfwTerminate();
}

void MainLoop() {
    while (running && !glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        camera->Update();
        if (renderer && scene) renderer->Render(*scene, *camera);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

int main() {
    if (!InitGLFW()) return -1;
    if (!InitGLEW()) return -1;
    InitOpenGL();
    LoadTestScene();
    MainLoop();
    Cleanup();
    return 0;
}
