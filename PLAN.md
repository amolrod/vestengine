# VestEngine Implementation Plan

## Visión General
VestEngine será un motor gráfico 3D multiplataforma con arquitectura modular. El repositorio se estructura en dos piezas principales:
- `VestEngine/`: librería estática reutilizable del motor.
- `Editor/`: aplicación de editor con interfaz ImGui.

Soporte objetivo: Windows (prioritario), Linux y macOS.

## Arquitectura
1. **Abstracción de Rendering API**
   - `RenderAPI` expone enum (OpenGL, Vulkan) y se selecciona en tiempo de ejecución mediante factory pattern.
   - `RendererAPI` define interfaz de bajo nivel (Init, Clear, DrawIndexed, etc.).
   - Backend OpenGL completo (contexto, buffers, shaders, texturas, framebuffers) bajo `Rendering/Platform/OpenGL/`.
   - Backend Vulkan solo con stubs para mostrar escalabilidad.

2. **Sistema de Capas**
   - `Layer` define los métodos virtuales (`OnAttach`, `OnUpdate`, `OnImGuiRender`, `OnEvent`, ...).
   - `LayerStack` gestiona capas y overlays con inserción y eliminación seguras.

3. **Integración ImGui con Docking**
   - Usar rama docking de ImGui (v1.91+).
   - Backends oficiales: `imgui_impl_glfw` + `imgui_impl_opengl3`.
   - `ImGuiLayer` crea contexto, habilita `Docking` y `Viewports`, y gestiona `Begin/End` para todo el aplicativo.

4. **Editor con Panels**
   - Estructura: `Editor/src/EditorLayer.cpp` y paneles en `Editor/src/Panels/` (Viewport, SceneHierarchy, Properties, ContentBrowser, Stats).
   - Viewport renderiza un framebuffer y se muestra con `ImGui::Image`.
   - Layout de docking persistente + menús (`File`, `Edit`, `View`, `Tools`, `Help`).
   - Toolbar con íconos de play/pause/step.
   - Interacción desde viewport (gizmos, selección, cámara) a futuro.

5. **Stack Tecnológico**
   - C++20, CMake 3.20+, GLFW 3.4, GLAD (OpenGL 4.x loader), GLM 1.0+, ImGui v1.91-docking, stb_image.
   - Opcional fase 2+: EnTT, yaml-cpp, spdlog.

6. **Decisiones de Diseño**
   - Factory para crear recursos dependientes de API (`Shader::Create`, `Buffer::Create`, etc.).
   - `RenderCommand` como fachada sobre `RendererAPI` para centralizar comandos estáticos.
   - OpenGL implementación completa (programas, uniformes, VAO/VBO/IBO, texturas, framebuffers).
   - Vulkan solo stubs con `assert(false)` + logs.

7. **Configuración CMake**
   - Top-level incluye options (`VEST_BUILD_EDITOR`, `VEST_RENDERER_API`, ...).
   - `FetchContent` para dependencias (glfw, glm, imgui). Glad se incluye localmente.
   - `VestEngine` como librería estática; `Editor` como ejecutable que la enlaza.

8. **Plan de Fases**
   - Fase 1: Core + OpenGL funcional (contexto, renderer API, triángulo).
   - Fase 2: Layer system + ImGui integrado.
   - Fase 3: Editor básico (Viewport, menús, stats).
   - Fase 4: Paneles avanzados (SceneHierarchy, Properties, ContentBrowser, Toolbar).
   - Fase 5: Arquitectura Vulkan (stubs, logging cuando se use).

9. **Estándares de Código**
   - C++20 con `std::unique_ptr`/`std::shared_ptr` (alias `Scope`/`Ref`).
   - Naming: Clases PascalCase, métodos PascalCase, variables camelCase, miembros privados `m_CamelCase`, estáticos `s_CamelCase`.
   - Comentarios estilo Doxygen para API pública, SOLID principles, asserts/logging según build.

## Próximos Pasos (Iterativos)
1. Renderizado real en el viewport: shaders + mesh básica para validar `Renderer::Submit`.
2. Integrar `stb_image` y completar carga de texturas.
3. Implementar cámara y matrices view/projection para escenas.
4. Conectar paneles del editor a datos reales (SceneHierarchy + Properties).
5. Añadir logging (spdlog) y serialización de layouts/escenas en fases posteriores.
