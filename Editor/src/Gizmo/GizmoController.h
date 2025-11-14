#pragma once

namespace Vest {

class GizmoController {
public:
    enum class Operation { Translate, Rotate, Scale };

    void SetOperation(Operation op) { m_Operation = op; }
    Operation GetOperation() const { return m_Operation; }

private:
    Operation m_Operation = Operation::Translate;
};

}  // namespace Vest
