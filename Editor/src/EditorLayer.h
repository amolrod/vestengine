#pragma once

#include <string>
#include <vector>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/quaternion.hpp>

#include <imgui.h>
#include <ImGuizmo.h>

#include "Core/Layer.h"
#include "Core/Input.h"
#include "Rendering/Framebuffer.h"
#include "Rendering/Renderer.h"
#include "Rendering/Shader.h"
#include "Rendering/VertexArray.h"
#include "Rendering/Texture.h"
#include "Serialization/SceneSerializer.h"

#include "Panels/ContentBrowserPanel.h"
#include "Panels/PropertiesPanel.h"
#include "Panels/SceneHierarchyPanel.h"
#include "Panels/StatsPanel.h"
#include "Panels/ViewportPanel.h"
#include <Scene/SceneObject.h>

#include "EditorCamera.h"
#include "Commands/CommandManager.h"
#include "Commands/TransformCommand.h"
#include "Commands/EntityCommands.h"
#include "Commands/MacroCommand.h"
#include "Rendering/SelectionRenderer.h"
#include "Rendering/GridRenderer.h"

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
    Ref<Texture2D> m_WhiteTexture;

    std::vector<SceneObject> m_SceneObjects;
    int m_SelectedEntityIndex = -1;
    int m_HoveredEntityIndex = -1;

    EditorCamera m_EditorCamera;
    SelectionRenderer m_SelectionRenderer;
    GridRenderer m_GridRenderer;
    bool m_ViewportFocused = false;
    bool m_ViewportHovered = false;
    glm::vec2 m_LastMousePos = glm::vec2(0.0f);
    glm::vec2 m_SelectedOutline[4];
    glm::vec2 m_HoveredOutline[4];
    bool m_DrawSelectionOutline = false;
    bool m_DrawHoveredOutline = false;
    ImGuizmo::OPERATION m_GizmoOperation = ImGuizmo::TRANSLATE;
    ImGuizmo::MODE m_GizmoMode = ImGuizmo::WORLD;
    bool m_GizmoWasUsing = false;
    glm::vec3 m_GizmoOldPosition;
    glm::vec3 m_GizmoOldRotation;
    glm::vec3 m_GizmoOldScale;

    CommandManager m_CommandManager;

    void HandleViewportCameraControls();
    void HandleViewportPicking();
    void HandleViewportHover();
    void HandleGizmos();
    void SaveScene(const std::string& filepath);
    void LoadScene(const std::string& filepath);
    void AddEntity();
    void DeleteSelected();
    void DuplicateSelected();
    glm::mat4 CalculateTransform(const SceneObject& object) const;
    void DecomposeTransform(const glm::mat4& transform, glm::vec3& translation, glm::vec3& rotation, glm::vec3& scale);
};

}  // namespace Vest
