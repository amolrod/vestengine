#include "Platform/Windows/WindowsWindow.h"

#include <cassert>

#include "Core/Event.h"
#include "Rendering/Platform/OpenGL/OpenGLContext.h"

namespace Vest {

static bool s_GLFWInitialized = false;

static void GLFWErrorCallback(int error, const char* description) {
    (void)error;
    (void)description;
}

WindowsWindow::WindowsWindow(const WindowProps& props) {
    Init(props);
}

WindowsWindow::~WindowsWindow() {
    Shutdown();
}

void WindowsWindow::Init(const WindowProps& props) {
    m_Data.title = props.title;
    m_Data.width = props.width;
    m_Data.height = props.height;

    if (!s_GLFWInitialized) {
        int success = glfwInit();
        assert(success && "Failed to initialize GLFW");
        glfwSetErrorCallback(GLFWErrorCallback);
        s_GLFWInitialized = true;
    }

#if defined(__APPLE__)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
#else
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif

    m_Window = glfwCreateWindow(static_cast<int>(props.width), static_cast<int>(props.height), m_Data.title.c_str(), nullptr, nullptr);
    assert(m_Window && "Failed to create window");

#if defined(__APPLE__)
    glfwShowWindow(m_Window);
    glfwFocusWindow(m_Window);
#endif

    m_Context = CreateScope<OpenGLContext>(m_Window);
    m_Context->Init();

    glfwSetWindowUserPointer(m_Window, &m_Data);
    SetVSync(true);

    glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height) {
        WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
        data.width = static_cast<unsigned int>(width);
        data.height = static_cast<unsigned int>(height);

        WindowResizeEvent event(data.width, data.height);
        if (data.eventCallback) {
            data.eventCallback(event);
        }
    });

    glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window) {
        WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
        WindowCloseEvent event;
        if (data.eventCallback) {
            data.eventCallback(event);
        }
    });
}

void WindowsWindow::Shutdown() {
    if (m_Window) {
        glfwDestroyWindow(m_Window);
        m_Window = nullptr;
    }
}

void WindowsWindow::OnUpdate() {
    glfwPollEvents();
    m_Context->SwapBuffers();
}

void WindowsWindow::SetVSync(bool enabled) {
    glfwSwapInterval(enabled ? 1 : 0);
    m_Data.vSync = enabled;
}

}  // namespace Vest
