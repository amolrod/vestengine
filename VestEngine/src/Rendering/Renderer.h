#pragma once

#include <glm/glm.hpp>

#include "Core/Base.h"
#include "Rendering/RenderCommand.h"
#include "Rendering/Shader.h"
#include "Rendering/VertexArray.h"

namespace Vest {

class Renderer {
public:
    static void Init();
    static void Shutdown();

    static void OnWindowResize(uint32_t width, uint32_t height);

    static void BeginScene(const glm::mat4& viewProjectionMatrix);
    static void EndScene();

    static void Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.0f));

private:
    struct SceneData {
        glm::mat4 ViewProjectionMatrix = glm::mat4(1.0f);
    };

    static Scope<SceneData> s_SceneData;
};

}  // namespace Vest
