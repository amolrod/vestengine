#include "EditorLayer.h"

#include <algorithm>
#include <filesystem>

#include <GLFW/glfw3.h>
#include <imgui.h>
#include <ImGuizmo.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Core/Input.h"
#include "Rendering/RenderCommand.h"
#include "Rendering/Buffer.h"

namespace Vest {

EditorLayer::EditorLayer()
    : Layer("EditorLayer"),
      m_SceneHierarchyPanel("Scene Hierarchy"),
      m_PropertiesPanel("Properties"),
      m_ContentBrowserPanel("assets"),
      m_StatsPanel("Stats") {}

void EditorLayer::OnAttach() {
    FramebufferSpecification spec;
    spec.width = 1280;
    spec.height = 720;
    m_Framebuffer = Framebuffer::Create(spec);
    m_ViewportPanel.SetFramebuffer(m_Framebuffer);

    m_SceneHierarchyPanel.SetSceneContext(&m_SceneObjects, &m_SelectedEntityIndex);
    m_PropertiesPanel.SetSceneContext(&m_SceneObjects, &m_SelectedEntityIndex);

    float vertices[] = {
        -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
         0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
         0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
    };

    uint32_t indices[] = {0, 1, 2};

    m_TriangleVA = VertexArray::Create();

    Ref<VertexBuffer> vertexBuffer = VertexBuffer::Create(vertices, sizeof(vertices));
    vertexBuffer->SetLayout({
        {ShaderDataType::Float3, "a_Position"},
        {ShaderDataType::Float3, "a_Color"},
    });
    m_TriangleVA->AddVertexBuffer(vertexBuffer);

    Ref<IndexBuffer> indexBuffer = IndexBuffer::Create(indices, 3);
    m_TriangleVA->SetIndexBuffer(indexBuffer);

    const std::string vertexSrc = R"(#version 410 core
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Color;

out vec3 v_Color;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;
uniform vec4 u_Color;

void main() {
    v_Color = a_Color;
    gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
})";

    const std::string fragmentSrc = R"(#version 410 core
layout(location = 0) out vec4 o_Color;

in vec3 v_Color;

uniform vec4 u_Color;

void main() {
    o_Color = vec4(v_Color, 1.0) * u_Color;
})";

    m_TriangleShader = Shader::Create("EditorTriangle", vertexSrc, fragmentSrc);

    float quadVertices[] = {
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
         0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
         0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
        -0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
    };

    uint32_t quadIndices[] = {0, 1, 2, 2, 3, 0};

    m_TextureVA = VertexArray::Create();
    Ref<VertexBuffer> quadVB = VertexBuffer::Create(quadVertices, sizeof(quadVertices));
    quadVB->SetLayout({
        {ShaderDataType::Float3, "a_Position"},
        {ShaderDataType::Float2, "a_TexCoord"},
    });
    m_TextureVA->AddVertexBuffer(quadVB);
    m_TextureVA->SetIndexBuffer(IndexBuffer::Create(quadIndices, 6));

    const std::string texturedVertexSrc = R"(#version 410 core
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;

out vec2 v_TexCoord;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

void main() {
    v_TexCoord = a_TexCoord;
    gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
})";

const std::string texturedFragmentSrc = R"(#version 410 core
layout(location = 0) out vec4 o_Color;

in vec2 v_TexCoord;

uniform sampler2D u_Texture;
uniform vec4 u_Color;

