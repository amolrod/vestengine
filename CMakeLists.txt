cmake_minimum_required(VERSION 3.20)
project(VestEngine VERSION 1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(VEST_BUILD_EDITOR "Build VestEngine Editor" ON)
option(VEST_BUILD_EXAMPLES "Build examples" OFF)
set(VEST_RENDERER_API "OpenGL" CACHE STRING "Rendering API (OpenGL/Vulkan)")
set_property(CACHE VEST_RENDERER_API PROPERTY STRINGS "OpenGL" "Vulkan")
option(VEST_ENABLE_SERIALIZATION "Enable scene serialization" ON)

include(FetchContent)

FetchContent_Declare(glfw URL https://github.com/glfw/glfw/releases/download/3.4/glfw-3.4.zip)
FetchContent_Declare(glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG 1.0.1
    GIT_SHALLOW TRUE
)
FetchContent_Declare(imgui URL https://github.com/ocornut/imgui/archive/refs/tags/v1.91.8-docking.tar.gz)
FetchContent_Declare(spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG v1.12.0
    GIT_SHALLOW TRUE
)
if(VEST_ENABLE_SERIALIZATION)
    FetchContent_Declare(nlohmann_json URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz)
endif()

FetchContent_MakeAvailable(glfw glm spdlog)
if(VEST_ENABLE_SERIALIZATION)
    FetchContent_MakeAvailable(nlohmann_json)
endif()

add_library(glad STATIC ${CMAKE_CURRENT_SOURCE_DIR}/external/glad/src/glad.c)
target_include_directories(glad PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/external/glad/include)

FetchContent_GetProperties(imgui)
if(NOT imgui_POPULATED)
    FetchContent_Populate(imgui)
    add_library(imgui STATIC
        ${imgui_SOURCE_DIR}/imgui.cpp
        ${imgui_SOURCE_DIR}/imgui_demo.cpp
        ${imgui_SOURCE_DIR}/imgui_draw.cpp
        ${imgui_SOURCE_DIR}/imgui_tables.cpp
        ${imgui_SOURCE_DIR}/imgui_widgets.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
    )
    target_include_directories(imgui PUBLIC ${imgui_SOURCE_DIR} ${imgui_SOURCE_DIR}/backends)
    target_link_libraries(imgui PUBLIC glfw)
    target_compile_definitions(imgui PUBLIC IMGUI_IMPL_OPENGL_LOADER_GLAD)
endif()

add_subdirectory(VestEngine)

if(VEST_BUILD_EDITOR)
    add_subdirectory(Editor)
endif()
