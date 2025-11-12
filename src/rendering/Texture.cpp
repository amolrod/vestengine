/**
 * @file Texture.cpp
 * @brief Implementación del sistema de texturas
 */

#include "Texture.h"
#include "../core/Logger.h"

#ifdef PLATFORM_MACOS
    #define GL_SILENCE_DEPRECATION
    #include <OpenGL/gl3.h>
#else
    #include <glad/gl.h>
#endif

#include <stb_image.h>

namespace Engine {

// =============================================================================
// Helper Functions
// =============================================================================

static GLenum TextureFormatToGL(TextureFormat format) {
    switch (format) {
        case TextureFormat::RGB8:  return GL_RGB;
        case TextureFormat::RGBA8: return GL_RGBA;
        case TextureFormat::RED:   return GL_RED;
        case TextureFormat::RG:    return GL_RG;
        default: break;
    }
    
    LOG_ERROR("Unknown TextureFormat!");
    return 0;
}

static GLenum TextureWrapToGL(TextureWrap wrap) {
    switch (wrap) {
        case TextureWrap::Repeat:         return GL_REPEAT;
        case TextureWrap::MirroredRepeat: return GL_MIRRORED_REPEAT;
        case TextureWrap::ClampToEdge:    return GL_CLAMP_TO_EDGE;
        case TextureWrap::ClampToBorder:  return GL_CLAMP_TO_BORDER;
        default: break;
    }
    
    LOG_ERROR("Unknown TextureWrap!");
    return GL_REPEAT;
}

static GLenum TextureFilterToGL(TextureFilter filter) {
    switch (filter) {
        case TextureFilter::Nearest:              return GL_NEAREST;
        case TextureFilter::Linear:               return GL_LINEAR;
        case TextureFilter::NearestMipmapNearest: return GL_NEAREST_MIPMAP_NEAREST;
        case TextureFilter::LinearMipmapNearest:  return GL_LINEAR_MIPMAP_NEAREST;
        case TextureFilter::NearestMipmapLinear:  return GL_NEAREST_MIPMAP_LINEAR;
        case TextureFilter::LinearMipmapLinear:   return GL_LINEAR_MIPMAP_LINEAR;
        default: break;
    }
    
    LOG_ERROR("Unknown TextureFilter!");
    return GL_LINEAR;
}

// =============================================================================
// Texture2D - Constructor desde archivo
// =============================================================================

Texture2D::Texture2D(const std::string& path, const TextureSpecification& spec)
    : m_path(path), m_format(spec.format) {
    
    // Configurar stb_image
    stbi_set_flip_vertically_on_load(spec.flipVertically);
    
    // Cargar imagen
    int width, height, channels;
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
    
    if (!data) {
        LOG_ERROR("Failed to load texture: {}", path);
        LOG_ERROR("stb_image error: {}", stbi_failure_reason());
        m_isLoaded = false;
        return;
    }
    
    m_width = static_cast<uint32_t>(width);
    m_height = static_cast<uint32_t>(height);
    
    // Determinar formato según canales
    GLenum internalFormat = GL_RGBA8;
    GLenum dataFormat = GL_RGBA;
    
    if (channels == 4) {
        internalFormat = GL_RGBA8;
        dataFormat = GL_RGBA;
        m_format = TextureFormat::RGBA8;
    } else if (channels == 3) {
        internalFormat = GL_RGB8;
        dataFormat = GL_RGB;
        m_format = TextureFormat::RGB8;
    } else if (channels == 1) {
        internalFormat = GL_R8;
        dataFormat = GL_RED;
        m_format = TextureFormat::RED;
    } else {
        LOG_WARN("Texture {} has {} channels, using RGBA", path, channels);
        internalFormat = GL_RGBA8;
        dataFormat = GL_RGBA;
    }
    
    // Crear textura OpenGL
    glGenTextures(1, &m_rendererID);
    glBindTexture(GL_TEXTURE_2D, m_rendererID);
    
    // Subir datos a GPU
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, m_width, m_height, 
                 0, dataFormat, GL_UNSIGNED_BYTE, data);
    
    // Configurar parámetros
    SetTextureParameters(spec);
    
    // Generar mipmaps si se solicitó
    if (spec.generateMipmaps) {
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    
    glBindTexture(GL_TEXTURE_2D, 0);
    
    // Liberar datos de CPU
    stbi_image_free(data);
    
    m_isLoaded = true;
    
    LOG_DEBUG("Texture2D cargada: {} ({}x{}, {} channels, ID: {})", 
              path, m_width, m_height, channels, m_rendererID);
}

// =============================================================================
// Texture2D - Constructor desde datos raw
// =============================================================================

Texture2D::Texture2D(uint32_t width, uint32_t height, void* data, 
                     const TextureSpecification& spec)
    : m_width(width), m_height(height), m_path(""), m_format(spec.format) {
    
    GLenum internalFormat = TextureFormatToGL(spec.format);
    GLenum dataFormat = internalFormat;
    
    // Crear textura OpenGL
    glGenTextures(1, &m_rendererID);
    glBindTexture(GL_TEXTURE_2D, m_rendererID);
    
    // Subir datos a GPU
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, m_width, m_height, 
                 0, dataFormat, GL_UNSIGNED_BYTE, data);
    
    // Configurar parámetros
    SetTextureParameters(spec);
    
    // Generar mipmaps si se solicitó
    if (spec.generateMipmaps) {
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    
    glBindTexture(GL_TEXTURE_2D, 0);
    
    m_isLoaded = true;
    
    LOG_DEBUG("Texture2D creada desde datos raw ({}x{}, ID: {})", 
              m_width, m_height, m_rendererID);
}

// =============================================================================
// Texture2D - Destructor
// =============================================================================

Texture2D::~Texture2D() {
    if (m_rendererID) {
        glDeleteTextures(1, &m_rendererID);
        LOG_DEBUG("Texture2D destruida (ID: {}, Path: {})", m_rendererID, m_path);
    }
}

// =============================================================================
// Texture2D - Bind/Unbind
// =============================================================================

void Texture2D::Bind(uint32_t slot) const {
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, m_rendererID);
}

void Texture2D::Unbind() const {
    glBindTexture(GL_TEXTURE_2D, 0);
}

// =============================================================================
// Texture2D - Configuration
// =============================================================================

void Texture2D::SetTextureParameters(const TextureSpecification& spec) {
    // Wrapping
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, TextureWrapToGL(spec.wrapS));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, TextureWrapToGL(spec.wrapT));
    
    // Filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, TextureFilterToGL(spec.minFilter));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, TextureFilterToGL(spec.magFilter));
}

// =============================================================================
// Texture2D - Factory Methods
// =============================================================================

Texture2D* Texture2D::Create(const std::string& path, const TextureSpecification& spec) {
    return new Texture2D(path, spec);
}

Texture2D* Texture2D::Create(uint32_t width, uint32_t height, void* data, 
                             const TextureSpecification& spec) {
    return new Texture2D(width, height, data, spec);
}

} // namespace Engine
