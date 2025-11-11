/**
 * @file Input.cpp
 * @brief Implementación del sistema de input
 */

#include "input/Input.h"
#include "core/Logger.h"

#include <GLFW/glfw3.h>

// Forward declaration
extern GLFWwindow* g_currentWindow;

namespace Engine {

bool Input::IsKeyPressed(Key key) {
    if (!g_currentWindow) {
        LOG_CORE_ERROR("No hay ventana activa para consultar input");
        return false;
    }
    
    int state = glfwGetKey(g_currentWindow, static_cast<int>(key));
    return state == GLFW_PRESS || state == GLFW_REPEAT;
}

bool Input::IsMouseButtonPressed(MouseButton button) {
    if (!g_currentWindow) {
        LOG_CORE_ERROR("No hay ventana activa para consultar input");
        return false;
    }
    
    int state = glfwGetMouseButton(g_currentWindow, static_cast<int>(button));
    return state == GLFW_PRESS;
}

glm::vec2 Input::GetMousePosition() {
    if (!g_currentWindow) {
        LOG_CORE_ERROR("No hay ventana activa para consultar input");
        return glm::vec2(0.0f);
    }
    
    double xpos, ypos;
    glfwGetCursorPos(g_currentWindow, &xpos, &ypos);
    
    return glm::vec2(static_cast<float>(xpos), static_cast<float>(ypos));
}

float Input::GetMouseX() {
    return GetMousePosition().x;
}

float Input::GetMouseY() {
    return GetMousePosition().y;
}

} // namespace Engine
