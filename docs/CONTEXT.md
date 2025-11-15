# VestEngine Context (Quick Reference)

**Last Updated**: 15 de noviembre de 2025  
**Version**: 0.2.0-alpha  
**Branch**: feature/phase2-editor-features  
**Purpose**: Quick context loading for AI assistants and new developers

---

## 🚀 TL;DR

**VestEngine** es un motor de juegos 2D/3D en C++20 con arquitectura modular, rendering abstraction layer (OpenGL/Vulkan), y editor profesional tipo Unity.

**Estado**: ✅ Phase 2 complete (5/5 subfases), listo para Phase 3 (ECS).

---

## 📊 Feature Matrix

| Feature | Status | Notes |
|---------|--------|-------|
| **Core Application** | ✅ | Singleton, LayerStack, main loop |
| **Event System** | ✅ | Mouse, keyboard, window events |
| **Input Polling** | ✅ | `Input::IsKeyPressed()` |
| **Logging** | ✅ | spdlog, multi-level, 8 tests |
| **Rendering API** | ✅ OpenGL / ⚠️ Vulkan stubs | Abstraction layer funcional |
| **Shader System** | ✅ | Compile, link, uniforms |
| **Texture System** | ✅ | PNG/JPG loading (stb_image) |
| **Framebuffer** | ✅ | Render-to-texture |
| **Editor UI** | ✅ | ImGui, docking, panels |
| **Viewport** | ✅ | Interactive, picking, hover |
| **Scene Hierarchy** | ✅ | Entity list, selection |
| **Properties Panel** | ✅ | Transform, color, mesh |
| **Content Browser** | ✅ | File navigation (básico) |
| **Stats Panel** | ✅ | FPS, frame time, object count |
| **Gizmos** | ✅ | Translate/Rotate/Scale (ImGuizmo) |
| **Grid + Snap** | ✅ | Adaptive grid, snap tools |
| **Selection System** | ✅ | Hover, selection, animated outlines |
| **Play Mode** | ✅ | Play/Pause/Stop, scene backup/restore |
| **Undo/Redo** | ✅ | Command pattern, 20 tests |
| **Serialization** | ✅ | JSON scene save/load, 9 tests |
| **ECS** | ❌ | **Phase 3** (critical) |
| **Advanced Rendering** | ❌ | **Phase 4** (PBR, shadows, post-processing) |
| **Vulkan Backend** | ❌ | **Phase 5** (architecture ready, no implementation) |
| **Physics** | ❌ | Backlog (Box2D integration) |
| **Audio** | ❌ | Backlog (FMOD/OpenAL) |
| **Scripting** | ❌ | Backlog (Lua/C#) |

**Legend**: ✅ Complete | 🚧 In Progress | ⚠️ Partial | ❌ Not Started

---

## 🏗️ Architecture Overview

```
┌─────────────────────────────────────┐
│        Editor (ImGui UI)            │
├─────────────────────────────────────┤
│  Panels | Commands | Camera | Grid  │
├─────────────────────────────────────┤
│       Engine Core (VestEngine)      │
│  Application | Layers | Input       │
├─────────────────────────────────────┤
│      Rendering Abstraction          │
│    RendererAPI (interface)          │
├─────────────────────────────────────┤
│  OpenGL ✅  |  Vulkan ⚠️ (stubs)    │
├─────────────────────────────────────┤
│    Platform (GLFW, OS APIs)         │
└─────────────────────────────────────┘
```

**Key Concepts**:
- **Layers**: Modular updates (GameLayer, EditorLayer, ImGuiLayer)
- **Commands**: Undo/redo system (TransformCommand, CreateEntityCommand, etc.)
- **RendererAPI**: Strategy pattern para multi-backend rendering
- **RenderCommand**: Static facade sobre RendererAPI

---

## 📂 Important Files

### Must-Read (Start Here)

| File | Purpose | LOC |
|------|---------|-----|
| `VestEngine/src/Core/Application.cpp` | Main loop, layer management | ~150 |
| `VestEngine/src/Rendering/RendererAPI.h` | Rendering abstraction interface | ~50 |
| `Editor/src/EditorLayer.h` | Main editor logic | ~100 |
| `Editor/src/EditorLayer.cpp` | Editor implementation | ~800 |
| `README.md` | Project overview, installation | ~300 |
| `CHANGELOG.md` | Version history | ~200 |

### Core Engine

| File | Purpose |
|------|---------|
| `VestEngine/src/Core/Application.{h,cpp}` | Singleton, window, layer stack |
| `VestEngine/src/Core/Layer.h` | Layer interface (OnUpdate, OnEvent, OnImGuiRender) |
| `VestEngine/src/Core/LayerStack.cpp` | Layer ordering and iteration |
| `VestEngine/src/Core/Input.cpp` | Static input polling (delegates to Platform) |
| `VestEngine/src/Core/Log.{h,cpp}` | spdlog initialization, macros |

### Rendering

| File | Purpose |
|------|---------|
| `VestEngine/src/Rendering/RendererAPI.h` | Abstract rendering interface |
| `VestEngine/src/Rendering/RenderCommand.h` | Static facade (Init, Clear, DrawIndexed, DrawLines) |
| `VestEngine/src/Rendering/Renderer.{h,cpp}` | High-level scene renderer (BeginScene, Submit, EndScene) |
| `VestEngine/src/Rendering/Shader.{h,cpp}` | Shader compilation, uniforms |
| `VestEngine/src/Rendering/Texture.cpp` | Texture loading (stb_image) |
| `VestEngine/src/Rendering/Framebuffer.{h,cpp}` | FBO for render-to-texture |
| `VestEngine/src/Rendering/Platform/OpenGL/OpenGLRendererAPI.cpp` | OpenGL implementation (✅ complete) |

### Editor

| File | Purpose |
|------|---------|
| `Editor/src/EditorLayer.{h,cpp}` | Main editor (viewport, panels, state management) |
| `Editor/src/EditorCamera.{h,cpp}` | Smooth pan/zoom camera |
| `Editor/src/Commands/CommandManager.{h,cpp}` | Undo/redo stack |
| `Editor/src/Commands/TransformCommand.cpp` | Move/rotate/scale entity |
| `Editor/src/Panels/ViewportPanel.cpp` | 3D viewport rendering |
| `Editor/src/Panels/SceneHierarchyPanel.cpp` | Entity list |
| `Editor/src/Panels/PropertiesPanel.cpp` | Entity properties editor |
| `Editor/src/Rendering/GridRenderer.{h,cpp}` | Adaptive grid + snap |
| `Editor/src/Rendering/SelectionRenderer.{h,cpp}` | Hover/selection outlines |

---

## 🛠️ Quick Commands

### Build

```bash
# Configure (first time)
cmake -B build

# Build (Debug)
cmake --build build --config Debug -j8

# Build (Release)
cmake --build build --config Release -j8

# Clean rebuild
rm -rf build && cmake -B build && cmake --build build -j8
```

### Run

```bash
# Editor
./build/Editor/VestEditor                    # macOS/Linux
.\build\Editor\Debug\VestEditor.exe          # Windows

# Tests
./build/Tests/VestTests                      # macOS/Linux
.\build\Tests\Debug\VestTests.exe            # Windows
```

### Git

```bash
# Current branch
git branch  # → feature/phase2-editor-features

# Status
git status

# Commit
git add .
git commit -m "feat(scope): Description"

# Push
git push origin feature/phase2-editor-features
```

---

## 🧪 Testing

**Run all tests**:
```bash
cd build/Tests
./VestTests  # 37 tests, should all pass
```

**Test coverage**:
- Commands: 20 tests ✅
- Serialization: 9 tests ✅
- Logging: 8 tests ✅
- **Total**: 37/37 passing

**Add new test**:
```cpp
// Tests/MyModule/MyTests.cpp
#include <gtest/gtest.h>

TEST(MyTest, DoesWhat_WithInput_ExpectedOutput) {
    // Arrange
    int input = 5;
    
    // Act
    int result = MyFunction(input);
    
    // Assert
    ASSERT_EQ(result, 10);
}
```

---

## 🎨 Coding Conventions

### Naming

```cpp
// Classes, Functions: PascalCase
class EditorLayer { };
void OnUpdate(Timestep ts);

// Variables: camelCase
int entityCount;
float deltaTime;

// Member variables: m_PascalCase
int m_SelectedEntityIndex;
float m_ZoomSpeed;

// Constants: UPPER_CASE
const int MAX_ENTITIES = 1000;
```

### Smart Pointers

```cpp
// Engine aliases
template<typename T>
using Scope = std::unique_ptr<T>;

template<typename T>
using Ref = std::shared_ptr<T>;

// Usage
Ref<Shader> shader = Shader::Create("shader.glsl");
Scope<VertexBuffer> vbo = CreateScope<VertexBuffer>(data, size);
```

### Logging

```cpp
VEST_CORE_TRACE("Debug info");       // Disabled in Release
VEST_CORE_INFO("Initialization OK");
VEST_CORE_WARN("Missing texture");
VEST_CORE_ERROR("Shader failed");
VEST_CORE_CRITICAL("Out of memory");

// Client logs (game/editor code)
VEST_INFO("Level loaded");
VEST_ERROR("Save corrupted");
```

---

## 🐛 Common Issues & Fixes

### Issue 1: Segfault at Startup

**Symptoms**: `Segmentation fault (core dumped)`

**Common Causes**:
- Zero-size buffer creation
- Null pointer dereference
- Invalid OpenGL calls before context

**Debug**:
```bash
gdb ./build/Editor/VestEditor
(gdb) run
(gdb) bt  # Backtrace
```

**Recent Fix**: GridRenderer → validate viewport size before rendering.

---

### Issue 2: Black Screen in Viewport

**Checklist**:
1. ✅ Framebuffer bound correctly?
2. ✅ Clear color not black? (`{0.2, 0.2, 0.2, 1.0}`)
3. ✅ Camera position/zoom valid?
4. ✅ Objects in scene? (`m_SceneObjects.size() > 0`)
5. ✅ Shader bound?

---

### Issue 3: ImGui Assertion

**Error**: `imgui.cpp:1234: Assertion 'g.WithinFrameScope' failed`

**Cause**: ImGui call outside `NewFrame()`/`Render()`.

**Fix**: Move all ImGui code to `OnImGuiRender()`:
```cpp
void EditorLayer::OnImGuiRender() {
    ImGui::Begin("Window");
    // ... ImGui calls here ...
    ImGui::End();
}
```

---

## 📋 TODOs & FIXMEs (Centralized)

### Critical (Before Phase 3)

- [ ] **Error Handling**: Standardize with `std::optional` or exceptions
- [ ] **Integration Tests**: Add editor workflow tests (create→move→save→load)
- [ ] **Memory Profiling**: Run Valgrind/ASan, fix any leaks
- [ ] **Refactor `EditorLayer::OnImGuiRender()`**: Split into functions per panel (CCN=25, too complex)

### High Priority

- [ ] **Documentation**: Add Doxygen comments to remaining 70% of functions
- [ ] **Asset Manager**: Implement hot-reloading, previews
- [ ] **Performance**: Profile with Tracy/Instruments
- [ ] **Vulkan**: Start Phase 5 implementation (10-12 weeks)

### Medium Priority

- [ ] **Batching**: Reduce draw calls (1000 objects → 10 batches)
- [ ] **Frustum Culling**: Don't render off-screen objects
- [ ] **Material System**: PBR materials, texture slots
- [ ] **Physics**: Integrate Box2D (2D physics)

### Low Priority

- [ ] **Scripting**: Lua/C# integration (backlog)
- [ ] **Networking**: Client-server (backlog)
- [ ] **Audio**: FMOD/OpenAL (backlog)

---

## 🎯 Technical Decisions (Recent)

### Decision 1: Use EnTT for ECS (Phase 3)

**Context**: Need high-performance ECS for Phase 3.

**Options**:
- A) Custom ECS (6-8 weeks, learning experience)
- B) EnTT (2-3 weeks, battle-tested)

