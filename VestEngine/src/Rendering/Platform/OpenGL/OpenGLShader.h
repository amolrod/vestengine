#pragma once

#include <string>
#include <unordered_map>

#include <glm/glm.hpp>

#include "Rendering/Shader.h"

namespace Vest {

class OpenGLShader : public Shader {
public:
    explicit OpenGLShader(const std::string& filepath);
    OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
    ~OpenGLShader() override;

    void Bind() const override;
    void Unbind() const override;

    const std::string& GetName() const override { return m_Name; }

    void SetInt(const std::string& name, int value) override;
    void SetFloat3(const std::string& name, const glm::vec3& value) override;
    void SetFloat4(const std::string& name, const glm::vec4& value) override;
    void SetMat4(const std::string& name, const glm::mat4& value) override;

private:
    std::string ReadFile(const std::string& filepath);
    std::unordered_map<uint32_t, std::string> PreProcess(const std::string& source);
    void Compile(const std::unordered_map<uint32_t, std::string>& shaderSources);

    int GetUniformLocation(const std::string& name);
    void UploadUniformInt(const std::string& name, int value);
    void UploadUniformFloat3(const std::string& name, const glm::vec3& value);
    void UploadUniformFloat4(const std::string& name, const glm::vec4& value);
    void UploadUniformMat4(const std::string& name, const glm::mat4& matrix);

    uint32_t m_RendererID = 0;
    std::string m_Name;
    std::unordered_map<std::string, int> m_UniformLocationCache;
};

}  // namespace Vest
