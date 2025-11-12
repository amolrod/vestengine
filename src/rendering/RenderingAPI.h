/**
 * @file RenderingAPI.h
 * @brief Enumeración de APIs de rendering soportadas
 */

#pragma once

namespace Engine {

/**
 * @enum RenderingAPI
 * @brief APIs de rendering disponibles
 */
enum class RenderingAPI {
    None = 0,
    OpenGL = 1,
    Vulkan = 2,
    DirectX11 = 3,  // Futuro
    DirectX12 = 4,  // Futuro
    Metal = 5       // Futuro (macOS/iOS)
};

} // namespace Engine
