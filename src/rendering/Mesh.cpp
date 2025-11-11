/**
 * @file Mesh.cpp
 * @brief Implementación del sistema de mallas 3D
 */

#include "Mesh.h"
#include "../core/Logger.h"

#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace Engine {

// =============================================================================
// Mesh
// =============================================================================

Mesh::Mesh(const std::vector<Vertex>& vertices, 
           const std::vector<uint32_t>& indices,
           const std::string& name)
    : m_vertexCount(static_cast<uint32_t>(vertices.size()))
    , m_indexCount(static_cast<uint32_t>(indices.size()))
    , m_name(name) {
    
    SetupBuffers(vertices, indices);
    
    LOG_DEBUG("Mesh '{}' creada (Vértices: {}, Índices: {})", 
              m_name, m_vertexCount, m_indexCount);
}

Mesh::~Mesh() {
    if (m_vertexArray) {
        delete m_vertexArray;
        m_vertexArray = nullptr;
    }
    
    LOG_DEBUG("Mesh '{}' destruida", m_name);
}

void Mesh::Bind() const {
    if (m_vertexArray) {
        m_vertexArray->Bind();
    }
}

void Mesh::Unbind() const {
    if (m_vertexArray) {
        m_vertexArray->Unbind();
    }
}

uint32_t Mesh::GetIndexCount() const {
    return m_indexCount;
}

void Mesh::SetupBuffers(const std::vector<Vertex>& vertices, 
                        const std::vector<uint32_t>& indices) {
    // Crear VAO
    m_vertexArray = VertexArray::Create();
    
    // Crear VBO con datos de vértices
    auto* vertexBuffer = VertexBuffer::Create(
        vertices.data(), 
        static_cast<uint32_t>(vertices.size() * sizeof(Vertex))
    );
    
    // Definir layout de vértice completo
    vertexBuffer->SetLayout({
        { ShaderDataType::Float3, "a_Position" },
        { ShaderDataType::Float3, "a_Normal" },
        { ShaderDataType::Float2, "a_TexCoord" },
        { ShaderDataType::Float4, "a_Color" }
    });
    
    m_vertexArray->AddVertexBuffer(vertexBuffer);
    
    // Crear IBO (Index Buffer)
    auto* indexBuffer = IndexBuffer::Create(
        indices.data(), 
        static_cast<uint32_t>(indices.size())
    );
    
    m_vertexArray->SetIndexBuffer(indexBuffer);
}

Mesh* Mesh::Create(const std::vector<Vertex>& vertices, 
                   const std::vector<uint32_t>& indices,
                   const std::string& name) {
    return new Mesh(vertices, indices, name);
}

// =============================================================================
// Mesh Factory - Cube
// =============================================================================

Mesh* MeshFactory::CreateCube(float size) {
    float half = size * 0.5f;
    
    std::vector<Vertex> vertices = {
        // Front face (Z+)
        { { -half, -half,  half }, { 0, 0, 1 }, { 0, 0 }, { 1, 1, 1, 1 } },
        { {  half, -half,  half }, { 0, 0, 1 }, { 1, 0 }, { 1, 1, 1, 1 } },
        { {  half,  half,  half }, { 0, 0, 1 }, { 1, 1 }, { 1, 1, 1, 1 } },
        { { -half,  half,  half }, { 0, 0, 1 }, { 0, 1 }, { 1, 1, 1, 1 } },
        
        // Back face (Z-)
        { {  half, -half, -half }, { 0, 0, -1 }, { 0, 0 }, { 1, 1, 1, 1 } },
        { { -half, -half, -half }, { 0, 0, -1 }, { 1, 0 }, { 1, 1, 1, 1 } },
        { { -half,  half, -half }, { 0, 0, -1 }, { 1, 1 }, { 1, 1, 1, 1 } },
        { {  half,  half, -half }, { 0, 0, -1 }, { 0, 1 }, { 1, 1, 1, 1 } },
        
        // Right face (X+)
        { {  half, -half,  half }, { 1, 0, 0 }, { 0, 0 }, { 1, 1, 1, 1 } },
        { {  half, -half, -half }, { 1, 0, 0 }, { 1, 0 }, { 1, 1, 1, 1 } },
        { {  half,  half, -half }, { 1, 0, 0 }, { 1, 1 }, { 1, 1, 1, 1 } },
        { {  half,  half,  half }, { 1, 0, 0 }, { 0, 1 }, { 1, 1, 1, 1 } },
        
        // Left face (X-)
        { { -half, -half, -half }, { -1, 0, 0 }, { 0, 0 }, { 1, 1, 1, 1 } },
        { { -half, -half,  half }, { -1, 0, 0 }, { 1, 0 }, { 1, 1, 1, 1 } },
        { { -half,  half,  half }, { -1, 0, 0 }, { 1, 1 }, { 1, 1, 1, 1 } },
        { { -half,  half, -half }, { -1, 0, 0 }, { 0, 1 }, { 1, 1, 1, 1 } },
        
        // Top face (Y+)
        { { -half,  half,  half }, { 0, 1, 0 }, { 0, 0 }, { 1, 1, 1, 1 } },
        { {  half,  half,  half }, { 0, 1, 0 }, { 1, 0 }, { 1, 1, 1, 1 } },
        { {  half,  half, -half }, { 0, 1, 0 }, { 1, 1 }, { 1, 1, 1, 1 } },
        { { -half,  half, -half }, { 0, 1, 0 }, { 0, 1 }, { 1, 1, 1, 1 } },
        
        // Bottom face (Y-)
        { { -half, -half, -half }, { 0, -1, 0 }, { 0, 0 }, { 1, 1, 1, 1 } },
        { {  half, -half, -half }, { 0, -1, 0 }, { 1, 0 }, { 1, 1, 1, 1 } },
        { {  half, -half,  half }, { 0, -1, 0 }, { 1, 1 }, { 1, 1, 1, 1 } },
        { { -half, -half,  half }, { 0, -1, 0 }, { 0, 1 }, { 1, 1, 1, 1 } }
    };
    
    std::vector<uint32_t> indices = {
        0,  1,  2,   2,  3,  0,   // Front
        4,  5,  6,   6,  7,  4,   // Back
        8,  9,  10,  10, 11, 8,   // Right
        12, 13, 14,  14, 15, 12,  // Left
        16, 17, 18,  18, 19, 16,  // Top
        20, 21, 22,  22, 23, 20   // Bottom
    };
    
    return Mesh::Create(vertices, indices, "Cube");
}

