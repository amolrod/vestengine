#pragma once

namespace Vest {

class VulkanContext {
public:
    explicit VulkanContext(void* /*windowHandle*/) {}

    void Init();
    void SwapBuffers();
};

}  // namespace Vest
