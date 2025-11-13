#pragma once

#include <GLFW/glfw3.h>

namespace Vest {

class OpenGLContext {
public:
    explicit OpenGLContext(GLFWwindow* windowHandle);

    void Init();
    void SwapBuffers();

private:
    GLFWwindow* m_WindowHandle = nullptr;
};

}  // namespace Vest
