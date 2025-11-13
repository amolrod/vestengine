#include "EditorLayer.h"

#include <filesystem>

#include <imgui.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Rendering/RenderCommand.h"
#include "Rendering/Buffer.h"

namespace Vest {

EditorLayer::EditorLayer()
    : Layer("EditorLayer"),
      m_SceneHierarchyPanel("Scene Hierarchy"),
      m_PropertiesPanel("Properties"),
      m_ContentBrowserPanel("Content Browser"),
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

void main() {
    v_Color = a_Color;
    gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
})";

    const std::string fragmentSrc = R"(#version 410 core
layout(location = 0) out vec4 o_Color;

in vec3 v_Color;

void main() {
    o_Color = vec4(v_Color, 1.0);
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

void main() {
    o_Color = texture(u_Texture, v_TexCoord);
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

    m_SceneObjects.push_back(SceneObject{
        .name = "Triangle",
        .position = glm::vec3(-0.2f, -0.1f, 0.0f),
        .scale = glm::vec3(1.0f),
        .textured = false});
    m_SceneObjects.push_back(SceneObject{
        .name = "Textured Quad",
        .position = glm::vec3(0.6f, 0.0f, 0.0f),
        .scale = glm::vec3(0.75f),
        .textured = true});
    m_SelectedEntityIndex = 0;
}

void EditorLayer::OnUpdate(Timestep ts) {
    m_FPS = ts.GetSeconds() > 0.0f ? 1.0f / ts.GetSeconds() : 0.0f;
    m_DrawCalls = 0;

    if (m_Framebuffer && m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f) {
        m_Framebuffer->Bind();
        RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1.0f});
        RenderCommand::Clear();

        const float aspect = m_ViewportSize.x / m_ViewportSize.y;
        const float orthoHeight = m_CameraZoom;
        const float orthoWidth = orthoHeight * aspect;
        glm::mat4 projection = glm::ortho(-orthoWidth, orthoWidth, -orthoHeight, orthoHeight, -1.0f, 10.0f);
        glm::mat4 view = glm::translate(glm::mat4(1.0f), -m_CameraPosition);

        Renderer::BeginScene(projection * view);
        for (const auto& object : m_SceneObjects) {
            glm::mat4 transform =
                glm::translate(glm::mat4(1.0f), object.position) *
                glm::scale(glm::mat4(1.0f), object.scale);

            if (object.textured) {
                if (m_TextureShader && m_TextureVA && m_CheckerTexture) {
                    m_CheckerTexture->Bind();
                    Renderer::Submit(m_TextureShader, m_TextureVA, transform);
                    ++m_DrawCalls;
                }
            } else {
                if (m_TriangleShader && m_TriangleVA) {
                    Renderer::Submit(m_TriangleShader, m_TriangleVA, transform);
                    ++m_DrawCalls;
                }
            }
        }
        Renderer::EndScene();

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

    if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            ImGui::MenuItem("New Scene", nullptr, false, true);
            if (ImGui::MenuItem("Exit")) {
                dockspaceOpen = false;
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
    ImGui::TextUnformatted("[Play] [Pause] [Step]");
    ImGui::End();

    m_ViewportPanel.OnImGuiRender();
    const glm::vec2 viewportSize = m_ViewportPanel.GetViewportSize();
    if (viewportSize.x > 0.0f && viewportSize.y > 0.0f && viewportSize != m_ViewportSize) {
        RebuildFramebuffer(static_cast<uint32_t>(viewportSize.x), static_cast<uint32_t>(viewportSize.y));
    }

    m_SceneHierarchyPanel.OnImGuiRender();
    m_PropertiesPanel.OnImGuiRender();
    m_ContentBrowserPanel.OnImGuiRender();
    m_StatsPanel.OnImGuiRender();

    ImGui::End();
}

void EditorLayer::OnEvent(Event&) {}

void EditorLayer::RebuildFramebuffer(uint32_t width, uint32_t height) {
    m_ViewportSize = {static_cast<float>(width), static_cast<float>(height)};

    FramebufferSpecification spec = m_Framebuffer ? m_Framebuffer->GetSpecification() : FramebufferSpecification{};
    spec.width = width;
    spec.height = height;
    m_Framebuffer = Framebuffer::Create(spec);
    m_ViewportPanel.SetFramebuffer(m_Framebuffer);
}

}  // namespace Vest
