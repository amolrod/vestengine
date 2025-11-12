/**
 * @file Buffer.cpp
 * @brief Implementación de abstracciones de buffers OpenGL
 */

#include "Buffer.h"
#include "../core/Logger.h"

#ifdef PLATFORM_MACOS
    #define GL_SILENCE_DEPRECATION
    #include <OpenGL/gl3.h>
#else
    #include <glad/gl.h>
#endif

namespace Engine {

// =============================================================================
// Shader Data Type Utilities
// =============================================================================

uint32_t ShaderDataTypeSize(ShaderDataType type) {
    switch (type) {
        case ShaderDataType::Float:    return 4;
        case ShaderDataType::Float2:   return 4 * 2;
        case ShaderDataType::Float3:   return 4 * 3;
        case ShaderDataType::Float4:   return 4 * 4;
        case ShaderDataType::Mat3:     return 4 * 3 * 3;
        case ShaderDataType::Mat4:     return 4 * 4 * 4;
        case ShaderDataType::Int:      return 4;
        case ShaderDataType::Int2:     return 4 * 2;
        case ShaderDataType::Int3:     return 4 * 3;
        case ShaderDataType::Int4:     return 4 * 4;
        case ShaderDataType::Bool:     return 1;
        default: break;
    }
    
    LOG_ERROR("Unknown ShaderDataType!");
    return 0;
}

uint32_t GetComponentCount(ShaderDataType type) {
    switch (type) {
        case ShaderDataType::Float:    return 1;
        case ShaderDataType::Float2:   return 2;
        case ShaderDataType::Float3:   return 3;
        case ShaderDataType::Float4:   return 4;
        case ShaderDataType::Mat3:     return 3 * 3;
        case ShaderDataType::Mat4:     return 4 * 4;
        case ShaderDataType::Int:      return 1;
        case ShaderDataType::Int2:     return 2;
        case ShaderDataType::Int3:     return 3;
        case ShaderDataType::Int4:     return 4;
        case ShaderDataType::Bool:     return 1;
        default: break;
    }
    
    LOG_ERROR("Unknown ShaderDataType!");
    return 0;
}

/**
 * @brief Convierte ShaderDataType a GLenum de OpenGL
 */
static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type) {
    switch (type) {
        case ShaderDataType::Float:
        case ShaderDataType::Float2:
        case ShaderDataType::Float3:
        case ShaderDataType::Float4:
        case ShaderDataType::Mat3:
        case ShaderDataType::Mat4:
            return GL_FLOAT;
        case ShaderDataType::Int:
        case ShaderDataType::Int2:
        case ShaderDataType::Int3:
        case ShaderDataType::Int4:
            return GL_INT;
        case ShaderDataType::Bool:
            return GL_BOOL;
        default:
            break;
    }
    
    LOG_ERROR("Unknown ShaderDataType!");
    return 0;
}

// =============================================================================
// Buffer Layout
// =============================================================================

void BufferLayout::CalculateOffsetsAndStride() {
    uint32_t offset = 0;
    m_stride = 0;
    
    for (auto& element : m_elements) {
        element.offset = offset;
        offset += element.size;
        m_stride += element.size;
    }
}

// =============================================================================
// Vertex Buffer
// =============================================================================

VertexBuffer::VertexBuffer(const void* vertices, uint32_t size) {
    glGenBuffers(1, &m_rendererID);
    glBindBuffer(GL_ARRAY_BUFFER, m_rendererID);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
    
    LOG_DEBUG("VertexBuffer creado (ID: {})", m_rendererID);
}

VertexBuffer::~VertexBuffer() {
    glDeleteBuffers(1, &m_rendererID);
    LOG_DEBUG("VertexBuffer destruido (ID: {})", m_rendererID);
}

void VertexBuffer::Bind() const {
    glBindBuffer(GL_ARRAY_BUFFER, m_rendererID);
}

void VertexBuffer::Unbind() const {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

VertexBuffer* VertexBuffer::Create(const void* vertices, uint32_t size) {
    return new VertexBuffer(vertices, size);
}

// =============================================================================
// Index Buffer
// =============================================================================

IndexBuffer::IndexBuffer(const uint32_t* indices, uint32_t count)
    : m_count(count) {
    glGenBuffers(1, &m_rendererID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
    
    LOG_DEBUG("IndexBuffer creado (ID: {}, Count: {})", m_rendererID, count);
}

IndexBuffer::~IndexBuffer() {
    glDeleteBuffers(1, &m_rendererID);
    LOG_DEBUG("IndexBuffer destruido (ID: {})", m_rendererID);
}

void IndexBuffer::Bind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererID);
}

void IndexBuffer::Unbind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

IndexBuffer* IndexBuffer::Create(const uint32_t* indices, uint32_t count) {
    return new IndexBuffer(indices, count);
}

// =============================================================================
// Vertex Array
// =============================================================================

VertexArray::VertexArray() {
    glGenVertexArrays(1, &m_rendererID);
    LOG_DEBUG("VertexArray creado (ID: {})", m_rendererID);
}

VertexArray::~VertexArray() {
    glDeleteVertexArrays(1, &m_rendererID);
    
    // Limpiar buffers asociados
    for (auto* vb : m_vertexBuffers) {
        delete vb;
    }
    
    if (m_indexBuffer) {
        delete m_indexBuffer;
    }
    
    LOG_DEBUG("VertexArray destruido (ID: {})", m_rendererID);
}

void VertexArray::Bind() const {
    glBindVertexArray(m_rendererID);
}

void VertexArray::Unbind() const {
    glBindVertexArray(0);
}

void VertexArray::AddVertexBuffer(VertexBuffer* vertexBuffer) {
    if (vertexBuffer->GetLayout().GetElements().empty()) {
        LOG_ERROR("VertexBuffer no tiene layout!");
        return;
    }
    
    glBindVertexArray(m_rendererID);
    vertexBuffer->Bind();
    
    const auto& layout = vertexBuffer->GetLayout();
    for (const auto& element : layout) {
        glEnableVertexAttribArray(m_vertexBufferIndex);
        glVertexAttribPointer(
            m_vertexBufferIndex,
            element.GetComponentCount(),
            ShaderDataTypeToOpenGLBaseType(element.type),
            element.normalized ? GL_TRUE : GL_FALSE,
            layout.GetStride(),
            reinterpret_cast<const void*>(static_cast<uintptr_t>(element.offset))
        );
        m_vertexBufferIndex++;
    }
    
    m_vertexBuffers.push_back(vertexBuffer);
    
    LOG_DEBUG("VertexBuffer añadido al VAO {} (Stride: {}, Atributos: {})", 
              m_rendererID, layout.GetStride(), layout.GetElements().size());
}

void VertexArray::SetIndexBuffer(IndexBuffer* indexBuffer) {
    glBindVertexArray(m_rendererID);
    indexBuffer->Bind();
    
    m_indexBuffer = indexBuffer;
    
    LOG_DEBUG("IndexBuffer añadido al VAO {} (Count: {})", 
              m_rendererID, indexBuffer->GetCount());
}

VertexArray* VertexArray::Create() {
    return new VertexArray();
}

} // namespace Engine
