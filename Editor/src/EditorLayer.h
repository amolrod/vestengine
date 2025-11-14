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
    Ref<Texture2D> m_WhiteTexture;

    std::vector<SceneObject> m_SceneObjects;
    int m_SelectedEntityIndex = -1;

    glm::vec3 m_CameraPosition = glm::vec3(0.0f, 0.0f, 1.0f);
    float m_CameraZoom = 1.5f;
    bool m_ViewportFocused = false;
    bool m_ViewportHovered = false;
    glm::vec2 m_LastMousePos = glm::vec2(0.0f);
    glm::mat4 m_ViewProjectionMatrix = glm::mat4(1.0f);
    glm::mat4 m_ViewMatrix = glm::mat4(1.0f);
    glm::mat4 m_ProjectionMatrix = glm::mat4(1.0f);
    glm::vec2 m_SelectedOutline[4];
    bool m_DrawSelectionOutline = false;
    ImGuizmo::OPERATION m_GizmoOperation = ImGuizmo::TRANSLATE;
    ImGuizmo::MODE m_GizmoMode = ImGuizmo::WORLD;
    bool m_GizmoWasUsing = false;

    std::vector<std::vector<SceneObject>> m_UndoStack;
    std::vector<std::vector<SceneObject>> m_RedoStack;

    void HandleViewportCameraControls();
    void HandleViewportPicking();
    void HandleGizmos();
    void SaveScene(const std::string& filepath);
    void LoadScene(const std::string& filepath);
    void PushUndoState();
    void Undo();
    void Redo();
    void AddEntity();
    void DeleteSelected();
    void DuplicateSelected();
    glm::mat4 CalculateTransform(const SceneObject& object) const;
    void DecomposeTransform(const glm::mat4& transform, glm::vec3& translation, glm::vec3& rotation, glm::vec3& scale);
};

}  // namespace Vest
