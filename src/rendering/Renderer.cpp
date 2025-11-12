#include "Renderer.h"
#include "RenderCommand.h"
#include "core/Logger.h"

namespace Engine {

Renderer::SceneData* Renderer::s_SceneData = nullptr;

void Renderer::Init() {
    RenderCommand::Init();
    
    s_SceneData = new SceneData();
    
    LOG_INFO("Renderer inicializado");
}

void Renderer::BeginScene(const Camera& camera) {
    s_SceneData->ViewMatrix = camera.GetViewMatrix();
    s_SceneData->ProjectionMatrix = camera.GetProjectionMatrix();
    s_SceneData->ViewProjectionMatrix = s_SceneData->ProjectionMatrix * s_SceneData->ViewMatrix;
}

void Renderer::Submit(const std::shared_ptr<Shader>& shader,
                     const std::shared_ptr<Mesh>& mesh,
                     const glm::mat4& transform) {
    shader->Bind();
    
    // Set MVP matrix
    glm::mat4 mvp = s_SceneData->ViewProjectionMatrix * transform;
    shader->SetMat4("u_MVP", mvp);
    
    // Optional: Set model matrix (for lighting calculations in shaders)
    shader->SetMat4("u_Model", transform);
    
    // Bind mesh and draw
    mesh->Bind();
    RenderCommand::DrawIndexed(mesh->GetIndexCount());
}

void Renderer::EndScene() {
    // Flush any pending operations (currently none, but extensible)
}

void Renderer::OnWindowResize(uint32_t width, uint32_t height) {
    RenderCommand::SetViewport(0, 0, width, height);
}

} // namespace Engine
