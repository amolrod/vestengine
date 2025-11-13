#pragma once

#include <string>

#include "Core/LayerStack.h"
#include "Core/Timestep.h"
#include "Core/Window.h"
#include "Rendering/Renderer.h"

namespace Vest {

class ImGuiLayer;

class Application {
public:
    explicit Application(const std::string& name = "VestEngine");
    virtual ~Application();

    void Run();
    void OnEvent(Event& event);

    void PushLayer(Layer* layer);
    void PushOverlay(Layer* layer);

    Window& GetWindow() const { return *m_Window; }

    static Application& Get() { return *s_Instance; }

protected:
    virtual void InitializeLayers() {}

private:
    bool OnWindowClose(WindowCloseEvent& event);
    bool OnWindowResize(WindowResizeEvent& event);

    Scope<Window> m_Window;
    bool m_Running = true;
    bool m_Minimized = false;
    LayerStack m_LayerStack;
    ImGuiLayer* m_ImGuiLayer = nullptr;
    float m_LastFrameTime = 0.0f;

    static Application* s_Instance;
};

Application* CreateApplication();

}  // namespace Vest
