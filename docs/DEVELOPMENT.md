# VestEngine Development Guide

**Última actualización**: 15 de noviembre de 2025  
**Versión**: 0.2.0-alpha  
**Audience**: New contributors, developers, AI assistants

---

## 📋 Tabla de Contenidos

- [Getting Started](#getting-started)
- [Development Environment](#development-environment)
- [Building the Project](#building-the-project)
- [Project Structure](#project-structure)
- [Coding Standards](#coding-standards)
- [Git Workflow](#git-workflow)
- [Testing](#testing)
- [Debugging](#debugging)
- [Common Issues](#common-issues)
- [Contributing](#contributing)

---

## Getting Started

### Prerequisites

**Required Software**:

| Tool | Version | Purpose |
|------|---------|---------|
| **CMake** | 3.20+ | Build system |
| **C++ Compiler** | C++20 support | Compilation |
| **Git** | 2.30+ | Version control |

**Platform-Specific Compilers**:

| Platform | Compiler | Notes |
|----------|----------|-------|
| **Windows** | MSVC 2022+ o MinGW | Visual Studio 2022 recomendado |
| **macOS** | Apple Clang 14+ | Incluido con Xcode Command Line Tools |
| **Linux** | GCC 11+ o Clang 14+ | `sudo apt install build-essential cmake` |

**Optional Tools**:
- **IDE**: VS Code, Visual Studio, CLion, Xcode
- **Debugger**: GDB (Linux), LLDB (macOS), MSVC Debugger (Windows)
- **Profiler**: Instruments (macOS), Visual Studio Profiler, Tracy

---

### Clone the Repository

```bash
git clone https://github.com/amolrod/VestEngine.git
cd VestEngine
```

**Note**: No necesitas clonar submodules, CMake descarga todas las dependencias automáticamente con `FetchContent`.

---

## Development Environment

### VS Code (Recommended)

**Extensions**:
```json
{
  "recommendations": [
    "ms-vscode.cpptools",           // C++ IntelliSense
    "ms-vscode.cmake-tools",        // CMake integration
    "twxs.cmake",                   // CMake syntax
    "llvm-vs-code-extensions.vscode-clangd", // Advanced C++ support
    "eamodio.gitlens",              // Git visualization
    "vadimcn.vscode-lldb"           // macOS/Linux debugger
  ]
}
```

**Settings** (`.vscode/settings.json`):
```json
{
  "cmake.configureOnOpen": true,
  "cmake.buildDirectory": "${workspaceFolder}/build",
  "C_Cpp.default.configurationProvider": "ms-vscode.cmake-tools",
  "files.associations": {
    "*.h": "cpp",
    "*.cpp": "cpp"
  },
  "editor.formatOnSave": true,
  "C_Cpp.clang_format_fallbackStyle": "Google"
}
```

---

### Visual Studio 2022 (Windows)

**Steps**:
1. Install Visual Studio 2022 con "Desktop development with C++"
2. Open folder: `File → Open → Folder` (select VestEngine root)
3. Wait for CMake configuration (automatic)
4. Select startup item: `VestEditor.exe`
5. Press F5 to build & run

---

### CLion (Cross-Platform)

**Steps**:
1. `File → Open` (select VestEngine root)
2. CLion detecta CMakeLists.txt automáticamente
3. Configure CMake profile:
   - Build type: Debug o Release
   - Toolchain: Default (system compiler)
4. Run configurations auto-generadas: `VestEditor`, `VestTests`

---

### Xcode (macOS)

**Generate Xcode Project**:
```bash
cd VestEngine
mkdir build
cd build
cmake -G Xcode ..
open VestEngine.xcodeproj
```

**Set Scheme**: Select `VestEditor` scheme, then Product → Run

---

## Building the Project

### Quick Build (All Platforms)

```bash
# 1. Configure CMake (first time only)
cmake -B build

# 2. Build
cmake --build build --config Debug -j8

# 3. Run Editor
./build/Editor/VestEditor        # macOS/Linux
.\build\Editor\Debug\VestEditor.exe  # Windows
```

**Explanation**:
- `-B build`: Out-of-source build directory
- `--config Debug`: Build type (Debug/Release/RelWithDebInfo)
- `-j8`: Parallel compilation (8 cores, adjust to your CPU)

---

### Platform-Specific Commands

#### macOS / Linux

```bash
# Clean build
rm -rf build
cmake -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build -j$(nproc)

# Run editor
./build/Editor/VestEditor

# Run tests
./build/Tests/VestTests
```

#### Windows (PowerShell)

```powershell
# Clean build
Remove-Item -Recurse -Force build -ErrorAction SilentlyContinue
cmake -B build -G "Visual Studio 17 2022"
cmake --build build --config Debug -j8

# Run editor
.\build\Editor\Debug\VestEditor.exe

# Run tests
.\build\Tests\Debug\VestTests.exe
```

---

### Build Types

| Build Type | Optimizations | Debug Info | Use Case |
|------------|--------------|------------|----------|
| **Debug** | None (-O0) | Full | Development, debugging |
| **Release** | Max (-O3) | None | Production, benchmarks |
| **RelWithDebInfo** | Moderate (-O2) | Full | Profiling |
| **MinSizeRel** | Size (-Os) | None | Embedded/web builds |

**Set Build Type**:
```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release
```

---

### CMake Options

**Custom Build Configuration**:

```bash
cmake -B build \
  -DCMAKE_BUILD_TYPE=Debug \
  -DVEST_BUILD_TESTS=ON \
  -DVEST_ENABLE_ASSERTS=ON \
  -DVEST_ENABLE_LOGGING=ON
```

**Available Options** (defined in root `CMakeLists.txt`):
- `VEST_BUILD_TESTS`: Enable/disable Google Test (default: ON)
- `VEST_ENABLE_ASSERTS`: Enable runtime assertions (default: ON in Debug)
- `VEST_ENABLE_LOGGING`: Enable spdlog (default: ON)

---

## Project Structure

### Top-Level Organization

```
VestEngine/
├── VestEngine/         ← Engine library (core, rendering, platform)
├── Editor/             ← Editor application (UI, panels, commands)
├── Tests/              ← Unit tests (Google Test)
├── external/           ← Third-party code (glad, stb, imguizmo)
├── assets/             ← Runtime assets (textures, scenes, shaders)
├── build/              ← Build artifacts (gitignored)
├── docs/               ← Documentation (MD files)
├── CMakeLists.txt      ← Root CMake configuration
└── README.md           ← Project README
```

---

### VestEngine Module Structure

```
VestEngine/src/
├── Core/
│   ├── Application.h/cpp       ← Main app singleton, layer management
│   ├── Layer.h/cpp             ← Layer interface
│   ├── LayerStack.h/cpp        ← Layer ordering
│   ├── Input.h/cpp             ← Input polling system
│   ├── Timestep.h              ← Delta time wrapper
│   ├── Log.h/cpp               ← spdlog initialization
│   └── Events/                 ← Event system (mouse, keyboard, window)
├── Rendering/
│   ├── RendererAPI.h/cpp       ← Rendering abstraction interface
│   ├── RenderCommand.h         ← Static facade for RendererAPI
│   ├── Renderer.h/cpp          ← High-level scene renderer
│   ├── Shader.h/cpp            ← Shader compilation, uniforms
│   ├── Texture.h/cpp           ← Texture loading (stb_image)
│   ├── VertexArray.h/cpp       ← VAO abstraction
│   ├── Buffer.h/cpp            ← VBO/IBO abstraction
│   ├── Framebuffer.h/cpp       ← FBO for render-to-texture
│   └── Platform/
│       ├── OpenGL/             ← OpenGL implementation (✅ Complete)
│       └── Vulkan/             ← Vulkan stubs (⚠️ Not implemented)
├── ImGui/
│   ├── ImGuiLayer.h/cpp        ← ImGui integration layer
│   └── ImGuiBuild.cpp          ← ImGui implementation compilation
├── Platform/
│   └── GLFW/
│       └── GLFWWindow.h/cpp    ← GLFW window implementation
├── Scene/
│   └── SceneObject.h           ← Simple scene object (pre-ECS)
└── Serialization/
    └── SceneSerializer.h/cpp   ← JSON scene save/load
```

**Key Files to Understand**:
1. `Core/Application.cpp`: Main loop, event handling
2. `Rendering/RendererAPI.h`: Abstraction interface (learn this first)
3. `Platform/OpenGL/OpenGLRendererAPI.cpp`: Concrete implementation
4. `ImGui/ImGuiLayer.cpp`: UI integration

---

### Editor Module Structure

```
Editor/src/
├── EditorApplication.cpp       ← main() entry point
├── EditorLayer.h/cpp           ← Main editor logic (viewport, panels, state)
├── EditorCamera.h/cpp          ← Smooth pan/zoom camera
├── Commands/
│   ├── ICommand.h              ← Command interface (undo/redo)
│   ├── CommandManager.h/cpp    ← Undo/redo stack
│   ├── TransformCommand.h/cpp  ← Move/rotate/scale entity
│   ├── CreateEntityCommand.h/cpp
│   ├── DeleteEntityCommand.h/cpp
│   └── MacroCommand.h/cpp      ← Group multiple commands
├── Gizmo/
│   └── (ImGuizmo integration)  ← Translate/rotate/scale gizmos
├── Panels/
│   ├── ViewportPanel.h/cpp     ← 3D viewport rendering
│   ├── SceneHierarchyPanel.h/cpp ← Entity list
│   ├── PropertiesPanel.h/cpp   ← Entity properties editor
│   ├── ContentBrowserPanel.h/cpp ← Asset browser
│   └── StatsPanel.h/cpp        ← FPS, object count, etc.
└── Rendering/
    ├── SelectionRenderer.h/cpp ← Hover/selection outlines
    └── GridRenderer.h/cpp      ← Adaptive grid + snap tools
```

**Entry Point Flow**:
```cpp
// EditorApplication.cpp
int main() {
    Application* app = new Application();
    app->PushLayer(new EditorLayer());
    app->PushOverlay(new ImGuiLayer());
    app->Run();  // Main loop
    delete app;
    return 0;
}
```

---

## Coding Standards

### C++ Style Guide

**We follow Google C++ Style Guide** with minor modifications.

**Naming Conventions**:

| Type | Convention | Example |
|------|------------|---------|
| **Classes** | PascalCase | `EditorLayer`, `RendererAPI` |
| **Functions** | PascalCase | `void OnUpdate()`, `int GetWidth()` |
| **Variables** | camelCase | `int entityCount`, `float deltaTime` |
| **Member Variables** | m_PascalCase | `int m_Width`, `float m_ZoomSpeed` |
| **Constants** | UPPER_CASE | `const int MAX_LIGHTS = 8` |
| **Namespaces** | PascalCase | `namespace Vest { }` |
| **Enums** | PascalCase | `enum class ShaderType { Vertex, Fragment }` |

**Example**:
```cpp
namespace Vest {

class Shader {
public:
    // Public methods: PascalCase
    void Bind() const;
    void Unbind() const;
    void SetInt(const std::string& name, int value);
    
    // Factory method
    static Ref<Shader> Create(const std::string& filepath);
    
private:
    // Member variables: m_PascalCase
    uint32_t m_RendererID;
    std::string m_Name;
    std::unordered_map<std::string, int> m_UniformLocationCache;
    
    // Private helper: PascalCase
    int GetUniformLocation(const std::string& name);
};

} // namespace Vest
```

---

### File Organization

**Header File** (`.h`):
```cpp
#pragma once  // Preferred over include guards

#include <VestEngine.h>  // Engine headers first
#include <string>        // Then STL
#include <glm/glm.hpp>   // Then third-party

namespace Vest {

class MyClass {
public:
    // Public interface first
    MyClass();
    ~MyClass();
    
    void PublicMethod();
    
private:
    // Private implementation last
    void PrivateMethod();
    
    // Member variables last
    int m_MemberVariable;
};

} // namespace Vest
```

**Implementation File** (`.cpp`):
```cpp
#include "MyClass.h"  // Own header first

#include <iostream>   // Then dependencies

namespace Vest {

MyClass::MyClass()
    : m_MemberVariable(0)  // Initializer list
{
    // Constructor body
}

void MyClass::PublicMethod() {
    // Implementation
}

} // namespace Vest
```

---

### Smart Pointers

**Always use smart pointers, never raw `new`/`delete`**:

```cpp
// Engine aliases (see Core/Base.h):
template<typename T>
using Scope = std::unique_ptr<T>;

template<typename T>
using Ref = std::shared_ptr<T>;

// Usage:
Ref<Shader> shader = Shader::Create("shader.glsl");
Scope<VertexBuffer> vbo = CreateScope<VertexBuffer>(vertices, size);

// BAD ❌
Shader* shader = new Shader();  // Manual memory management
delete shader;

// GOOD ✅
Ref<Shader> shader = Shader::Create();  // Automatic cleanup
```

---

### Error Handling

**Use assertions for programmer errors**:
```cpp
#include "Core/Assert.h"

void SetViewport(uint32_t width, uint32_t height) {
    VEST_CORE_ASSERT(width > 0, "Viewport width must be > 0");
    VEST_CORE_ASSERT(height > 0, "Viewport height must be > 0");
    // ...
}
```

**Use logging for runtime issues**:
```cpp
#include "Core/Log.h"

Ref<Texture2D> Texture2D::Create(const std::string& path) {
    if (!std::filesystem::exists(path)) {
        VEST_CORE_ERROR("Texture file not found: {}", path);
        return nullptr;  // Return nullptr, don't crash
    }
    // ...
}
```

**Future**: Consider `std::optional` or exceptions for recoverable errors.

---

### Logging Guidelines

**Use appropriate log levels**:

```cpp
VEST_CORE_TRACE("Low-level debug info (disabled in Release)");
VEST_CORE_INFO("Initialization complete, window size: {}x{}", width, height);
VEST_CORE_WARN("Shader compilation warning: {}", warning);
VEST_CORE_ERROR("Failed to load texture: {}", filepath);
VEST_CORE_CRITICAL("Out of memory, cannot continue");

// Client logs (for game/editor code):
VEST_TRACE("Player position: ({}, {})", x, y);
VEST_INFO("Level loaded successfully");
VEST_WARN("Low health: {}", health);
VEST_ERROR("Save file corrupted");
```

**Rules**:
- ❌ Don't log in tight loops (every frame = spam)
- ✅ Log important events (initialization, errors, user actions)
- ✅ Use formatting: `VEST_INFO("FPS: {}", fps)` (not `std::cout`)

---

### Comments and Documentation

**Use Doxygen-style comments for public APIs**:

```cpp
/**
 * @brief Renders an adaptive grid with major and minor lines.
 * 
 * The grid adapts its spacing based on camera zoom to maintain
 * visual clarity at any scale.
 * 
 * @param viewportSize Viewport size in pixels (width, height)
 * @param camera Editor camera providing position and zoom
 * @param settings Grid configuration (color, spacing, etc.)
 * 
 * @note Grid is only rendered in Edit mode.
 * @see GridSettings for configuration options
 */
void RenderGrid(glm::vec2 viewportSize, 
                EditorCamera& camera,
                const GridSettings& settings);
```

**Inline comments for complex logic**:
```cpp
// Calculate adaptive grid spacing based on zoom
// At zoom 1.0: 50px spacing
// At zoom 10.0: 20px spacing (more detail)
float baseSpacing = 50.0f;
float adaptiveSpacing = baseSpacing / std::sqrt(camera.GetZoom());
adaptiveSpacing = std::clamp(adaptiveSpacing, 20.0f, 100.0f);
```

**Avoid obvious comments**:
```cpp
// BAD ❌
int count = 0;  // Initialize count to 0

// GOOD ✅
int entityCount = 0;  // No comment needed, name is clear
```

---

## Git Workflow

### Branching Strategy

**Main Branches**:
- `main`: Production-ready code, tagged releases only
- `develop`: Integration branch for features (currently we merge feature branches to main directly)

**Feature Branches**:
```
feature/phase2-editor-features  ← Phase 2 work
feature/phase3-ecs              ← Phase 3 (future)
feature/vulkan-backend          ← Phase 5 (future)
bugfix/grid-crash-fix           ← Bugfixes
hotfix/critical-memory-leak     ← Critical production fixes
```

**Naming**:
- `feature/<descriptive-name>`: New features
- `bugfix/<issue-number-or-description>`: Bug fixes
- `hotfix/<critical-issue>`: Emergency fixes
- `refactor/<what>`: Code refactoring
- `docs/<what>`: Documentation updates

---

### Commit Messages

**Format** (Conventional Commits):
```
<type>(<scope>): <subject>

<body>

<footer>
```

**Types**:
- `feat`: New feature
- `fix`: Bug fix
- `refactor`: Code refactoring (no feature change)
- `docs`: Documentation changes
- `test`: Adding/fixing tests
- `style`: Code style changes (formatting, no logic change)
- `perf`: Performance improvements
- `chore`: Maintenance tasks (dependencies, build config)

**Examples**:
```bash
git commit -m "feat(editor): Add Play/Pause/Stop mode system"
git commit -m "fix(rendering): Fix grid crash with zero-size viewport"
git commit -m "refactor(commands): Extract MacroCommand to separate file"
git commit -m "docs: Update README with Phase 2 completion status"
git commit -m "test(serialization): Add JSON versioning tests"
```

**Good Commit Practices**:
- ✅ Atomic commits (one logical change per commit)
- ✅ Clear, descriptive messages
- ✅ Reference issues: `fix(rendering): Fix #42 - Framebuffer leak`
- ❌ Avoid "WIP", "temp", "asdf" commits in history (squash before merge)

---

### Pull Request Process

**Before Creating PR**:
1. ✅ All tests passing (`./build/Tests/VestTests`)
2. ✅ Code compiles without warnings
3. ✅ Code formatted (follow style guide)
4. ✅ Documentation updated (if adding features)
5. ✅ Commit messages follow conventions

**PR Template**:
```markdown
## Description
Brief description of changes.

## Changes
- Added X feature
- Fixed Y bug
- Refactored Z

## Testing
- [ ] Unit tests added/updated
- [ ] Manual testing performed
- [ ] No regressions found

## Screenshots (if UI changes)
![Screenshot](url)

## Related Issues
Closes #42
```

---

## Testing

### Running Tests

```bash
# Build tests
cmake --build build --target VestTests

# Run all tests
./build/Tests/VestTests  # macOS/Linux
.\build\Tests\Debug\VestTests.exe  # Windows

# Run specific test suite
./build/Tests/VestTests --gtest_filter=CommandTests.*

# Run with verbose output
./build/Tests/VestTests --gtest_verbose=1
```

---

### Writing Tests

**Test File Structure**:
```cpp
// Tests/Commands/CommandTests.cpp
#include <gtest/gtest.h>
#include "Commands/TransformCommand.h"

// Test fixture (optional, for shared setup)
class TransformCommandTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Setup before each test
        entity = SceneObject{
            .name = "TestEntity",
            .position = glm::vec3(0.0f)
        };
    }
    
    SceneObject entity;
};

// Test case
TEST_F(TransformCommandTest, ExecuteSetsPosition) {
    glm::vec3 newPos(1.0f, 2.0f, 3.0f);
    TransformCommand cmd(entity, {newPos, {}, {}});
    
    cmd.Execute();
    
    ASSERT_EQ(entity.position, newPos);
}

TEST_F(TransformCommandTest, UndoRestoresOriginalPosition) {
    glm::vec3 originalPos = entity.position;
    glm::vec3 newPos(1.0f, 2.0f, 3.0f);
    
    TransformCommand cmd(entity, {newPos, {}, {}});
    cmd.Execute();
    cmd.Undo();
    
    ASSERT_EQ(entity.position, originalPos);
}
```

**Naming Conventions**:
- Test suite: `ClassNameTest`
- Test case: `MethodName_StateUnderTest_ExpectedBehavior`

**Example**:
```cpp
TEST(ShaderTest, Compile_ValidGLSL_ReturnsNonZeroID) { ... }
TEST(ShaderTest, Compile_InvalidGLSL_LogsError) { ... }
TEST(TextureTest, Load_NonExistentFile_ReturnsNullptr) { ... }
```

---

### Test Coverage Goals

| Module | Current | Target |
|--------|---------|--------|
| Commands | 80% | 90% |
| Serialization | 60% | 80% |
| Logging | 90% | 95% |
| Rendering | 10% | 60% |
| **Overall** | ~40% | >70% |

---

## Debugging

### Common Debugging Tools

#### GDB (Linux)

```bash
# Build with debug symbols
cmake -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build

# Run with GDB
gdb ./build/Editor/VestEditor

# GDB commands:
(gdb) break EditorLayer::OnUpdate  # Set breakpoint
(gdb) run                          # Run program
(gdb) bt                           # Backtrace (stack trace)
(gdb) print m_SelectedEntityIndex  # Print variable
(gdb) continue                     # Continue execution
```

---

#### LLDB (macOS)

```bash
# Build with debug symbols
cmake -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build

# Run with LLDB
lldb ./build/Editor/VestEditor

# LLDB commands:
(lldb) breakpoint set --name EditorLayer::OnUpdate
(lldb) run
(lldb) bt         # Backtrace
(lldb) frame variable m_SelectedEntityIndex
(lldb) continue
```

---

#### Visual Studio (Windows)

1. Open folder in Visual Studio
2. Set breakpoint: Click left margin in code
3. Press F5 (Start Debugging)
4. Debug windows:
   - **Locals**: View variables in current scope
   - **Call Stack**: View function calls
   - **Autos**: View variables used near current line
   - **Watch**: Add custom expressions

---

### Debugging OpenGL

**Enable Debug Context** (in `Platform/GLFW/GLFWWindow.cpp`):

```cpp
#ifdef VEST_DEBUG
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif
```

**Set Debug Callback**:
```cpp
// In OpenGLRendererAPI::Init()
glEnable(GL_DEBUG_OUTPUT);
glDebugMessageCallback([](GLenum source, GLenum type, GLuint id, 
                          GLenum severity, GLsizei length, 
                          const GLchar* message, const void* userParam) {
    if (severity == GL_DEBUG_SEVERITY_HIGH) {
        VEST_CORE_ERROR("OpenGL Error: {}", message);
    }
}, nullptr);
```

**Common OpenGL Errors**:
- `GL_INVALID_ENUM`: Wrong enum passed (ej: `glEnable(999)`)
- `GL_INVALID_VALUE`: Invalid value (ej: negative size)
- `GL_INVALID_OPERATION`: Operation not allowed (ej: drawing without bound VAO)
- `GL_OUT_OF_MEMORY`: Out of GPU memory

**Tools**:
- [RenderDoc](https://renderdoc.org/): Frame capture and analysis (Linux/Windows)
- [Nsight Graphics](https://developer.nvidia.com/nsight-graphics): NVIDIA GPU debugger
- [Instruments](https://developer.apple.com/instruments/): macOS profiling (GPU trace)

---

### Memory Leak Detection

#### Valgrind (Linux)

```bash
# Build with debug symbols
cmake -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build

# Run with Valgrind
valgrind --leak-check=full --show-leak-kinds=all ./build/Editor/VestEditor
```

#### AddressSanitizer (All Platforms)

```bash
# Enable AddressSanitizer
cmake -B build -DCMAKE_BUILD_TYPE=Debug \
  -DCMAKE_CXX_FLAGS="-fsanitize=address -fno-omit-frame-pointer"
cmake --build build

# Run (will report leaks on exit)
./build/Editor/VestEditor
```

---

## Common Issues

### 1. "CMake cannot find GLFW"

**Error**:
```
CMake Error: Could not find package configuration file provided by "glfw3"
```

**Solution**:
CMake debería descargar GLFW con FetchContent. Si falla:
1. Verifica conexión a internet
2. Limpia build: `rm -rf build`
3. Re-configura: `cmake -B build`

---

### 2. "Segmentation Fault at Startup"

**Error**:
```
Segmentation fault (core dumped)
```

**Common Causes**:
- Zero-size buffer creation
- Null pointer dereference
- Invalid OpenGL calls before context creation

**Debug**:
```bash
gdb ./build/Editor/VestEditor
(gdb) run
# ... crash ...
(gdb) bt  # See where it crashed
```

**Recent Fix**: GridRenderer crash → add viewport validation.

---

### 3. "Shader Compilation Failed"

**Error in logs**:
```
[ERROR] Shader compilation failed: 
0:10(5): error: syntax error
```

**Solution**:
1. Check shader file syntax (line 10, column 5)
2. Verify GLSL version matches OpenGL context:
   ```glsl
   #version 410 core  // OpenGL 4.1 (macOS max)
   ```
3. Check shader logs:
   ```cpp
   VEST_CORE_ERROR("Vertex shader: {}", glGetShaderInfoLog(...));
   ```

---

### 4. "ImGui Assertion Failed"

**Error**:
```
imgui.cpp:1234: Assertion `g.WithinFrameScope` failed
```

**Cause**: ImGui function called outside `NewFrame()`/`Render()`.

**Solution**: Ensure all ImGui calls are in `OnImGuiRender()`:
```cpp
void EditorLayer::OnImGuiRender() {
    ImGui::Begin("Window");
    // ... ImGui calls here ...
    ImGui::End();
}
```

---

### 5. "Black Screen in Viewport"

**Possible Causes**:
1. Framebuffer not bound correctly
2. Clear color = background color (all black)
3. Camera position/zoom invalid
4. No objects in scene
5. Shader not bound

**Debug Checklist**:
```cpp
// 1. Verify framebuffer
VEST_CORE_INFO("Framebuffer ID: {}", m_Framebuffer->GetRendererID());

// 2. Check clear color
RenderCommand::SetClearColor({0.2f, 0.2f, 0.2f, 1.0f});  // Gray, not black

// 3. Log camera
VEST_CORE_INFO("Camera pos: ({}, {}), zoom: {}", pos.x, pos.y, zoom);

// 4. Verify objects
VEST_CORE_INFO("Scene has {} objects", m_SceneObjects.size());

// 5. Check shader binding
shader->Bind();
VEST_CORE_INFO("Shader bound: {}", shader->GetName());
```

---

## Contributing

### Code Review Checklist

**Before submitting PR**:
- [ ] Code compiles without warnings (`-Wall -Wextra`)
- [ ] All tests pass (37/37)
- [ ] No memory leaks (Valgrind/ASan)
- [ ] Code follows style guide (naming, formatting)
- [ ] Public APIs documented (Doxygen comments)
- [ ] Commit messages follow conventions
- [ ] No `TODO` or `FIXME` without issue reference

---

### Getting Help

**Resources**:
- **Documentation**: See `docs/` folder (ARCHITECTURE.md, PLAN.md, etc.)
- **Code Examples**: Browse existing code in `VestEngine/` and `Editor/`
- **Issues**: Check GitHub issues for known problems
- **Logs**: Enable verbose logging (`VEST_CORE_TRACE` in Debug)

**Contact**:
- GitHub Issues: Report bugs, request features
- Discussions: Ask questions, propose ideas

---

### License

VestEngine is licensed under the **MIT License**. See `LICENSE` file.

**Contribution Agreement**:
By contributing, you agree to license your contributions under MIT.

---

**Última actualización**: 15 de noviembre de 2025  
**Autor**: VestEngine Team  
**Versión del documento**: 1.0
