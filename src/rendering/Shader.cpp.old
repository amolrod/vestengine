/**
 * @file Shader.cpp
 * @brief Implementación del sistema de shaders
 */

#include "rendering/Shader.h"
#include "core/Logger.h"

#include <fstream>
#include <sstream>
#include <array>

// Platform-specific OpenGL headers
#ifdef PLATFORM_MACOS
    #define GL_SILENCE_DEPRECATION
    #include <OpenGL/gl3.h>
#else
    #include <glad/gl.h>
#endif

namespace Engine {

// =============================================================================
// Constructor & Destructor
// =============================================================================

Shader::Shader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
    : m_name(name) {
    
    LOG_CORE_DEBUG("Compilando shader: {}", name);
    
    // Compilar shaders individuales
    uint32_t vertexShader = CompileShader(GL_VERTEX_SHADER, vertexSrc);
    uint32_t fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentSrc);
    
    // Linkear programa
    LinkProgram(vertexShader, fragmentShader);
    
    // Limpiar shaders intermedios (ya están linkeados)
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    LOG_CORE_INFO("Shader '{}' compilado correctamente (ID: {})", name, m_rendererID);
}

Shader::~Shader() {
    if (m_rendererID != 0) {
        glDeleteProgram(m_rendererID);
        LOG_CORE_DEBUG("Shader '{}' destruido", m_name);
    }
}

// =============================================================================
// Bind/Unbind
// =============================================================================

void Shader::Bind() const {
    glUseProgram(m_rendererID);
}

void Shader::Unbind() const {
    glUseProgram(0);
}

// =============================================================================
// Compilación
// =============================================================================

uint32_t Shader::CompileShader(uint32_t type, const std::string& source) {
    uint32_t shader = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);
    
    // Verificar errores de compilación
    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    
    if (!success) {
        int length;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
        
        std::string infoLog(length, ' ');
        glGetShaderInfoLog(shader, length, &length, &infoLog[0]);
        
        const char* shaderType = (type == GL_VERTEX_SHADER) ? "VERTEX" : "FRAGMENT";
        LOG_CORE_ERROR("Error compilando {} shader:", shaderType);
        LOG_CORE_ERROR("{}", infoLog);
        
        glDeleteShader(shader);
        return 0;
    }
    
    return shader;
}

void Shader::LinkProgram(uint32_t vertexShader, uint32_t fragmentShader) {
    m_rendererID = glCreateProgram();
    
    glAttachShader(m_rendererID, vertexShader);
    glAttachShader(m_rendererID, fragmentShader);
    glLinkProgram(m_rendererID);
    
    // Verificar errores de linking
    int success;
    glGetProgramiv(m_rendererID, GL_LINK_STATUS, &success);
    
    if (!success) {
        int length;
        glGetProgramiv(m_rendererID, GL_INFO_LOG_LENGTH, &length);
        
        std::string infoLog(length, ' ');
        glGetProgramInfoLog(m_rendererID, length, &length, &infoLog[0]);
        
        LOG_CORE_ERROR("Error linkeando shader program:");
        LOG_CORE_ERROR("{}", infoLog);
        
        glDeleteProgram(m_rendererID);
        m_rendererID = 0;
    }
}

// =============================================================================
// Factory Methods
// =============================================================================

std::shared_ptr<Shader> Shader::Create(const std::string& filepath) {
    std::string source = ReadFile(filepath);
    auto shaderSources = PreProcess(source);
    
    if (shaderSources.find(GL_VERTEX_SHADER) == shaderSources.end() ||
        shaderSources.find(GL_FRAGMENT_SHADER) == shaderSources.end()) {
        LOG_CORE_ERROR("Shader '{}' no contiene vertex y/o fragment shader", filepath);
        return nullptr;
    }
    
    // Extraer nombre del archivo
    auto lastSlash = filepath.find_last_of("/\\");
    lastSlash = (lastSlash == std::string::npos) ? 0 : lastSlash + 1;
    auto lastDot = filepath.rfind('.');
    auto count = (lastDot == std::string::npos) ? filepath.size() - lastSlash : lastDot - lastSlash;
    std::string name = filepath.substr(lastSlash, count);
    
    return std::make_shared<Shader>(name, 
                                    shaderSources[GL_VERTEX_SHADER],
                                    shaderSources[GL_FRAGMENT_SHADER]);
}

std::shared_ptr<Shader> Shader::Create(const std::string& name, 
                                       const std::string& vertexSrc, 
                                       const std::string& fragmentSrc) {
    return std::make_shared<Shader>(name, vertexSrc, fragmentSrc);
}

// =============================================================================
// File I/O
// =============================================================================

