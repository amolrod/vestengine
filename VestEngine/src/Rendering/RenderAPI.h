#pragma once

#include <string>
#include <string_view>

namespace Vest {

enum class RenderAPI {
    None = 0,
    OpenGL,
    Vulkan
};

inline std::string ToString(RenderAPI api) {
    switch (api) {
        case RenderAPI::OpenGL:
            return "OpenGL";
        case RenderAPI::Vulkan:
            return "Vulkan";
        default:
            return "None";
    }
}

inline RenderAPI ParseRenderAPI(std::string_view name) {
    if (name == "Vulkan") {
        return RenderAPI::Vulkan;
    }
    if (name == "OpenGL") {
        return RenderAPI::OpenGL;
    }
    return RenderAPI::None;
}

inline RenderAPI ResolveDefaultRenderAPI() {
#ifdef VEST_RENDERER_API_DEFAULT
    constexpr std::string_view configured = VEST_RENDERER_API_DEFAULT;
#else
    constexpr std::string_view configured = "OpenGL";
#endif
    RenderAPI api = ParseRenderAPI(configured);
    return api == RenderAPI::None ? RenderAPI::OpenGL : api;
}

}  // namespace Vest
