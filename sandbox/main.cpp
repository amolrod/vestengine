/**
 * @file main.cpp
 * @brief Sandbox - Aplicación de prueba del Motor Gráfico 3D
 * 
 * Esta aplicación demuestra las capacidades básicas del motor:
 * - Creación de ventana
 * - Game loop
 * - Sistema de input
 * - Logging
 * - Time management
 */

#include <core/Application.h>
#include <core/Logger.h>
#include <core/Time.h>
#include <input/Input.h>
#include <core/EntryPoint.h>
#include <rendering/Shader.h>
#include <rendering/Buffer.h>
#include <rendering/Mesh.h>
#include <rendering/Texture.h>

// En macOS, OpenGL está disponible directamente
#ifdef PLATFORM_MACOS
    #define GL_SILENCE_DEPRECATION
    #define GLFW_INCLUDE_NONE
    #include <OpenGL/gl3.h>
#else
    #include <glad/glad.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

/**
 * @class SandboxApp
 * @brief Aplicación de demostración
 */
class SandboxApp : public Engine::Application {
public:
    SandboxApp() : Engine::Application(Engine::WindowProps("Sandbox - Motor Gráfico 3D", 1280, 720)) {
        LOG_INFO("Constructor de SandboxApp");
    }
    
    ~SandboxApp() override {
        LOG_INFO("Destructor de SandboxApp");
    }
    
    void OnInit() override {
        LOG_INFO("╔══════════════════════════════════════════════╗");
        LOG_INFO("║           SANDBOX INICIALIZADO               ║");
        LOG_INFO("╚══════════════════════════════════════════════╝");
        LOG_INFO("");
        LOG_INFO("Controles:");
        LOG_INFO("  ESC       - Cerrar aplicación");
        LOG_INFO("  1,2,3,4   - Cambiar color de fondo");
        LOG_INFO("  SPACE     - Color aleatorio");
        LOG_INFO("  F1        - Toggle VSync");
        LOG_INFO("");
        
        // Configuración inicial de OpenGL
        glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
        
        m_backgroundColor = glm::vec3(0.1f, 0.1f, 0.15f);
        
        // =====================================================================
        // FASE 2: Test de Shader + Buffer + Mesh + Texture
        // =====================================================================
        
        // Cargar shader de texturas
        const char* shaderPath = "/Users/angel/Desktop/motor-grafico/shaders/Texture.glsl";
        LOG_INFO("Cargando shader desde: {}", shaderPath);
        m_shader = Engine::Shader::Create(shaderPath);
        
        if (!m_shader) {
            LOG_ERROR("No se pudo cargar el shader!");
            return;
        }
        
        // Crear cubo usando MeshFactory
        m_cubeMesh = Engine::MeshFactory::CreateCube(1.0f);
        
        // Crear textura procedural (checkerboard)
        CreateCheckerboardTexture();
        
        LOG_INFO("✅ Shader, Mesh y Texture creados");
        LOG_INFO("✅ Cubo con {} vértices, {} índices y textura checkerboard", 
                 m_cubeMesh->GetVertexCount(), m_cubeMesh->GetIndexCount());
    }
    
    void OnUpdate(float deltaTime) override {
        // Input: Cerrar con ESC
        if (Engine::Input::IsKeyPressed(Engine::Key::Escape)) {
            LOG_INFO("ESC presionado - Cerrando aplicación");
            Close();
        }
        
        // Input: Cambiar colores de fondo
        if (Engine::Input::IsKeyPressed(Engine::Key::D1)) {
            m_backgroundColor = glm::vec3(0.2f, 0.3f, 0.5f);  // Azul
            LOG_INFO("Color: Azul");
        }
        if (Engine::Input::IsKeyPressed(Engine::Key::D2)) {
            m_backgroundColor = glm::vec3(0.3f, 0.5f, 0.3f);  // Verde
            LOG_INFO("Color: Verde");
        }
        if (Engine::Input::IsKeyPressed(Engine::Key::D3)) {
            m_backgroundColor = glm::vec3(0.5f, 0.3f, 0.3f);  // Rojo
            LOG_INFO("Color: Rojo");
        }
        if (Engine::Input::IsKeyPressed(Engine::Key::D4)) {
            m_backgroundColor = glm::vec3(0.1f, 0.1f, 0.15f); // Gris oscuro
            LOG_INFO("Color: Gris oscuro");
        }
        
        // Input: Color aleatorio con SPACE
        static bool spaceWasPressed = false;
        bool spacePressed = Engine::Input::IsKeyPressed(Engine::Key::Space);
        if (spacePressed && !spaceWasPressed) {
            m_backgroundColor = glm::vec3(
                static_cast<float>(rand()) / RAND_MAX,
                static_cast<float>(rand()) / RAND_MAX,
                static_cast<float>(rand()) / RAND_MAX
            );
            LOG_INFO("Color aleatorio: ({:.2f}, {:.2f}, {:.2f})", 
                m_backgroundColor.r, m_backgroundColor.g, m_backgroundColor.b);
        }
        spaceWasPressed = spacePressed;
        
        // Input: Toggle VSync con F1
        static bool f1WasPressed = false;
        bool f1Pressed = Engine::Input::IsKeyPressed(Engine::Key::F1);
        if (f1Pressed && !f1WasPressed) {
            bool currentVSync = GetWindow().IsVSync();
            GetWindow().SetVSync(!currentVSync);
            LOG_INFO("VSync: {}", !currentVSync ? "ON" : "OFF");
        }
        f1WasPressed = f1Pressed;
        
        // Mostrar posición del mouse (cada 1 segundo)
        static float mouseLogTimer = 0.0f;
        mouseLogTimer += deltaTime;
        if (mouseLogTimer >= 1.0f) {
            if (Engine::Input::IsMouseButtonPressed(Engine::MouseButton::Left)) {
                glm::vec2 mousePos = Engine::Input::GetMousePosition();
                LOG_DEBUG("Mouse: ({:.0f}, {:.0f}) | LMB presionado", mousePos.x, mousePos.y);
            }
            mouseLogTimer = 0.0f;
        }
        
        // Animación de color (pulsación suave)
        m_colorPulse += deltaTime * 0.5f;
        float pulse = (glm::sin(m_colorPulse) + 1.0f) * 0.5f;  // 0.0 a 1.0
        
        glm::vec3 finalColor = m_backgroundColor * (0.8f + pulse * 0.2f);
        glClearColor(finalColor.r, finalColor.g, finalColor.b, 1.0f);
    }
    