// =============================================================================
// Mesh Factory - Plane
// =============================================================================

Mesh* MeshFactory::CreatePlane(float width, float depth) {
    float halfW = width * 0.5f;
    float halfD = depth * 0.5f;
    
    std::vector<Vertex> vertices = {
        { { -halfW, 0, -halfD }, { 0, 1, 0 }, { 0, 0 }, { 1, 1, 1, 1 } },
        { {  halfW, 0, -halfD }, { 0, 1, 0 }, { 1, 0 }, { 1, 1, 1, 1 } },
        { {  halfW, 0,  halfD }, { 0, 1, 0 }, { 1, 1 }, { 1, 1, 1, 1 } },
        { { -halfW, 0,  halfD }, { 0, 1, 0 }, { 0, 1 }, { 1, 1, 1, 1 } }
    };
    
    std::vector<uint32_t> indices = {
        0, 1, 2,
        2, 3, 0
    };
    
    return Mesh::Create(vertices, indices, "Plane");
}

// =============================================================================
// Mesh Factory - Quad
// =============================================================================

Mesh* MeshFactory::CreateQuad(float width, float height) {
    float halfW = width * 0.5f;
    float halfH = height * 0.5f;
    
    std::vector<Vertex> vertices = {
        { { -halfW, -halfH, 0 }, { 0, 0, 1 }, { 0, 0 }, { 1, 1, 1, 1 } },
        { {  halfW, -halfH, 0 }, { 0, 0, 1 }, { 1, 0 }, { 1, 1, 1, 1 } },
        { {  halfW,  halfH, 0 }, { 0, 0, 1 }, { 1, 1 }, { 1, 1, 1, 1 } },
        { { -halfW,  halfH, 0 }, { 0, 0, 1 }, { 0, 1 }, { 1, 1, 1, 1 } }
    };
    
    std::vector<uint32_t> indices = {
        0, 1, 2,
        2, 3, 0
    };
    
    return Mesh::Create(vertices, indices, "Quad");
}

// =============================================================================
// Mesh Factory - Sphere
// =============================================================================

Mesh* MeshFactory::CreateSphere(float radius, uint32_t sectors, uint32_t stacks) {
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    
    const float sectorStep = 2.0f * static_cast<float>(M_PI) / static_cast<float>(sectors);
    const float stackStep = static_cast<float>(M_PI) / static_cast<float>(stacks);
    
    // Generar vértices
    for (uint32_t i = 0; i <= stacks; ++i) {
        float stackAngle = static_cast<float>(M_PI) / 2.0f - static_cast<float>(i) * stackStep;  // [-π/2, π/2]
        float xy = radius * cosf(stackAngle);
        float z = radius * sinf(stackAngle);
        
        for (uint32_t j = 0; j <= sectors; ++j) {
            float sectorAngle = static_cast<float>(j) * sectorStep;  // [0, 2π]
            
            Vertex vertex;
            vertex.position.x = xy * cosf(sectorAngle);
            vertex.position.y = xy * sinf(sectorAngle);
            vertex.position.z = z;
            
            // Normal (normalizada desde el origen)
            vertex.normal = glm::normalize(vertex.position);
            
            // UV
            vertex.texCoord.x = static_cast<float>(j) / static_cast<float>(sectors);
            vertex.texCoord.y = static_cast<float>(i) / static_cast<float>(stacks);
            
            vertex.color = glm::vec4(1.0f);
            
            vertices.push_back(vertex);
        }
    }
    
    // Generar índices
    for (uint32_t i = 0; i < stacks; ++i) {
        uint32_t k1 = i * (sectors + 1);
        uint32_t k2 = k1 + sectors + 1;
        
        for (uint32_t j = 0; j < sectors; ++j, ++k1, ++k2) {
            if (i != 0) {
                indices.push_back(k1);
                indices.push_back(k2);
                indices.push_back(k1 + 1);
            }
            
            if (i != (stacks - 1)) {
                indices.push_back(k1 + 1);
                indices.push_back(k2);
                indices.push_back(k2 + 1);
            }
        }
    }
    
    return Mesh::Create(vertices, indices, "Sphere");
}

