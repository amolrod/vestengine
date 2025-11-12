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

#include <cstdio>
#include <core/Application.h>
#include <core/Logger.h>
#include <core/Time.h>
#include <input/Input.h>
#include <core/EntryPoint.h>
#include <rendering/Shader.h>
#include <rendering/Buffer.h>
#include <rendering/Mesh.h>
#include <rendering/Texture.h>
#include <rendering/Camera.h>
#include <rendering/Renderer.h>
#include <rendering/RenderCommand.h>

// En macOS, OpenGL está disponible directamente
#ifdef PLATFORM_MACOS
    #define GL_SILENCE_DEPRECATION
    #define GLFW_INCLUDE_NONE
    #include <OpenGL/gl3.h>
#else
    #include <glad/gl.h>
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
        LOG_INFO("========================================");
        LOG_INFO("Constructor de SandboxApp - INICIO");
        LOG_INFO("========================================");
    }
    
    ~SandboxApp() override {
        LOG_INFO("========================================");
        LOG_INFO("Destructor de SandboxApp");
        LOG_INFO("========================================");
    }
    
    void OnInit() override {
        LOG_INFO("╔══════════════════════════════════════════════╗");
        LOG_INFO("║           SANDBOX INICIALIZADO               ║");
        LOG_INFO("╚══════════════════════════════════════════════╝");
        LOG_INFO("");
        LOG_INFO("Controles:");
        LOG_INFO("  ESC       - Cerrar aplicación");
        LOG_INFO("  WASD      - Mover cámara (Forward/Left/Back/Right)");
        LOG_INFO("  Q/E       - Mover cámara (Down/Up)");
        LOG_INFO("  Mouse     - Rotar cámara (mantén clic izquierdo)");
        LOG_INFO("  1,2,3,4   - Cambiar color de fondo");
        LOG_INFO("  SPACE     - Color aleatorio");
        LOG_INFO("  F1        - Toggle VSync");
        LOG_INFO("");
        
        // =====================================================================
        // FASE 2: Inicializar Renderer API
        // =====================================================================
        LOG_INFO("[OnInit] Paso 1: Inicializando Renderer...");
        Engine::Renderer::Init();
        LOG_INFO("[OnInit] Paso 1: Renderer inicializado OK");
        
        // Configurar color de fondo inicial
        LOG_INFO("[OnInit] Paso 2: Configurando color de fondo...");
        m_backgroundColor = glm::vec3(0.1f, 0.1f, 0.15f);
        Engine::RenderCommand::SetClearColor(glm::vec4(m_backgroundColor, 1.0f));
        LOG_INFO("[OnInit] Paso 2: Color de fondo configurado OK");
        
        // =====================================================================
        // Crear recursos: Shader + Mesh + Texture + Camera
        // =====================================================================
        
        // Cargar shader de texturas (ruta relativa desde el ejecutable)
        LOG_INFO("[OnInit] Paso 3: Cargando shader...");
        const char* shaderPath = "shaders/Texture.glsl";
        LOG_INFO("Cargando shader desde: {}", shaderPath);
        m_shader = Engine::Shader::Create(shaderPath);
        LOG_INFO("[OnInit] Paso 3: Shader::Create completado");
        
        if (!m_shader) {
            LOG_ERROR("No se pudo cargar el shader!");
            return;
        }
        LOG_INFO("[OnInit] Paso 3: Shader cargado OK");
        
        // Crear cubo usando MeshFactory
        LOG_INFO("[OnInit] Paso 4: Creando cubo...");
        m_cubeMesh = std::shared_ptr<Engine::Mesh>(Engine::MeshFactory::CreateCube(1.0f));
        LOG_INFO("[OnInit] Paso 4: Cubo creado OK");
        
        // Crear textura procedural (checkerboard)
        LOG_INFO("[OnInit] Paso 5: Creando textura...");
        CreateCheckerboardTexture();
        LOG_INFO("[OnInit] Paso 5: Textura creada OK");
        
        // Crear cámara 3D
        LOG_INFO("[OnInit] Paso 6: Creando cámara...");
        Engine::PerspectiveCameraConfig cameraConfig;
        cameraConfig.position = glm::vec3(0.0f, 0.0f, 5.0f);
        cameraConfig.fov = 45.0f;
        cameraConfig.aspectRatio = 1280.0f / 720.0f;
        cameraConfig.movementSpeed = 3.0f;
        cameraConfig.mouseSensitivity = 0.15f;
        
        m_camera = new Engine::PerspectiveCamera(cameraConfig);
        
        LOG_INFO("✅ Renderer API inicializado");
        LOG_INFO("✅ Shader, Mesh, Texture y Camera creados");
        LOG_INFO("✅ Sistema de rendering completo inicializado");
    }
    
    void OnUpdate(float deltaTime) override {
        // =====================================================================
        // Input: Cerrar aplicación
        // =====================================================================
        if (Engine::Input::IsKeyPressed(Engine::Key::Escape)) {
            LOG_INFO("ESC presionado - Cerrando aplicación");
            Close();
        }
        
        // =====================================================================
        // Input: Controles de cámara (WASD + QE)
        // =====================================================================
        if (m_camera) {
            if (Engine::Input::IsKeyPressed(Engine::Key::W)) {
                m_camera->ProcessKeyboard(Engine::CameraMovement::Forward, deltaTime);
            }
            if (Engine::Input::IsKeyPressed(Engine::Key::S)) {
                m_camera->ProcessKeyboard(Engine::CameraMovement::Backward, deltaTime);
            }
            if (Engine::Input::IsKeyPressed(Engine::Key::A)) {
                m_camera->ProcessKeyboard(Engine::CameraMovement::Left, deltaTime);
            }
            if (Engine::Input::IsKeyPressed(Engine::Key::D)) {
                m_camera->ProcessKeyboard(Engine::CameraMovement::Right, deltaTime);
            }
            if (Engine::Input::IsKeyPressed(Engine::Key::E)) {
                m_camera->ProcessKeyboard(Engine::CameraMovement::Up, deltaTime);
            }
            if (Engine::Input::IsKeyPressed(Engine::Key::Q)) {
                m_camera->ProcessKeyboard(Engine::CameraMovement::Down, deltaTime);
            }
            
            // Mouse look (solo si el botón izquierdo está presionado)
            if (Engine::Input::IsMouseButtonPressed(Engine::MouseButton::Left)) {
                glm::vec2 currentMousePos = Engine::Input::GetMousePosition();
                
                if (m_firstMouse) {
                    m_lastMousePos = currentMousePos;
                    m_firstMouse = false;
                }
                
                float xoffset = currentMousePos.x - m_lastMousePos.x;
                float yoffset = m_lastMousePos.y - currentMousePos.y;  // Invertido (Y crece hacia abajo)
                
                m_lastMousePos = currentMousePos;
                
                m_camera->ProcessMouseMovement(xoffset, yoffset);
            } else {
                m_firstMouse = true;
            }
        }
        
        // =====================================================================
        // Input: Cambiar colores de fondo
        // =====================================================================
        if (Engine::Input::IsKeyPressed(Engine::Key::D1)) {
            m_backgroundColor = glm::vec3(0.2f, 0.3f, 0.5f);  // Azul
            Engine::RenderCommand::SetClearColor(glm::vec4(m_backgroundColor, 1.0f));
            LOG_INFO("Color: Azul");
        }
        if (Engine::Input::IsKeyPressed(Engine::Key::D2)) {
            m_backgroundColor = glm::vec3(0.3f, 0.5f, 0.3f);  // Verde
            Engine::RenderCommand::SetClearColor(glm::vec4(m_backgroundColor, 1.0f));
            LOG_INFO("Color: Verde");
        }
        if (Engine::Input::IsKeyPressed(Engine::Key::D3)) {
            m_backgroundColor = glm::vec3(0.5f, 0.3f, 0.3f);  // Rojo
            Engine::RenderCommand::SetClearColor(glm::vec4(m_backgroundColor, 1.0f));
            LOG_INFO("Color: Rojo");
        }
        if (Engine::Input::IsKeyPressed(Engine::Key::D4)) {
            m_backgroundColor = glm::vec3(0.1f, 0.1f, 0.15f); // Gris oscuro
            Engine::RenderCommand::SetClearColor(glm::vec4(m_backgroundColor, 1.0f));
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
            Engine::RenderCommand::SetClearColor(glm::vec4(m_backgroundColor, 1.0f));
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
    }
    
    void OnRender() override {
        // Limpiar pantalla usando RenderCommand
        Engine::RenderCommand::Clear();
        
        // Verificar que todos los recursos estén listos
        if (!m_shader || !m_cubeMesh || !m_texture || !m_camera) {
            return;
        }
        
        // =====================================================================
        // FASE 2: Usar Renderer API (BeginScene/Submit/EndScene)
        // =====================================================================
        
        // 1. Begin scene con la cámara
        Engine::Renderer::BeginScene(*m_camera);
        
        // 2. Preparar transformación del cubo (rotación automática)
        static float rotation = 0.0f;
        rotation += Engine::Time::DeltaTime() * 45.0f;  // 45 grados/segundo
        
        glm::mat4 transform = glm::mat4(1.0f);
        transform = glm::translate(transform, glm::vec3(0.0f, 0.0f, 0.0f));
        transform = glm::rotate(transform, glm::radians(rotation), glm::vec3(0, 1, 0));  // Rotar Y
        transform = glm::rotate(transform, glm::radians(rotation * 0.5f), glm::vec3(1, 0, 0));  // Rotar X
        
        // 3. Bind texture antes de submit
        m_texture->Bind(0);
        m_shader->SetInt("u_Texture", 0);
        
        // 4. Submit mesh para renderizado
        Engine::Renderer::Submit(m_shader, m_cubeMesh, transform);
        
        // 5. End scene
        Engine::Renderer::EndScene();
        
        m_texture->Unbind();
    }
    
    void OnShutdown() override {
        LOG_INFO("Limpieza de SandboxApp");
        
        // Liberar cámara (único recurso sin smart pointer)
        if (m_camera) {
            delete m_camera;
            m_camera = nullptr;
        }
        
        // Los smart pointers se liberan automáticamente
    }
    
    void OnWindowResize(uint32_t width, uint32_t height) override {
        LOG_INFO("Ventana redimensionada: {}x{}", width, height);
        Engine::Renderer::OnWindowResize(width, height);
        
        // Actualizar aspect ratio de la cámara
        if (m_camera) {
            m_camera->SetAspectRatio(static_cast<float>(width) / static_cast<float>(height));
        }
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
        
        m_texture = std::shared_ptr<Engine::Texture2D>(
            Engine::Texture2D::Create(size, size, pixels, spec)
        );
        
        delete[] pixels;
        
        LOG_INFO("✅ Textura checkerboard creada ({}x{})", size, size);
    }

private:
    glm::vec3 m_backgroundColor;
    
    // FASE 2: Rendering completo con Camera
    std::shared_ptr<Engine::Shader> m_shader;
    std::shared_ptr<Engine::Mesh> m_cubeMesh;
    std::shared_ptr<Engine::Texture2D> m_texture;
    Engine::PerspectiveCamera* m_camera = nullptr;
    
    // Mouse input
    glm::vec2 m_lastMousePos = glm::vec2(0.0f);
    bool m_firstMouse = true;
};

/**
 * @brief Función de creación de aplicación
 * Requerida por EntryPoint.h
 */
Engine::Application* CreateApplication() {
    // Debug: Esto se imprime ANTES de crear la aplicación
    fprintf(stderr, "[DEBUG] CreateApplication() - INICIO\n");
    fflush(stderr);
    
    auto* app = new SandboxApp();
    
    fprintf(stderr, "[DEBUG] CreateApplication() - SandboxApp creado\n");
    fflush(stderr);
    
    return app;
}