void main() {
    o_Color = texture(u_Texture, v_TexCoord) * u_Color;
})";

    m_TextureShader = Shader::Create("EditorTexture", texturedVertexSrc, texturedFragmentSrc);
    m_TextureShader->Bind();
    m_TextureShader->SetInt("u_Texture", 0);

    std::filesystem::path assetRoot = std::filesystem::path(VEST_ASSET_DIR);
    std::filesystem::path checkerPath = assetRoot / "textures" / "Checkerboard.png";
    if (!std::filesystem::exists(checkerPath)) {
        // Fall back to relative path in case assets directory moved.
        checkerPath = std::filesystem::path("assets/textures/Checkerboard.png");
    }
    m_CheckerTexture = Texture2D::Create(checkerPath.string());
    std::filesystem::path whitePath = std::filesystem::path(VEST_ASSET_DIR) / "textures" / "White.png";
    if (!std::filesystem::exists(whitePath)) {
        whitePath = std::filesystem::path("assets/textures/White.png");
    }
    m_WhiteTexture = Texture2D::Create(whitePath.string());

    m_SceneObjects.push_back(SceneObject{
        .name = "Triangle",
        .position = glm::vec3(-0.2f, -0.1f, 0.0f),
        .scale = glm::vec3(1.0f),
        .rotation = glm::vec3(0.0f),
        .color = glm::vec4(1.0f),
        .textured = false,
        .mesh = SceneObject::MeshType::Triangle});
    m_SceneObjects.push_back(SceneObject{
        .name = "Textured Quad",
        .position = glm::vec3(0.6f, 0.0f, 0.0f),
        .scale = glm::vec3(0.75f),
        .rotation = glm::vec3(0.0f),
        .color = glm::vec4(1.0f),
        .textured = true,
        .mesh = SceneObject::MeshType::Quad});
    m_SelectedEntityIndex = 0;
}

void EditorLayer::OnUpdate(Timestep ts) {
    m_FPS = ts.GetSeconds() > 0.0f ? 1.0f / ts.GetSeconds() : 0.0f;
    m_DrawCalls = 0;

    if (m_ViewportFocused) {
        const float cameraSpeed = 2.0f;
        if (Input::IsKeyPressed(GLFW_KEY_A)) {
            m_CameraPosition.x -= cameraSpeed * ts.GetSeconds();
        }
        if (Input::IsKeyPressed(GLFW_KEY_D)) {
            m_CameraPosition.x += cameraSpeed * ts.GetSeconds();
        }
        if (Input::IsKeyPressed(GLFW_KEY_W)) {
            m_CameraPosition.y += cameraSpeed * ts.GetSeconds();
        }
        if (Input::IsKeyPressed(GLFW_KEY_S)) {
            m_CameraPosition.y -= cameraSpeed * ts.GetSeconds();
        }
    }

    if (m_Framebuffer && m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f) {
        m_Framebuffer->Bind();
        RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1.0f});
        RenderCommand::Clear();

        const float aspect = m_ViewportSize.x / m_ViewportSize.y;
        const float orthoHeight = m_CameraZoom;
        const float orthoWidth = orthoHeight * aspect;
        glm::mat4 projection = glm::ortho(-orthoWidth, orthoWidth, -orthoHeight, orthoHeight, -1.0f, 10.0f);
        glm::mat4 view = glm::translate(glm::mat4(1.0f), -m_CameraPosition);
        m_ProjectionMatrix = projection;
        m_ViewMatrix = view;
        m_ViewProjectionMatrix = projection * view;

        Renderer::BeginScene(m_ViewProjectionMatrix);
        for (const auto& object : m_SceneObjects) {
            glm::mat4 transform = CalculateTransform(object);

            if (object.mesh == SceneObject::MeshType::Quad) {
                if (m_TextureShader && m_TextureVA) {
                    Ref<Texture2D> tex = object.textured ? m_CheckerTexture : m_WhiteTexture;
                    if (tex) {
                        m_TextureShader->Bind();
                        m_TextureShader->SetFloat4("u_Color", object.color);
                        tex->Bind();
                        Renderer::Submit(m_TextureShader, m_TextureVA, transform);
                        ++m_DrawCalls;
                    }
                }
            } else {
                if (m_TriangleShader && m_TriangleVA) {
                    m_TriangleShader->Bind();
                    m_TriangleShader->SetFloat4("u_Color", object.color);
                    Renderer::Submit(m_TriangleShader, m_TriangleVA, transform);
                    ++m_DrawCalls;
                }
            }
        }
        Renderer::EndScene();

        m_DrawSelectionOutline = false;
        if (m_SelectedEntityIndex >= 0 && m_SelectedEntityIndex < static_cast<int>(m_SceneObjects.size())) {
            const SceneObject& selected = m_SceneObjects[static_cast<size_t>(m_SelectedEntityIndex)];
            glm::mat4 outlineTransform = CalculateTransform(selected) * glm::scale(glm::mat4(1.0f), glm::vec3(1.05f));

            glm::vec4 corners[4] = {
                outlineTransform * glm::vec4(-0.5f, -0.5f, 0.0f, 1.0f),
                outlineTransform * glm::vec4(0.5f, -0.5f, 0.0f, 1.0f),
                outlineTransform * glm::vec4(0.5f, 0.5f, 0.0f, 1.0f),
                outlineTransform * glm::vec4(-0.5f, 0.5f, 0.0f, 1.0f)};

            for (int i = 0; i < 4; ++i) {
                glm::vec4 clip = m_ViewProjectionMatrix * corners[i];
                glm::vec3 ndc = glm::vec3(clip) / clip.w;
                m_SelectedOutline[i].x = (ndc.x * 0.5f + 0.5f) * m_ViewportSize.x;
                m_SelectedOutline[i].y = (1.0f - (ndc.y * 0.5f + 0.5f)) * m_ViewportSize.y;
            }
            m_DrawSelectionOutline = true;
        }

        m_Framebuffer->Unbind();
    }

    m_StatsPanel.Update(m_FPS, m_DrawCalls);
}

