#include "Shader.hpp"

#include "glad/gl.h"

#include <exception>
#include <filesystem>
#include <sstream>
#include <vector>

#include <iostream>
#include <stdexcept>

std::unordered_set<Shader*> Shader::s_ShaderCache;

Shader::Shader()
{
    s_ShaderCache.insert(this);
}

Shader::~Shader()
{
    s_ShaderCache.erase(this);
}

uint32_t Shader::getID()
{
    if (m_ID)
    {
        return m_ID.value();
    }
    else
    {
        throw shader_existence_error();
    }
}

void Shader::compileFromPath(const std::string& vertexPath, const std::string& fragmentPath)
{
    const std::string vertexSource = readContentsOfFile(vertexPath);
    const std::string fragmentSource = readContentsOfFile(fragmentPath);

    compileFromSource(vertexSource, fragmentSource);

    addFileToCache(vertexPath, ShaderType::VERTEX);
    addFileToCache(fragmentPath, ShaderType::FRAGMENT);
}

void Shader::compileFromSource(const std::string& vertexSource, const std::string& fragmentSource)
{
    uint32_t sVertex;
    uint32_t sFragment;

    try
    {
        sVertex = compileVertex(vertexSource);
    }
    catch (const std::runtime_error& e)
    {
        sVertex = 0;
    }

    try
    {
        sFragment = compileFragment(fragmentSource);
    }
    catch (const std::runtime_error& e)
    {
        sFragment = 0;
    }

    m_ID = glCreateProgram();
    glAttachShader(getID(), sVertex);
    glAttachShader(getID(), sFragment);
    glLinkProgram(getID());
    checkProgramError(getID());

    glDeleteShader(sVertex);
    glDeleteShader(sFragment);
}

void Shader::bind()
{
    glUseProgram(getID());
}

void Shader::unbind()
{
    glUseProgram(0);
}

void Shader::checkFileChange()
{
    for (auto it = s_ShaderCache.begin(); it != s_ShaderCache.end(); it++)
    {
        (*it)->checkSourceFileChanges();
    }
}

void Shader::setFloat(const std::string& name, float value)
{
    glUniform1f(getUniformLocation(name), value);
}

void Shader::setVec2(const std::string& name, const glm::vec2& value)
{
    glUniform2f(getUniformLocation(name), value.x, value.y);
}

void Shader::setVec3(const std::string& name, const glm::vec3& value)
{
    glUniform3f(getUniformLocation(name), value.x, value.y, value.z);
}

void Shader::setMat4(const std::string& name, const glm::mat4& value)
{
    glUniformMatrix4fv(getUniformLocation(name), 1, false, glm::value_ptr(value));
}

uint32_t Shader::compileVertex(const std::string& source)
{
    uint32_t sVertex = glCreateShader(GL_VERTEX_SHADER);
    const char* vSource = source.c_str();
    glShaderSource(sVertex, 1, &vSource, NULL);
    glCompileShader(sVertex);
    checkShaderUnitError(sVertex, "Vertex");

    return sVertex;
}

uint32_t Shader::compileFragment(const std::string& source)
{
    uint32_t sFragment = glCreateShader(GL_FRAGMENT_SHADER);
    const char* fSource = source.c_str();
    glShaderSource(sFragment, 1, &fSource, NULL);
    glCompileShader(sFragment);
    checkShaderUnitError(sFragment, "Fragment");

    return sFragment;
}

int32_t Shader::getUniformLocation(const std::string& name)
{
    return glGetUniformLocation(getID(), name.c_str());
}

void Shader::addFileToCache(const std::string& path, ShaderType type)
{
    fileCache cache;
    cache.lastWrite = std::filesystem::last_write_time(path);
    cache.type = type;

    m_FileCache[std::filesystem::path{path}] = cache;
}

void Shader::checkSourceFileChanges()
{
    for (auto it = m_FileCache.begin(); it != m_FileCache.end(); it++)
    {
        try
        {
            if (it->second.lastWrite != std::filesystem::last_write_time(it->first))
            {
                updateShader();
                it->second.lastWrite = std::filesystem::last_write_time(it->first);
                return;
            }
        }
        catch (const std::filesystem::filesystem_error& e)
        {
            std::cout << "Failed to read/update time\n";
        }
    }
}

bool Shader::updateShader()
{
    std::vector<uint32_t> units;
    try
    {
        for (auto it = m_FileCache.begin(); it != m_FileCache.end(); it++)
        {
            uint32_t s;
            std::string source = readContentsOfFile(it->first.string());
            switch (it->second.type)
            {
                case ShaderType::VERTEX:
                    s = compileVertex(source);
                    break;
                case ShaderType::FRAGMENT:
                    s = compileFragment(source);
                    break;
            }
            units.push_back(s);
        }
    }
    catch (const std::runtime_error& e)
    {
        return false;
    }

    glDeleteProgram(getID());
    m_ID.reset();

    m_ID = glCreateProgram();

    for (uint32_t u : units)
        glAttachShader(getID(), u);

    glLinkProgram(getID());
    checkProgramError(getID());

    for (uint32_t u : units)
        glDeleteShader(u);

    return true;
}

void Shader::checkShaderUnitError(uint32_t object, const std::string& unitType)
{
    int32_t success;
    char infoLog[1024];
    glGetShaderiv(object, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(object, 1024, NULL, infoLog);
        std::cerr << "SHADER:COMPILER_ERROR:" << unitType << "\n" << infoLog << "\n";
        throw std::runtime_error("Shader compile error");
    }
}

void Shader::checkProgramError(uint32_t object)
{
    int32_t success;
    char infoLog[1024];

    glGetProgramiv(object, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(object, 1024, NULL, infoLog);
        std::cerr << "SHADER:LINK_ERROR:\n" << infoLog << "\n";
        throw std::runtime_error("Shader Link Error");
    }
}

const std::string Shader::readContentsOfFile(const std::string& filename)
{
    std::string source;

    std::ifstream file;
    file.open(filename, std::ios::in);
    if (file.is_open())
    {
        std::stringstream stream;

        stream << file.rdbuf();
        file.close();

        source = stream.str();

        return source;
    }
    else
    {
        throw std::runtime_error("Failed to open " + std::string(filename));
    }
}
