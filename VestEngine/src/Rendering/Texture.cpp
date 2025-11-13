#include "Rendering/Texture.h"

#include <cassert>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "Rendering/RendererAPI.h"
#include "Rendering/Platform/OpenGL/OpenGLTexture.h"

namespace Vest {

Ref<Texture2D> Texture2D::Create(uint32_t width, uint32_t height) {
    switch (RendererAPI::GetAPI()) {
        case RenderAPI::OpenGL:
            return CreateRef<OpenGLTexture2D>(width, height);
        case RenderAPI::Vulkan:
        case RenderAPI::None:
        default:
            assert(false && "Texture2D not supported for selected API");
            return nullptr;
    }
}

Ref<Texture2D> Texture2D::Create(const std::string& path) {
    switch (RendererAPI::GetAPI()) {
        case RenderAPI::OpenGL:
            return CreateRef<OpenGLTexture2D>(path);
        case RenderAPI::Vulkan:
        case RenderAPI::None:
        default:
            assert(false && "Texture2D not supported for selected API");
            return nullptr;
    }
}

}  // namespace Vest
