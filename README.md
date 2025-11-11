# 🎮 Motor Gráfico 3D en C++20

[![C++20](https://img.shields.io/badge/C%2B%2B-20-blue.svg)](https://en.cppreference.com/w/cpp/20)
[![OpenGL](https://img.shields.io/badge/OpenGL-4.1%2F4.6-green.svg)](https://www.opengl.org/)
[![CMake](https://img.shields.io/badge/CMake-3.20+-orange.svg)](https://cmake.org/)
[![Platform](https://img.shields.io/badge/Platform-macOS%20%7C%20Windows%20%7C%20Linux-lightgrey.svg)]()

Motor gráfico moderno y modular desarrollado en **C++20** con **OpenGL**, diseñado como proyecto de portfolio profesional. Arquitectura escalable con sistemas separados, siguiendo principios SOLID y patrones modernos de C++.

---

## 📋 Tabla de Contenidos

- [Características](#-características)
- [Arquitectura](#-arquitectura)
- [Estado del Proyecto](#-estado-del-proyecto)
- [Requisitos](#-requisitos)
- [Compilación](#️-compilación)
- [Uso](#-uso)
- [Estructura del Proyecto](#-estructura-del-proyecto)
- [Roadmap](#-roadmap)
- [Tecnologías](#-tecnologías)
- [Documentación](#-documentación)

---

## ✨ Características

### ✅ **FASE 1 - Core Engine** (Completada)
- **Logger System**: Wrapper sobre spdlog con colores y niveles de log
- **Window Management**: Abstracción de GLFW con control de VSync
- **Time System**: Delta time, FPS counter, elapsed time con precisión de microsegundos
- **Input Polling**: Sistema estático para teclado y ratón
- **Application Framework**: Clase base con game loop y hooks virtuales

### ✅ **FASE 2 - Rendering Fundamentals** (Completada)
- **Shader System**:
  - Compilación y linkado de shaders GLSL con detección de errores
  - Parser para shaders multi-sección (#shader vertex/fragment)
  - Uniforms completos: float, vec2/3/4, mat3/4, int/samplers
  - Manejo automático de recursos con RAII

- **Buffer Abstractions**:
  - `VertexBuffer`: VBO con RAII y uso dinámico/estático
  - `IndexBuffer`: IBO con conteo automático de índices
  - `VertexArray`: VAO con layouts flexibles
  - `BufferLayout`: Descriptor de atributos con cálculo automático de stride/offset

- **Mesh System**:
  - Estructura `Vertex` completa: posición, normal, UV, color (48 bytes)
  - `MeshFactory` con 6 primitivas procedurales:
    - Cubo (24 vértices, normales correctas)
    - Esfera (subdivisión configurable, UVs esféricas)
    - Plano (grid configurable)
    - Quad (primitiva básica 2D/3D)
    - Cilindro (tapas opcionales, UVs cilíndricas)
    - Cono (base configurable, UVs cónicas)

- **Texture System**:
  - Carga de texturas con **stb_image** (PNG, JPG, BMP, TGA, PSD, HDR)
  - `TextureSpecification`: wrapping (repeat/clamp/mirror), filtering (nearest/linear), mipmaps
  - Generación procedural (checkerboard, gradientes)
  - Gestión automática de memoria con smart pointers

- **Camera 3D**:
  - `PerspectiveCamera`: cámara 3D con controles FPS
    - Movimiento: WASD (horizontal), QE (vertical)
    - Rotación: arrastre del ratón con Euler angles
    - Zoom: scroll del ratón (FOV dinámico)
  - `OrthographicCamera`: cámara 2D para UI/debug
  - Matrices View y Projection automáticas

### 🔄 **En Progreso**
- Renderer API (BeginScene/EndScene pattern) - Opcional para Fase 2

### 📦 **Pendiente**
- FASE 3: Entity Component System (ECS)
- FASE 4: Material System & Lighting
- FASE 5: Asset Management
- FASE 6: Physics Integration & Audio

---

## 🏗️ Arquitectura

```
┌─────────────────────────────────────────┐
│         APPLICATION LAYER               │
│     (Sandbox, Game, Editor)             │
└─────────────────────────────────────────┘
                  ↓
┌─────────────────────────────────────────┐
│       HIGH-LEVEL SYSTEMS                │
│  (Scene, ECS, Materials, Assets)        │
└─────────────────────────────────────────┘
                  ↓
┌─────────────────────────────────────────┐
│         SUBSYSTEMS                      │
│  (Rendering, Input, Audio, Physics)     │
└─────────────────────────────────────────┘
                  ↓
┌─────────────────────────────────────────┐
│           CORE ENGINE                   │
│  (Logger, Window, Time, Application)    │
└─────────────────────────────────────────┘
```

### Principios de Diseño
- **SOLID**: Responsabilidad única, Open/Closed, Liskov, segregación de interfaces, inversión de dependencias
- **RAII**: Gestión automática de recursos OpenGL (shaders, buffers, texturas, VAOs)
- **Smart Pointers**: std::unique_ptr/std::shared_ptr para memoria segura
- **Patrón Singleton**: Logger global thread-safe
- **Template Method**: Application con hooks virtuales
- **Factory Pattern**: MeshFactory para primitivas geométricas
- **Strategy Pattern**: TextureSpecification para configuración flexible

---

## 🚀 Estado del Proyecto

| Fase | Sistema | Estado | LOC | Archivos |
|------|---------|--------|-----|----------|
| **1** | Core Engine | ✅ Completo | ~800 | 11 |
| **2** | Rendering Fundamentals | ✅ Completo | ~1500 | 13 |
| **3** | ECS & Scene | 📦 Pendiente | - | - |
| **4** | Materials & Lighting | 📦 Pendiente | - | - |
| **5** | Asset Management | 📦 Pendiente | - | - |
| **6** | Physics & Audio | 📦 Pendiente | - | - |

**Métricas Actuales:**
- **Líneas de código**: ~2,300 LOC (sin dependencias externas)
- **Archivos C++**: 24 archivos (.h/.cpp)
- **Shaders GLSL**: 2 shaders (FlatColor, Texture)
- **FPS**: 72-74 FPS estables (VSync activo en macOS)
- **OpenGL**: 4.1 Core Profile (macOS) / 4.6 (Windows/Linux)

---

## 📦 Requisitos

### Software
- **CMake**: 3.20 o superior
- **Compilador C++20**:
  - macOS: Clang 12+ (Xcode 13+)
  - Windows: MSVC 2022 o MinGW-w64 11+
  - Linux: GCC 11+ o Clang 12+
- **OpenGL**:
  - macOS: OpenGL 4.1 (nativo)
  - Windows/Linux: OpenGL 4.6+ (drivers GPU actualizados)

### Dependencias (descargadas automáticamente por CMake)
- **GLFW 3.4**: Gestión de ventanas y entrada
- **GLM 1.0.1**: Matemáticas (vectores, matrices, transformaciones)
- **spdlog 1.14.1**: Logging rápido y estructurado
- **stb_image**: Carga de imágenes (header-only, incluido en `/external/stb/`)

---

## 🛠️ Compilación

### macOS / Linux

```bash
# Clonar el repositorio
git clone https://github.com/tu-usuario/motor-grafico.git
cd motor-grafico

# Crear directorio de build
mkdir -p build && cd build

# Generar proyecto CMake
cmake ..

# Compilar (usar -j para paralelizar)
cmake --build . -j8

# Ejecutar sandbox
./bin/Sandbox
```

### Windows (Visual Studio)

```powershell
# Clonar el repositorio
git clone https://github.com/tu-usuario/motor-grafico.git
cd motor-grafico

# Crear directorio de build
mkdir build
cd build

# Generar solución de Visual Studio
cmake -G "Visual Studio 17 2022" ..

# Abrir motor-grafico.sln con Visual Studio
# O compilar desde línea de comandos:
cmake --build . --config Release

# Ejecutar sandbox
.\bin\Release\Sandbox.exe
```

### Limpieza

```bash
# Desde el directorio raíz
rm -rf build/
```

---

## 🎮 Uso

### Ejemplo Básico (Sandbox)

```cpp
#include "core/Application.h"
#include "rendering/Shader.h"
#include "rendering/Mesh.h"
#include "rendering/Texture.h"
#include "rendering/Camera.h"

class MyApp : public Engine::Application {
private:
    std::shared_ptr<Engine::Shader> m_shader;
    std::shared_ptr<Engine::Mesh> m_mesh;
    std::shared_ptr<Engine::Texture2D> m_texture;
    std::unique_ptr<Engine::PerspectiveCamera> m_camera;

public:
    void OnInit() override {
        // Crear shader
        m_shader = std::make_shared<Engine::Shader>(
            "/ruta/absoluta/shaders/Texture.glsl"
        );
        
        // Crear mesh (cubo)
        m_mesh = Engine::MeshFactory::CreateCube();
        
        // Crear textura
        Engine::TextureSpecification spec;
        spec.WrapS = Engine::TextureWrap::Repeat;
        spec.WrapT = Engine::TextureWrap::Repeat;
        spec.MinFilter = Engine::TextureFilter::Linear;
        spec.MagFilter = Engine::TextureFilter::Linear;
        spec.GenerateMipmaps = true;
        
        m_texture = std::make_shared<Engine::Texture2D>(
            "/ruta/textura.png", spec
        );
        
        // Crear cámara
        m_camera = std::make_unique<Engine::PerspectiveCamera>(
            45.0f, 1280.0f / 720.0f, 0.1f, 100.0f
        );
        m_camera->SetPosition({0.0f, 0.0f, 5.0f});
        
        LOG_INFO("✅ Aplicación inicializada");
    }

    void OnUpdate(float deltaTime) override {
        // Controles de cámara (WASD + QE + mouse)
        m_camera->OnUpdate(deltaTime);
    }

    void OnRender() override {
        // Limpiar pantalla
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // Activar shader y textura
        m_shader->Bind();
        m_texture->Bind(0);
        m_shader->SetUniformInt("u_Texture", 0);
        
        // Matrices MVP
        glm::mat4 model = glm::rotate(
            glm::mat4(1.0f),
            Engine::Time::ElapsedTime(),
            {0.0f, 1.0f, 0.0f}
        );
        glm::mat4 mvp = m_camera->GetProjectionMatrix() * 
                        m_camera->GetViewMatrix() * model;
        
        m_shader->SetUniformMat4("u_MVP", mvp);
        
        // Renderizar mesh
        m_mesh->GetVertexArray()->Bind();
        glDrawElements(GL_TRIANGLES, 
                       m_mesh->GetIndexBuffer()->GetCount(),
                       GL_UNSIGNED_INT, nullptr);
    }

    void OnShutdown() override {
        LOG_INFO("🛑 Cerrando aplicación");
    }
};

// Entry point
Engine::Application* Engine::CreateApplication() {
    return new MyApp();
}
```

### Controles (Sandbox actual)
- **WASD**: Mover cámara (horizontal)
- **Q/E**: Subir/bajar cámara
- **Arrastre ratón**: Rotar cámara (pitch/yaw)
- **Scroll ratón**: Zoom (FOV dinámico)
- **ESC**: Cerrar aplicación

---

## 📁 Estructura del Proyecto

```
motor-grafico/
├── CMakeLists.txt              # CMake raíz
├── README.md                   # Este archivo
├── .gitignore                  # Ignorar build/, logs/, etc.
│
├── external/                   # Dependencias externas
│   ├── CMakeLists.txt          # FetchContent para GLFW/GLM/spdlog
│   └── stb/
│       ├── stb_image.h         # Header-only de stb_image
│       └── stb_image_impl.cpp  # Implementación wrapper
│
├── src/                        # Motor engine
│   ├── CMakeLists.txt
│   │
│   ├── core/                   # Núcleo del motor
│   │   ├── Application.h/cpp   # Clase base con game loop
│   │   ├── EntryPoint.h        # main() genérico
│   │   ├── Logger.h/cpp        # Sistema de logging
│   │   ├── Time.h/cpp          # Delta time, FPS
│   │   └── Window.h/cpp        # Ventana GLFW + OpenGL
│   │
│   ├── input/                  # Sistema de entrada
│   │   └── Input.h/cpp         # Polling estático
│   │
│   └── rendering/              # Sistemas de rendering
│       ├── Shader.h/cpp        # Shaders GLSL
│       ├── Buffer.h/cpp        # VBO/IBO/VAO/Layouts
│       ├── Mesh.h/cpp          # Mesh + MeshFactory
│       ├── Texture.h/cpp       # Texture2D + stb_image
│       └── Camera.h/cpp        # PerspectiveCamera, OrthographicCamera
│
├── shaders/                    # Shaders GLSL
│   ├── FlatColor.glsl          # Shader de color sólido
│   └── Texture.glsl            # Shader con texturas
│
├── sandbox/                    # Aplicación de prueba
│   ├── CMakeLists.txt
│   └── main.cpp                # Demo con cubo texturizado + cámara
│
├── tests/                      # Tests unitarios (futuro)
│   └── CMakeLists.txt
│
└── build/                      # Archivos de compilación (generado)
    ├── bin/Sandbox             # Ejecutable
    └── ...
```

---

## 🗺️ Roadmap

### ✅ Fase 1: Core Engine (Completada)
- [x] Logger system (spdlog wrapper)
- [x] Window management (GLFW)
- [x] Time system (delta time, FPS)
- [x] Input polling (keyboard, mouse)
- [x] Application framework (game loop)

### ✅ Fase 2: Rendering Fundamentals (Completada)
- [x] Shader system (GLSL compilation)
- [x] Buffer abstractions (VBO, IBO, VAO)
- [x] Mesh system (primitivas procedurales)
- [x] Texture system (stb_image)
- [x] Camera 3D (Perspective + Orthographic)
- [ ] Renderer API (BeginScene/EndScene) - Opcional

### 📦 Fase 3: Entity Component System
- [ ] Entity manager
- [ ] Component system
- [ ] Scene graph
- [ ] Transform hierarchy
- [ ] GameObject abstraction

### 📦 Fase 4: Material System & Lighting
- [ ] Material properties
- [ ] PBR workflow
- [ ] Directional/Point/Spot lights
- [ ] Shadow mapping
- [ ] Deferred rendering

### 📦 Fase 5: Asset Management
- [ ] Asset loader (GLTF, OBJ, FBX)
- [ ] Resource caching
- [ ] Hot-reload
- [ ] Serialization

### 📦 Fase 6: Physics & Audio
- [ ] Physics integration (PhysX/Bullet)
- [ ] Collision detection
- [ ] Audio system (OpenAL/FMOD)
- [ ] Sound spatial 3D

### 📦 Fase 7: Advanced Rendering
- [ ] Post-processing stack
- [ ] HDR + Bloom
- [ ] SSAO (Screen-Space Ambient Occlusion)
- [ ] Particle systems

### 📦 Fase 8: Tools & Editor
- [ ] ImGui integration
- [ ] Scene editor
- [ ] Inspector panel
- [ ] Performance profiler

### 📦 Fase 9: Optimizations
- [ ] Frustum culling
- [ ] Occlusion culling
- [ ] Instanced rendering
- [ ] LOD system

---

## 🔧 Tecnologías

| Categoría | Tecnología | Versión | Uso |
|-----------|-----------|---------|-----|
| **Lenguaje** | C++20 | ISO/IEC 14882:2020 | Core del motor |
| **Gráficos** | OpenGL | 4.1 (macOS) / 4.6 (otros) | Rendering API |
| **Ventanas** | GLFW | 3.4 | Window/Input/Context |
| **Matemáticas** | GLM | 1.0.1 | Vectores, matrices |
| **Logging** | spdlog | 1.14.1 | Sistema de logs |
| **Imágenes** | stb_image | 2.28 | Carga de texturas |
| **Build** | CMake | 3.20+ | Sistema de build |

---

## 📚 Documentación

### Recursos de Aprendizaje
- [LearnOpenGL](https://learnopengl.com/) - Tutorial completo de OpenGL
- [The Cherno - Game Engine Series](https://www.youtube.com/playlist?list=PLlrATfBNZ98dC-V-N3m0Go4deliWHPFwT) - Serie de YouTube sobre motores
- [OpenGL Reference](https://registry.khronos.org/OpenGL-Refpages/gl4/) - Documentación oficial
- [GLM Documentation](https://glm.g-truc.net/) - Documentación de GLM
- [GLFW Documentation](https://www.glfw.org/documentation.html) - Docs de GLFW

### Patrones de Diseño Aplicados
- **Singleton**: Logger global thread-safe
- **Template Method**: Application::Run() con hooks virtuales
- **RAII**: Gestión de recursos OpenGL (destructores automáticos)
- **Factory**: MeshFactory::CreateCube/Sphere/...
- **Strategy**: TextureSpecification para configuración flexible
- **Facade**: Shader/Window/Texture simplifican APIs complejas

### Decisiones de Arquitectura
1. **C++20**: Uso de conceptos modernos (smart pointers, move semantics, constexpr)
2. **RAII sobre todo**: Todos los recursos OpenGL se gestionan con destructores
3. **Separación de capas**: Core → Subsystems → High-level → Application
4. **Sin dependencias pesadas**: Solo GLFW/GLM/spdlog (bibliotecas ligeras)
5. **Multiplataforma desde el inicio**: Macros y condicionales para macOS/Windows/Linux

---

## 📝 Licencia

Este proyecto es de código abierto bajo la **Licencia MIT**.

---

## 👤 Autor

**Angel** - Estudiante de 2º DAW  
Motor gráfico desarrollado como proyecto de portfolio profesional.

📧 Contacto: [Tu email aquí]  
🔗 GitHub: [Tu perfil aquí]

---

## 🙏 Agradecimientos

- **The Cherno** por su excelente serie de Game Engine
- **LearnOpenGL** por los tutoriales detallados
- **Comunidad de OpenGL/C++** por recursos y soporte
- **GLFW, GLM, spdlog** por sus excelentes bibliotecas

---

**Estado**: 🚀 En desarrollo activo | **Última actualización**: 2024  
**Fase actual**: 2/9 completadas | **Próximo hito**: ECS (Entity Component System)