**Decision**: **EnTT** (pragmatic for v1.0, custom for v2.0 if needed)

**Rationale**: Focus on features, not reinventing ECS. EnTT is proven, fast, header-only.

---

### Decision 2: OpenGL First, Vulkan Later

**Context**: Need working renderer quickly.

**Decision**: **OpenGL complete → then Vulkan** (Phase 5)

**Rationale**:
- OpenGL: Universal compatibility, fast development
- Vulkan: 5-10x more code, steeper learning curve
- Validate architecture with OpenGL first

---

### Decision 3: JSON for Serialization

**Context**: Need scene save/load format.

**Options**:
- A) JSON (readable, easy to debug)
- B) Binary (compact, fast)
- C) XML (verbose, standard)

**Decision**: **JSON** (with optional binary for large scenes)

**Rationale**: Git-friendly, human-readable, extensible. Binary can be added later for optimization.

---

## 📊 Performance Benchmarks

**Hardware**: MacBook Pro M1 (2021), 16GB RAM, macOS 14.5

**Scene**: 100 quads with textures

| Metric | Value | Target |
|--------|-------|--------|
| **FPS** | 120-130 | >60 |
| **Frame Time** | 7-8ms | <16ms |
| **Startup Time** | 2.5s | <3s |
| **Memory Usage** | ~150MB | <500MB |
| **Draw Calls** | 100 | <10 (with batching) |

