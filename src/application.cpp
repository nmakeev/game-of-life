#include <iostream>

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

bool projection_changed = false;

ScreenSettings screenSettings { 800, 600 };
Camera camera {screenSettings};

bool leftPressed = false;
bool rightPressed = false;

void updateProjection(const ResourceManager& resourceManager)
{
    auto shader = resourceManager.GetShader("quad");
    shader->Use();
    shader->SetMatrix4("projection", camera.GetProjectionMatrix());
}

void Application::Run()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, false);

    GLFWwindow* window = glfwCreateWindow(screenSettings.GetWidth(), screenSettings.GetHeight(), "Game of Life", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return;
    }

    glfwSetKeyCallback(window, key_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

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
    Renderer renderer { shader };

    //-------------------

    while (!glfwWindowShouldClose(window))
    {
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

        if (leftPressed)
        {
            auto position = camera.GetPosition();
            position.x += 2.f;
            camera.SetPosition(position);
            projection_changed = true;
        }

        if (rightPressed)
        {
            auto position = camera.GetPosition();
            position.x -= 2.f;
            camera.SetPosition(position);
            projection_changed = true;
        }

        if (projection_changed)
        {
            updateProjection(resourceManager);
            projection_changed = false;
        }

        int size = 100;
        renderer.DrawRectangle(glm::vec2(-size/2, -size/2),
                               glm::vec2(size, size), glm::vec4(1, 0, 0, 1));

        glfwSwapBuffers(window);
    }

    glfwTerminate();
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    // when a user presses the escape key, we set the WindowShouldClose property to true, closing the application
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (key == GLFW_KEY_LEFT)
    {
        leftPressed = action != GLFW_RELEASE;
        return;
    }

    if (key == GLFW_KEY_RIGHT)
    {
        rightPressed = action != GLFW_RELEASE;
        return;
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

