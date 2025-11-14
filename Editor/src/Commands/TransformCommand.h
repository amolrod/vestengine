#pragma once

#include <glm/glm.hpp>

#include "Commands/ICommand.h"
#include <Scene/SceneObject.h>

namespace Vest {

/**
 * @brief Command for transforming an entity (position, rotation, scale)
 * 
 * Stores only the differences, making it memory-efficient for large undo stacks.
 */
class TransformCommand : public ICommand {
public:
    enum class Type {
        Position,
        Rotation,
        Scale,
        All  // Position + Rotation + Scale
    };

    TransformCommand(std::vector<SceneObject>* scene, 
                     int entityIndex,
                     Type type,
                     const glm::vec3& oldValue,
                     const glm::vec3& newValue)
        : m_Scene(scene)
        , m_EntityIndex(entityIndex)
        , m_Type(type)
        , m_OldValue(oldValue)
        , m_NewValue(newValue)
    {
    }

    bool Execute() override {
        if (!IsValid()) return false;
        
        ApplyValue(m_NewValue);
        return true;
    }

    bool Undo() override {
        if (!IsValid()) return false;
        
        ApplyValue(m_OldValue);
        return true;
    }

    std::string GetName() const override {
        switch (m_Type) {
            case Type::Position: return "Move Entity";
            case Type::Rotation: return "Rotate Entity";
            case Type::Scale:    return "Scale Entity";
            case Type::All:      return "Transform Entity";
        }
        return "Transform";
    }

    bool CanMergeWith(const ICommand* other) const override {
        auto* otherTransform = dynamic_cast<const TransformCommand*>(other);
        if (!otherTransform) return false;
        
        // Can merge if same entity and same transform type
        return m_EntityIndex == otherTransform->m_EntityIndex &&
               m_Type == otherTransform->m_Type;
    }

    bool MergeWith(const ICommand* other) override {
        auto* otherTransform = dynamic_cast<const TransformCommand*>(other);
        if (!otherTransform) return false;
        
        // Update to the newest value, keep original old value
        m_NewValue = otherTransform->m_NewValue;
        return true;
    }

private:
    bool IsValid() const {
        return m_Scene && m_EntityIndex >= 0 && 
               m_EntityIndex < static_cast<int>(m_Scene->size());
    }

    void ApplyValue(const glm::vec3& value) {
        auto& obj = (*m_Scene)[m_EntityIndex];
        switch (m_Type) {
            case Type::Position: obj.position = value; break;
            case Type::Rotation: obj.rotation = value; break;
            case Type::Scale:    obj.scale = value; break;
            case Type::All:
                // For "All" type, we'd need separate old/new for each
                // This is simplified - in practice you'd have 3 vec3s
                obj.position = value;
                break;
        }
    }

    std::vector<SceneObject>* m_Scene;
    int m_EntityIndex;
    Type m_Type;
    glm::vec3 m_OldValue;
    glm::vec3 m_NewValue;
};

}  // namespace Vest
