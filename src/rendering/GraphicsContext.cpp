/**
 * @file GraphicsContext.cpp
 * @brief Implementación del factory de GraphicsContext
 */

#include "GraphicsContext.h"
#include "RendererAPI.h"
#include "Platform/OpenGL/OpenGLContext.h"
#include "Platform/Vulkan/VulkanContext.h"
#include "core/Logger.h"

namespace Engine {

std::unique_ptr<GraphicsContext> GraphicsContext::Create(void* window) {
    switch (RendererAPI::GetAPI()) {
        case RenderingAPI::OpenGL:
            return std::make_unique<OpenGLContext>(window);

        case RenderingAPI::Vulkan:
            return std::make_unique<VulkanContext>(window);

        case RenderingAPI::None:
        default:
            LOG_CORE_CRITICAL("RendererAPI::None no soportado para GraphicsContext!");
            return nullptr;
    }
}

} // namespace Engine
