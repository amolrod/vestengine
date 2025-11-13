#include "EditorLayer.h"

#include "Core/EntryPoint.h"

namespace Vest {

class VestEditorApplication : public Application {
public:
    VestEditorApplication() : Application("VestEditor") {
        PushLayer(new EditorLayer());
    }
};

Application* CreateApplication() {
    return new VestEditorApplication();
}

}  // namespace Vest
