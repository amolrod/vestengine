#include "Rendering/Framebuffer.h"

#include <cassert>

#include "Rendering/RendererAPI.h"
#include "Rendering/Platform/OpenGL/OpenGLFramebuffer.h"

namespace Vest {

Ref<Framebuffer> Framebuffer::Create(const FramebufferSpecification& spec) {
    switch (RendererAPI::GetAPI()) {
        case RenderAPI::OpenGL:
            return CreateRef<OpenGLFramebuffer>(spec);
        case RenderAPI::Vulkan:
        case RenderAPI::None:
        default:
            assert(false && "Framebuffer not supported for selected API");
            return nullptr;
    }
}

}  // namespace Vest
