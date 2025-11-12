/**
 * @file RendererAPI.cpp
 * @brief Implementación del factory de RendererAPI
 */

#include "RendererAPI.h"
#include "Platform/OpenGL/OpenGLRendererAPI.h"
#include "Platform/Vulkan/VulkanRendererAPI.h"
#include "core/Logger.h"

namespace Engine {

// Por defecto, usar OpenGL
#if defined(VEST_OPENGL)
    RenderingAPI RendererAPI::s_API = RenderingAPI::OpenGL;
#elif defined(VEST_VULKAN)
    RenderingAPI RendererAPI::s_API = RenderingAPI::Vulkan;
#else
    RenderingAPI RendererAPI::s_API = RenderingAPI::OpenGL;  // Fallback
#endif

std::unique_ptr<RendererAPI> RendererAPI::Create() {
    switch (s_API) {
        case RenderingAPI::OpenGL:
            LOG_CORE_INFO("Creando RendererAPI: OpenGL");
            return std::make_unique<OpenGLRendererAPI>();

        case RenderingAPI::Vulkan:
            LOG_CORE_WARN("Creando RendererAPI: Vulkan (stub - no implementado)");
            return std::make_unique<VulkanRendererAPI>();

        case RenderingAPI::None:
        default:
            LOG_CORE_CRITICAL("RendererAPI::None no es soportado!");
            return nullptr;
    }
}

} // namespace Engine