// =============================================================================
// Mesh Factory - Cylinder
// =============================================================================

Mesh* MeshFactory::CreateCylinder(float radius, float height, uint32_t segments) {
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    
    float halfHeight = height * 0.5f;
    float angleStep = 2.0f * static_cast<float>(M_PI) / static_cast<float>(segments);
    
    // Vértices del cuerpo (lados)
    for (uint32_t i = 0; i <= segments; ++i) {
        float angle = static_cast<float>(i) * angleStep;
        float x = radius * cosf(angle);
        float z = radius * sinf(angle);
        
        glm::vec3 normal = glm::normalize(glm::vec3(x, 0, z));
        float u = static_cast<float>(i) / static_cast<float>(segments);
        
        // Top
        vertices.push_back({ { x, halfHeight, z }, normal, { u, 1 }, { 1, 1, 1, 1 } });
        // Bottom
        vertices.push_back({ { x, -halfHeight, z }, normal, { u, 0 }, { 1, 1, 1, 1 } });
    }
    
    // Índices del cuerpo
    for (uint32_t i = 0; i < segments; ++i) {
        uint32_t top1 = i * 2;
        uint32_t bot1 = i * 2 + 1;
        uint32_t top2 = (i + 1) * 2;
        uint32_t bot2 = (i + 1) * 2 + 1;
        
        indices.push_back(top1);
        indices.push_back(bot1);
        indices.push_back(top2);
        
        indices.push_back(top2);
        indices.push_back(bot1);
        indices.push_back(bot2);
    }
    
    // Centro de las tapas
    uint32_t topCenter = static_cast<uint32_t>(vertices.size());
    vertices.push_back({ { 0, halfHeight, 0 }, { 0, 1, 0 }, { 0.5f, 0.5f }, { 1, 1, 1, 1 } });
    
    uint32_t bottomCenter = static_cast<uint32_t>(vertices.size());
    vertices.push_back({ { 0, -halfHeight, 0 }, { 0, -1, 0 }, { 0.5f, 0.5f }, { 1, 1, 1, 1 } });
    
    // Tapas (caps)
    for (uint32_t i = 0; i < segments; ++i) {
        // Top cap
        indices.push_back(topCenter);
        indices.push_back(i * 2);
        indices.push_back((i + 1) * 2);
        
        // Bottom cap
        indices.push_back(bottomCenter);
        indices.push_back((i + 1) * 2 + 1);
        indices.push_back(i * 2 + 1);
    }
    
    return Mesh::Create(vertices, indices, "Cylinder");
}

// =============================================================================
// Mesh Factory - Cone
// =============================================================================

Mesh* MeshFactory::CreateCone(float radius, float height, uint32_t segments) {
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    
    float angleStep = 2.0f * static_cast<float>(M_PI) / static_cast<float>(segments);
    
    // Vértice superior (apex)
    uint32_t apex = 0;
    vertices.push_back({ { 0, height, 0 }, { 0, 1, 0 }, { 0.5f, 1 }, { 1, 1, 1, 1 } });
    
    // Vértices de la base
    for (uint32_t i = 0; i <= segments; ++i) {
        float angle = static_cast<float>(i) * angleStep;
        float x = radius * cosf(angle);
        float z = radius * sinf(angle);
        
        float u = static_cast<float>(i) / static_cast<float>(segments);
        
        vertices.push_back({ { x, 0, z }, { 0, -1, 0 }, { u, 0 }, { 1, 1, 1, 1 } });
    }
    
    // Índices de los lados
    for (uint32_t i = 1; i <= segments; ++i) {
        indices.push_back(apex);
        indices.push_back(i);
        indices.push_back(i + 1);
    }
    
    // Centro de la base
    uint32_t baseCenter = static_cast<uint32_t>(vertices.size());
    vertices.push_back({ { 0, 0, 0 }, { 0, -1, 0 }, { 0.5f, 0.5f }, { 1, 1, 1, 1 } });
    
    // Índices de la base
    for (uint32_t i = 1; i <= segments; ++i) {
        indices.push_back(baseCenter);
        indices.push_back(i + 1);
        indices.push_back(i);
    }
    
    return Mesh::Create(vertices, indices, "Cone");
}

} // namespace Engine
