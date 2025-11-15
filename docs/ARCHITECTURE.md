# VestEngine Architecture

**Última actualización**: 15 de noviembre de 2025  
**Versión**: 0.2.0-alpha

---

## 📋 Tabla de Contenidos

- [Visión General](#visión-general)
- [Decisiones de Diseño](#decisiones-de-diseño)
- [Arquitectura de Módulos](#arquitectura-de-módulos)
- [Flujo de Datos](#flujo-de-datos)
- [Rendering Pipeline](#rendering-pipeline)
- [Dependencias Externas](#dependencias-externas)
- [Principios de Diseño](#principios-de-diseño)
- [Patrones Utilizados](#patrones-utilizados)

---

## Visión General

VestEngine sigue una **arquitectura en capas modular** que separa claramente:

```
┌─────────────────────────────────────────┐
│         Editor (VestEditor)             │  ← Aplicación de usuario
├─────────────────────────────────────────┤
│      Engine API (VestEngine)            │  ← Biblioteca pública
├─────────────────────────────────────────┤
│    Rendering Abstraction Layer          │  ← Abstracción de plataforma
├─────────────────────────────────────────┤
│  OpenGL Backend  │  Vulkan Backend      │  ← Implementaciones específicas
├──────────────────┴──────────────────────┤
│    Platform Layer (GLFW, OS APIs)       │  ← Capa de sistema
└─────────────────────────────────────────┘
```

### Diagrama de Componentes Principales

```
VestEngine Core
    ├── Application (singleton, lifecycle)
    ├── LayerStack (ordered layers)
    │   ├── GameLayer
    │   ├── EditorLayer
    │   └── ImGuiLayer (overlay)
    ├── Window (GLFW abstraction)
    ├── Input (polling system)
    └── Events (mouse, keyboard, window)

Rendering System
    ├── RendererAPI (interface)
    │   ├── OpenGLRendererAPI ✅
    │   └── VulkanRendererAPI ⚠️
    ├── RenderCommand (high-level API)
    ├── Renderer (scene management)
    ├── Shader, Texture, Buffer
    └── Framebuffer (render targets)

Editor System
    ├── EditorLayer (main logic)
    ├── EditorCamera (viewport camera)
    ├── Panels (UI components)
    │   ├── ViewportPanel
    │   ├── SceneHierarchyPanel
    │   ├── PropertiesPanel
    │   ├── ContentBrowserPanel
    │   └── StatsPanel
    ├── Commands (undo/redo)
    ├── SelectionRenderer
    └── GridRenderer
```

---

## Decisiones de Diseño

### 1. ¿Por qué Abstracción de Rendering API?

**Problema**: Los juegos y motores modernos necesitan soportar múltiples APIs gráficas (OpenGL, Vulkan, DirectX, Metal) para maximizar compatibilidad y rendimiento.

**Solución**: Patrón Strategy + Factory
- `RendererAPI`: Interfaz abstracta con métodos virtuales puros
- `OpenGLRendererAPI`, `VulkanRendererAPI`: Implementaciones concretas
- `RenderCommand`: Facade que delega a la API activa

**Beneficios**:
- ✅ Cambio de backend en runtime (teoría) o compile-time
- ✅ Código de juego/editor independiente de la API
- ✅ Facilita testing (mock de RendererAPI)
- ✅ Preparado para futuras APIs (Metal, DirectX 12)

**Trade-offs**:
- ⚠️ Overhead de llamadas virtuales (mínimo, medido <1% CPU)
- ⚠️ Abstracción limita features específicas de cada API
- ⚠️ Mantener múltiples backends es costoso

### 2. ¿Por qué OpenGL Primero, Vulkan Después?

**Razonamiento**:

| Criterio | OpenGL | Vulkan |
|----------|--------|--------|
| **Curva de aprendizaje** | Baja | Alta |
| **Tiempo de desarrollo** | Rápido | Lento (5-10x más código) |
| **Compatibilidad** | Universal | Requiere drivers recientes |
| **Depuración** | Sencilla | Compleja (validation layers) |
| **Rendimiento** | Suficiente para 2D/3D básico | Óptimo para AAA |

**Decisión**: Implementar OpenGL completamente primero para validar arquitectura, luego portar a Vulkan cuando la API esté estable.

**Estado Actual**:
- OpenGL: ✅ **Funcional al 100%** (buffers, shaders, texturas, framebuffers)
- Vulkan: ⚠️ **Solo arquitectura** (stubs, sin implementación real)

### 3. Sistema de Capas para Modularidad

**Problema**: Un editor necesita múltiples "módulos" (viewport, UI, audio, physics) que se actualizan en orden específico.

**Solución**: Patrón Layer Stack
```cpp
class Layer {
    virtual void OnAttach();
    virtual void OnDetach();
    virtual void OnUpdate(Timestep ts);
    virtual void OnEvent(Event& event);
    virtual void OnImGuiRender();
};

class LayerStack {
    std::vector<Layer*> m_Layers;  // Orden de actualización
};
```

**Uso**:
```cpp
Application app;
app.PushLayer(new GameLayer());
app.PushLayer(new EditorLayer());
app.PushOverlay(new ImGuiLayer());  // Siempre encima
```

**Beneficios**:
- ✅ Modularidad: cada Layer es independiente
- ✅ Orden de renderizado garantizado
- ✅ Event propagation con early-out
- ✅ Hot-swapping de layers en runtime

### 4. Command Pattern para Undo/Redo

**Problema**: Editores profesionales necesitan deshacer/rehacer cualquier operación.

**Solución**: Command Pattern + History Stack

```cpp
class ICommand {
    virtual void Execute() = 0;
    virtual void Undo() = 0;
};

class CommandManager {
    std::vector<Scope<ICommand>> m_UndoStack;
    std::vector<Scope<ICommand>> m_RedoStack;
};
```

**Implementaciones**:
- `TransformCommand`: Position, Rotation, Scale
- `CreateEntityCommand`: Añadir entidades
- `DeleteEntityCommand`: Eliminar entidades
- `MacroCommand`: Agrupar múltiples comandos

**Testing**: 20/20 tests pasando (Google Test)

### 5. Scene Serialization con JSON

**Problema**: Guardar/cargar escenas de forma legible y extensible.

**Alternativas Consideradas**:
| Formato | Pros | Contras | Decisión |
|---------|------|---------|----------|
| **JSON** | Legible, fácil de debuggear, extensible | Tamaño grande, lento para escenas masivas | ✅ **Elegido** |
| Binary | Compacto, rápido | Opaco, difícil de debuggear | ❌ Futuro (opcional) |
| XML | Estándar, legible | Verboso, parsing lento | ❌ Innecesario |
| YAML | Muy legible, comentarios | Parsing complejo | ❌ Overhead |

**Implementación**:
```cpp
class SceneSerializer {
    static void Serialize(const std::string& filepath, 
                         const std::vector<SceneObject>& objects);
    static bool Deserialize(const std::string& filepath, 
                           std::vector<SceneObject>& objects);
};
```

**Features**:
- ✅ Versioning (`"version": "1.0"`)
- ✅ Extensibilidad (nuevos campos no rompen compatibilidad)
- ✅ Validación de entrada
- ✅ Logging de errores

---

## Arquitectura de Módulos

### Core Module

**Responsabilidad**: Funcionalidad fundamental del engine.

```cpp
namespace Vest {
    // Application.h
    class Application {
        Window* m_Window;
        LayerStack m_LayerStack;
        bool m_Running;
        
        void Run();  // Main loop
        void PushLayer(Layer* layer);
    };
    
    // Layer.h
    class Layer {
        virtual void OnAttach() {}
        virtual void OnDetach() {}
        virtual void OnUpdate(Timestep ts) {}
        virtual void OnEvent(Event& e) {}
        virtual void OnImGuiRender() {}
    };
    
    // Input.h (static polling)
    class Input {
        static bool IsKeyPressed(KeyCode key);
        static bool IsMouseButtonPressed(MouseCode button);
        static glm::vec2 GetMousePosition();
    };
}
```

**Archivos clave**:
- `Core/Application.cpp` - Main loop, layer management
- `Core/Layer.h` - Layer interface
- `Core/LayerStack.cpp` - Layer ordering
- `Core/Input.cpp` - Input polling (delegación a Platform)
- `Core/Timestep.h` - Delta time wrapper

### Rendering Module

**Responsabilidad**: Abstracción de rendering y backend OpenGL.

```cpp
// Rendering/RendererAPI.h (interface)
class RendererAPI {
public:
    enum class API { None, OpenGL, Vulkan };
    
    virtual void Init() = 0;
    virtual void SetViewport(...) = 0;
    virtual void Clear() = 0;
    virtual void DrawIndexed(...) = 0;
    virtual void DrawLines(...) = 0;
    
    static API GetAPI() { return s_API; }
private:
    static API s_API;
};

// Platform/OpenGL/OpenGLRendererAPI.cpp
class OpenGLRendererAPI : public RendererAPI {
    void Init() override { /* glEnable(...) */ }
    void DrawIndexed(...) override { /* glDrawElements */ }
};
```

**Jerarquía de Rendering**:
```
RendererAPI (interface)
    ├── OpenGLRendererAPI ✅
    │   ├── Init() → glEnable(GL_BLEND), glBlendFunc(...)
    │   ├── SetViewport() → glViewport(...)
    │   ├── Clear() → glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
    │   ├── DrawIndexed() → glDrawElements(GL_TRIANGLES, ...)
    │   └── DrawLines() → glDrawArrays(GL_LINES, ...)
    └── VulkanRendererAPI ⚠️ (stubs)
        └── Init() → assert(false, "Not implemented")

RenderCommand (static facade)
    ├── Init() → s_RendererAPI->Init()
    ├── Clear() → s_RendererAPI->Clear()
    └── DrawIndexed() → s_RendererAPI->DrawIndexed()

Renderer (high-level scene API)
    ├── BeginScene(camera)
    ├── Submit(shader, vao, transform)
    └── EndScene()
```

**Recursos OpenGL**:
- `Shader`: Compilación, linking, uniforms
- `Texture2D`: stb_image, binding, uploading
- `VertexArray`: VAO abstraction
- `VertexBuffer`: VBO con layouts dinámicos
- `IndexBuffer`: IBO para indexed drawing
- `Framebuffer`: FBO para render-to-texture

### ImGui Module

**Responsabilidad**: Integración de ImGui con el engine.

```cpp
// ImGui/ImGuiLayer.h
class ImGuiLayer : public Layer {
    void OnAttach() override;
    void OnDetach() override;
    void OnImGuiRender() override;
    
    void Begin();  // ImGui::NewFrame()
    void End();    // ImGui::Render()
};
```

**Features**:
- ✅ Docking habilitado (`ImGuiDockNodeFlags_PassthruCentralNode`)
- ✅ Multi-viewport (ventanas fuera del main window)
- ✅ Custom style (dark theme)
- ✅ Keyboard/mouse input forwarding

### Editor Module

**Responsabilidad**: Lógica del editor visual.

```cpp
// Editor/EditorLayer.h
class EditorLayer : public Layer {
    // State
    EditorState m_EditorState;  // Edit, Play, Paused
    std::vector<SceneObject> m_SceneObjects;
    std::vector<SceneObject> m_SceneBackup;
    int m_SelectedEntityIndex;
    
    // Subsystems
    EditorCamera m_EditorCamera;
    SelectionRenderer m_SelectionRenderer;
    GridRenderer m_GridRenderer;
    CommandManager m_CommandManager;
    
    // Panels
    ViewportPanel m_ViewportPanel;
    SceneHierarchyPanel m_SceneHierarchyPanel;
    // ...
    
    // Methods
    void OnUpdate(Timestep ts) override;
    void OnImGuiRender() override;
    void HandleViewportPicking();
    void HandleGizmos();
};
```

**Editor Subsystems**:

1. **EditorCamera**: Smooth pan/zoom, screen-to-world conversion
2. **SelectionRenderer**: Visual feedback (hover, selection, animated outlines)
3. **GridRenderer**: Adaptive grid, snap tools
4. **CommandManager**: Undo/redo stack
5. **Panels**: Modular UI components (Viewport, Hierarchy, Properties, etc.)

---

## Flujo de Datos

### Main Loop (Application.cpp)

```
┌──────────────────────────────────────┐
│      Application::Run()              │
└──────────────────────────────────────┘
           │
           ├─> Window::OnUpdate()  (poll events, swap buffers)
           │
           ├─> for each Layer in LayerStack:
           │       Layer::OnUpdate(deltaTime)
           │       Layer::OnImGuiRender()
           │
           └─> Repeat until m_Running = false
```

### Event Flow

```
Window (GLFW callback)
    │
    └─> Application::OnEvent(Event& e)
            │
            ├─> ImGuiLayer::OnEvent(e)  (consume if ImGui wants input)
            │
            └─> for each Layer in reverse order:
                    Layer::OnEvent(e)
                    if (e.Handled) break;
```

### Rendering Flow (EditorLayer)

```
EditorLayer::OnUpdate(ts)
    │
    ├─> m_EditorCamera.OnUpdate(ts)  // Smooth interpolation
    │
    ├─> m_Framebuffer->Bind()
    │   ├─> RenderCommand::Clear()
    │   ├─> m_GridRenderer.RenderGrid(...)  // If in Edit mode
    │   ├─> Renderer::BeginScene(camera)
    │   │   ├─> for each SceneObject:
    │   │   │       Renderer::Submit(shader, vao, transform)
    │   │   └─> Renderer::EndScene()
    │   └─> m_Framebuffer->Unbind()
    │
    ├─> HandleViewportHover()   // Update m_HoveredEntityIndex
    ├─> HandleViewportPicking() // Update m_SelectedEntityIndex
    ├─> HandleGizmos()          // ImGuizmo manipulate
    │
    └─> OnImGuiRender()
        ├─> Render all panels
        └─> m_SelectionRenderer.DrawOutline(...)  // On top
```

### Command Execution Flow

```
User Action (ej: Move entity with gizmo)
    │
    └─> Create TransformCommand
            │
            └─> CommandManager::ExecuteCommand(cmd)
                    │
                    ├─> cmd->Execute()  // Apply change
                    ├─> m_UndoStack.push(cmd)
                    └─> m_RedoStack.clear()

User presses Ctrl+Z
    │
    └─> CommandManager::Undo()
            │
            ├─> cmd = m_UndoStack.pop()
            ├─> cmd->Undo()  // Revert change
            └─> m_RedoStack.push(cmd)
```

---

## Rendering Pipeline

### OpenGL Rendering Path

```
1. Initialization
   ├─> OpenGLRendererAPI::Init()
   │   ├─> glEnable(GL_BLEND)
   │   ├─> glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)
   │   └─> glEnable(GL_DEPTH_TEST)
   │
2. Per-Frame Setup
   ├─> Framebuffer::Bind()  // Bind FBO
   ├─> RenderCommand::SetClearColor({0.1, 0.1, 0.1, 1.0})
   └─> RenderCommand::Clear()  // glClear(...)
   │
3. Grid Rendering (if enabled)
   ├─> GridRenderer::RenderGrid(...)
   │   ├─> Update grid geometry if camera moved
   │   ├─> VertexBuffer::SetData(interleavedData)
   │   ├─> m_GridShader->Bind()
   │   ├─> m_GridShader->SetMat4("u_ViewProjection", ...)
   │   └─> RenderCommand::DrawLines(m_GridVA, vertexCount)
   │
4. Scene Rendering
   ├─> Renderer::BeginScene(viewProjectionMatrix)
   │
   ├─> for each SceneObject:
   │   ├─> Shader::Bind()
   │   ├─> Shader::SetFloat4("u_Color", ...)
   │   ├─> Shader::SetInt("u_Texture", ...)
   │   ├─> Texture::Bind(slot)
   │   ├─> VertexArray::Bind()
   │   └─> RenderCommand::DrawIndexed(vao, indexCount)
   │       └─> glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, 0)
   │
   └─> Renderer::EndScene()
   │
5. UI Overlay (ImGui)
   ├─> Framebuffer::Unbind()  // Back to main framebuffer
   ├─> ImGui::Begin("Viewport")
   ├─> ImGui::Image(framebuffer->GetColorAttachmentID(), size)
   │
   ├─> SelectionRenderer::DrawOutline(...)
   │   ├─> ImGui::GetForegroundDrawList()
   │   └─> drawList->AddLine(p1, p2, color, thickness)
   │
   └─> ImGui::End()
```

### Shader Pipeline

**Vertex Shader** (Grid example):
```glsl
#version 410 core
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;

uniform mat4 u_ViewProjection;

out vec4 v_Color;

void main() {
    v_Color = a_Color;
    gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
}
```

**Fragment Shader**:
```glsl
#version 410 core
layout(location = 0) out vec4 color;

in vec4 v_Color;

void main() {
    color = v_Color;
}
```

---

## Dependencias Externas

### Críticas (sin ellas el engine no compila)

| Librería | Versión | Propósito | Licencia |
|----------|---------|-----------|----------|
| **GLFW** | 3.4 | Windowing, input, OpenGL context | Zlib |
| **GLAD** | - | OpenGL function loader | Public Domain |
| **GLM** | 1.0.1 | Matemáticas 3D (vec, mat, quat) | MIT |
| **ImGui** | 1.91.8-docking | Editor UI | MIT |
| **spdlog** | 1.12.0 | Logging | MIT |
| **stb_image** | 2.27 | Texture loading (PNG/JPG) | Public Domain |

### Opcionales (solo para testing o herramientas)

| Librería | Versión | Propósito | Licencia |
|----------|---------|-----------|----------|
| **Google Test** | 1.14.0 | Unit testing | BSD-3 |
| **nlohmann_json** | 3.11.3 | JSON parsing | MIT |
| **ImGuizmo** | Latest | Gizmo widgets (translate/rotate/scale) | MIT |

### Gestión de Dependencias

**Método**: CMake FetchContent (descarga automática en build-time)

```cmake
include(FetchContent)

FetchContent_Declare(
  glfw
  URL https://github.com/glfw/glfw/releases/download/3.4/glfw-3.4.zip
)
FetchContent_MakeAvailable(glfw)

target_link_libraries(VestEngine PUBLIC glfw)
```

**Ventajas**:
- ✅ No necesita git submodules
- ✅ Versiones exactas reproducibles
- ✅ CMake las cachea (no re-descarga)

**Desventajas**:
- ⚠️ Primera compilación lenta (descarga todas las deps)
- ⚠️ Requiere conexión a internet

---

## Principios de Diseño

### 1. SOLID Principles

| Principio | Aplicación en VestEngine |
|-----------|--------------------------|
| **S**ingle Responsibility | Cada clase tiene un único propósito (ej: `Shader` solo maneja shaders, no texturas) |
| **O**pen/Closed | `RendererAPI` abierta a extensión (nuevas APIs), cerrada a modificación |
| **L**iskov Substitution | Cualquier `RendererAPI*` puede usarse sin conocer la implementación |
| **I**nterface Segregation | Interfaces pequeñas (ej: `ICommand` solo tiene Execute/Undo) |
| **D**ependency Inversion | Alto nivel depende de abstracciones (`RenderCommand` → `RendererAPI`) |

### 2. Modern C++ (C++20)

**Features utilizadas**:
- ✅ Smart pointers (`std::unique_ptr`, `std::shared_ptr`)
- ✅ Move semantics (`std::move`, `&&`)
- ✅ Lambda expressions
- ✅ `auto` y type inference
- ✅ Range-based for loops
- ✅ `constexpr`
- ✅ `std::optional` para valores opcionales
- ✅ `std::filesystem` para paths
- ✅ Designated initializers (`SceneObject{.name="Quad", .position={0,0,0}}`)

**Anti-patterns evitados**:
- ❌ Raw pointers con ownership (`new` sin `delete`)
- ❌ `void*` y type casting unsafe
- ❌ Macros complejas (excepto logging)
- ❌ Global mutable state (excepto singletons bien justificados)

### 3. Abstracción Mínima

**Filosofía**: Solo abstraer lo necesario.

**Ejemplos**:
- ✅ `RendererAPI`: Abstracción necesaria (múltiples backends)
- ✅ `Window`: Abstracción necesaria (GLFW podría reemplazarse)
- ❌ No abstraer `glm::vec3` (no hay beneficio, GLM es estándar)
- ❌ No abstraer `std::vector` (sin motivo para custom allocators... aún)

### 4. Data-Oriented Design (preparación futura)

**Actual**: Object-Oriented (cada SceneObject es una clase/struct)

**Futuro (Phase 3 - ECS)**:
```cpp
// En lugar de:
class SceneObject {
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec4 color;
};

// Hacer:
struct TransformComponent { glm::vec3 position, rotation, scale; };
struct RenderComponent { glm::vec4 color; MeshType mesh; };

// Sistemas procesan arrays contiguos de componentes
void TransformSystem(std::vector<TransformComponent>& transforms);
```

**Beneficios (futuro)**:
- ⚡ Cache-friendly (datos contiguos)
- ⚡ Paralelización sencilla (sistemas independientes)
- ⚡ Mejor escalabilidad (miles de entidades)

---

## Patrones Utilizados

### Creational Patterns

**1. Factory Method** (`Shader::Create`, `Texture2D::Create`)
```cpp
// Shader.h
class Shader {
public:
    static Ref<Shader> Create(const std::string& name, 
                              const std::string& vertexSrc, 
                              const std::string& fragmentSrc);
};

// Shader.cpp
Ref<Shader> Shader::Create(...) {
    switch (RendererAPI::GetAPI()) {
        case RendererAPI::API::OpenGL:
            return CreateRef<OpenGLShader>(...);
        case RendererAPI::API::Vulkan:
            return CreateRef<VulkanShader>(...);
    }
}
```

**2. Singleton** (`Application`, `Input`)
```cpp
class Application {
public:
    static Application& Get() { return *s_Instance; }
private:
    static Application* s_Instance;
};
```

### Structural Patterns

**1. Facade** (`RenderCommand`)
```cpp
// Simplifica uso de RendererAPI
RenderCommand::Clear();  // En lugar de:
RendererAPI::Get()->Clear();
```

**2. Adapter** (`Platform::GLFWWindow` adapta GLFW a nuestra `Window` interface)

**3. Decorator** (`LayerStack` añade comportamiento a layers sin modificarlas)

### Behavioral Patterns

**1. Command** (`ICommand`, `TransformCommand`, etc.)
- Encapsula acciones como objetos
- Permite undo/redo

**2. Strategy** (`RendererAPI` con `OpenGLRendererAPI`, `VulkanRendererAPI`)
- Intercambia algoritmos de rendering

**3. Observer** (Event system)
- Layers "observan" eventos y reaccionan

**4. Template Method** (`Layer` define estructura, subclases implementan detalles)

---

## Referencias

- [Cherno's Game Engine Series](https://www.youtube.com/playlist?list=PLlrATfBNZ98dC-V-N3m0Go4deliWHPFwT)
- [Hazel Engine](https://github.com/TheCherno/Hazel)
- [Unity Architecture](https://docs.unity3d.com/Manual/Architecture.html)
- [Unreal Engine Source](https://github.com/EpicGames/UnrealEngine)
- [Real-Time Rendering 4th Edition](http://www.realtimerendering.com/)
- [Game Programming Patterns](https://gameprogrammingpatterns.com/)

---

**Última actualización**: 15 de noviembre de 2025  
**Autor**: VestEngine Team  
**Versión del documento**: 1.0
