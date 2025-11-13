#include "Rendering/Platform/OpenGL/OpenGLShader.h"

#include <cassert>
#include <cstring>
#include <fstream>
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <sstream>
#include <vector>

namespace Vest {

static GLenum ShaderTypeFromString(const std::string& type) {
    if (type == "vertex") {
        return GL_VERTEX_SHADER;
    }
    if (type == "fragment" || type == "pixel") {
        return GL_FRAGMENT_SHADER;
    }
    assert(false && "Unknown shader type");
    return 0;
}

OpenGLShader::OpenGLShader(const std::string& filepath) {
    std::string source = ReadFile(filepath);
    auto shaderSources = PreProcess(source);
    Compile(shaderSources);

    auto lastSlash = filepath.find_last_of("/\\");
    auto lastDot = filepath.find_last_of('.');
    auto count = lastDot == std::string::npos ? filepath.size() - lastSlash : lastDot - lastSlash - 1;
    m_Name = filepath.substr(lastSlash + 1, count);
}

OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc) : m_Name(name) {
    std::unordered_map<uint32_t, std::string> sources;
    sources[GL_VERTEX_SHADER] = vertexSrc;
    sources[GL_FRAGMENT_SHADER] = fragmentSrc;
    Compile(sources);
}

OpenGLShader::~OpenGLShader() {
    glDeleteProgram(m_RendererID);
}

void OpenGLShader::Bind() const {
    glUseProgram(m_RendererID);
}

void OpenGLShader::Unbind() const {
    glUseProgram(0);
}

void OpenGLShader::SetInt(const std::string& name, int value) {
    UploadUniformInt(name, value);
}

void OpenGLShader::SetFloat3(const std::string& name, const glm::vec3& value) {
    UploadUniformFloat3(name, value);
}

void OpenGLShader::SetFloat4(const std::string& name, const glm::vec4& value) {
    UploadUniformFloat4(name, value);
}

void OpenGLShader::SetMat4(const std::string& name, const glm::mat4& value) {
    UploadUniformMat4(name, value);
}

std::string OpenGLShader::ReadFile(const std::string& filepath) {
    std::ifstream in(filepath, std::ios::in | std::ios::binary);
    if (!in) {
        return "";
    }

    std::stringstream ss;
    ss << in.rdbuf();
    return ss.str();
}

std::unordered_map<uint32_t, std::string> OpenGLShader::PreProcess(const std::string& source) {
    std::unordered_map<uint32_t, std::string> shaderSources;

    const char* typeToken = "#type";
    size_t typeTokenLength = strlen(typeToken);
    size_t pos = source.find(typeToken, 0);

    while (pos != std::string::npos) {
        size_t eol = source.find_first_of("\r\n", pos);
        assert(eol != std::string::npos && "Syntax error");
        size_t begin = pos + typeTokenLength + 1;
        std::string type = source.substr(begin, eol - begin);
        size_t nextLinePos = source.find_first_not_of("\r\n", eol);
        pos = source.find(typeToken, nextLinePos);
        shaderSources[ShaderTypeFromString(type)] = source.substr(nextLinePos, pos - nextLinePos);
    }

    return shaderSources;
}

void OpenGLShader::Compile(const std::unordered_map<uint32_t, std::string>& shaderSources) {
    GLuint program = glCreateProgram();
    std::vector<GLuint> shaderIDs;
    shaderIDs.reserve(shaderSources.size());

    for (auto& [type, source] : shaderSources) {
        GLuint shader = glCreateShader(type);
        const GLchar* sourceCStr = source.c_str();
        glShaderSource(shader, 1, &sourceCStr, nullptr);
        glCompileShader(shader);

        GLint isCompiled = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
        if (isCompiled == GL_FALSE) {
            GLint maxLength = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

            std::vector<GLchar> infoLog(static_cast<size_t>(maxLength));
            glGetShaderInfoLog(shader, maxLength, &maxLength, infoLog.data());
            glDeleteShader(shader);
            assert(false && "Shader compilation failure");
            break;
        }

        glAttachShader(program, shader);
        shaderIDs.push_back(shader);
    }

    glLinkProgram(program);

    GLint isLinked = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &isLinked);
    if (isLinked == GL_FALSE) {
        GLint maxLength = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

        std::vector<GLchar> infoLog(static_cast<size_t>(maxLength));
        glGetProgramInfoLog(program, maxLength, &maxLength, infoLog.data());
        glDeleteProgram(program);
        for (auto id : shaderIDs) {
            glDeleteShader(id);
        }
        assert(false && "Shader link failure");
        return;
    }

    for (auto id : shaderIDs) {
        glDetachShader(program, id);
        glDeleteShader(id);
    }

    m_RendererID = program;
}

int OpenGLShader::GetUniformLocation(const std::string& name) {
    auto it = m_UniformLocationCache.find(name);
    if (it != m_UniformLocationCache.end()) {
        return it->second;
    }

    GLint location = glGetUniformLocation(m_RendererID, name.c_str());
    m_UniformLocationCache[name] = location;
    return location;
}

void OpenGLShader::UploadUniformInt(const std::string& name, int value) {
    glUniform1i(GetUniformLocation(name), value);
}

void OpenGLShader::UploadUniformFloat3(const std::string& name, const glm::vec3& value) {
    glUniform3fv(GetUniformLocation(name), 1, glm::value_ptr(value));
}

void OpenGLShader::UploadUniformFloat4(const std::string& name, const glm::vec4& value) {
    glUniform4fv(GetUniformLocation(name), 1, glm::value_ptr(value));
}

void OpenGLShader::UploadUniformMat4(const std::string& name, const glm::mat4& matrix) {
    glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(matrix));
}

}  // namespace Vest
