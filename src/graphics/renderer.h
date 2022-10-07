//
// Created by nsmak on 06.10.2022.
//

#ifndef GAME_OF_LIFE_RENDERER_H
#define GAME_OF_LIFE_RENDERER_H


#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shader.h"


class Renderer
{
public:
    Renderer(Shader* shader);
    ~Renderer();

    void DrawSprite(glm::vec2 position, glm::vec2 size = glm::vec2(10.0f, 10.0f), glm::vec4 color = glm::vec4(1.0f));
private:
    // Render state
    Shader* m_shader;
    unsigned int m_quadVAO;

    void initRenderData();
};


#endif //GAME_OF_LIFE_RENDERER_H