**Bottlenecks**:
1. 🔴 1 draw call per object (no batching)
2. 🟡 ImGui rendering (~1ms/frame)
3. 🟡 No frustum culling (renders everything)

---

## 📚 Dependencies

| Library | Version | Purpose | License |
|---------|---------|---------|---------|
| **GLFW** | 3.4 | Windowing, input | Zlib |
| **GLAD** | - | OpenGL loader | Public Domain |
| **GLM** | 1.0.1 | Math (vec, mat, quat) | MIT |
| **ImGui** | 1.91.8-docking | Editor UI | MIT |
| **ImGuizmo** | Latest | Gizmos (translate/rotate/scale) | MIT |
| **spdlog** | 1.12.0 | Logging | MIT |
| **stb_image** | 2.27 | Texture loading | Public Domain |
| **nlohmann_json** | 3.11.3 | JSON parsing | MIT |
| **Google Test** | 1.14.0 | Unit testing | BSD-3 |

**Managed via**: CMake FetchContent (automatic download)

---

## 🔄 Last Session Summary

### Completed

✅ **Phase 2.5: Play Mode System** (commit `eaf8b24`)
- Play/Pause/Stop buttons with color coding
- Scene backup/restore functionality
- Edit protection during play (no gizmos, picking, entity controls)
- State indicator in menu bar
- Grid hidden during play mode

