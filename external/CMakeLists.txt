# =============================================================================
# Dependencias Externas
# =============================================================================
# Este archivo gestiona todas las librerías de terceros que usa el motor.
# Usaremos una combinación de FetchContent (para simplicidad) y git submodules.

include(FetchContent)

# =============================================================================
# GLFW - Window & Input Management
# =============================================================================
message(STATUS "Fetching GLFW...")
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.4
)
FetchContent_MakeAvailable(glfw)

# =============================================================================
# GLM - Math Library (header-only)
# =============================================================================
message(STATUS "Fetching GLM...")
FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG 1.0.1
)
FetchContent_MakeAvailable(glm)

# =============================================================================
# spdlog - Fast Logging (header-only)
# =============================================================================
message(STATUS "Fetching spdlog...")
FetchContent_Declare(
    spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG v1.14.1
)
FetchContent_MakeAvailable(spdlog)

# =============================================================================
# GLAD - OpenGL Loader
# =============================================================================
# NOTA: GLAD se debe generar desde https://glad.dav1d.de/
# Para empezar rápido, lo incluiremos como código fuente directamente.
# Configuración: OpenGL 4.6, Core Profile

add_library(glad STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/glad/src/glad.c
)

target_include_directories(glad PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/glad/include
)

# Link con dl en Linux
if(UNIX AND NOT APPLE)
    target_link_libraries(glad PRIVATE dl)
endif()

# =============================================================================
# stb_image - Image Loading (header-only)
# =============================================================================
# stb_image es una librería header-only para cargar imágenes (PNG, JPG, etc.)
# La implementación se compila una sola vez en stb_image_impl.cpp

add_library(stb_image STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/stb/stb_image_impl.cpp
)

target_include_directories(stb_image PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/stb
)

# =============================================================================
# Información de dependencias
# =============================================================================
message(STATUS "External dependencies configured:")
message(STATUS "  - GLFW: Window management")
message(STATUS "  - GLM: Math library")
message(STATUS "  - spdlog: Logging")
message(STATUS "  - GLAD: OpenGL loader")
message(STATUS "  - stb_image: Image loading")
