#pragma once

#include <string>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Core/Layer.h"
#include "Core/Input.h"
#include "Rendering/Framebuffer.h"
#include "Rendering/Renderer.h"
#include "Rendering/Shader.h"
#include "Rendering/VertexArray.h"
#include "Rendering/Texture.h"

#include "Panels/ContentBrowserPanel.h"
#include "Panels/PropertiesPanel.h"
#include "Panels/SceneHierarchyPanel.h"
#include "Panels/StatsPanel.h"
#include "Panels/ViewportPanel.h"
#include "Scene/SceneObject.h"

namespace Vest {

class EditorLayer : public Layer {
public:
    EditorLayer();
    ~EditorLayer() override = default;

    void OnAttach() override;
    void OnUpdate(Timestep ts) override;
    void OnImGuiRender() override;
    void OnEvent(Event& event) override;

private:
    void RebuildFramebuffer(uint32_t width, uint32_t height);

    Ref<Framebuffer> m_Framebuffer;
    glm::vec2 m_ViewportSize = {0.0f, 0.0f};

    ViewportPanel m_ViewportPanel;
    SceneHierarchyPanel m_SceneHierarchyPanel;
    PropertiesPanel m_PropertiesPanel;
    ContentBrowserPanel m_ContentBrowserPanel;
    StatsPanel m_StatsPanel;

    float m_FPS = 0.0f;
    uint32_t m_DrawCalls = 0;

    Ref<Shader> m_TriangleShader;
    Ref<VertexArray> m_TriangleVA;
    Ref<Shader> m_TextureShader;
    Ref<VertexArray> m_TextureVA;
    Ref<Texture2D> m_CheckerTexture;

    std::vector<SceneObject> m_SceneObjects;
    int m_SelectedEntityIndex = -1;

    glm::vec3 m_CameraPosition = glm::vec3(0.0f, 0.0f, 1.0f);
    float m_CameraZoom = 1.5f;
    bool m_ViewportFocused = false;
    bool m_ViewportHovered = false;
    glm::vec2 m_LastMousePos = glm::vec2(0.0f);
    glm::mat4 m_ViewProjectionMatrix = glm::mat4(1.0f);
    glm::vec2 m_SelectedOutline[4];
    bool m_DrawSelectionOutline = false;

    void HandleViewportCameraControls();
    void HandleViewportPicking();
};

}  // namespace Vest
