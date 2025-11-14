#include "Rendering/Platform/OpenGL/OpenGLTexture.h"

#include <cassert>

#include <stb_image.h>

namespace Vest {

OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height) : m_Width(width), m_Height(height) {
    m_InternalFormat = GL_RGBA8;
    m_DataFormat = GL_RGBA;
    Allocate();
    InitializeParameters();
}

OpenGLTexture2D::OpenGLTexture2D(const std::string& path) : m_Path(path) {
    int width = 0;
    int height = 0;
    int channels = 0;
    stbi_set_flip_vertically_on_load(true);
    stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
    const bool loadedFromDisk = data != nullptr;

    if (!loadedFromDisk) {
        width = height = 1;
        channels = 4;
        static uint8_t fallbackData[4] = {255, 0, 255, 255};
        data = fallbackData;
    }

    m_Width = static_cast<uint32_t>(width);
    m_Height = static_cast<uint32_t>(height);

    if (channels < 3) {
        if (loadedFromDisk) {
            stbi_image_free(data);
            data = stbi_load(path.c_str(), &width, &height, &channels, STBI_rgb_alpha);
            channels = 4;
            m_Width = static_cast<uint32_t>(width);
            m_Height = static_cast<uint32_t>(height);
        } else {
            channels = 4;
        }
    }

    if (channels == 4) {
        m_InternalFormat = GL_RGBA8;
        m_DataFormat = GL_RGBA;
    } else if (channels == 3) {
        m_InternalFormat = GL_RGB8;
        m_DataFormat = GL_RGB;
    } else {
        assert(false && "Unsupported texture format");
    }

    Allocate(data);
    InitializeParameters();

    if (loadedFromDisk && data) {
        stbi_image_free(data);
        m_IsLoaded = true;
    }
}

OpenGLTexture2D::~OpenGLTexture2D() {
    glDeleteTextures(1, &m_RendererID);
}

void OpenGLTexture2D::Allocate(const void* data) {
    glGenTextures(1, &m_RendererID);
    glBindTexture(GL_TEXTURE_2D, m_RendererID);
    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 static_cast<GLint>(m_InternalFormat),
                 static_cast<GLsizei>(m_Width),
                 static_cast<GLsizei>(m_Height),
                 0,
                 m_DataFormat,
                 GL_UNSIGNED_BYTE,
                 data);
}

void OpenGLTexture2D::InitializeParameters() const {
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void OpenGLTexture2D::Bind(uint32_t slot) const {
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, m_RendererID);
}

}  // namespace Vest
