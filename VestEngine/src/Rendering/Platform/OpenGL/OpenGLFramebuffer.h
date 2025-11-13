#pragma once

#include "Rendering/Framebuffer.h"

namespace Vest {

class OpenGLFramebuffer : public Framebuffer {
public:
    explicit OpenGLFramebuffer(const FramebufferSpecification& spec);
    ~OpenGLFramebuffer() override;

    void Bind() override;
    void Unbind() override;

    uint32_t GetColorAttachmentRendererID() const override { return m_ColorAttachment; }

    const FramebufferSpecification& GetSpecification() const override { return m_Specification; }

    void Invalidate();

private:
    uint32_t m_RendererID = 0;
    uint32_t m_ColorAttachment = 0;
    FramebufferSpecification m_Specification;
};

}  // namespace Vest
