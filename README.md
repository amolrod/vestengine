# VestEngine & VestEditor

VestEngine es un motor gráfico 3D modular escrito en C++20 con un editor visual (VestEditor) basado en ImGui. El objetivo es disponer de un runtime reutilizable con una arquitectura de capas, abstracciones de render y una interfaz para crear escenas con flujo multiplataforma (Windows, macOS, Linux).

## Características actuales
- **Motor (VestEngine)**
  - Sistema de capas (`Layer`, `LayerStack`) e ImGui overlay integrado.
  - Abstracción de Renderer API (OpenGL funcional + stubs Vulkan).
  - Recursos OpenGL: buffers, VAO, shaders, texturas (con stb_image) y framebuffers.
  - Sistema de comandos (`RenderCommand`) y renderer de alto nivel.
  - Serialización JSON de escenas (`SceneSerializer`, basado en nlohmann_json).
- **Editor (VestEditor)**
  - Docking + multi-viewport de ImGui con controles personalizados.
  - Paneles: Viewport, SceneHierarchy, Properties, ContentBrowser, Stats.
  - Renderiza triángulo y quad texturizado mediante Framebuffer → `ImGui::Image`.
  - Picking 2D, resaltado visual y gizmo (ImGuizmo) para translate/rotate/scale.
  - CRUD de entidades, duplicado, undo/redo, guardar/cargar escenas.
  - Toolbar con toggles de gizmo, botones de Add/Duplicate/Delete y accesos rápidos (Ctrl+S, Ctrl+L, Ctrl+Z, Ctrl+Y, Ctrl+D).

## Estructura del repositorio
```
.
├── CMakeLists.txt             # build raíz (glfw/glm/imgui/nlohmann_json)
├── VestEngine/                # librería estática
│   ├── src/Core               # runtime (Application, Layer, Window, Input)
│   ├── src/Rendering          # Renderer, abstracciones y backend OpenGL
│   ├── src/Serialization      # SceneSerializer (JSON)
│   └── ...
├── Editor/                    # ejecutable del editor
│   ├── src/EditorLayer.*
│   ├── src/Panels/*
│   └── src/Scene/SceneObject.h
├── external/                  # dependencias integradas (glad, imgui, imguiZmo, stb, ...)
└── assets/
    ├── textures/Checkerboard.png
    ├── textures/White.png
    └── scenes/Default.json
```

## Compilación
```
cmake -S . -B build
cmake --build build
```
El ejecutable del editor queda en `build/Editor/VestEditor`. En macOS se requiere CMake ≥ 3.20 y clang/AppleClang; en Windows el proyecto se abre con Visual Studio 2022.

## Controles
- **Viewport**
  - WASD: mover cámara ortográfica.
  - Scroll: zoom (0.5x–5x).
  - Clic derecho o botón medio: pan continuo.
  - Clic izquierdo: seleccionar entidad bajo el cursor.
  - Gizmo (cuando hay selección): Translate/Rotate/Scale.
- **Atajos**
  - Ctrl+S / Ctrl+L: guardar/cargar escena (`assets/scenes/Default.json`).
  - Ctrl+Z / Ctrl+Y: undo/redo.
  - Ctrl+D: duplicar entidad.

## Próximos pasos sugeridos
1. Persistir layout del editor y rutas avanzadas de assets.
2. Completar serialización con referencias a texturas/meshes externas.
3. Añadir gizmos locales/mundo avanzados (snap, clamp) y gizmos por eje.
4. Sistemas de undo/redo más robustos (agrupación de operaciones, historial persistente).
5. Integrar un ECS (EnTT) y escenas jerárquicas reales.
6. Guardar configuración del usuario y layout docking.
7. Incorporar ImGuizmo ViewManipulate, gizmos de escala por eje y gizmos de rotación 3D.
8. Pipeline de build multiplataforma (Windows/Linux) + documentación final.

## Licencia y dependencias
- **GLFW** (Zlib), **GLAD** (MIT), **GLM** (MIT), **ImGui** (MIT), **ImGuizmo** (MIT), **stb_image** (public domain) y **nlohmann_json** (MIT). Utiliza estos paquetes localmente mediante FetchContent o carpetas `external/`.