void EditorLayer::OnImGuiRender() {
    static bool dockspaceOpen = true;
    static bool optFullscreen = true;
    ImGuiDockNodeFlags dockspaceFlags = ImGuiDockNodeFlags_None;

    ImGuiWindowFlags windowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->Pos);
    ImGui::SetNextWindowSize(viewport->Size);
    ImGui::SetNextWindowViewport(viewport->ID);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    windowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
    windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

    if (optFullscreen) {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    }

    ImGui::Begin("DockSpace", &dockspaceOpen, windowFlags);

    if (optFullscreen) {
        ImGui::PopStyleVar();
    }

    ImGui::PopStyleVar(2);

    ImGuiID dockspaceId = ImGui::GetID("VestEditorDockspace");
    ImGui::DockSpace(dockspaceId, ImVec2(0.0f, 0.0f), dockspaceFlags);

    ImGuiIO& io = ImGui::GetIO();
    if (io.KeyCtrl && ImGui::IsKeyPressed(ImGuiKey_Z)) {
        m_CommandManager.Undo();
    }
    if (io.KeyCtrl && ImGui::IsKeyPressed(ImGuiKey_Y)) {
        m_CommandManager.Redo();
    }
    if (io.KeyCtrl && ImGui::IsKeyPressed(ImGuiKey_D)) {
        DuplicateSelected();
    }

    if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Save Scene", "Ctrl+S")) {
                SaveScene("assets/scenes/Default.json");
            }
            if (ImGui::MenuItem("Load Scene", "Ctrl+L")) {
                LoadScene("assets/scenes/Default.json");
            }
            ImGui::Separator();
            if (ImGui::MenuItem("Exit")) {
                dockspaceOpen = false;
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit")) {
            bool canUndo = m_CommandManager.CanUndo();
            bool canRedo = m_CommandManager.CanRedo();
            
            std::string undoLabel = canUndo ? "Undo " + m_CommandManager.GetUndoCommandName() : "Undo";
            std::string redoLabel = canRedo ? "Redo " + m_CommandManager.GetRedoCommandName() : "Redo";
            
            if (ImGui::MenuItem(undoLabel.c_str(), "Ctrl+Z", false, canUndo)) {
                m_CommandManager.Undo();
            }
            if (ImGui::MenuItem(redoLabel.c_str(), "Ctrl+Y", false, canRedo)) {
                m_CommandManager.Redo();
            }
            ImGui::Separator();
            if (ImGui::MenuItem("Duplicate", "Ctrl+D", false, m_SelectedEntityIndex >= 0)) {
                DuplicateSelected();
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("View")) {
            ImGui::MenuItem("Viewport", nullptr, true, true);
            ImGui::MenuItem("Scene Hierarchy", nullptr, true, true);
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }

    ImGui::Begin("Toolbar", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar);
    if (ImGui::RadioButton("Translate", m_GizmoOperation == ImGuizmo::TRANSLATE)) {
        m_GizmoOperation = ImGuizmo::TRANSLATE;
    }
    ImGui::SameLine();
    if (ImGui::RadioButton("Rotate", m_GizmoOperation == ImGuizmo::ROTATE)) {
        m_GizmoOperation = ImGuizmo::ROTATE;
    }
    ImGui::SameLine();
    if (ImGui::RadioButton("Scale", m_GizmoOperation == ImGuizmo::SCALE)) {
        m_GizmoOperation = ImGuizmo::SCALE;
    }

    ImGui::SameLine(0.0f, 20.0f);
    bool hasSelection = m_SelectedEntityIndex >= 0;
    if (ImGui::Button("Add")) {
        AddEntity();
    }
    ImGui::SameLine();
    ImGui::BeginDisabled(!hasSelection);
    if (ImGui::Button("Duplicate")) {
        DuplicateSelected();
    }
    ImGui::SameLine();
    if (ImGui::Button("Delete")) {
        DeleteSelected();
    }
    ImGui::EndDisabled();

    ImGui::End();

    m_ViewportPanel.OnImGuiRender();
    const glm::vec2 viewportSize = m_ViewportPanel.GetViewportSize();
    if (viewportSize.x > 0.0f && viewportSize.y > 0.0f && viewportSize != m_ViewportSize) {
        RebuildFramebuffer(static_cast<uint32_t>(viewportSize.x), static_cast<uint32_t>(viewportSize.y));
    }
    m_ViewportFocused = m_ViewportPanel.IsFocused();
    m_ViewportHovered = m_ViewportPanel.IsHovered();
    HandleViewportCameraControls();
    HandleViewportPicking();
    HandleGizmos();

    m_SceneHierarchyPanel.OnImGuiRender();
    m_PropertiesPanel.OnImGuiRender();
    m_ContentBrowserPanel.OnImGuiRender();
    m_StatsPanel.OnImGuiRender();

    if (m_DrawSelectionOutline) {
        const glm::vec2* bounds = m_ViewportPanel.GetBounds();
        if (bounds) {
            ImDrawList* drawList = ImGui::GetForegroundDrawList();
            for (int i = 0; i < 4; ++i) {
                glm::vec2 offset1 = m_SelectedOutline[i];
                glm::vec2 offset2 = m_SelectedOutline[(i + 1) % 4];
                ImVec2 p1(bounds[0].x + offset1.x, bounds[0].y + offset1.y);
                ImVec2 p2(bounds[0].x + offset2.x, bounds[0].y + offset2.y);
                drawList->AddLine(p1, p2, IM_COL32(255, 255, 0, 255), 2.0f);
            }
        }
    }

    ImGui::End();
}

