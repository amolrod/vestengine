cmake_minimum_required(VERSION 3.20)
project(MotorGrafico3D VERSION 1.0.0 LANGUAGES C CXX)

# =============================================================================
# Configuración del compilador C++20
# =============================================================================
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# =============================================================================
# Opciones de build
# =============================================================================
option(BUILD_TESTS "Build unit tests" ON)
option(BUILD_SANDBOX "Build sandbox application" ON)

# =============================================================================
# Configuración de warnings (máxima calidad)
# =============================================================================
# NOTA: Solo para nuestro código, no para dependencias externas
function(set_project_warnings target_name)
    if(MSVC)
        target_compile_options(${target_name} PRIVATE /W4 /WX /utf-8)
    else()
        target_compile_options(${target_name} PRIVATE
            -Wall
            -Wextra
            -Wpedantic
            -Werror
        )
    endif()
endfunction()

# =============================================================================
# Configuración de output directories (organización limpia)
# =============================================================================
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# =============================================================================
# Subdirectorios
# =============================================================================
add_subdirectory(external)  # Dependencias externas
add_subdirectory(src)       # Motor principal

# Sandbox (aplicación de ejemplo)
if(BUILD_SANDBOX)
    add_subdirectory(sandbox)
endif()

# Tests unitarios
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

# =============================================================================
# Información de build
# =============================================================================
message(STATUS "===================================")
message(STATUS "Motor Gráfico 3D - Build Configuration")
message(STATUS "===================================")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Compiler: ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "Build tests: ${BUILD_TESTS}")
message(STATUS "Build sandbox: ${BUILD_SANDBOX}")
message(STATUS "===================================")
