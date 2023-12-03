#pragma once

#include <cstdint>
#include <optional>
#include <fstream>

class shader_existence_error : public std::exception
{
public:
    shader_existence_error() { }

    const char* what() const noexcept { return "Shader does not exist"; } 
};

class Shader
{
public:
    Shader() {};

    uint32_t getID();

    void compileFromPath(const std::string& vertexPath, const std::string& fragmentPath);
    void compileFromSource(const std::string& vertexString, const std::string& fragmentString);

    void bind();
    static void unbind();


    void setFloat(const std::string& name, float value);
private:
    std::optional<uint32_t> m_ID;
private:
    int32_t getUniformLocation(const std::string& name);

    static void checkShaderUnitError(uint32_t object, const std::string& type);
    static void checkProgramError(uint32_t object);

    static const std::string readContentsOfFile(const std::string& filename);
};
