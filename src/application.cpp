#include <iostream>
#include <algorithm>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "application.h"
#include "graphics/resource_manager.h"
#include "graphics/renderer.h"
#include "graphics/screen_settings.h"
#include "graphics/camera.h"

// GLFW function declarations
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void scroll_callback(GLFWwindow* window, double xOffset, double yOffset);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);

bool projection_changed = false;

ScreenSettings screenSettings { 800, 600 };
Camera camera {screenSettings};

const float maxZoom = 1.f;
const float minZoom = .05f;
const float zoomStep = .1f;

bool drag = false;

void updateProjection(const ResourceManager& resourceManager)
{
    auto shader = resourceManager.GetShader("quad");
    shader->Use();
    shader->SetMatrix4("projection", camera.GetProjectionMatrix());
}

void Application::Run() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, false);

    GLFWwindow *window = glfwCreateWindow(screenSettings.GetWidth(), screenSettings.GetHeight(), "Game of Life",
                                          nullptr, nullptr);
    glfwMakeContextCurrent(window);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return;
    }

    glfwSetKeyCallback(window, key_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);

    // OpenGL configuration
    // --------------------
    glViewport(0, 0, screenSettings.GetWidth(), screenSettings.GetHeight());
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // deltaTime variables
    // -------------------
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    ResourceManager resourceManager;

    //-------------------

    resourceManager.LoadShader("res/quad.vert", "res/quad.frag", nullptr, "quad");
    // configure shaders

    updateProjection(resourceManager);

    auto shader = resourceManager.GetShader("quad");
    Renderer renderer{shader};

    //-------------------

    while (!glfwWindowShouldClose(window)) {
        // calculate delta time
        // --------------------
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        glfwPollEvents();

        // render
        // ------
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        if (projection_changed) {
            updateProjection(resourceManager);
            projection_changed = false;
        }

        int size = 100;
        renderer.DrawRectangle(glm::vec2(-size / 2, -size / 2),
                               glm::vec2(size, size), glm::vec4(1, 0, 0, 1));

        glfwSwapBuffers(window);
    }

    glfwTerminate();
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

void scroll_callback(GLFWwindow* window, double xOffset, double yOffset)
{
    float newZoom = camera.GetZoom() + yOffset * zoomStep;
    newZoom = std::clamp(newZoom, minZoom, maxZoom);
    camera.SetZoom(newZoom);
    projection_changed = true;
}

double xOrigin = 0;
double yOrigin = 0;

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        drag = action == GLFW_PRESS;
        glfwGetCursorPos(window, &xOrigin, &yOrigin);
    }
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
    if (drag) {
        double xDelta = -(xpos - xOrigin);
        double yDelta = -(ypos - yOrigin);

        auto position = camera.GetPosition();
        position.x += xDelta;
        position.y += yDelta;
        camera.SetPosition(position);

        projection_changed = true;

        xOrigin = xpos;
        yOrigin = ypos;
    }
}