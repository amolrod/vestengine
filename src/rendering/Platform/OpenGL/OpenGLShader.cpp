/**
 * @file OpenGLShader.cpp
 * @brief Implementación OpenGL de la interfaz Shader
 */

#include "OpenGLShader.h"
#include "core/Logger.h"

#include <fstream>
#include <sstream>
#include <glad/gl.h>

namespace Engine {

// =============================================================================
// Constructor & Destructor
// =============================================================================

OpenGLShader::OpenGLShader(const std::string& filepath) {
    std::string source = ReadFile(filepath);
    auto shaderSources = PreProcess(source);
    
    if (shaderSources.find(GL_VERTEX_SHADER) == shaderSources.end() ||
        shaderSources.find(GL_FRAGMENT_SHADER) == shaderSources.end()) {
        LOG_CORE_ERROR("Shader '{}' no contiene vertex y/o fragment shader", filepath);
        return;
    }
    
    // Extraer nombre del archivo
    auto lastSlash = filepath.find_last_of("/\\");
    lastSlash = (lastSlash == std::string::npos) ? 0 : lastSlash + 1;
    auto lastDot = filepath.rfind('.');
    auto count = (lastDot == std::string::npos) ? filepath.size() - lastSlash : lastDot - lastSlash;
    m_name = filepath.substr(lastSlash, count);
    
    CompileAndLink(shaderSources[GL_VERTEX_SHADER], shaderSources[GL_FRAGMENT_SHADER]);
}

OpenGLShader::OpenGLShader(const std::string& name, 
                           const std::string& vertexSrc, 
                           const std::string& fragmentSrc)
    : m_name(name) {
    
    CompileAndLink(vertexSrc, fragmentSrc);
}

OpenGLShader::~OpenGLShader() {
    if (m_rendererID != 0) {
        glDeleteProgram(m_rendererID);
        LOG_CORE_DEBUG("OpenGLShader '{}' destruido", m_name);
    }
}

// =============================================================================
// Bind/Unbind
// =============================================================================

void OpenGLShader::Bind() const {
    glUseProgram(m_rendererID);
}

void OpenGLShader::Unbind() const {
    glUseProgram(0);
}

// =============================================================================
// Uniforms
// =============================================================================

void OpenGLShader::SetInt(const std::string& name, int value) {
    glUniform1i(GetUniformLocation(name), value);
}

void OpenGLShader::SetIntArray(const std::string& name, int* values, uint32_t count) {
    glUniform1iv(GetUniformLocation(name), count, values);
}

void OpenGLShader::SetFloat(const std::string& name, float value) {
    glUniform1f(GetUniformLocation(name), value);
}

void OpenGLShader::SetFloat2(const std::string& name, const glm::vec2& value) {
    glUniform2f(GetUniformLocation(name), value.x, value.y);
}

void OpenGLShader::SetFloat3(const std::string& name, const glm::vec3& value) {
    glUniform3f(GetUniformLocation(name), value.x, value.y, value.z);
}

void OpenGLShader::SetFloat4(const std::string& name, const glm::vec4& value) {
    glUniform4f(GetUniformLocation(name), value.x, value.y, value.z, value.w);
}

void OpenGLShader::SetMat3(const std::string& name, const glm::mat3& matrix) {
    glUniformMatrix3fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]);
}

void OpenGLShader::SetMat4(const std::string& name, const glm::mat4& matrix) {
    glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]);
}

// =============================================================================
// Internal Helpers
// =============================================================================

void OpenGLShader::CompileAndLink(const std::string& vertexSrc, const std::string& fragmentSrc) {
    LOG_CORE_DEBUG("Compilando OpenGL shader: {}", m_name);
    
    // Compilar shaders individuales
    uint32_t vertexShader = CompileShader(GL_VERTEX_SHADER, vertexSrc);
    uint32_t fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentSrc);
    
    if (vertexShader == 0 || fragmentShader == 0) {
        LOG_CORE_ERROR("Failed to compile shaders for '{}'", m_name);
        if (vertexShader != 0) glDeleteShader(vertexShader);
        if (fragmentShader != 0) glDeleteShader(fragmentShader);
        return;
    }
    
    // Linkear programa
    LinkProgram(vertexShader, fragmentShader);
    
    // Limpiar shaders intermedios (ya están linkeados)
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    LOG_CORE_INFO("OpenGL Shader '{}' compilado correctamente (ID: {})", m_name, m_rendererID);
}

uint32_t OpenGLShader::CompileShader(uint32_t type, const std::string& source) {
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

void OpenGLShader::LinkProgram(uint32_t vertexShader, uint32_t fragmentShader) {
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

int OpenGLShader::GetUniformLocation(const std::string& name) {
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

std::string OpenGLShader::ReadFile(const std::string& filepath) {
    std::ifstream file(filepath, std::ios::in | std::ios::binary);
    
    if (!file) {
        LOG_CORE_ERROR("No se pudo abrir archivo de shader: {}", filepath);
        return "";
    }
    
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

std::unordered_map<uint32_t, std::string> OpenGLShader::PreProcess(const std::string& source) {
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

} // namespace Engine