    void OnRender() override {
        // Renderizar cubo texturizado
        if (m_shader && m_cubeMesh && m_texture) {
            m_shader->Bind();
            
            // Matrices simples (2D por ahora, sin cámara)
            glm::mat4 viewProjection = glm::mat4(1.0f);  // Identidad
            
            // Transformación: escala más pequeña y rotación
            static float rotation = 0.0f;
            rotation += Engine::Time::DeltaTime() * 45.0f;  // 45 grados/segundo
            
            glm::mat4 transform = glm::mat4(1.0f);
            transform = glm::rotate(transform, glm::radians(rotation), glm::vec3(0, 1, 0));  // Rotar Y
            transform = glm::rotate(transform, glm::radians(rotation * 0.5f), glm::vec3(1, 0, 0));  // Rotar X
            transform = glm::scale(transform, glm::vec3(0.5f));  // Escala 0.5
            
            // Set uniforms
            m_shader->SetUniformMat4("u_ViewProjection", viewProjection);
            m_shader->SetUniformMat4("u_Transform", transform);
            m_shader->SetUniformInt("u_Texture", 0);  // Texture unit 0
            
            // Bind texture
            m_texture->Bind(0);
            
            // Draw con índices (glDrawElements)
            m_cubeMesh->Bind();
            glDrawElements(GL_TRIANGLES, m_cubeMesh->GetIndexCount(), GL_UNSIGNED_INT, nullptr);
            m_cubeMesh->Unbind();
            
            m_texture->Unbind();
            m_shader->Unbind();
        }
    }
    
    void OnShutdown() override {
        LOG_INFO("Limpieza de SandboxApp");
        
        // Liberar recursos gráficos
        if (m_texture) {
            delete m_texture;
            m_texture = nullptr;
        }
        
        if (m_cubeMesh) {
            delete m_cubeMesh;
            m_cubeMesh = nullptr;
        }
    }
    
    void OnWindowResize(uint32_t width, uint32_t height) override {
        LOG_INFO("Ventana redimensionada: {}x{}", width, height);
    }

private:
    /**
     * @brief Crea una textura checkerboard procedural (8x8)
     */
    void CreateCheckerboardTexture() {
        const uint32_t size = 8;  // 8x8 píxeles
        const uint32_t totalPixels = size * size;
        uint32_t* pixels = new uint32_t[totalPixels];
        
        // Generar patrón checkerboard
        for (uint32_t y = 0; y < size; ++y) {
            for (uint32_t x = 0; x < size; ++x) {
                bool isWhite = ((x / 2) + (y / 2)) % 2 == 0;
                uint32_t color = isWhite ? 0xFFFFFFFF : 0xFF404040;  // Blanco o gris oscuro (RGBA)
                pixels[y * size + x] = color;
            }
        }
        
        // Crear textura desde datos
        Engine::TextureSpecification spec;
        spec.width = size;
        spec.height = size;
        spec.format = Engine::TextureFormat::RGBA8;
        spec.wrapS = Engine::TextureWrap::Repeat;
        spec.wrapT = Engine::TextureWrap::Repeat;
        spec.minFilter = Engine::TextureFilter::Nearest;  // Pixelated look
        spec.magFilter = Engine::TextureFilter::Nearest;
        spec.generateMipmaps = false;
        
        m_texture = Engine::Texture2D::Create(size, size, pixels, spec);
        
        delete[] pixels;
        
        LOG_INFO("✅ Textura checkerboard creada ({}x{})", size, size);
    }

private:
    glm::vec3 m_backgroundColor;
    float m_colorPulse = 0.0f;
    
    // FASE 2: Rendering con Mesh + Texture
    std::shared_ptr<Engine::Shader> m_shader;
    Engine::Mesh* m_cubeMesh = nullptr;
    Engine::Texture2D* m_texture = nullptr;
};

/**
 * @brief Función de creación de aplicación
 * Requerida por EntryPoint.h
 */
Engine::Application* CreateApplication() {
    return new SandboxApp();
}
