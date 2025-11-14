#include "Rendering/Platform/Vulkan/VulkanRendererAPI.h"

#include "Core/Log.h"

namespace Vest {

void VulkanRendererAPI::Init() {
    VEST_CORE_ERROR("Vulkan renderer API not implemented - Init() called");
    assert(false && "Vulkan renderer API not implemented");
}

void VulkanRendererAPI::SetViewport(uint32_t, uint32_t, uint32_t, uint32_t) {
    VEST_CORE_ERROR("Vulkan renderer API not implemented - SetViewport() called");
    assert(false && "Vulkan renderer API not implemented");
}

void VulkanRendererAPI::SetClearColor(const glm::vec4&) {
    VEST_CORE_ERROR("Vulkan renderer API not implemented - SetClearColor() called");
    assert(false && "Vulkan renderer API not implemented");
}

void VulkanRendererAPI::Clear() {
    VEST_CORE_ERROR("Vulkan renderer API not implemented - Clear() called");
    assert(false && "Vulkan renderer API not implemented");
}

void VulkanRendererAPI::DrawIndexed(const Ref<VertexArray>&, uint32_t) {
    VEST_CORE_ERROR("Vulkan renderer API not implemented - DrawIndexed() called");
    assert(false && "Vulkan renderer API not implemented");
}

}  // namespace Vest
