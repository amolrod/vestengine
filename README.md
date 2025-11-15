# VestEngine

[![Build Status](https://img.shields.io/badge/build-passing-brightgreen)]()
[![Version](https://img.shields.io/badge/version-0.2.0--alpha-blue)]()
[![License](https://img.shields.io/badge/license-MIT-orange)]()
[![C++](https://img.shields.io/badge/C%2B%2B-20-blue)]()
[![Platform](https://img.shields.io/badge/platform-Windows%20%7C%20macOS%20%7C%20Linux-lightgrey)]()

**VestEngine** es un motor gráfico modular 3D/2D escrito en C++20 con arquitectura de rendering abstrae (OpenGL/Vulkan), sistema de capas, y un editor visual profesional (VestEditor) construido con ImGui.

> 🎯 **Estado actual**: Phase 2 completa - Editor profesional con asset browser, camera system, selection highlights, grid/snap tools y play mode.

---

## 📋 Tabla de Contenidos

- [Características](#-características)
- [Capturas](#-capturas)
- [Requisitos](#-requisitos)
- [Instalación](#-instalación)
- [Estructura del Proyecto](#-estructura-del-proyecto)
- [Estado del Desarrollo](#-estado-del-desarrollo)
- [Documentación](#-documentación)
- [Contribuir](#-contribuir)
- [Licencia](#-licencia)

---

## ✨ Características

### Motor (VestEngine)

**Core Systems:**
- ✅ Sistema de capas modular (`Layer`, `LayerStack`)
- ✅ Event system completo con propagación
- ✅ ImGui integration con docking y multi-viewport
- ✅ Timestep y delta time para actualizaciones consistentes
- ✅ Logging profesional con spdlog (colores, timestamps, niveles)
- ✅ Window abstraction (GLFW backend)

**Rendering:**
- ✅ Abstracción de Rendering API (OpenGL ✅, Vulkan ⚠️ stubs)
- ✅ Vertex Array Objects, Vertex/Index Buffers
- ✅ Shader system con uniforms
- ✅ Texture loading (stb_image, PNG/JPG support)
- ✅ Framebuffer system para render-to-texture
- ✅ Basic 2D renderer (quads, texturas, transformaciones)
- ✅ Line rendering para grid y debug visualization

**Scene Management:**
- ✅ Scene serialization/deserialization (JSON con nlohmann_json)
- ✅ SceneObject con transform, color, mesh type
- ✅ Version control en archivos de escena

**Commands & Undo/Redo:**
- ✅ Command pattern implementation
- ✅ Undo/Redo system completo
- ✅ Transform, Create, Delete, Macro commands
- ✅ Historia de comandos con límites configurables

### Editor (VestEditor)

**Phase 1 - Foundation (✅ 100%):**
- ✅ Decoupling completo motor/editor
- ✅ Sistema de logging profesional
- ✅ Serialización con versionado
- ✅ Command system con undo/redo
- ✅ 37 unit tests (Google Test)

**Phase 2 - Editor Features (✅ 100%):**
- ✅ **Asset Browser**: Navegación filesystem completa, tree + grid view, search/filter
- ✅ **Camera System**: Pan/zoom suave con interpolación, screen-to-world conversion
- ✅ **Selection & Highlights**: Hover highlights, animated selection outlines, corner indicators
- ✅ **Grid & Snap Tools**: Adaptive grid, configurable snap, visual alignment guides
- ✅ **Play Mode**: Play/Pause/Stop con scene backup/restore, edit protection

**UI & Panels:**
- ✅ Viewport panel con framebuffer rendering
- ✅ Scene Hierarchy con selección y drag-drop (preparado)
- ✅ Properties panel para edición de transforms
- ✅ Content Browser para asset management
- ✅ Stats panel (FPS, draw calls, métricas)
- ✅ Toolbar con gizmo controls y play buttons

**Editing Tools:**
- ✅ ImGuizmo integration (Translate/Rotate/Scale)
- ✅ Viewport picking con ray-casting 2D
- ✅ CRUD de entidades (Add/Delete/Duplicate)
- ✅ Keyboard shortcuts (Ctrl+S/L/Z/Y/D)
- ✅ Entity selection visual feedback
- ✅ Multi-state editor (Edit/Play/Paused)

---

## 📸 Capturas

```
[TODO: Añadir captura de pantalla del editor cuando esté disponible]
- Viewport con grid adaptativo
- Asset browser con iconos
- Properties panel con gizmos
- Play mode controls
```

---

## 🔧 Requisitos

### Sistemas Operativos
- ✅ **Windows 10/11** (MSVC 2019+)
- ✅ **macOS 11+** (Apple Silicon / Intel, Xcode 13+)
- ✅ **Linux** (Ubuntu 20.04+, GCC 10+ o Clang 12+)

### Herramientas de Build
- **CMake** ≥ 3.20
- **Compilador C++20**:
  - MSVC 19.29+ (Visual Studio 2019 16.11+)
  - GCC 10+
  - Clang 12+
  - AppleClang 13+

### Dependencias (gestionadas por CMake)
Todas las dependencias se descargan automáticamente vía FetchContent:
- **GLFW** 3.4 - Windowing y input
- **GLAD** - OpenGL loader
- **GLM** 1.0.1 - Matemáticas 3D
- **ImGui** 1.91.8-docking - UI del editor
- **ImGuizmo** - Gizmos de transformación
- **spdlog** 1.12.0 - Logging
- **nlohmann_json** 3.11.3 - Serialización JSON
- **Google Test** 1.14.0 - Testing framework
- **stb_image** - Carga de texturas

---

## 🚀 Instalación

### 1. Clonar el Repositorio

```bash
git clone https://github.com/amolrod/VestEngine.git
cd VestEngine
```

### 2. Configurar con CMake

**Windows (Visual Studio):**
```bash
cmake -S . -B build -G "Visual Studio 17 2022" -A x64
```

**macOS / Linux:**
```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
```

### 3. Compilar

**Windows:**
```bash
cmake --build build --config Release
# O abrir build/VestEngine.sln en Visual Studio
```

**macOS / Linux:**
```bash
cmake --build build --parallel 8
# O usar make directamente:
cd build && make -j8
```

### 4. Ejecutar el Editor

**Windows:**
```bash
build\Editor\Release\VestEditor.exe
```

**macOS / Linux:**
```bash
./build/Editor/VestEditor
```

### 5. (Opcional) Ejecutar Tests

```bash
cd build
ctest --output-on-failure
# O ejecutar directamente:
./Tests/VestTests
```

---

## 📁 Estructura del Proyecto

```
VestEngine/
├── CMakeLists.txt                 # Build raíz con FetchContent
├── README.md                      # Este archivo
├── LICENSE                        # Licencia MIT
├── CHANGELOG.md                   # Historial de cambios
│
├── VestEngine/                    # 🔧 Motor (librería estática)
│   ├── CMakeLists.txt
│   └── src/
│       ├── Core/                  # Application, Layer, Input, Window
│       ├── ImGui/                 # ImGuiLayer integration
│       ├── Platform/              # GLFW window implementation
│       ├── Rendering/             # Renderer, Shader, Buffer, Texture
│       │   └── Platform/          # OpenGL & Vulkan implementations
│       ├── Scene/                 # SceneObject, Components
│       └── Serialization/         # SceneSerializer (JSON)
│
├── Editor/                        # 🎨 Editor (ejecutable)
│   ├── CMakeLists.txt
│   └── src/
│       ├── EditorApplication.cpp  # Entry point
│       ├── EditorLayer.*          # Main editor layer
│       ├── EditorCamera.*         # Editor camera system
│       ├── Commands/              # Undo/Redo system
│       ├── Gizmo/                 # (Future) Custom gizmo tools
│       ├── Panels/                # UI panels (Viewport, Hierarchy, etc.)
│       └── Rendering/             # SelectionRenderer, GridRenderer
│
├── Tests/                         # ✅ Unit tests (Google Test)
│   ├── CMakeLists.txt
│   ├── TestMain.cpp
│   ├── Commands/                  # Command system tests
│   ├── Core/                      # Log tests
│   └── Serialization/             # Scene serializer tests
│
├── external/                      # 📦 Third-party code
│   ├── glad/                      # OpenGL loader
│   ├── stb/                       # stb_image
│   └── imguizmo/                  # ImGuizmo widgets
│
├── assets/                        # 🎨 Assets del proyecto
│   ├── textures/                  # Checkerboard.png, White.png
│   └── scenes/                    # Default.json
│
├── docs/                          # 📚 Documentación
│   ├── ARCHITECTURE.md            # Diseño técnico
│   ├── ANALYSIS.md                # Estado actual
│   ├── PLAN.md                    # Roadmap
│   ├── DEVELOPMENT.md             # Guía del desarrollador
│   └── CONTEXT.md                 # Contexto para IA
│
└── build/                         # 🏗️ Directorio de build (generado)
    ├── Editor/VestEditor          # Ejecutable del editor
    ├── Tests/VestTests            # Ejecutable de tests
    └── lib/                       # Bibliotecas compiladas
```

---

## 📊 Estado del Desarrollo

### ✅ Completado (Production Ready)

| Componente | Estado | Cobertura |
|------------|--------|-----------|
| **Core Systems** | ✅ Completo | Layers, Events, ImGui |
| **OpenGL Renderer** | ✅ Funcional | Buffers, Shaders, Textures, Framebuffers |
| **Scene Serialization** | ✅ Completo | JSON save/load, versioning |
| **Command System** | ✅ Completo | Undo/Redo, 20/20 tests passing |
| **Editor UI** | ✅ Completo | Docking, panels, toolbar |
| **Asset Browser** | ✅ Completo | Filesystem nav, search, metadata |
| **Camera System** | ✅ Completo | Smooth controls, interpolation |
| **Selection System** | ✅ Completo | Hover, animated outlines |
| **Grid & Snap** | ✅ Completo | Adaptive grid, configurable snap |
| **Play Mode** | ✅ Completo | Play/Pause/Stop, state management |

### 🚧 En Progreso

| Componente | Estado | Notas |
|------------|--------|-------|
| **Vulkan Backend** | ⚠️ Solo stubs | Arquitectura lista, sin implementación |
| **3D Rendering** | ❌ No iniciado | Actualmente 2D ortográfico |
| **ECS System** | ❌ Planeado | Phase 3 |
| **Physics** | ❌ Planeado | Phase 4+ |

### 📅 Próximas Fases

- **Phase 3**: Entity Component System (ECS)
- **Phase 4**: Advanced Rendering (3D, lighting, shadows)
- **Phase 5**: Vulkan Implementation
- **Phase 6**: Physics & Audio

Ver [docs/PLAN.md](docs/PLAN.md) para roadmap detallado.

---

## 📚 Documentación

- **[ARCHITECTURE.md](docs/ARCHITECTURE.md)** - Diseño técnico y decisiones arquitectónicas
- **[ANALYSIS.md](docs/ANALYSIS.md)** - Análisis del estado actual y métricas
- **[PLAN.md](docs/PLAN.md)** - Roadmap y próximos pasos
- **[DEVELOPMENT.md](docs/DEVELOPMENT.md)** - Guía para desarrolladores
- **[CONTEXT.md](docs/CONTEXT.md)** - Contexto rápido para IA/nuevos devs
- **[CHANGELOG.md](CHANGELOG.md)** - Historial de cambios

---

## 🤝 Contribuir

¡Contribuciones bienvenidas! Por favor:

1. Fork el repositorio
2. Crea una feature branch (`git checkout -b feature/AmazingFeature`)
3. Commit tus cambios (`git commit -m 'Add some AmazingFeature'`)
4. Push a la rama (`git push origin feature/AmazingFeature`)
5. Abre un Pull Request

Ver [DEVELOPMENT.md](docs/DEVELOPMENT.md) para guías de estilo y workflow.

---

## 📄 Licencia

Este proyecto está bajo la Licencia MIT - ver el archivo [LICENSE](LICENSE) para detalles.

---

## 📧 Contacto

**Proyecto**: [VestEngine](https://github.com/amolrod/VestEngine)  
**Issues**: [GitHub Issues](https://github.com/amolrod/VestEngine/issues)

---

**Última actualización**: 15 de noviembre de 2025  
**Versión**: 0.2.0-alpha (Phase 2 Complete)
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
