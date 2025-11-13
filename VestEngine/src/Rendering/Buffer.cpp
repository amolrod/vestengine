#include "Rendering/Buffer.h"

#include <cassert>

#include "Rendering/Platform/OpenGL/OpenGLBuffer.h"
#include "Rendering/Platform/Vulkan/VulkanBuffer.h"

namespace Vest {

uint32_t BufferElement::GetComponentCount() const {
    switch (type) {
        case ShaderDataType::Float:
        case ShaderDataType::Int:
            return 1;
        case ShaderDataType::Float2:
        case ShaderDataType::Int2:
            return 2;
        case ShaderDataType::Float3:
        case ShaderDataType::Int3:
            return 3;
        case ShaderDataType::Float4:
        case ShaderDataType::Int4:
            return 4;
        case ShaderDataType::Mat3:
            return 3 * 3;
        case ShaderDataType::Mat4:
            return 4 * 4;
        case ShaderDataType::Bool:
            return 1;
        default:
            return 0;
    }
}

BufferLayout::BufferLayout(std::initializer_list<BufferElement> elements) : m_Elements(elements) {
    CalculateOffsetsAndStride();
}

void BufferLayout::CalculateOffsetsAndStride() {
    size_t offset = 0;
    m_Stride = 0;
    for (auto& element : m_Elements) {
        element.offset = offset;
        offset += element.size;
        m_Stride += element.size;
    }
}

Ref<VertexBuffer> VertexBuffer::Create(uint32_t size) {
    switch (RendererAPI::GetAPI()) {
        case RenderAPI::OpenGL:
            return CreateRef<OpenGLVertexBuffer>(size);
        case RenderAPI::Vulkan:
            return CreateRef<VulkanVertexBuffer>(size);
        case RenderAPI::None:
        default:
            assert(false && "Unknown RenderAPI");
            return nullptr;
    }
}

Ref<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size) {
    switch (RendererAPI::GetAPI()) {
        case RenderAPI::OpenGL:
            return CreateRef<OpenGLVertexBuffer>(vertices, size);
        case RenderAPI::Vulkan:
            return CreateRef<VulkanVertexBuffer>(vertices, size);
        case RenderAPI::None:
        default:
            assert(false && "Unknown RenderAPI");
            return nullptr;
    }
}

Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t count) {
    switch (RendererAPI::GetAPI()) {
        case RenderAPI::OpenGL:
            return CreateRef<OpenGLIndexBuffer>(indices, count);
        case RenderAPI::Vulkan:
            return CreateRef<VulkanIndexBuffer>(indices, count);
        case RenderAPI::None:
        default:
            assert(false && "Unknown RenderAPI");
            return nullptr;
    }
}

}  // namespace Vest
