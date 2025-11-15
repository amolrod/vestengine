#pragma once

#include "Rendering/RendererAPI.h"

namespace Vest {

class VulkanRendererAPI : public RendererAPI {
public:
    void Init() override;
    void SetViewport(uint32_t, uint32_t, uint32_t, uint32_t) override;
    void SetClearColor(const glm::vec4&) override;
    void Clear() override;
    void DrawIndexed(const Ref<VertexArray>&, uint32_t) override;
    void DrawLines(const Ref<VertexArray>&, uint32_t) override;
};

}  // namespace Vest
