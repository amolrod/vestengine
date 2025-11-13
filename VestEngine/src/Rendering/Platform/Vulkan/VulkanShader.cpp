#include "Rendering/Platform/Vulkan/VulkanShader.h"

#include <cassert>

namespace Vest {

VulkanShader::VulkanShader(const std::string& filepath) : m_Name(filepath) {
    assert(false && "Vulkan shader backend not implemented");
}

VulkanShader::VulkanShader(const std::string& name, const std::string&, const std::string&) : m_Name(name) {
    assert(false && "Vulkan shader backend not implemented");
}

}  // namespace Vest
