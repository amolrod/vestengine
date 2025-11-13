#pragma once

#include "Rendering/Shader.h"

namespace Vest {

class VulkanShader : public Shader {
public:
    explicit VulkanShader(const std::string& filepath);
    VulkanShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
    ~VulkanShader() override = default;

    void Bind() const override {}
    void Unbind() const override {}

    const std::string& GetName() const override { return m_Name; }

    void SetInt(const std::string&, int) override {}
    void SetFloat3(const std::string&, const glm::vec3&) override {}
    void SetFloat4(const std::string&, const glm::vec4&) override {}
    void SetMat4(const std::string&, const glm::mat4&) override {}

private:
    std::string m_Name;
};

}  // namespace Vest
