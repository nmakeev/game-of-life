/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/

#ifndef GAME_OF_LIFE_SHADER_H
#define GAME_OF_LIFE_SHADER_H

#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader {

public:
    Shader();
    ~Shader();

    Shader(Shader&& shader);

    // sets the current shader as active
    Shader &Use();

    // compiles the shader from given source code
    void Compile(const char *vertexSource, const char *fragmentSource,
                 const char *geometrySource = nullptr); // note: geometry source code is optional

    // utility functions
    void SetFloat(const char *name, float value, bool useShader = false);

    void SetInteger(const char *name, int value, bool useShader = false);

    void SetVector2f(const char *name, float x, float y, bool useShader = false);

    void SetVector2f(const char *name, const glm::vec2 &value, bool useShader = false);

    void SetVector3f(const char *name, float x, float y, float z, bool useShader = false);

    void SetVector3f(const char *name, const glm::vec3 &value, bool useShader = false);

    void SetVector4f(const char *name, float x, float y, float z, float w, bool useShader = false);

    void SetVector4f(const char *name, const glm::vec4 &value, bool useShader = false);

    void SetMatrix4(const char *name, const glm::mat4 &matrix, bool useShader = false);

private:
    unsigned int m_id;
};

#endif //GAME_OF_LIFE_SHADER_H