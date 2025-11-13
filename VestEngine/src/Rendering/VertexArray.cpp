#include "Rendering/VertexArray.h"

#include <cassert>

#include "Rendering/Platform/OpenGL/OpenGLVertexArray.h"

namespace Vest {

Ref<VertexArray> VertexArray::Create() {
    switch (RendererAPI::GetAPI()) {
        case RenderAPI::OpenGL:
            return CreateRef<OpenGLVertexArray>();
        case RenderAPI::Vulkan:
        case RenderAPI::None:
        default:
            assert(false && "VertexArray not supported for selected API");
            return nullptr;
    }
}

}  // namespace Vest
