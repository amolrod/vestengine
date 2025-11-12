/**
 * @file OpenGLShader.h
 * @brief Implementación OpenGL del sistema de shaders
 */

#pragma once

#include "rendering/Shader.h"
#include <string>
#include <unordered_map>
#include <glm/glm.hpp>

namespace Engine {

/**
 * @class OpenGLShader
 * @brief Implementación concreta de Shader para OpenGL
 */
class OpenGLShader : public Shader {
public:
    OpenGLShader(const std::string& filepath);
    OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
    virtual ~OpenGLShader();

    void Bind() const override;
    void Unbind() const override;

    void SetInt(const std::string& name, int value) override;
    void SetIntArray(const std::string& name, int* values, uint32_t count) override;
    void SetFloat(const std::string& name, float value) override;
    void SetFloat2(const std::string& name, const glm::vec2& value) override;
    void SetFloat3(const std::string& name, const glm::vec3& value) override;
    void SetFloat4(const std::string& name, const glm::vec4& value) override;
    void SetMat3(const std::string& name, const glm::mat3& value) override;
    void SetMat4(const std::string& name, const glm::mat4& value) override;

    const std::string& GetName() const override { return m_name; }

private:
    void CompileAndLink(const std::string& vertexSrc, const std::string& fragmentSrc);
    uint32_t CompileShader(uint32_t type, const std::string& source);
    void LinkProgram(uint32_t vertexShader, uint32_t fragmentShader);
    int GetUniformLocation(const std::string& name);
    
    std::string ReadFile(const std::string& filepath);
    std::unordered_map<uint32_t, std::string> PreProcess(const std::string& source);

private:
    uint32_t m_rendererID = 0;
    std::string m_name;
    mutable std::unordered_map<std::string, int> m_uniformLocationCache;
};

} // namespace Engine