void EditorLayer::OnEvent(Event&) {}

void EditorLayer::HandleViewportCameraControls() {
    if (!m_ViewportHovered) {
        return;
    }

    ImGuiIO& io = ImGui::GetIO();
    if (io.MouseWheel != 0.0f) {
        m_CameraZoom -= io.MouseWheel * 0.2f;
        m_CameraZoom = std::clamp(m_CameraZoom, 0.5f, 5.0f);
    }

    const bool panWithMiddle = ImGui::IsMouseDown(ImGuiMouseButton_Middle);
    const bool panWithRight = ImGui::IsMouseDown(ImGuiMouseButton_Right);
    if (panWithMiddle || panWithRight) {
        ImVec2 mousePosIm = ImGui::GetMousePos();
        glm::vec2 mousePos = {mousePosIm.x, mousePosIm.y};
        glm::vec2 delta = mousePos - m_LastMousePos;
        const float panSpeed = 0.002f * m_CameraZoom;
        m_CameraPosition.x -= delta.x * panSpeed;
        m_CameraPosition.y += delta.y * panSpeed;
        m_LastMousePos = mousePos;
    } else {
        ImVec2 mousePosIm = ImGui::GetMousePos();
        m_LastMousePos = {mousePosIm.x, mousePosIm.y};
    }
}

static bool PointInEntity(const glm::vec2& point, const SceneObject& object) {
    glm::mat4 transform =
        glm::translate(glm::mat4(1.0f), object.position) *
        glm::rotate(glm::mat4(1.0f), glm::radians(object.rotation.z), glm::vec3(0, 0, 1)) *
        glm::scale(glm::mat4(1.0f), object.scale);

    glm::mat4 invTransform = glm::inverse(transform);
    glm::vec4 local = invTransform * glm::vec4(point, 0.0f, 1.0f);
    return local.x >= -0.5f && local.x <= 0.5f && local.y >= -0.5f && local.y <= 0.5f;
}

