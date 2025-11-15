# VestEngine Development Plan

**Última actualización**: 15 de noviembre de 2025  
**Versión**: 0.2.0-alpha  
**Roadmap hasta**: v1.0.0 (estimado: Q3 2026)

---

## 📋 Tabla de Contenidos

- [Estado Actual](#estado-actual)
- [Roadmap General](#roadmap-general)
- [Phase 3: Entity Component System](#phase-3-entity-component-system)
- [Phase 4: Advanced Rendering](#phase-4-advanced-rendering)
- [Phase 5: Vulkan Implementation](#phase-5-vulkan-implementation)
- [Backlog (Post v1.0)](#backlog-post-v10)
- [Dependencias Bloqueantes](#dependencias-bloqueantes)
- [Decisiones Pendientes](#decisiones-pendientes)
- [Milestones y Fechas](#milestones-y-fechas)

---

## Estado Actual

### Phase Completion Status

```
✅ Phase 0: Initial Setup (v0.0.1-alpha)
✅ Phase 1: Foundation Refactor (v0.1.0-alpha) 
✅ Phase 2: Editor Features (v0.2.0-alpha)  ← YOU ARE HERE
🚧 Phase 3: Entity Component System
⚪ Phase 4: Advanced Rendering
⚪ Phase 5: Vulkan Implementation
```

### Last Completed Work

**Phase 2.5: Play Mode System** (commit `eaf8b24`)
- ✅ Play/Pause/Stop buttons
- ✅ Scene backup/restore
- ✅ Edit protection during play
- ✅ State indicator UI

**Branch**: `feature/phase2-editor-features` (ready to merge)

---

## Roadmap General

### Vision Statement

> **VestEngine v1.0** será un motor de juegos 2D/3D open-source, modular y extensible, con soporte completo para OpenGL/Vulkan, ECS architecture, advanced rendering, y un editor profesional tipo Unity.

### High-Level Timeline

```
Q4 2024 ──────────────────────────── Q1 2025 ──────────────────────────── Q2 2025 ────────────────────────── Q3 2025 ───────────────── Q4 2025
  │                                       │                                   │                                │                            │
  │ Phase 0-1-2                          │ Phase 3                            │ Phase 4                        │ Phase 5                   │ v1.0
  │ (Completado)                         │ (ECS)                              │ (Advanced Rendering)           │ (Vulkan)                  │
  │                                       │                                   │                                │                            │
  └─ v0.0.1, v0.1.0, v0.2.0             └─ v0.3.0-alpha                    └─ v0.4.0-beta                  └─ v0.5.0-rc               └─ v1.0.0
```

**Duración estimada total**: 12-15 meses (desde Phase 3 inicio)

---

## Phase 3: Entity Component System

**Objetivo**: Refactorizar arquitectura de `SceneObject` a ECS moderno (Entity-Component-System).

**Estado**: ⚪ No iniciado  
**Prioridad**: 🔴 Crítica (bloqueante para Phases 4-5)  
**Estimación**: 6-8 semanas  
**Target Release**: v0.3.0-alpha (Enero 2025)

### 3.1: Core ECS Architecture (2 semanas)

**Tasks**:
- [ ] Diseñar API de ECS (Entity, Component, System)
- [ ] Implementar Entity Manager (ID generation, lifecycle)
- [ ] Implementar Component Storage (arrays contiguos)
- [ ] Implementar System Manager (update order, dependencies)
- [ ] Migrar `SceneObject` a componentes básicos:
  - [ ] `TransformComponent`
  - [ ] `RenderComponent`
  - [ ] `TagComponent`
  - [ ] `IDComponent`

**Archivos afectados**:
```
VestEngine/src/Scene/
  ├── Entity.h/cpp           (NEW)
  ├── Component.h            (NEW)
  ├── ComponentStorage.h/cpp (NEW)
  ├── System.h/cpp           (NEW)
  ├── Registry.h/cpp         (NEW)
  └── SceneObject.h/cpp      (REFACTOR → deprecated)
```

**Tests requeridos**:
- [ ] Entity creation/destruction
- [ ] Component add/remove/get
- [ ] System registration/execution
- [ ] Memory efficiency (1M entities < 100MB)

**Referencias**:
- [EnTT (Entity Component Toolkit)](https://github.com/skypjack/entt)
- [Unity DOTS](https://unity.com/dots)
- [Hazel ECS Implementation](https://github.com/TheCherno/Hazel)

---

### 3.2: Editor ECS Integration (2 semanas)

**Tasks**:
- [ ] Actualizar `EditorLayer` para usar Registry
- [ ] Refactorizar panels:
  - [ ] `SceneHierarchyPanel`: Mostrar entidades con componentes
  - [ ] `PropertiesPanel`: Editor por componente (Transform, Render, etc.)
  - [ ] `AddComponentButton`: UI para añadir componentes dinámicamente
- [ ] Actualizar comandos:
  - [ ] `CreateEntityCommand`: Usar Registry
  - [ ] `DeleteEntityCommand`: Cleanup de componentes
  - [ ] `AddComponentCommand` (NEW)
  - [ ] `RemoveComponentCommand` (NEW)
- [ ] Serialization:
  - [ ] Guardar/cargar entidades con componentes variables

**UI Mockup** (Properties Panel con componentes):
```
┌─────────────────────────────────────┐
│ Entity: Player                  [X] │
├─────────────────────────────────────┤
│ [+] Add Component                   │
│                                     │
│ ▼ Transform Component          [−] │
│   Position: (0.0, 0.0, 0.0)        │
│   Rotation: (0.0, 0.0, 0.0)        │
│   Scale:    (1.0, 1.0, 1.0)        │
│                                     │
│ ▼ Render Component             [−] │
│   Color: [■] (1.0, 0.0, 0.0, 1.0)  │
│   Mesh:  [Cube ▼]                  │
│   Material: [Default ▼]            │
│                                     │
│ ▼ Tag Component                [−] │
│   Tag: "Player"                    │
└─────────────────────────────────────┘
```

**Tests**:
- [ ] Crear entidad desde editor
- [ ] Añadir/remover componentes desde UI
- [ ] Undo/redo con componentes
- [ ] Serializar escena con componentes mixtos

---

### 3.3: Advanced Asset System (2 semanas)

**Tasks**:
- [ ] Implementar `AssetManager`:
  - [ ] Registry de assets (Texture, Shader, Mesh, Material)
  - [ ] Asset loading/unloading
  - [ ] Reference counting
  - [ ] Hot-reloading (watch file changes)
- [ ] Mejorar `ContentBrowserPanel`:
  - [ ] Previews de texturas/modelos
  - [ ] Drag-and-drop a scene
  - [ ] Import settings UI
  - [ ] Búsqueda y filtros
- [ ] Implementar `MaterialSystem`:
  - [ ] Material editor (albedo, normal, roughness, metallic)
  - [ ] Material preview sphere
  - [ ] Shader hot-reloading

**Assets soportados**:
| Tipo | Formatos | Features |
|------|----------|----------|
| **Textures** | PNG, JPG, TGA | Mipmaps, compression, import settings |
| **Meshes** | OBJ, FBX (futuro) | LOD, bounding boxes |
| **Shaders** | .glsl | Hot-reload, error reporting |
| **Materials** | .mat (JSON) | PBR parameters, texture slots |
| **Scenes** | .scene (JSON) | Versioning, partial loading |

**Tests**:
- [ ] Load 1000 textures (memory usage < 500MB)
- [ ] Hot-reload shader sin crashear
- [ ] Drag-and-drop texture a entity
- [ ] Material editor updates in real-time

---

### 3.4: Native Scripting (2 semanas)

**Tasks**:
- [ ] Implementar `ScriptComponent`:
  - [ ] Base class `ScriptableEntity`
  - [ ] Virtual methods: `OnCreate()`, `OnUpdate()`, `OnDestroy()`
  - [ ] Acceso a componentes: `GetComponent<T>()`
- [ ] Implementar `ScriptingSystem`:
  - [ ] Registro de scripts
  - [ ] Update loop con delta time
  - [ ] Hot-reloading de scripts (C++ compilation)
- [ ] Editor UI:
  - [ ] Attach script a entity
  - [ ] Script parameters inspector
  - [ ] Play mode testing

**Ejemplo de script**:
```cpp
// PlayerController.h
class PlayerController : public ScriptableEntity {
public:
    float m_Speed = 5.0f;
    
    void OnCreate() override {
        VEST_INFO("Player created");
    }
    
    void OnUpdate(Timestep ts) override {
        auto& transform = GetComponent<TransformComponent>();
        
        if (Input::IsKeyPressed(KeyCode::W))
            transform.position.y += m_Speed * ts;
        if (Input::IsKeyPressed(KeyCode::S))
            transform.position.y -= m_Speed * ts;
        if (Input::IsKeyPressed(KeyCode::A))
            transform.position.x -= m_Speed * ts;
        if (Input::IsKeyPressed(KeyCode::D))
            transform.position.x += m_Speed * ts;
    }
};

// En editor:
Entity player = scene.CreateEntity("Player");
player.AddComponent<ScriptComponent>("PlayerController");
```

**Alternativa (futuro)**: Lua/C# scripting (más flexible, sin recompilación)

**Tests**:
- [ ] Script moves entity en Play mode
- [ ] Script accede a componentes correctamente
- [ ] Múltiples scripts en misma entity
- [ ] Script hot-reload sin perder state

---

### Phase 3: Definition of Done

**Criteria**:
- ✅ ECS completamente funcional (Entity, Component, System)
- ✅ Editor usa ECS internamente
- ✅ Asset Manager con hot-reloading
- ✅ Native scripting operativo
- ✅ Todos los tests de Phase 3 pasando (>50 tests)
- ✅ Performance: 10k entidades a >60 FPS
- ✅ Documentación actualizada (ARCHITECTURE.md, ANALYSIS.md)
- ✅ Merge a `main` branch
- ✅ Release v0.3.0-alpha

---

## Phase 4: Advanced Rendering

**Objetivo**: Implementar técnicas de rendering modernas (PBR, shadows, post-processing).

**Estado**: ⚪ No iniciado  
**Prioridad**: 🟡 Alta (tras Phase 3)  
**Estimación**: 8-10 semanas  
**Target Release**: v0.4.0-beta (Abril 2025)

### 4.1: Physically Based Rendering (3 semanas)

**Tasks**:
- [ ] Implementar PBR shader:
  - [ ] Cook-Torrance BRDF
  - [ ] Image-Based Lighting (IBL)
  - [ ] Normal mapping
  - [ ] Parallax mapping (opcional)
- [ ] Material system:
  - [ ] Albedo, Normal, Roughness, Metallic, AO maps
  - [ ] Material editor UI
  - [ ] Material presets (metal, plastic, wood, etc.)
- [ ] Lighting:
  - [ ] Point lights
  - [ ] Directional lights
  - [ ] Spot lights
  - [ ] Light probes (futuro)

**Referencias**:
- [Learn OpenGL - PBR Theory](https://learnopengl.com/PBR/Theory)
- [Real Shading in Unreal Engine 4](https://blog.selfshadow.com/publications/s2013-shading-course/)

**Tests**:
- [ ] Render sphere with PBR material
- [ ] Light positions afectan shading correctamente
- [ ] Normal map aplicado correctamente

---

### 4.2: Shadow Mapping (2 semanas)

**Tasks**:
- [ ] Implementar shadow maps:
  - [ ] Depth pass para cada luz
  - [ ] Shadow map framebuffer
  - [ ] PCF (Percentage Closer Filtering)
  - [ ] Cascade shadow maps para directional lights
- [ ] Editor UI:
  - [ ] Toggle shadows per light
  - [ ] Shadow resolution setting
  - [ ] Shadow bias controls

**Challenges**:
- ⚠️ Shadow acne (solución: depth bias)
- ⚠️ Peter panning (solución: normal offset bias)
- ⚠️ Performance (múltiples shadow maps caros)

---

### 4.3: Post-Processing (2 semanas)

**Tasks**:
- [ ] Implementar post-processing stack:
  - [ ] Bloom (HDR glow)
  - [ ] Tone mapping (HDR → LDR)
  - [ ] Color grading (LUT-based)
  - [ ] Vignette
  - [ ] Chromatic aberration
  - [ ] FXAA (anti-aliasing)
- [ ] Editor UI:
  - [ ] Post-processing panel
  - [ ] Enable/disable effects
  - [ ] Real-time parameter tweaking

**Pipeline**:
```
Scene Render
    ↓
Framebuffer (HDR)
    ↓
Bloom Pass → Blur → Add to original
    ↓
Tone Mapping (Reinhard/ACES)
    ↓
Color Grading (LUT)
    ↓
FXAA
    ↓
Final Output
```

---

### 4.4: Batching & Instancing (2 semanas)

**Tasks**:
- [ ] Implementar batch renderer:
  - [ ] Agrupar objetos con mismo shader/texture
  - [ ] Generar batches dinámicos
  - [ ] Quad batch renderer (2D sprites)
- [ ] Implementar instanced rendering:
  - [ ] `glDrawElementsInstanced` para múltiples copies
  - [ ] Instance buffer con transforms
  - [ ] Grass/trees rendering con instancing
- [ ] Performance benchmarks:
  - [ ] Medir draw calls antes/después
  - [ ] Objetivo: 1000 objetos en 10-20 draw calls

**Resultado esperado**:
```
ANTES:
- 1000 quads → 1000 draw calls → 30 FPS

DESPUÉS:
- 1000 quads → 1 batch → 120 FPS
```

---

### 4.5: Advanced Features (1 semana)

**Tasks**:
- [ ] Frustum culling (no renderizar fuera de cámara)
- [ ] Occlusion culling (básico)
- [ ] LOD (Level of Detail) system
- [ ] Skybox rendering
- [ ] Cubemap support
- [ ] HDR framebuffers

---

### Phase 4: Definition of Done

**Criteria**:
- ✅ PBR materials funcionales
- ✅ Shadow mapping implementado
- ✅ Post-processing stack operativo
- ✅ Batching reduce draw calls >10x
- ✅ Performance: 5k entidades + shadows + post-processing a >60 FPS
- ✅ Documentación técnica completa
- ✅ Release v0.4.0-beta

---

## Phase 5: Vulkan Implementation

**Objetivo**: Portar el rendering backend de OpenGL a Vulkan.

**Estado**: ⚪ No iniciado  
**Prioridad**: 🟡 Media (opcional para v1.0)  
**Estimación**: 10-12 semanas  
**Target Release**: v0.5.0-rc (Julio 2025)

### 5.1: Vulkan Core Setup (3 semanas)

**Tasks**:
- [ ] Vulkan initialization:
  - [ ] Instance creation
  - [ ] Physical device selection
  - [ ] Logical device + queues
  - [ ] Swapchain creation
- [ ] Validation layers:
  - [ ] Enable en Debug builds
  - [ ] Custom debug callback
- [ ] Memory management:
  - [ ] VMA (Vulkan Memory Allocator) integration
  - [ ] Staging buffers
- [ ] Command buffers:
  - [ ] Command pool per thread
  - [ ] Primary/secondary command buffers

**Referencias**:
- [Vulkan Tutorial](https://vulkan-tutorial.com/)
- [Vulkan Guide](https://vkguide.dev/)

---

### 5.2: Vulkan Rendering Pipeline (4 semanas)

**Tasks**:
- [ ] Render passes:
  - [ ] Forward rendering pass
  - [ ] Shadow pass
  - [ ] Post-processing passes
- [ ] Pipelines:
  - [ ] Vertex input state
  - [ ] Shader stages (SPIR-V compilation)
  - [ ] Rasterizer state
  - [ ] Depth/stencil state
- [ ] Descriptors:
  - [ ] Descriptor sets (uniforms, textures)
  - [ ] Push constants
  - [ ] Descriptor pools
- [ ] Synchronization:
  - [ ] Semaphores (GPU-GPU sync)
  - [ ] Fences (CPU-GPU sync)
  - [ ] Pipeline barriers

---

### 5.3: Vulkan Resource Management (2 semanas)

**Tasks**:
- [ ] `VulkanVertexBuffer`, `VulkanIndexBuffer`
- [ ] `VulkanTexture2D` (image views, samplers)
- [ ] `VulkanShader` (SPIR-V compilation con glslangValidator)
- [ ] `VulkanFramebuffer` (render targets)
- [ ] `VulkanUniformBuffer` (descriptor updates)

---

### 5.4: Vulkan Features Parity (3 semanas)

**Tasks**:
- [ ] Implementar todas las features de OpenGL backend:
  - [ ] PBR rendering
  - [ ] Shadow mapping
  - [ ] Post-processing
  - [ ] Batching (indirect drawing)
- [ ] Performance tuning:
  - [ ] Multi-threaded command buffer recording
  - [ ] Pipeline caching
  - [ ] Memory aliasing
- [ ] Benchmarks:
  - [ ] Comparar OpenGL vs Vulkan (FPS, frame time)

**Resultado esperado**:
```
Scene: 10k entities + PBR + shadows

OpenGL:
- FPS: 60-80
- Frame time: 12-16ms
- CPU usage: 40%

Vulkan:
- FPS: 100-120  (+50%)
- Frame time: 8-10ms
- CPU usage: 25%  (multi-threaded)
```

---

### Phase 5: Definition of Done

**Criteria**:
- ✅ Vulkan backend feature-complete (paridad con OpenGL)
- ✅ Runtime selection: `--api opengl` o `--api vulkan`
- ✅ Performance: Vulkan >20% más rápido que OpenGL
- ✅ Validation layers sin errores
- ✅ Soporta Windows/Linux (macOS con MoltenVK)
- ✅ Release v0.5.0-rc

---

## Backlog (Post v1.0)

### Short-Term (1-2 meses)

**Physics Integration**:
- [ ] Integrar Box2D (2D physics)
- [ ] `RigidBodyComponent`, `ColliderComponent`
- [ ] Physics debug rendering
- [ ] Editor gizmos para colliders

**Audio System**:
- [ ] Integrar FMOD o OpenAL
- [ ] `AudioSourceComponent`, `AudioListenerComponent`
- [ ] 3D spatial audio
- [ ] Audio mixer

**Animation System**:
- [ ] Skeletal animation (básico)
- [ ] Animation state machine
- [ ] Blend trees (futuro)

---

### Medium-Term (3-6 meses)

**Scripting (Lua/C#)**:
- [ ] Lua embedding con Sol2
- [ ] C# scripting con Mono (como Unity)
- [ ] Script hot-reloading
- [ ] Debugger integration

**Networking**:
- [ ] Client-server architecture
- [ ] Replicación de entidades
- [ ] RPC system
- [ ] Lag compensation

**AI Systems**:
- [ ] Navigation meshes
- [ ] Pathfinding (A*)
- [ ] Behavior trees
- [ ] Steering behaviors

---

### Long-Term (6+ meses)

**Advanced Rendering**:
- [ ] Ray tracing (RTX)
- [ ] Global Illumination (DDGI, RTGI)
- [ ] Volumetric fog
- [ ] Screen-space reflections (SSR)

**Tooling**:
- [ ] Profiler integrado (CPU, GPU, memory)
- [ ] Asset pipeline (custom importers)
- [ ] Visual scripting (node-based)
- [ ] Terrain editor

**Deployment**:
- [ ] Build system (exportar .exe, .app)
- [ ] WebAssembly support (web games)
- [ ] Mobile support (iOS, Android con OpenGL ES)

---

## Dependencias Bloqueantes

### Critical Path (Phase 3 → 4 → 5)

```
Phase 3 (ECS)
    ↓ BLOCKS
Phase 4 (Advanced Rendering)
    ↓ BLOCKS (optional)
Phase 5 (Vulkan)
```

**Justificación**:
- Phase 4 necesita ECS para components como `LightComponent`, `CameraComponent`
- Phase 5 puede hacerse en paralelo con Phase 4, pero complica testing

---

### External Dependencies

| Dependency | Required For | Risk Level |
|------------|-------------|------------|
| **GLFW 3.4+** | Windowing | 🟢 Low (estable) |
| **GLM 1.0+** | Math | 🟢 Low (estable) |
| **ImGui 1.91+** | Editor UI | 🟡 Medium (updates frecuentes) |
| **spdlog 1.12+** | Logging | 🟢 Low (estable) |
| **EnTT (futuro)** | ECS | 🟡 Medium (puede implementarse custom) |
| **Vulkan SDK** | Phase 5 | 🟡 Medium (versioning complejo) |

**Mitigación**:
- Pin de versiones exactas en CMake
- Tests de compatibilidad en CI/CD
- Fallback implementations para critical deps

---

## Decisiones Pendientes

### 1. ¿ECS Custom vs Library (EnTT)?

**Opciones**:
- **A) Custom ECS**: Control total, learning experience
- **B) EnTT**: Battle-tested, performance óptimo

**Pros/Cons**:
| Aspecto | Custom | EnTT |
|---------|--------|------|
| **Tiempo de dev** | 6-8 semanas | 2-3 semanas |
| **Learning** | Alto | Bajo |
| **Performance** | 🟡 Bueno (si bien hecho) | 🟢 Excelente |
| **Mantenimiento** | 🔴 Alto | 🟢 Bajo |
| **Features** | Solo lo que necesitamos | Muchas (groups, observers, etc.) |

**Recomendación**: **EnTT** (pragmático para v1.0, custom para v2.0 si es necesario)

---

### 2. ¿Scripting en C++, Lua o C#?

**Opciones**:
| Language | Pros | Cons |
|----------|------|------|
| **C++ Native** | Sin overhead, acceso total | Requiere recompilación |
| **Lua** | Ligero, fácil de embeber | Sin tipos estáticos, menos popular |
| **C#** | Como Unity, ecosistema grande | Mono/CoreCLR pesado, integración compleja |

**Recomendación**: **C++ Native en Phase 3**, Lua/C# en backlog (post v1.0)

---

### 3. ¿Vulkan Obligatorio para v1.0?

**Pregunta**: ¿Debería v1.0 incluir Vulkan o solo OpenGL?

**Argumentos a favor**:
- ✅ Vulkan es el futuro (OpenGL deprecated en macOS)
- ✅ Performance gain significativo

**Argumentos en contra**:
- ❌ Complejidad (10+ semanas de trabajo)
- ❌ OpenGL funciona bien para 90% de casos
- ❌ MoltenVK en macOS tiene bugs

**Recomendación**: **Opcional para v1.0** (Vulkan = v1.1 o v2.0)

---

## Milestones y Fechas

### Q1 2025

**Milestone**: v0.3.0-alpha (Phase 3 Complete)

**Fecha objetivo**: 31 de Enero de 2025

**Deliverables**:
- ✅ ECS architecture funcional
- ✅ Editor integrado con ECS
- ✅ Asset Manager con hot-reloading
- ✅ Native scripting operativo
- ✅ 50+ tests pasando

---

### Q2 2025

**Milestone**: v0.4.0-beta (Phase 4 Complete)

**Fecha objetivo**: 30 de Abril de 2025

**Deliverables**:
- ✅ PBR rendering
- ✅ Shadow mapping
- ✅ Post-processing stack
- ✅ Batching & instancing
- ✅ Performance: 5k entities a 60+ FPS

---

### Q3 2025

**Milestone**: v0.5.0-rc (Phase 5 Complete - Optional)

**Fecha objetivo**: 31 de Julio de 2025

**Deliverables**:
- ✅ Vulkan backend funcional
- ✅ Feature parity con OpenGL
- ✅ Performance benchmarks publicados

---

### Q4 2025

**Milestone**: v1.0.0 (Production Release)

**Fecha objetivo**: 31 de Octubre de 2025

**Deliverables**:
- ✅ Todas las phases 3-4-5 completas
- ✅ Documentación comprehensiva
- ✅ Ejemplo de juego completo (demo)
- ✅ Website + tutoriales
- ✅ Community guidelines (contributing, code of conduct)

---

## Tracking Progress

### GitHub Project Board

**Columnas**:
- 📋 Backlog
- 🚀 Todo (Phase actual)
- 🔨 In Progress
- 🧪 Testing
- ✅ Done

**Issues por Phase**:
- Tag: `phase-3-ecs`, `phase-4-rendering`, etc.
- Milestones: `v0.3.0-alpha`, `v0.4.0-beta`, etc.

---

### Weekly Review

**Cada viernes**:
- Review de progreso vs plan
- Update de estimaciones
- Identificar blockers
- Priorizar siguiente semana

---

**Última actualización**: 15 de noviembre de 2025  
**Autor**: VestEngine Team  
**Versión del documento**: 1.0
