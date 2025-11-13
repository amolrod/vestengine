#include "Rendering/Platform/Vulkan/VulkanRendererAPI.h"

#include <cassert>

namespace Vest {

void VulkanRendererAPI::Init() {
    assert(false && "Vulkan renderer API not implemented");
}

void VulkanRendererAPI::SetViewport(uint32_t, uint32_t, uint32_t, uint32_t) {
    assert(false && "Vulkan renderer API not implemented");
}

void VulkanRendererAPI::SetClearColor(const glm::vec4&) {
    assert(false && "Vulkan renderer API not implemented");
}

void VulkanRendererAPI::Clear() {
    assert(false && "Vulkan renderer API not implemented");
}

void VulkanRendererAPI::DrawIndexed(const Ref<VertexArray>&, uint32_t) {
    assert(false && "Vulkan renderer API not implemented");
}

}  // namespace Vest
