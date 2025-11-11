/**
 * @file Mesh.h
 * @brief Sistema de mallas 3D con factory de primitivas
 * 
 * Encapsula geometría (vértices + índices) y la convierte en buffers GPU.
 * 
 * Arquitectura:
 * - Mesh: Contenedor de geometría con VertexArray + material info
 * - MeshFactory: Factory pattern para generar primitivas comunes
 * - Vertex: Estructura de datos de vértice (posición, normal, UV, color)
 */

#pragma once

#include "Buffer.h"
#include <glm/glm.hpp>
#include <vector>
#include <memory>
#include <string>

namespace Engine {

// =============================================================================
// Vertex Structure
// =============================================================================

/**
 * @struct Vertex
 * @brief Estructura de datos de un vértice completo
 * 
 * Layout estándar para vértices 3D:
 * - Position (Float3): Posición en espacio objeto
 * - Normal (Float3): Normal para iluminación
 * - TexCoord (Float2): Coordenadas de textura UV
 * - Color (Float4): Color del vértice (opcional)
 */
struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoord;
    glm::vec4 color;
    
    Vertex() 
        : position(0.0f), normal(0.0f, 1.0f, 0.0f), 
          texCoord(0.0f), color(1.0f) {}
    
    Vertex(const glm::vec3& pos, const glm::vec3& norm, 
           const glm::vec2& uv, const glm::vec4& col = glm::vec4(1.0f))
        : position(pos), normal(norm), texCoord(uv), color(col) {}
};

// =============================================================================
// Mesh
// =============================================================================

/**
 * @class Mesh
 * @brief Representa una malla 3D (geometría + buffers GPU)
 * 
 * Patrón: Composite (contiene VertexArray con buffers)
 * Responsabilidad: Encapsular geometría y facilitar rendering
 * 
 * Uso:
 *   auto mesh = Mesh::Create(vertices, indices, "CubeMesh");
 *   mesh->Bind();
 *   // ... draw ...
 */
class Mesh {
public:
    /**
     * @brief Destructor
     */
    virtual ~Mesh();
    
    /**
     * @brief Activa el VAO de esta mesh
     */
    virtual void Bind() const;
    
    /**
     * @brief Desactiva el VAO
     */
    virtual void Unbind() const;
    
    /**
     * @brief Obtiene el número de índices (para glDrawElements)
     */
    virtual uint32_t GetIndexCount() const;
    
    /**
     * @brief Obtiene el número de vértices
     */
    virtual uint32_t GetVertexCount() const { return m_vertexCount; }
    
    /**
     * @brief Obtiene el VertexArray
     */
    virtual const VertexArray* GetVertexArray() const { return m_vertexArray; }
    
    /**
     * @brief Obtiene el nombre de la mesh
     */
    virtual const std::string& GetName() const { return m_name; }
    
    /**
     * @brief Factory method
     * @param vertices Vector de vértices
     * @param indices Vector de índices
     * @param name Nombre descriptivo
     */
    static Mesh* Create(const std::vector<Vertex>& vertices, 
                        const std::vector<uint32_t>& indices,
                        const std::string& name = "Mesh");
    
    /**
     * @brief Constructor público
     */
    Mesh(const std::vector<Vertex>& vertices, 
         const std::vector<uint32_t>& indices,
         const std::string& name = "Mesh");

private:
    /**
     * @brief Configura los buffers OpenGL con los datos de vértices
     */
    void SetupBuffers(const std::vector<Vertex>& vertices, 
                      const std::vector<uint32_t>& indices);

private:
    VertexArray* m_vertexArray = nullptr;
    uint32_t m_vertexCount = 0;
    uint32_t m_indexCount = 0;
    std::string m_name;
};

// =============================================================================
// Mesh Factory
// =============================================================================

/**
 * @class MeshFactory
 * @brief Factory para generar primitivas geométricas comunes
 * 
 * Patrón: Factory + Singleton
 * Responsabilidad: Crear meshes predefinidas (Cube, Sphere, Plane...)
 * 
 * Uso:
 *   auto cube = MeshFactory::CreateCube();
 *   auto sphere = MeshFactory::CreateSphere(32, 16);
 */
class MeshFactory {
public:
    /**
     * @brief Crea un cubo unitario centrado en el origen
     * @param size Tamaño del cubo (default: 1.0)
     * @return Mesh* con geometría de cubo
     * 
     * Detalles:
     * - 24 vértices (4 por cara para normales correctas)
     * - 36 índices (12 triángulos, 2 por cara)
     * - Normales hacia afuera
     * - UVs mapeadas correctamente
     */
    static Mesh* CreateCube(float size = 1.0f);
    
    /**
     * @brief Crea un plano horizontal (XZ)
     * @param width Ancho del plano
     * @param depth Profundidad del plano
     * @return Mesh* con geometría de plano
     * 
     * Detalles:
     * - 4 vértices
     * - 6 índices (2 triángulos)
     * - Normal hacia arriba (Y+)
     * - UVs [0,0] a [1,1]
     */
    static Mesh* CreatePlane(float width = 1.0f, float depth = 1.0f);
    
    /**
     * @brief Crea un quad 2D (para UI, billboards, sprites)
     * @param width Ancho
     * @param height Alto
     * @return Mesh* con geometría de quad
     * 
     * Detalles:
     * - 4 vértices
     * - 6 índices (2 triángulos)
     * - Centrado en origen
     * - Normal hacia cámara (Z+)
     */
    static Mesh* CreateQuad(float width = 1.0f, float height = 1.0f);
    
    /**
     * @brief Crea una esfera UV
     * @param radius Radio de la esfera
     * @param sectors Subdivisiones horizontales (longitud)
     * @param stacks Subdivisiones verticales (latitud)
     * @return Mesh* con geometría de esfera
     * 
     * Detalles:
     * - Vértices = (sectors + 1) * (stacks + 1)
     * - Índices = sectors * stacks * 6
     * - Normales suaves
     * - UVs esféricas
     * 
     * Nota: Más sectors/stacks = esfera más suave pero más polígonos
     */
    static Mesh* CreateSphere(float radius = 1.0f, uint32_t sectors = 36, uint32_t stacks = 18);
    
    /**
     * @brief Crea un cilindro
     * @param radius Radio de las bases
     * @param height Altura
     * @param segments Subdivisiones circunferenciales
     * @return Mesh* con geometría de cilindro
     */
    static Mesh* CreateCylinder(float radius = 0.5f, float height = 1.0f, uint32_t segments = 32);
    
    /**
     * @brief Crea un cono
     * @param radius Radio de la base
     * @param height Altura
     * @param segments Subdivisiones circunferenciales
     * @return Mesh* con geometría de cono
     */
    static Mesh* CreateCone(float radius = 0.5f, float height = 1.0f, uint32_t segments = 32);

private:
    MeshFactory() = delete;  // Clase estática, no instanciable
};

} // namespace Engine
