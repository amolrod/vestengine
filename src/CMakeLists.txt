# =============================================================================
# Motor Gráfico 3D - Librería Principal
# =============================================================================

# Recolectar todos los archivos fuente del motor
file(GLOB_RECURSE MOTOR_SOURCES
    "core/*.cpp"
    "rendering/*.cpp"
    "ecs/*.cpp"
    "physics/*.cpp"
    "input/*.cpp"
    "scene/*.cpp"
    "assets/*.cpp"
    "utils/*.cpp"
)

file(GLOB_RECURSE MOTOR_HEADERS
    "core/*.h"
    "rendering/*.h"
    "ecs/*.h"
    "physics/*.h"
    "input/*.h"
    "scene/*.h"
    "assets/*.h"
    "utils/*.h"
)

# Crear librería estática del motor
add_library(MotorGrafico STATIC
    ${MOTOR_SOURCES}
    ${MOTOR_HEADERS}
)

# Incluir directorios
target_include_directories(MotorGrafico PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)

# Linkear dependencias externas
target_link_libraries(MotorGrafico PUBLIC
    glfw
    glad
    glm::glm
    spdlog::spdlog
    stb_image
)

# Linkear OpenGL según plataforma
if(APPLE)
    find_library(OPENGL_LIBRARY OpenGL REQUIRED)
    target_link_libraries(MotorGrafico PUBLIC ${OPENGL_LIBRARY})
elseif(WIN32)
    # En Windows, necesitamos linkear con opengl32
    target_link_libraries(MotorGrafico PUBLIC opengl32)
elseif(UNIX)
    # En Linux, necesitamos OpenGL y dl
    find_package(OpenGL REQUIRED)
    target_link_libraries(MotorGrafico PUBLIC OpenGL::GL ${CMAKE_DL_LIBS})
endif()

# Aplicar warnings estrictos solo a nuestro código
set_project_warnings(MotorGrafico)

# Definiciones de preprocesador según plataforma
if(WIN32)
    target_compile_definitions(MotorGrafico PUBLIC
        PLATFORM_WINDOWS
        _CRT_SECURE_NO_WARNINGS
    )
elseif(APPLE)
    target_compile_definitions(MotorGrafico PUBLIC
        PLATFORM_MACOS
    )
elseif(UNIX)
    target_compile_definitions(MotorGrafico PUBLIC
        PLATFORM_LINUX
    )
endif()

# Configuración Debug/Release
target_compile_definitions(MotorGrafico PUBLIC
    $<$<CONFIG:Debug>:DEBUG_BUILD>
    $<$<CONFIG:Release>:RELEASE_BUILD>
)

message(STATUS "Motor Gráfico library configured")
