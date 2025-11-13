#pragma once

#include <cstdint>

#include "Core/Base.h"

namespace Vest {

struct FramebufferSpecification {
    uint32_t width = 1280;
    uint32_t height = 720;
    uint32_t samples = 1;
    bool swapChainTarget = false;
};

class Framebuffer {
public:
    virtual ~Framebuffer() = default;

    virtual void Bind() = 0;
    virtual void Unbind() = 0;

    virtual uint32_t GetColorAttachmentRendererID() const = 0;

    virtual const FramebufferSpecification& GetSpecification() const = 0;

    static Ref<Framebuffer> Create(const FramebufferSpecification& spec);
};

}  // namespace Vest
