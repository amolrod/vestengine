# 🎮 Motor Gráfico 3D - C++20 Modular Engine

Motor gráfico moderno desarrollado en C++20 con arquitectura modular y diseño orientado a datos.

## 🎯 Estado del Proyecto

### ✅ FASE 1 COMPLETADA: Core Engine
- [x] Sistema de logging (spdlog)
- [x] Gestión de ventanas (GLFW + OpenGL)
- [x] Sistema de tiempo (DeltaTime, FPS)
- [x] Input polling (teclado y ratón)
- [x] Application framework (game loop)
- [x] CMake multiplataforma

### 🚧 Próximas Fases
- [ ] **FASE 2**: Rendering (Shaders, Meshes, Texturas, Cámara)
- [ ] **FASE 3**: ECS (Entity Component System con EnTT)
- [ ] **FASE 4**: Asset Pipeline
- [ ] **FASE 5**: Physics básica
- [ ] **FASE 6**: Scene Management
- [ ] **FASE 7**: Advanced Rendering
- [ ] **FASE 8**: ImGui Integration

## 🏗️ Estructura del Proyecto

```
motor-grafico/
├── CMakeLists.txt              # Configuración CMake principal
├── src/                        # Código fuente del motor
│   ├── core/                   # Núcleo (Engine, Window, Time, Logger)
│   ├── rendering/              # Sistema de renderizado (TODO)
│   ├── ecs/                    # Entity Component System (TODO)
│   ├── physics/                # Sistema de física (TODO)
│   ├── input/                  # Sistema de input
│   ├── scene/                  # Gestión de escenas (TODO)
│   ├── assets/                 # Carga de assets (TODO)
│   └── utils/                  # Utilidades (TODO)
├── external/                   # Dependencias externas
│   ├── glfw/                   # Window management
│   ├── glad/                   # OpenGL loader
│   ├── glm/                    # Math library
│   └── spdlog/                 # Logging
├── sandbox/                    # Aplicación de prueba
├── assets/                     # Recursos (modelos, texturas, etc.)
├── shaders/                    # Shaders GLSL
└── tests/                      # Tests unitarios (TODO)
```

## 🚀 Compilación

### Requisitos Previos

**macOS:**
```bash
# Instalar Xcode Command Line Tools
xcode-select --install

# CMake (vía Homebrew)
brew install cmake
```

**Linux (Ubuntu/Debian):**
```bash
sudo apt update
sudo apt install build-essential cmake libx11-dev libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev libgl1-mesa-dev
```

**Windows:**
- Visual Studio 2022 o MinGW-w64
- CMake 3.20+

### Pasos de Compilación

```bash
# Clonar el repositorio
cd motor-grafico

# Crear directorio de build
mkdir build && cd build

# Generar archivos de build (Release)
cmake .. -DCMAKE_BUILD_TYPE=Release

# Compilar
cmake --build . --config Release

# Ejecutar Sandbox
./bin/Sandbox  # macOS/Linux
# o
bin\Sandbox.exe  # Windows
```

### Compilación en Debug

```bash
cmake .. -DCMAKE_BUILD_TYPE=Debug
cmake --build . --config Debug
./bin/Sandbox
```

## 🎮 Sandbox - Controles

La aplicación de prueba `Sandbox` demuestra las capacidades actuales del motor:

| Tecla | Acción |
|-------|--------|
| **ESC** | Cerrar aplicación |
| **1-4** | Cambiar color de fondo (Azul, Verde, Rojo, Gris) |
| **SPACE** | Color de fondo aleatorio |
| **F1** | Toggle VSync |
| **Mouse** | Visualizar posición (con click izquierdo) |

## 📚 Arquitectura

### Principios de Diseño

- **SOLID**: Cada módulo respeta Single Responsibility, Open/Closed, etc.
- **Data-Oriented Design**: Componentes como POD structs, cache-friendly
- **Separation of Concerns**: Capas bien definidas sin acoplamiento fuerte
- **Modern C++20**: Smart pointers, constexpr, concepts, modules

### Patrones Aplicados

| Patrón | Ubicación | Propósito |
|--------|-----------|-----------|
| **Singleton** | `Application` | Una sola instancia de app |
| **Template Method** | `Application::Run()` | Game loop con hooks virtuales |
| **Facade** | `Window` | Oculta complejidad de GLFW |
| **Observer** | Sistema de eventos (TODO) | Desacoplar notificaciones |

## 🧪 Testing

```bash
# Compilar con tests
cmake .. -DBUILD_TESTS=ON
cmake --build .

# Ejecutar tests
ctest --output-on-failure
```

## 📖 Documentación

### Uso Básico

Crear tu propia aplicación heredando de `Application`:

```cpp
#include <core/Application.h>
#include <core/EntryPoint.h>

class MyGame : public Engine::Application {
public:
    MyGame() : Application(Engine::WindowProps("Mi Juego", 1920, 1080)) {}
    
    void OnInit() override {
        // Inicializar recursos
    }
    
    void OnUpdate(float deltaTime) override {
        // Lógica del juego
    }
    
    void OnRender() override {
        // Renderizado
    }
};

Engine::Application* CreateApplication() {
    return new MyGame();
}
```

### Logging

```cpp
LOG_INFO("Mensaje informativo");
LOG_WARN("Advertencia: {}", variable);
LOG_ERROR("Error al cargar {}", filename);
```

### Input

```cpp
if (Input::IsKeyPressed(Key::W)) {
    player.MoveForward(Time::DeltaTime());
}

glm::vec2 mousePos = Input::GetMousePosition();
```

## 🤝 Contribuir

Este es un proyecto de portfolio personal, pero sugerencias y mejoras son bienvenidas.

## 📜 Licencia

MIT License - Ver `LICENSE` para más detalles.

## 📧 Contacto

- **Autor**: Ángel
- **Curso**: 2º DAW

---

**Motor Gráfico 3D** - C++20 Modular Engine © 2025
