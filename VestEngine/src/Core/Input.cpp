#include "Core/Input.h"

#include <GLFW/glfw3.h>

#include "Core/Application.h"

namespace Vest {

bool Input::IsKeyPressed(int keycode) {
    GLFWwindow* window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
    int state = glfwGetKey(window, keycode);
    return state == GLFW_PRESS || state == GLFW_REPEAT;
}

glm::vec2 Input::GetMousePosition() {
    GLFWwindow* window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
    double xPos = 0.0;
    double yPos = 0.0;
    glfwGetCursorPos(window, &xPos, &yPos);
    return {static_cast<float>(xPos), static_cast<float>(yPos)};
}

}  // namespace Vest
