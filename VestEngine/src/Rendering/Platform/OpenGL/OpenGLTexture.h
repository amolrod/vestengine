#pragma once

#include <string>
#include <cstdint>

#include <string>

#include <glad/glad.h>

#include "Rendering/Texture.h"

namespace Vest {

class OpenGLTexture2D : public Texture2D {
public:
    OpenGLTexture2D(uint32_t width, uint32_t height);
    explicit OpenGLTexture2D(const std::string& path);
    ~OpenGLTexture2D() override;

    uint32_t GetWidth() const override { return m_Width; }
    uint32_t GetHeight() const override { return m_Height; }

    void Bind(uint32_t slot = 0) const override;

private:
    void Allocate(const void* data = nullptr);
    void InitializeParameters() const;

    std::string m_Path;
    uint32_t m_Width = 0;
    uint32_t m_Height = 0;
    GLenum m_InternalFormat = 0;
    GLenum m_DataFormat = 0;
    uint32_t m_RendererID = 0;
    bool m_IsLoaded = false;
};

}  // namespace Vest
