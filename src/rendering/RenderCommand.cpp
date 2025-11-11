#include "RenderCommand.h"
#include "core/Logger.h"

namespace Engine {

void RenderCommand::Init() {
    // Enable depth testing by default
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // Enable backface culling by default
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    // Disable blending by default (enable for transparency)
    glDisable(GL_BLEND);

    LOG_INFO("RenderCommand inicializado");
}

void RenderCommand::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
    glViewport(static_cast<GLint>(x), static_cast<GLint>(y), 
               static_cast<GLsizei>(width), static_cast<GLsizei>(height));
}

void RenderCommand::SetClearColor(const glm::vec4& color) {
    glClearColor(color.r, color.g, color.b, color.a);
}

void RenderCommand::Clear() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RenderCommand::DrawIndexed(uint32_t indexCount) {
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indexCount), GL_UNSIGNED_INT, nullptr);
}

void RenderCommand::EnableDepthTest() {
    glEnable(GL_DEPTH_TEST);
}

void RenderCommand::DisableDepthTest() {
    glDisable(GL_DEPTH_TEST);
}

void RenderCommand::EnableCulling() {
    glEnable(GL_CULL_FACE);
}

void RenderCommand::DisableCulling() {
    glDisable(GL_CULL_FACE);
}

void RenderCommand::EnableBlending() {
    glEnable(GL_BLEND);
}

void RenderCommand::DisableBlending() {
    glDisable(GL_BLEND);
}

void RenderCommand::SetBlendFunc(uint32_t src, uint32_t dst) {
    glBlendFunc(src, dst);
}

} // namespace Engine
