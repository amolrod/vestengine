#pragma once

#include <cstdint>
#include <glm/glm.hpp>

#include "Core/Base.h"
#include "Rendering/RenderAPI.h"

namespace Vest {

class VertexArray;

class RendererAPI {
public:
    virtual ~RendererAPI() = default;

    virtual void Init() = 0;
    virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;
    virtual void SetClearColor(const glm::vec4& color) = 0;
    virtual void Clear() = 0;
    virtual void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount = 0) = 0;
    virtual void DrawLines(const Ref<VertexArray>& vertexArray, uint32_t vertexCount) = 0;

    static RenderAPI GetAPI() { return s_API; }
    static void SetAPI(RenderAPI api) { s_API = api; }

private:
    inline static RenderAPI s_API = RenderAPI::OpenGL;
};

}  // namespace Vest
