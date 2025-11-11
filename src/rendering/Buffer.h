/**
 * @file Buffer.h
 * @brief Abstracciones de buffers OpenGL (Vertex, Index, Uniform)
 * 
 * Clases RAII para gestionar buffers de OpenGL de forma segura y tipada.
 * 
 * Jerarquía:
 * - VertexBuffer: Buffer de vértices (posiciones, colores, UVs, normals, etc.)
 * - IndexBuffer: Buffer de índices (para indexed drawing)
 * - BufferLayout: Describe el layout de un VertexBuffer
 * - VertexArray: VAO que combina VertexBuffers + IndexBuffer
 */

#pragma once

#include <cstdint>
#include <vector>
#include <string>
#include <initializer_list>

namespace Engine {

// =============================================================================
// Shader Data Types
// =============================================================================

/**
 * @enum ShaderDataType
 * @brief Tipos de datos en shaders GLSL
 */
enum class ShaderDataType {
    None = 0,
    Float, Float2, Float3, Float4,
    Mat3, Mat4,
    Int, Int2, Int3, Int4,
    Bool
};

/**
 * @brief Obtiene el tamaño en bytes de un ShaderDataType
 */
uint32_t ShaderDataTypeSize(ShaderDataType type);

/**
 * @brief Obtiene el número de componentes de un ShaderDataType
 * Ej: Float3 → 3, Mat4 → 16
 */
uint32_t GetComponentCount(ShaderDataType type);

// =============================================================================
// Buffer Element (atributo de vértice)
// =============================================================================

/**
 * @struct BufferElement
 * @brief Describe un atributo de vértice (posición, color, UV, etc.)
 * 
 * Ejemplo:
 *   BufferElement("a_Position", ShaderDataType::Float3)
 *   BufferElement("a_Color", ShaderDataType::Float4)
 */
struct BufferElement {
    std::string name;
    ShaderDataType type;
    uint32_t size;
    uint32_t offset;
    bool normalized;
    
    BufferElement() = default;
    
    BufferElement(ShaderDataType type, const std::string& name, bool normalized = false)
        : name(name), type(type), size(ShaderDataTypeSize(type)), 
          offset(0), normalized(normalized) {}
    
    uint32_t GetComponentCount() const {
        return Engine::GetComponentCount(type);
    }
};

// =============================================================================
// Buffer Layout
// =============================================================================

/**
 * @class BufferLayout
 * @brief Define el layout (estructura) de un VertexBuffer
 * 
 * Especifica qué atributos tiene cada vértice y cómo están organizados.
 * 
 * Ejemplo:
 *   BufferLayout layout = {
 *       { ShaderDataType::Float3, "a_Position" },
 *       { ShaderDataType::Float4, "a_Color" },
 *       { ShaderDataType::Float2, "a_TexCoord" }
 *   };
 */
class BufferLayout {
public:
    BufferLayout() = default;
    
    BufferLayout(const std::initializer_list<BufferElement>& elements)
        : m_elements(elements) {
        CalculateOffsetsAndStride();
    }
    
    uint32_t GetStride() const { return m_stride; }
    const std::vector<BufferElement>& GetElements() const { return m_elements; }
    
    std::vector<BufferElement>::iterator begin() { return m_elements.begin(); }
    std::vector<BufferElement>::iterator end() { return m_elements.end(); }
    std::vector<BufferElement>::const_iterator begin() const { return m_elements.begin(); }
    std::vector<BufferElement>::const_iterator end() const { return m_elements.end(); }

private:
    void CalculateOffsetsAndStride();

private:
    std::vector<BufferElement> m_elements;
    uint32_t m_stride = 0;
};

// =============================================================================
// Vertex Buffer
// =============================================================================

/**
 * @class VertexBuffer
 * @brief Buffer de vértices (VBO)
 * 
 * Patrón: RAII, Resource Handle
 * Responsabilidad: Gestionar un buffer de vértices en GPU
 * 
 * Uso:
 *   auto vb = VertexBuffer::Create(vertices, sizeof(vertices));
 *   vb->SetLayout({
 *       { ShaderDataType::Float3, "a_Position" },
 *       { ShaderDataType::Float2, "a_TexCoord" }
 *   });
 */
class VertexBuffer {
public:
    /**
     * @brief Destructor - limpia recursos OpenGL
     */
    virtual ~VertexBuffer();
    
