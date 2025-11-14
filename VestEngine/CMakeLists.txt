set(VESTENGINE_HEADERS
    src/Core/Base.h
    src/Core/Timestep.h
    src/Core/Event.h
    src/Core/Window.h
    src/Core/Application.h
    src/Core/Layer.h
    src/Core/LayerStack.h
    src/Core/EntryPoint.h
    src/Core/Log.h
    src/Serialization/SceneSerializer.h
    src/Core/Input.h
    src/Rendering/RenderAPI.h
    src/Rendering/RendererAPI.h
    src/Rendering/Renderer.h
    src/Rendering/RenderCommand.h
    src/Rendering/Shader.h
    src/Rendering/Buffer.h
    src/Rendering/VertexArray.h
    src/Rendering/Texture.h
    src/Rendering/Framebuffer.h
    src/Rendering/Platform/OpenGL/OpenGLContext.h
    src/Rendering/Platform/OpenGL/OpenGLShader.h
    src/Rendering/Platform/OpenGL/OpenGLBuffer.h
    src/Rendering/Platform/OpenGL/OpenGLVertexArray.h
    src/Rendering/Platform/OpenGL/OpenGLTexture.h
    src/Rendering/Platform/OpenGL/OpenGLFramebuffer.h
    src/Rendering/Platform/OpenGL/OpenGLRendererAPI.h
    src/Rendering/Platform/Vulkan/VulkanContext.h
    src/Rendering/Platform/Vulkan/VulkanShader.h
    src/Rendering/Platform/Vulkan/VulkanBuffer.h
    src/Rendering/Platform/Vulkan/VulkanRendererAPI.h
    src/ImGui/ImGuiLayer.h
    src/Platform/Windows/WindowsWindow.h
    src/Scene/SceneObject.h
)

set(VESTENGINE_SOURCES
    src/Core/Window.cpp
    src/Core/Application.cpp
    src/Core/Layer.cpp
    src/Core/LayerStack.cpp
    src/Core/Log.cpp
    src/Serialization/SceneSerializer.cpp
    src/Core/Input.cpp
    src/Rendering/Renderer.cpp
    src/Rendering/RenderCommand.cpp
    src/Rendering/Shader.cpp
    src/Rendering/Buffer.cpp
    src/Rendering/VertexArray.cpp
    src/Rendering/Texture.cpp
    src/Rendering/Framebuffer.cpp
    src/Rendering/Platform/OpenGL/OpenGLContext.cpp
    src/Rendering/Platform/OpenGL/OpenGLShader.cpp
    src/Rendering/Platform/OpenGL/OpenGLBuffer.cpp
    src/Rendering/Platform/OpenGL/OpenGLVertexArray.cpp
    src/Rendering/Platform/OpenGL/OpenGLTexture.cpp
    src/Rendering/Platform/OpenGL/OpenGLFramebuffer.cpp
    src/Rendering/Platform/OpenGL/OpenGLRendererAPI.cpp
    src/Rendering/Platform/Vulkan/VulkanContext.cpp
    src/Rendering/Platform/Vulkan/VulkanShader.cpp
    src/Rendering/Platform/Vulkan/VulkanBuffer.cpp
    src/Rendering/Platform/Vulkan/VulkanRendererAPI.cpp
    src/ImGui/ImGuiLayer.cpp
    src/Platform/Windows/WindowsWindow.cpp
)

add_library(VestEngine STATIC ${VESTENGINE_HEADERS} ${VESTENGINE_SOURCES})

target_include_directories(VestEngine PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${glfw_SOURCE_DIR}/include
    ${glm_SOURCE_DIR}
    ${glad_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/external/stb
)

target_link_libraries(VestEngine PUBLIC glfw glad imgui spdlog::spdlog)
if(VEST_ENABLE_SERIALIZATION)
    target_link_libraries(VestEngine PUBLIC nlohmann_json::nlohmann_json)
endif()

target_compile_definitions(VestEngine PUBLIC
    GLFW_INCLUDE_NONE
    VEST_RENDERER_API_DEFAULT="${VEST_RENDERER_API}"
    VEST_ASSET_DIR="${CMAKE_SOURCE_DIR}/assets"
)