void EditorLayer::HandleViewportPicking() {
    if (!m_ViewportHovered || !ImGui::IsMouseClicked(ImGuiMouseButton_Left) || ImGuizmo::IsUsing()) {
        return;
    }

    const glm::vec2* bounds = m_ViewportPanel.GetBounds();
    if (!bounds) {
        return;
    }

    ImVec2 mouseIm = ImGui::GetMousePos();
    glm::vec2 mouse = {mouseIm.x, mouseIm.y};
    if (mouse.x < bounds[0].x || mouse.y < bounds[0].y || mouse.x > bounds[1].x || mouse.y > bounds[1].y) {
        return;
    }

    glm::vec2 viewportPoint = mouse - bounds[0];
    glm::vec2 ndc;
    ndc.x = (viewportPoint.x / m_ViewportSize.x) * 2.0f - 1.0f;
    ndc.y = 1.0f - (viewportPoint.y / m_ViewportSize.y) * 2.0f;

    glm::vec4 clip = {ndc.x, ndc.y, 0.0f, 1.0f};
    glm::vec4 world = glm::inverse(m_ViewProjectionMatrix) * clip;
    if (world.w != 0.0f) {
        world /= world.w;
    }

    glm::vec2 point = {world.x, world.y};
    for (int i = static_cast<int>(m_SceneObjects.size()) - 1; i >= 0; --i) {
        if (PointInEntity(point, m_SceneObjects[static_cast<size_t>(i)])) {
            m_SelectedEntityIndex = i;
            return;
        }
    }
}


void EditorLayer::RebuildFramebuffer(uint32_t width, uint32_t height) {
    m_ViewportSize = {static_cast<float>(width), static_cast<float>(height)};

    FramebufferSpecification spec = m_Framebuffer ? m_Framebuffer->GetSpecification() : FramebufferSpecification{};
    spec.width = width;
    spec.height = height;
    m_Framebuffer = Framebuffer::Create(spec);
    m_ViewportPanel.SetFramebuffer(m_Framebuffer);
}

void EditorLayer::SaveScene(const std::string& filepath) {
    std::filesystem::path path = filepath;
    if (!path.is_absolute()) {
        path = std::filesystem::path(VEST_ASSET_DIR) / "scenes" / path;
    }
    std::filesystem::create_directories(path.parent_path());
    SceneSerializer::Serialize(path.string(), m_SceneObjects);
}

void EditorLayer::LoadScene(const std::string& filepath) {
    std::filesystem::path path = filepath;
    if (!path.is_absolute()) {
        path = std::filesystem::path(VEST_ASSET_DIR) / "scenes" / path;
    }
    std::vector<SceneObject> loaded;
    if (SceneSerializer::Deserialize(path.string(), loaded)) {
        m_SceneObjects = std::move(loaded);
        m_SelectedEntityIndex = m_SceneObjects.empty() ? -1 : 0;
        m_CommandManager.Clear();
    }
}

void EditorLayer::AddEntity() {
    SceneObject entity;
    entity.name = "Entity " + std::to_string(m_SceneObjects.size());
    entity.mesh = SceneObject::MeshType::Quad;
    
    auto cmd = CreateScope<CreateEntityCommand>(&m_SceneObjects, entity);
    if (m_CommandManager.ExecuteCommand(std::move(cmd))) {
        m_SelectedEntityIndex = static_cast<int>(m_SceneObjects.size()) - 1;
    }
}

void EditorLayer::DeleteSelected() {
    if (m_SelectedEntityIndex < 0 || m_SelectedEntityIndex >= static_cast<int>(m_SceneObjects.size())) {
        return;
    }
    auto cmd = CreateScope<DeleteEntityCommand>(&m_SceneObjects, m_SelectedEntityIndex);
    if (m_CommandManager.ExecuteCommand(std::move(cmd))) {
        if (m_SelectedEntityIndex >= static_cast<int>(m_SceneObjects.size())) {
            m_SelectedEntityIndex = static_cast<int>(m_SceneObjects.size()) - 1;
        }
    }
}

