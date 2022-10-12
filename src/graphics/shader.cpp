/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/

#include <iostream>
#include <string>
#include <glad/glad.h>

#include "shader.h"

//TODO: NULL -> nullptr?

void checkCompileErrors(unsigned int object, std::string&& type);

Shader::Shader() : m_id(glCreateProgram()) {
}

Shader::~Shader() {
    glDeleteProgram(m_id);
}

Shader& Shader::Use()
{
    glUseProgram(m_id);
    return *this;
}

void Shader::Compile(const char* vertexSource, const char* fragmentSource, const char* geometrySource)
{
    unsigned int sVertex, sFragment, gShader;
    // vertex Shader
    sVertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(sVertex, 1, &vertexSource, NULL);
    glCompileShader(sVertex);
    checkCompileErrors(sVertex, "VERTEX");
    // fragment Shader
    sFragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(sFragment, 1, &fragmentSource, NULL);
    glCompileShader(sFragment);
    checkCompileErrors(sFragment, "FRAGMENT");
    // if geometry shader source code is given, also compile geometry shader
    if (geometrySource != nullptr)
    {
        gShader = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(gShader, 1, &geometrySource, NULL);
        glCompileShader(gShader);
        checkCompileErrors(gShader, "GEOMETRY");
    }

    // shader program
    glAttachShader(m_id, sVertex);
    glAttachShader(m_id, sFragment);
    if (geometrySource != nullptr)
        glAttachShader(m_id, gShader);
    glLinkProgram(m_id);
    checkCompileErrors(m_id, "PROGRAM");

    // delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(sVertex);
    glDeleteShader(sFragment);
    if (geometrySource != nullptr)
        glDeleteShader(gShader);
}

//TODO: get rid of useShader
void Shader::SetFloat(const char *name, float value, bool useShader)
{
    if (useShader)
        this->Use();
    glUniform1f(glGetUniformLocation(m_id, name), value);
}

void Shader::SetInteger(const char *name, int value, bool useShader)
{
    if (useShader)
        this->Use();
    glUniform1i(glGetUniformLocation(m_id, name), value);
}

void Shader::SetVector2f(const char *name, float x, float y, bool useShader)
{
    if (useShader)
        this->Use();
    glUniform2f(glGetUniformLocation(m_id, name), x, y);
}

void Shader::SetVector2f(const char *name, const glm::vec2 &value, bool useShader)
{
    if (useShader)
        this->Use();
    glUniform2f(glGetUniformLocation(m_id, name), value.x, value.y);
}

void Shader::SetVector3f(const char *name, float x, float y, float z, bool useShader)
{
    if (useShader)
        this->Use();
    glUniform3f(glGetUniformLocation(m_id, name), x, y, z);
}

void Shader::SetVector3f(const char *name, const glm::vec3 &value, bool useShader)
{
    if (useShader)
        this->Use();
    glUniform3f(glGetUniformLocation(m_id, name), value.x, value.y, value.z);
}

void Shader::SetVector4f(const char *name, float x, float y, float z, float w, bool useShader)
{
    if (useShader)
        this->Use();
    glUniform4f(glGetUniformLocation(m_id, name), x, y, z, w);
}

void Shader::SetVector4f(const char *name, const glm::vec4 &value, bool useShader)
{
    if (useShader)
        this->Use();
    auto location = glGetUniformLocation(m_id, name);
    std::cout << "ShaderId = " << m_id << ", name = " << name << ", location = " << location << std::endl;
    glUniform4f(glGetUniformLocation(m_id, name), value.x, value.y, value.z, value.w);
}

void Shader::SetMatrix4(const char *name, const glm::mat4 &matrix, bool useShader)
{
    if (useShader)
        this->Use();
    auto location = glGetUniformLocation(m_id, name);
    std::cout << "ShaderId = " << m_id << ", name = " << name << ", location = " << location << std::endl;
    glUniformMatrix4fv(glGetUniformLocation(m_id, name), 1, false, glm::value_ptr(matrix));
}

void checkCompileErrors(unsigned int object, std::string&& type)
{
    int success;
    char infoLog[1024];
    if (type != "PROGRAM") {
        glGetShaderiv(object, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(object, 1024, NULL, infoLog);
            std::cout << "| ERROR::SHADER: Compile-time error: Type: " << type << "\n"
                 << infoLog << "\n -- --------------------------------------------------- -- "
                 << std::endl;
        }
    } else {
        glGetProgramiv(object, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(object, 1024, NULL, infoLog);
            std::cout << "| ERROR::Shader: Link-time error: Type: " << type << "\n"
                 << infoLog << "\n -- --------------------------------------------------- -- "
                 << std::endl;
        }
    }
}

void printActiveUniforms(unsigned int programId)
{
    int uniforms = -2;
    glGetProgramiv(programId, GL_ACTIVE_UNIFORMS, &uniforms);
    std::cout << "Shader id = " << programId << std::endl;
    std::cout << "Shader uniform count = " << uniforms << std::endl;

    GLchar uniform_name[256];
    GLsizei length;
    GLint size;
    GLenum type;
    for (int i = 0; i < uniforms; i++)
    {
        glGetActiveUniform(programId, i, sizeof(uniform_name), &length, &size, &type, uniform_name);

        std::cout << "Uniform #" << i << std::endl;
        std::cout << "  name = " << uniform_name << std::endl;
        std::cout << "  type = " << type << std::endl;
    }
}