# Changelog

Todos los cambios notables en VestEngine serán documentados en este archivo.

El formato está basado en [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
y este proyecto adhiere a [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

### Planned
- Entity Component System (ECS) architecture
- 3D rendering pipeline
- Advanced lighting system
- Vulkan backend implementation

---

## [0.2.0-alpha] - 2025-11-15

### 🎉 Phase 2 Complete: Editor Features

#### Added
- **Asset Browser System** (`93d4d41`)
  - Complete filesystem navigation with tree + grid view
  - Search and filter functionality
  - File metadata display
  - Context menu (New Folder, Delete, Rename)
  - Drag & drop preparation
  - Icons cache system

- **Professional Camera System** (`725abee`)
  - EditorCamera class with smooth pan/zoom
  - Exponential interpolation for smooth movement
  - Screen-to-world and world-to-screen conversion
  - FocusOn() and FrameBounds() operations
  - Configurable zoom limits (0.25-10.0)
  - Aspect ratio handling

- **Selection & Highlights System** (`bff40d1`)
  - SelectionRenderer with 4 visual states
  - Animated selection outlines with pulse effect
  - Hover highlights with semi-transparent fill
  - Corner dots on selected entities
  - Color coding: blue=hover, yellow=selected, orange=both
  - Multi-selection box rendering support

- **Grid & Snap Tools** (`b27bafe`, `2500f0b`)
  - GridRenderer with adaptive spacing based on zoom
  - Major/minor grid lines with different colors
  - Axis lines (X=red, Y=green) for orientation
  - Snap to grid with configurable size (0.1-10.0)
  - DrawLines rendering infrastructure
  - UI controls: checkboxes + drag slider
  - Grid automatically adapts 20-100px between lines

- **Play Mode System** (`eaf8b24`)
  - EditorState enum (Edit/Play/Paused)
  - Play/Pause/Stop buttons with visual feedback
  - Automatic scene backup before play
  - Full scene restoration on stop
  - Edit protection during play mode
  - State indicator in menu bar
  - Grid auto-hide during play

#### Changed
- Camera controls now use smooth interpolation
- Selection system completely redesigned with animations
- Viewport rendering order optimized for grid and outlines
- Editor UI reorganized with play controls in toolbar

#### Fixed
- GridRenderer crash with invalid viewport/buffer (`2500f0b`)
- Zero-size VertexBuffer allocation issues
- Viewport validation before rendering

---

## [0.1.0-alpha] - 2025-11-14

### 🎉 Phase 1 Complete: Foundation & Consolidation

#### Added
- **Core Engine Architecture**
  - Layer system (`Layer`, `LayerStack`) for modular application structure
  - Event system with propagation and blocking
  - Timestep for frame-independent updates
  - Application lifecycle management
  - ImGui integration with docking and multi-viewport

- **Rendering System**
  - RendererAPI abstraction (OpenGL implemented, Vulkan stubs)
  - Vertex Array Objects, Vertex/Index Buffers
  - Shader system with uniform support
  - Texture loading via stb_image (PNG/JPG)
  - Framebuffer system for render-to-texture
  - Basic 2D renderer (quads, transformations)
  - Line rendering support

- **Scene Management**
  - SceneObject with transform, color, mesh type
  - Scene serialization/deserialization (JSON)
  - Version control in scene files
  - SceneSerializer with nlohmann_json

- **Command System**
  - Command pattern implementation
  - Full Undo/Redo functionality
  - Transform, Create, Delete, Macro commands
  - Command history with configurable limits
  - 20/20 command tests passing

- **Professional Logging**
  - spdlog integration
  - Colored console output
  - Timestamps and log levels
  - Core and client loggers separated
  - Macros for easy logging (VEST_CORE_INFO, VEST_INFO, etc.)

- **Editor Features**
  - Viewport panel with framebuffer rendering
  - Scene Hierarchy panel
  - Properties panel for transform editing
  - Content Browser panel (basic)
  - Stats panel (FPS, draw calls)
  - ImGuizmo integration (Translate/Rotate/Scale)
  - Viewport picking with 2D ray-casting
  - CRUD operations for entities
  - Keyboard shortcuts (Ctrl+S/L/Z/Y/D)

- **Testing Infrastructure**
  - Google Test integration
  - 37 unit tests implemented
  - Command system fully tested
  - Log system tests
  - Scene serialization tests

#### Technical Details
- **Build System**: CMake 3.20+ with FetchContent
- **Dependencies**: GLFW 3.4, GLAD, GLM 1.0.1, ImGui 1.91.8-docking, spdlog 1.12.0, nlohmann_json 3.11.3, Google Test 1.14.0
- **Platforms**: Windows 10/11, macOS 11+, Linux (Ubuntu 20.04+)
- **Compiler**: C++20 (MSVC 19.29+, GCC 10+, Clang 12+)

---

## [0.0.1-alpha] - 2025-11-01

### Initial Release

#### Added
- Basic project structure
- CMake build system
- GLFW window creation
- OpenGL context initialization
- ImGui basic integration
- Simple triangle rendering
- Basic shader compilation

---

## Version History Summary

| Version | Date | Phase | Status | Highlights |
|---------|------|-------|--------|------------|
| 0.2.0-alpha | 2025-11-15 | Phase 2 | ✅ Complete | Editor Features (5/5 subfases) |
| 0.1.0-alpha | 2025-11-14 | Phase 1 | ✅ Complete | Foundation (5/5 subfases) |
| 0.0.1-alpha | 2025-11-01 | Phase 0 | ✅ Complete | Initial Setup |

---

## Links

- **Repository**: [GitHub](https://github.com/amolrod/VestEngine)
- **Issues**: [GitHub Issues](https://github.com/amolrod/VestEngine/issues)
- **Documentation**: [docs/](docs/)

---

**Legend:**
- ✅ Complete
- 🚧 In Progress
- ❌ Not Started
- ⚠️ Stubs Only
- 🐛 Bug Fix
- ⚡ Performance
- 🔒 Security
