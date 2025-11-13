#include "Rendering/Shader.h"

#include <cassert>

#include "Rendering/RendererAPI.h"
#include "Rendering/Platform/OpenGL/OpenGLShader.h"
#include "Rendering/Platform/Vulkan/VulkanShader.h"

namespace Vest {

Ref<Shader> Shader::Create(const std::string& filepath) {
    switch (RendererAPI::GetAPI()) {
        case RenderAPI::OpenGL:
            return CreateRef<OpenGLShader>(filepath);
        case RenderAPI::Vulkan:
            return CreateRef<VulkanShader>(filepath);
        case RenderAPI::None:
        default:
            assert(false && "Unknown RenderAPI");
            return nullptr;
    }
}

Ref<Shader> Shader::Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc) {
    switch (RendererAPI::GetAPI()) {
        case RenderAPI::OpenGL:
            return CreateRef<OpenGLShader>(name, vertexSrc, fragmentSrc);
        case RenderAPI::Vulkan:
            return CreateRef<VulkanShader>(name, vertexSrc, fragmentSrc);
        case RenderAPI::None:
        default:
            assert(false && "Unknown RenderAPI");
            return nullptr;
    }
}

void ShaderLibrary::Add(const Ref<Shader>& shader) {
    m_Shaders[shader->GetName()] = shader;
}

Ref<Shader> ShaderLibrary::Load(const std::string& filepath) {
    Ref<Shader> shader = Shader::Create(filepath);
    if (shader) {
        Add(shader);
    }
    return shader;
}

Ref<Shader> ShaderLibrary::Get(const std::string& name) const {
    auto it = m_Shaders.find(name);
    return it != m_Shaders.end() ? it->second : nullptr;
}

}  // namespace Vest
