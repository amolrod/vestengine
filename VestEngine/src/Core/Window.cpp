#include "Core/Window.h"

#include "Platform/Windows/WindowsWindow.h"

namespace Vest {

Scope<Window> Window::Create(const WindowProps& props) {
    return CreateScope<WindowsWindow>(props);
}

}  // namespace Vest