std::string Shader::ReadFile(const std::string& filepath) {
    std::ifstream file(filepath, std::ios::in | std::ios::binary);
    
    if (!file) {
        LOG_CORE_ERROR("No se pudo abrir archivo de shader: {}", filepath);
        return "";
    }
    
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

std::unordered_map<uint32_t, std::string> Shader::PreProcess(const std::string& source) {
    std::unordered_map<uint32_t, std::string> shaderSources;
    
    const char* typeToken = "#shader";
    size_t typeTokenLength = strlen(typeToken);
    size_t pos = source.find(typeToken, 0);
    
    while (pos != std::string::npos) {
        size_t eol = source.find_first_of("\r\n", pos);
        size_t begin = pos + typeTokenLength + 1;
        std::string type = source.substr(begin, eol - begin);
        
        size_t nextLinePos = source.find_first_not_of("\r\n", eol);
        pos = source.find(typeToken, nextLinePos);
        
        uint32_t shaderType = 0;
        if (type == "vertex") {
            shaderType = GL_VERTEX_SHADER;
        } else if (type == "fragment" || type == "pixel") {
            shaderType = GL_FRAGMENT_SHADER;
        } else {
            LOG_CORE_WARN("Tipo de shader desconocido: {}", type);
        }
        
        if (shaderType != 0) {
            shaderSources[shaderType] = (pos == std::string::npos) 
                ? source.substr(nextLinePos) 
                : source.substr(nextLinePos, pos - nextLinePos);
        }
    }
    
    return shaderSources;
}

// =============================================================================
// Uniforms
// =============================================================================

int Shader::GetUniformLocation(const std::string& name) {
    if (m_uniformLocationCache.find(name) != m_uniformLocationCache.end()) {
        return m_uniformLocationCache[name];
    }
    
    int location = glGetUniformLocation(m_rendererID, name.c_str());
    
    if (location == -1) {
        LOG_CORE_WARN("Uniform '{}' no encontrado en shader '{}'", name, m_name);
    }
    
    m_uniformLocationCache[name] = location;
    return location;
}

void Shader::SetUniformInt(const std::string& name, int value) {
    glUniform1i(GetUniformLocation(name), value);
}

void Shader::SetUniformIntArray(const std::string& name, int* values, uint32_t count) {
    glUniform1iv(GetUniformLocation(name), count, values);
}

void Shader::SetUniformFloat(const std::string& name, float value) {
    glUniform1f(GetUniformLocation(name), value);
}

void Shader::SetUniformFloat2(const std::string& name, const glm::vec2& value) {
    glUniform2f(GetUniformLocation(name), value.x, value.y);
}

void Shader::SetUniformFloat3(const std::string& name, const glm::vec3& value) {
    glUniform3f(GetUniformLocation(name), value.x, value.y, value.z);
}

void Shader::SetUniformFloat4(const std::string& name, const glm::vec4& value) {
    glUniform4f(GetUniformLocation(name), value.x, value.y, value.z, value.w);
}

void Shader::SetUniformMat3(const std::string& name, const glm::mat3& matrix) {
    glUniformMatrix3fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]);
}

void Shader::SetUniformMat4(const std::string& name, const glm::mat4& matrix) {
    glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]);
}

// =============================================================================
// ShaderLibrary Implementation
// =============================================================================

void ShaderLibrary::Add(const std::shared_ptr<Shader>& shader) {
    auto& name = shader->GetName();
    Add(name, shader);
}

void ShaderLibrary::Add(const std::string& name, const std::shared_ptr<Shader>& shader) {
    if (Exists(name)) {
        LOG_CORE_WARN("Shader '{}' ya existe en la biblioteca", name);
        return;
    }
    
    m_shaders[name] = shader;
    LOG_CORE_DEBUG("Shader '{}' añadido a biblioteca", name);
}

std::shared_ptr<Shader> ShaderLibrary::Load(const std::string& filepath) {
    auto shader = Shader::Create(filepath);
    if (shader) {
        Add(shader);
    }
    return shader;
}

std::shared_ptr<Shader> ShaderLibrary::Load(const std::string& name, const std::string& filepath) {
    auto shader = Shader::Create(filepath);
    if (shader) {
        Add(name, shader);
    }
    return shader;
}

std::shared_ptr<Shader> ShaderLibrary::Get(const std::string& name) {
    if (!Exists(name)) {
        LOG_CORE_ERROR("Shader '{}' no encontrado en biblioteca", name);
        return nullptr;
    }
    
    return m_shaders[name];
}

bool ShaderLibrary::Exists(const std::string& name) const {
    return m_shaders.find(name) != m_shaders.end();
}

} // namespace Engine
