#pragma once

#include <cstdint>
#include <optional>
#include <fstream>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <filesystem>

class shader_existence_error : public std::exception
{
public:
    shader_existence_error() { }

    const char* what() const noexcept { return "Shader does not exist"; } 
};

class Shader
{
public:
    Shader();
    ~Shader();

    uint32_t getID();

    void compileFromPath(const std::string& vertexPath, const std::string& fragmentPath);
    void compileFromSource(const std::string& vertexString, const std::string& fragmentString);

    void bind();
    static void unbind();

    static void checkFileChange();

    void setFloat(const std::string& name, float value);
    void setVec2(const std::string& name, const glm::vec2& value);
    void setVec3(const std::string& name, const glm::vec3& value);
    void setMat4(const std::string& name, const glm::mat4& value);
private:

    std::optional<uint32_t> m_ID;

    enum class ShaderType
    {
        VERTEX,
        FRAGMENT
    };

    struct fileCache
    {
        std::filesystem::file_time_type lastWrite;
        ShaderType type;
    };

    static std::unordered_set<Shader*> s_ShaderCache;
    std::unordered_map<std::filesystem::path, fileCache> m_FileCache;
private:
    static uint32_t compileVertex(const std::string& source);
    static uint32_t compileFragment(const std::string& source);

    int32_t getUniformLocation(const std::string& name);

    void addFileToCache(const std::string& path, ShaderType type);
    void checkSourceFileChanges();
    bool updateShader();

    static void checkShaderUnitError(uint32_t object, const std::string& type);
    static void checkProgramError(uint32_t object);

    static const std::string readContentsOfFile(const std::string& filename);
};