✅ **Documentation Update** (5/7 docs complete)
- README.md: Complete rewrite with badges, features, installation
- CHANGELOG.md: Version history (0.0.1 → 0.2.0)
- docs/ARCHITECTURE.md: Technical design, modules, patterns
- docs/ANALYSIS.md: Strengths, weaknesses, metrics, comparison
- docs/PLAN.md: Roadmap, milestones, backlog
- docs/DEVELOPMENT.md: Setup, build, workflow, debugging
- docs/CONTEXT.md: This file (quick reference)

### Current Status

**Phase 2**: ✅ **100% Complete** (5/5 subfases)
- 2.1: Asset Browser ✅
- 2.2: Camera System ✅
- 2.3: Selection & Highlights ✅
- 2.4: Grid & Snap Tools ✅
- 2.5: Play Mode ✅

**Branch**: `feature/phase2-editor-features` (ready to merge)

**Next**: Start **Phase 3: ECS** (6-8 weeks, target: January 2025)

---

## 🚀 Next Steps (Immediate)

### 1. Merge Phase 2 to Main

```bash
git checkout main
git merge feature/phase2-editor-features
git tag v0.2.0-alpha
git push origin main --tags
```

### 2. Create Phase 3 Branch

```bash
git checkout -b feature/phase3-ecs
```

### 3. Start Phase 3.1: Core ECS Architecture

**Tasks** (2 weeks):
- [ ] Design ECS API (Entity, Component, System)
- [ ] Implement Entity Manager
- [ ] Implement Component Storage
- [ ] Implement System Manager
- [ ] Migrate SceneObject to components
- [ ] Write 20+ tests

---

## 💡 Tips for New Contributors

1. **Start with**: `README.md` → `docs/ARCHITECTURE.md` → `docs/DEVELOPMENT.md`
2. **Understand flow**: `Application::Run()` → `Layer::OnUpdate()` → `EditorLayer::OnImGuiRender()`
3. **Key files**: `Application.cpp`, `RendererAPI.h`, `EditorLayer.cpp`
4. **Run tests**: Ensure 37/37 passing before changes
5. **Follow style**: PascalCase classes, camelCase variables, m_PascalCase members
6. **Use smart pointers**: `Ref<T>`, `Scope<T>` (never raw `new`/`delete`)
7. **Log everything**: `VEST_CORE_INFO`, `VEST_CORE_ERROR` (not `std::cout`)
8. **Write tests**: For every new feature, add at least 1 test
9. **Ask questions**: Open GitHub issue if stuck

---

## 📞 Contact & Resources

**GitHub**: [VestEngine Repository](https://github.com/yourusername/VestEngine)  
**Issues**: Report bugs, request features  
**Discussions**: Ask questions, propose ideas

**Documentation**:
- `README.md`: Project overview
- `CHANGELOG.md`: Version history
- `docs/ARCHITECTURE.md`: Technical design
- `docs/ANALYSIS.md`: Current state analysis
- `docs/PLAN.md`: Roadmap and backlog
- `docs/DEVELOPMENT.md`: Developer guide
- `docs/CONTEXT.md`: This file (quick reference)

**References**:
- [Cherno's Game Engine Series](https://www.youtube.com/playlist?list=PLlrATfBNZ98dC-V-N3m0Go4deliWHPFwT)
- [Learn OpenGL](https://learnopengl.com/)
- [Vulkan Tutorial](https://vulkan-tutorial.com/)
- [EnTT Documentation](https://github.com/skypjack/entt)

---

## 🏆 Project Stats

- **Total Commits**: ~20 (across all phases)
- **Lines of Code**: ~10,200 (engine + editor + tests)
- **Test Coverage**: ~40-50% (target: >70%)
- **Contributors**: 1 (open to more!)
- **License**: MIT
- **Development Time**: ~3 months (Phase 0-2)

---

**Última actualización**: 15 de noviembre de 2025  
**Autor**: VestEngine Team  
**Versión del documento**: 1.0  
**Next Review**: After Phase 3 completion (January 2025)
