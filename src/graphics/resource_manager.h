/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/

#ifndef GAME_OF_LIFE_RESOURCE_MANAGER_H
#define GAME_OF_LIFE_RESOURCE_MANAGER_H

#include <map>
#include <string>
#include <memory>

#include <glad/glad.h>

#include "shader.h"

class ResourceManager {
public:

    ResourceManager();
    ~ResourceManager();

    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;

    Shader* LoadShader(const char *vShaderFile, const char *fShaderFile, const char *gShaderFile, const std::string& name);
    Shader* GetShader(const std::string& name);

    void Clear();

private:
    std::map<std::string, std::unique_ptr<Shader>> m_shaders;
};

#endif //GAME_OF_LIFE_RESOURCE_MANAGER_H
