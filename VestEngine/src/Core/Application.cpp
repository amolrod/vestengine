#include "Core/Application.h"

#include <cassert>
#include <chrono>

#include "Core/Event.h"
#include "Core/Log.h"
#include "ImGui/ImGuiLayer.h"
#include "Rendering/RenderCommand.h"

namespace Vest {

Application* Application::s_Instance = nullptr;

Application::Application(const std::string& name) {
    assert(!s_Instance && "Application already exists!");
    s_Instance = this;

    // Initialize logging system
    Log::Init();
    VEST_CORE_INFO("VestEngine Application '{0}' starting...", name);

    WindowProps props;
    props.title = name;
    m_Window = Window::Create(props);
    m_Window->SetEventCallback([this](Event& event) { OnEvent(event); });
    VEST_CORE_INFO("Window created: {0} ({1}x{2})", props.title, props.width, props.height);

    Renderer::Init();
    VEST_CORE_INFO("Renderer initialized");

    m_ImGuiLayer = new ImGuiLayer();
    PushOverlay(m_ImGuiLayer);

    InitializeLayers();
}

Application::~Application() {
    VEST_CORE_INFO("VestEngine Application shutting down...");
    Renderer::Shutdown();
    s_Instance = nullptr;
}

void Application::Run() {
    VEST_CORE_INFO("Entering main loop...");
    auto lastTime = std::chrono::steady_clock::now();

    while (m_Running) {
        auto now = std::chrono::steady_clock::now();
        std::chrono::duration<float> delta = now - lastTime;
        lastTime = now;

        Timestep timestep(delta.count());

        if (!m_Minimized) {
            for (Layer* layer : m_LayerStack) {
                layer->OnUpdate(timestep);
            }
        }

        m_ImGuiLayer->Begin();
        for (Layer* layer : m_LayerStack) {
            layer->OnImGuiRender();
        }
        m_ImGuiLayer->End();

        m_Window->OnUpdate();
    }
}

void Application::OnEvent(Event& event) {
    EventDispatcher dispatcher(event);
    dispatcher.Dispatch<WindowCloseEvent>([this](WindowCloseEvent& e) { return OnWindowClose(e); });
    dispatcher.Dispatch<WindowResizeEvent>([this](WindowResizeEvent& e) { return OnWindowResize(e); });

    for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it) {
        if (event.handled) {
            break;
        }
        (*it)->OnEvent(event);
    }
}

void Application::PushLayer(Layer* layer) {
    m_LayerStack.PushLayer(layer);
}

void Application::PushOverlay(Layer* layer) {
    m_LayerStack.PushOverlay(layer);
}

bool Application::OnWindowClose(WindowCloseEvent&) {
    m_Running = false;
    return true;
}

bool Application::OnWindowResize(WindowResizeEvent& event) {
    if (event.GetWidth() == 0 || event.GetHeight() == 0) {
        m_Minimized = true;
        return false;
    }

    m_Minimized = false;
    Renderer::OnWindowResize(event.GetWidth(), event.GetHeight());
    return false;
}

}  // namespace Vest
