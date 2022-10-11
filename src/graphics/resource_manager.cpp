#include <iostream>
#include <sstream>
#include <fstream>

#include "resource_manager.h"

std::unique_ptr<Shader> loadShaderFromFile(const char *vShaderFile, const char *fShaderFile, const char *gShaderFile);

ResourceManager::ResourceManager() : m_shaders()
{
}

ResourceManager::~ResourceManager()
{
    m_shaders.clear();
}

Shader* ResourceManager::LoadShader(const char *vShaderFile, const char *fShaderFile, const char *gShaderFile,
                                   const std::string& name) {
    auto [iterator, _] = m_shaders.emplace(name, loadShaderFromFile(vShaderFile, fShaderFile, gShaderFile));
    auto& value = iterator->second;
    return value.get();
}

Shader* ResourceManager::GetShader(const std::string& name) const
{
    auto& result = m_shaders.at(name);
    return result.get();
}

void ResourceManager::Clear() {
    m_shaders.clear();
}

std::unique_ptr<Shader> loadShaderFromFile(const char *vShaderFile, const char *fShaderFile, const char *gShaderFile)
{
    // 1. retrieve the vertex/fragment source code from filePath
    std::string vertexCode;
    std::string fragmentCode;
    std::string geometryCode;
    try
    {
        // open files
        std::ifstream vertexShaderFile(vShaderFile);
        std::ifstream fragmentShaderFile(fShaderFile);
        std::stringstream vShaderStream, fShaderStream;
        // read file's buffer contents into streams
        vShaderStream << vertexShaderFile.rdbuf();
        fShaderStream << fragmentShaderFile.rdbuf();
        // close file handlers
        vertexShaderFile.close();
        fragmentShaderFile.close();
        // convert stream into string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
        // if geometry shader path is present, also load a geometry shader
        if (gShaderFile != nullptr)
        {
            std::ifstream geometryShaderFile(gShaderFile);
            std::stringstream gShaderStream;
            gShaderStream << geometryShaderFile.rdbuf();
            geometryShaderFile.close();
            geometryCode = gShaderStream.str();
        }
    }
    catch (std::exception& e)
    {
        std::cout << "ERROR::SHADER: Failed to read shader files" << std::endl;
    }

    const char *vShaderCode = vertexCode.c_str();
    const char *fShaderCode = fragmentCode.c_str();
    const char *gShaderCode = geometryCode.c_str();


    auto shader = std::make_unique<Shader>();
    shader->Compile(vShaderCode, fShaderCode, gShaderFile != nullptr ? gShaderCode : nullptr);
    return shader;
}