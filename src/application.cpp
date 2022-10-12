#include <iostream>
#include <algorithm>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "application.h"
#include "graphics/resource_manager.h"
#include "graphics/renderer.h"
#include "graphics/screen_settings.h"
#include "graphics/camera.h"

#include <glm/gtx/string_cast.hpp>

// GLFW function declarations
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void scroll_callback(GLFWwindow* window, double xOffset, double yOffset);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);

bool projection_changed = false;

ScreenSettings screenSettings { 1280, 768 };
Camera camera {screenSettings};

const float maxZoom = 2.f;
const float minZoom = .05f;
const float zoomStep = .1f;

bool drag = false;

float currentSize = 4;
float sizeStep = 1;

float maxSize = 8;
float minSize = 1;

void updateProjection(const ResourceManager& resourceManager) {
    auto shader = resourceManager.GetShader("quad");
    shader->SetMatrix4("screenTransform", camera.GetProjectionMatrix(), true);

    std::cout << glm::to_string(camera.GetProjectionMatrix()) << std::endl;
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
    shader->Use().SetVector4f("quad_color", glm::vec4(0.8f, 0.8f, 0.8f, 1.f));
    Renderer renderer{shader};

    int maxX = 1000;
    int maxY = 1000;

    std::vector<float> points;
    points.reserve(maxX * maxY * 2);

    //-------------------

    bool first = true;

    unsigned int vbo;
    unsigned int vao;

    while (!glfwWindowShouldClose(window)) {
        // calculate delta time
        // --------------------
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        glfwPollEvents();

        // render
        // ------
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        float size = currentSize;
        float separator = 1;

        if (projection_changed) {
            updateProjection(resourceManager);
            projection_changed = false;
        }

        glPointSize(size);

        float fieldWidth = maxX * size + (maxX - 1) * separator;
        float fieldHeight = maxY * size + (maxY - 1) * separator;

        float startX = -fieldWidth / 2;
        float startY = -fieldHeight / 2;

        points.clear();

        for (int i = 0; i < maxX; i++)
        {
            for (int j = 0; j < maxY; j++)
            {
                float x = startX + size * i + separator * i;
                float y = startY + size * j + separator * j;

                points.emplace_back(x);
                points.emplace_back(y);
            }
        }


        if (first)
        {
            glGenBuffers(1, &vbo);
        }


        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * points.size(), points.data(), GL_DYNAMIC_DRAW);


        if (first) {
            glGenVertexArrays(1, &vao);
            glBindVertexArray(vao);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0); //TODO: nullptr?
        }

        first = false;

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        glBindVertexArray(vao);
        glDrawArrays(GL_POINTS, 0, points.size() / 2);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
    }

    glfwTerminate();
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void changeZoom(double yOffset)
{
    float oldZoom = camera.GetZoom();

    float newZoom = oldZoom + yOffset * zoomStep;
    newZoom = std::clamp(newZoom, minZoom, maxZoom);

    if (abs(oldZoom - newZoom) < 0.001)
    {
        return;
    }

    camera.SetZoom(newZoom);
    projection_changed = true;

    currentSize = currentSize + sizeStep * yOffset;
    currentSize = std::clamp(currentSize, minSize, maxSize);
}

void scroll_callback(GLFWwindow* window, double xOffset, double yOffset)
{
    changeZoom(yOffset);
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
        position.x += static_cast<float>(xDelta);
        position.y += static_cast<float>(yDelta);
        camera.SetPosition(position);

        projection_changed = true;

        xOrigin = xpos;
        yOrigin = ypos;
    }
}