#pragma once

#include <string>

#include <GLFW/glfw3.h>

#include "Core/Window.h"

namespace Vest {

class OpenGLContext;

class WindowsWindow : public Window {
public:
    explicit WindowsWindow(const WindowProps& props);
    ~WindowsWindow() override;

    void OnUpdate() override;

    unsigned int GetWidth() const override { return m_Data.width; }
    unsigned int GetHeight() const override { return m_Data.height; }

    void SetEventCallback(const EventCallbackFn& callback) override { m_Data.eventCallback = callback; }
    void SetVSync(bool enabled) override;
    bool IsVSync() const override { return m_Data.vSync; }

    void* GetNativeWindow() const override { return m_Window; }

private:
    void Init(const WindowProps& props);
    void Shutdown();

    GLFWwindow* m_Window = nullptr;
    Scope<OpenGLContext> m_Context;

    struct WindowData {
        std::string title;
        unsigned int width = 0;
        unsigned int height = 0;
        bool vSync = true;
        EventCallbackFn eventCallback;
    } m_Data;
};

}  // namespace Vest
