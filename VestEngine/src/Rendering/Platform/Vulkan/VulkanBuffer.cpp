#include "Rendering/Platform/Vulkan/VulkanBuffer.h"

#include <cassert>

namespace Vest {

VulkanVertexBuffer::VulkanVertexBuffer(uint32_t) {
    assert(false && "Vulkan vertex buffer not implemented");
}

VulkanVertexBuffer::VulkanVertexBuffer(float*, uint32_t) {
    assert(false && "Vulkan vertex buffer not implemented");
}

VulkanIndexBuffer::VulkanIndexBuffer(uint32_t*, uint32_t count) : m_Count(count) {
    assert(false && "Vulkan index buffer not implemented");
}

}  // namespace Vest
