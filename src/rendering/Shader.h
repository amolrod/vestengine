/**
 * @file Shader.h
 * @brief Sistema de shaders GLSL (Vertex + Fragment)
 * 
 * Abstracción de shaders de OpenGL con compilación, linking y uniforms.
 * Soporta cargar desde archivos o strings.
 * 
 * Características:
 * - Compilación con error reporting
 * - Uniforms (int, float, vec2/3/4, mat3/4)
 * - Cache de uniform locations
 * - RAII (bind/unbind automático)
 * 
 * Uso:
 *   auto shader = Shader::Create("basic.glsl");
 *   shader->Bind();
 *   shader->SetUniformMat4("u_ViewProjection", camera.GetVP());
 *   shader->SetUniformFloat3("u_Color", {1.0f, 0.0f, 0.0f});
 */

#pragma once

#include <string>
#include <unordered_map>
#include <memory>
#include <glm/glm.hpp>

namespace Engine {

/**
 * @class Shader
 * @brief Programa de shader (Vertex + Fragment)
 * 
 * Patrón: Resource (RAII), Flyweight (compartido vía shared_ptr)
 */
class Shader {
public:
    /**
     * @brief Destructor - limpia recursos OpenGL
     */
    ~Shader();
    
    /**
     * @brief Activa este shader para renderizado
     */
    void Bind() const;
    
    /**
     * @brief Desactiva este shader
     */
    void Unbind() const;
    
    /**
     * @brief Obtiene el ID del programa OpenGL
     */
    uint32_t GetRendererID() const { return m_rendererID; }
    
    /**
     * @brief Obtiene el nombre del shader
     */
    const std::string& GetName() const { return m_name; }
    
    // =========================================================================
    // Uniforms - Setters
    // =========================================================================
    
    void SetUniformInt(const std::string& name, int value);
    void SetUniformIntArray(const std::string& name, int* values, uint32_t count);
    
    void SetUniformFloat(const std::string& name, float value);
    void SetUniformFloat2(const std::string& name, const glm::vec2& value);
    void SetUniformFloat3(const std::string& name, const glm::vec3& value);
    void SetUniformFloat4(const std::string& name, const glm::vec4& value);
    
    void SetUniformMat3(const std::string& name, const glm::mat3& matrix);
    void SetUniformMat4(const std::string& name, const glm::mat4& matrix);
    
    // =========================================================================
    // Factory Methods
    // =========================================================================
    
    /**
     * @brief Crea shader desde archivo (auto-detecta vertex/fragment)
     * @param filepath Ruta al archivo .glsl con ambos shaders
     * @return Shared pointer al shader compilado
     * 
     * Formato esperado del archivo:
     * #shader vertex
     * ... código vertex shader ...
     * #shader fragment
     * ... código fragment shader ...
     */
    static std::shared_ptr<Shader> Create(const std::string& filepath);
    
    /**
     * @brief Crea shader desde código fuente separado
     * @param name Nombre identificador
     * @param vertexSrc Código del vertex shader
     * @param fragmentSrc Código del fragment shader
     */
    static std::shared_ptr<Shader> Create(const std::string& name, 
                                          const std::string& vertexSrc, 
                                          const std::string& fragmentSrc);

public:
    /**
     * @brief Constructor (usar preferiblemente los factory methods Create)
     * @note Público para compatibilidad con std::make_shared
     */
    Shader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);

private:
    
    /**
     * @brief Lee archivo de shader y separa vertex/fragment
     */
    static std::string ReadFile(const std::string& filepath);
    
    /**
     * @brief Parsea archivo con formato #shader vertex/fragment
     */
    static std::unordered_map<uint32_t, std::string> PreProcess(const std::string& source);
    
    /**
     * @brief Compila un shader individual (vertex o fragment)
     */
    uint32_t CompileShader(uint32_t type, const std::string& source);
    
    /**
     * @brief Linkea vertex + fragment en un programa
     */
    void LinkProgram(uint32_t vertexShader, uint32_t fragmentShader);
    
    /**
     * @brief Obtiene location de uniform (con cache)
     */
    int GetUniformLocation(const std::string& name);

private:
    uint32_t m_rendererID = 0;              // ID del programa OpenGL
    std::string m_name;                      // Nombre identificador
    
    // Cache de uniform locations para performance
    mutable std::unordered_map<std::string, int> m_uniformLocationCache;
};

/**
 * @class ShaderLibrary
 * @brief Gestor de shaders (evita duplicados)
 * 
 * Patrón: Repository, Singleton implícito
 */
class ShaderLibrary {
public:
    /**
     * @brief Añade shader a la biblioteca
     */
    void Add(const std::shared_ptr<Shader>& shader);
    
    /**
     * @brief Añade shader con nombre custom
     */
    void Add(const std::string& name, const std::shared_ptr<Shader>& shader);
    
    /**
     * @brief Carga shader desde archivo y lo añade
     */
    std::shared_ptr<Shader> Load(const std::string& filepath);
    
    /**
     * @brief Carga shader con nombre custom
     */
    std::shared_ptr<Shader> Load(const std::string& name, const std::string& filepath);
    
    /**
     * @brief Obtiene shader por nombre
     */
    std::shared_ptr<Shader> Get(const std::string& name);
    
    /**
     * @brief Verifica si existe un shader
     */
    bool Exists(const std::string& name) const;

private:
    std::unordered_map<std::string, std::shared_ptr<Shader>> m_shaders;
};

} // namespace Engine