    /**
     * @brief Activa este buffer
     */
    virtual void Bind() const;
    
    /**
     * @brief Desactiva este buffer
     */
    virtual void Unbind() const;
    
    /**
     * @brief Establece el layout de vértices
     */
    virtual void SetLayout(const BufferLayout& layout) { m_layout = layout; }
    
    /**
     * @brief Obtiene el layout actual
     */
    virtual const BufferLayout& GetLayout() const { return m_layout; }
    
    /**
     * @brief Factory method
     * @param vertices Puntero a datos de vértices
     * @param size Tamaño en bytes
     */
    static VertexBuffer* Create(const void* vertices, uint32_t size);
    
    /**
     * @brief Constructor público para make_shared
     */
    VertexBuffer(const void* vertices, uint32_t size);

private:
    uint32_t m_rendererID = 0;
    BufferLayout m_layout;
};

// =============================================================================
// Index Buffer
// =============================================================================

/**
 * @class IndexBuffer
 * @brief Buffer de índices (IBO/EBO)
 * 
 * Patrón: RAII, Resource Handle
 * Responsabilidad: Gestionar índices para indexed drawing
 * 
 * Uso:
 *   uint32_t indices[] = { 0, 1, 2, 2, 3, 0 };
 *   auto ib = IndexBuffer::Create(indices, 6);
 */
class IndexBuffer {
public:
    /**
     * @brief Destructor - limpia recursos OpenGL
     */
    virtual ~IndexBuffer();
    
    /**
     * @brief Activa este buffer
     */
    virtual void Bind() const;
    
    /**
     * @brief Desactiva este buffer
     */
    virtual void Unbind() const;
    
    /**
     * @brief Obtiene el número de índices
     */
    virtual uint32_t GetCount() const { return m_count; }
    
    /**
     * @brief Factory method
     * @param indices Puntero a array de índices
     * @param count Número de índices
     */
    static IndexBuffer* Create(const uint32_t* indices, uint32_t count);
    
    /**
     * @brief Constructor público para make_shared
     */
    IndexBuffer(const uint32_t* indices, uint32_t count);

private:
    uint32_t m_rendererID = 0;
    uint32_t m_count = 0;
};

// =============================================================================
// Vertex Array
// =============================================================================

/**
 * @class VertexArray
 * @brief Vertex Array Object (VAO)
 * 
 * Patrón: Composite (contiene VertexBuffers + IndexBuffer)
 * Responsabilidad: Encapsula VAO + sus VBOs + IBO
 * 
 * Uso:
 *   auto vao = VertexArray::Create();
 *   vao->AddVertexBuffer(vertexBuffer);
 *   vao->SetIndexBuffer(indexBuffer);
 *   vao->Bind();
 *   // ... draw ...
 */
class VertexArray {
public:
    /**
     * @brief Destructor - limpia recursos OpenGL
     */
    virtual ~VertexArray();
    
    /**
     * @brief Activa este VAO
     */
    virtual void Bind() const;
    
    /**
     * @brief Desactiva este VAO
     */
    virtual void Unbind() const;
    
    /**
     * @brief Añade un VertexBuffer al VAO
     * Configura automáticamente los atributos según el layout
     */
    virtual void AddVertexBuffer(VertexBuffer* vertexBuffer);
    
    /**
     * @brief Establece el IndexBuffer
     */
    virtual void SetIndexBuffer(IndexBuffer* indexBuffer);
    
    /**
     * @brief Obtiene los VertexBuffers
     */
    virtual const std::vector<VertexBuffer*>& GetVertexBuffers() const { return m_vertexBuffers; }
    
    /**
     * @brief Obtiene el IndexBuffer
     */
    virtual const IndexBuffer* GetIndexBuffer() const { return m_indexBuffer; }
    
    /**
     * @brief Factory method
     */
    static VertexArray* Create();
    
    /**
     * @brief Constructor público
     */
    VertexArray();

private:
    uint32_t m_rendererID = 0;
    uint32_t m_vertexBufferIndex = 0;
    std::vector<VertexBuffer*> m_vertexBuffers;
    IndexBuffer* m_indexBuffer = nullptr;
};

} // namespace Engine
