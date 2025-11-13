#include "Rendering/RenderCommand.h"

#include <cassert>

#include "Rendering/Platform/OpenGL/OpenGLRendererAPI.h"
#include "Rendering/Platform/Vulkan/VulkanRendererAPI.h"

namespace Vest {

Scope<RendererAPI> RenderCommand::s_RendererAPI;

void RenderCommand::Init() {
    RendererAPI::SetAPI(ResolveDefaultRenderAPI());

    switch (RendererAPI::GetAPI()) {
        case RenderAPI::OpenGL:
            s_RendererAPI = CreateScope<OpenGLRendererAPI>();
            break;
        case RenderAPI::Vulkan:
            s_RendererAPI = CreateScope<VulkanRendererAPI>();
            break;
        case RenderAPI::None:
        default:
            assert(false && "Unknown RenderAPI");
    }

    s_RendererAPI->Init();
}

}  // namespace Vest