void EditorLayer::DuplicateSelected() {
    if (m_SelectedEntityIndex < 0 || m_SelectedEntityIndex >= static_cast<int>(m_SceneObjects.size())) {
        return;
    }
    SceneObject copy = m_SceneObjects[static_cast<size_t>(m_SelectedEntityIndex)];
    copy.name += " Copy";
    
    auto cmd = CreateScope<CreateEntityCommand>(&m_SceneObjects, copy);
    if (m_CommandManager.ExecuteCommand(std::move(cmd))) {
        m_SelectedEntityIndex = static_cast<int>(m_SceneObjects.size()) - 1;
    }
}

void EditorLayer::HandleGizmos() {
    if (m_SelectedEntityIndex < 0 || m_SelectedEntityIndex >= static_cast<int>(m_SceneObjects.size())) {
        m_GizmoWasUsing = false;
        return;
    }

    if (!m_ViewportHovered) {
        m_GizmoWasUsing = false;
        return;
    }

    const glm::vec2* bounds = m_ViewportPanel.GetBounds();
    if (!bounds) {
        return;
    }

    ImGuizmo::SetOrthographic(true);
    ImGuizmo::BeginFrame();
    ImGuizmo::SetDrawlist();
    ImGuizmo::SetRect(bounds[0].x, bounds[0].y, m_ViewportSize.x, m_ViewportSize.y);

    SceneObject& object = m_SceneObjects[static_cast<size_t>(m_SelectedEntityIndex)];
    glm::mat4 transform = CalculateTransform(object);

    ImGuizmo::Manipulate(glm::value_ptr(m_ViewMatrix), glm::value_ptr(m_ProjectionMatrix), m_GizmoOperation, m_GizmoMode, glm::value_ptr(transform));

    if (ImGuizmo::IsUsing()) {
        if (!m_GizmoWasUsing) {
            // Start of drag - save old values
            m_GizmoOldPosition = object.position;
            m_GizmoOldRotation = object.rotation;
            m_GizmoOldScale = object.scale;
        }
        m_GizmoWasUsing = true;

        glm::vec3 translation;
        glm::vec3 rotation;
        glm::vec3 scale;
        DecomposeTransform(transform, translation, rotation, scale);
        object.position = translation;
        object.rotation = glm::vec3(0.0f, 0.0f, rotation.z);
        object.scale = scale;
    } else if (m_GizmoWasUsing) {
        // End of drag - create command for undo history
        m_GizmoWasUsing = false;
        
        Scope<ICommand> cmd = nullptr;
        switch (m_GizmoOperation) {
            case ImGuizmo::TRANSLATE:
                cmd = CreateScope<TransformCommand>(&m_SceneObjects, m_SelectedEntityIndex, 
                    TransformCommand::Type::Position, m_GizmoOldPosition, object.position);
                break;
            case ImGuizmo::ROTATE:
                cmd = CreateScope<TransformCommand>(&m_SceneObjects, m_SelectedEntityIndex, 
                    TransformCommand::Type::Rotation, m_GizmoOldRotation, object.rotation);
                break;
            case ImGuizmo::SCALE:
                cmd = CreateScope<TransformCommand>(&m_SceneObjects, m_SelectedEntityIndex, 
                    TransformCommand::Type::Scale, m_GizmoOldScale, object.scale);
                break;
        }
        
        if (cmd) {
            // Register without executing (already applied by gizmo)
            m_CommandManager.RegisterExecutedCommand(std::move(cmd));
        }
    }
}

glm::mat4 EditorLayer::CalculateTransform(const SceneObject& object) const {
    return glm::translate(glm::mat4(1.0f), object.position) *
           glm::rotate(glm::mat4(1.0f), glm::radians(object.rotation.z), glm::vec3(0, 0, 1)) *
           glm::scale(glm::mat4(1.0f), object.scale);
}

void EditorLayer::DecomposeTransform(const glm::mat4& transform, glm::vec3& translation, glm::vec3& rotation, glm::vec3& scale) {
    glm::vec3 skew;
    glm::vec4 perspective;
    glm::quat orientation;
    glm::decompose(transform, scale, orientation, translation, skew, perspective);
    rotation = glm::degrees(glm::eulerAngles(orientation));
}

}  // namespace Vest
