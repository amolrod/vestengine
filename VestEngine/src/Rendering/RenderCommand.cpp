#include "Rendering/RenderCommand.h"

#include <cassert>

#include "Core/Log.h"
#include "Rendering/Platform/OpenGL/OpenGLRendererAPI.h"
#include "Rendering/Platform/Vulkan/VulkanRendererAPI.h"

namespace Vest {

Scope<RendererAPI> RenderCommand::s_RendererAPI;

void RenderCommand::Init() {
    RendererAPI::SetAPI(ResolveDefaultRenderAPI());
    
    VEST_CORE_INFO("Initializing Render API: {0}", ToString(RendererAPI::GetAPI()));

    switch (RendererAPI::GetAPI()) {
        case RenderAPI::OpenGL:
            s_RendererAPI = CreateScope<OpenGLRendererAPI>();
            break;
        case RenderAPI::Vulkan:
            VEST_CORE_WARN("Vulkan API selected but not fully implemented");
            s_RendererAPI = CreateScope<VulkanRendererAPI>();
            break;
        case RenderAPI::None:
        default:
            VEST_CORE_CRITICAL("Unknown or unsupported RenderAPI");
            assert(false && "Unknown RenderAPI");
    }

    s_RendererAPI->Init();
}

}  // namespace Vest
