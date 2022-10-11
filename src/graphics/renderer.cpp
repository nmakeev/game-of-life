#include <glad/glad.h>

#include "renderer.h"

Renderer::Renderer(Shader* shader) : m_shader(shader), m_quadVAO(0)
{
    this->initRenderData();
}

Renderer::~Renderer()
{
    glDeleteVertexArrays(1, &m_quadVAO);
}

void Renderer::DrawRectangle(glm::vec2 position, glm::vec2 size, glm::vec4 color)
{
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(position, 0.0f));
    model = glm::scale(model, glm::vec3(size, 1.0f));

    m_shader->Use();
    m_shader->SetMatrix4("model", model);
    m_shader->SetVector4f("quad_color", color);

    glBindVertexArray(m_quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

void Renderer::initRenderData()
{
    // configure VAO/VBO
    unsigned int VBO;
    float vertices[] = {
            // pos
            0.0f, 1.0f,
            1.0f, 0.0f,
            0.0f, 0.0f,

            0.0f, 1.0f,
            1.0f, 1.0f,
            1.0f, 0.0f
    };

    glGenVertexArrays(1, &m_quadVAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(m_quadVAO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0); //TODO: nullptr?
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}