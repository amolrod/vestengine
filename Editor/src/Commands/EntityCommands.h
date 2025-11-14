#pragma once

#include "Commands/ICommand.h"
#include <Scene/SceneObject.h>

namespace Vest {

/**
 * @brief Command for creating a new entity
 */
class CreateEntityCommand : public ICommand {
public:
    CreateEntityCommand(std::vector<SceneObject>* scene, const SceneObject& entity)
        : m_Scene(scene)
        , m_Entity(entity)
        , m_Index(-1)
    {
    }

    bool Execute() override {
        if (!m_Scene) return false;
        
        m_Scene->push_back(m_Entity);
        m_Index = static_cast<int>(m_Scene->size()) - 1;
        return true;
    }

    bool Undo() override {
        if (!m_Scene || m_Index < 0 || m_Index >= static_cast<int>(m_Scene->size())) {
            return false;
        }
        
        m_Scene->erase(m_Scene->begin() + m_Index);
        return true;
    }

    std::string GetName() const override {
        return "Create Entity: " + m_Entity.name;
    }

    int GetCreatedIndex() const { return m_Index; }

private:
    std::vector<SceneObject>* m_Scene;
    SceneObject m_Entity;
    int m_Index;
};

/**
 * @brief Command for deleting an entity
 */
class DeleteEntityCommand : public ICommand {
public:
    DeleteEntityCommand(std::vector<SceneObject>* scene, int entityIndex)
        : m_Scene(scene)
        , m_EntityIndex(entityIndex)
    {
        // Store the entity before deletion
        if (m_Scene && entityIndex >= 0 && entityIndex < static_cast<int>(m_Scene->size())) {
            m_DeletedEntity = (*m_Scene)[entityIndex];
        }
    }

    bool Execute() override {
        if (!m_Scene || m_EntityIndex < 0 || m_EntityIndex >= static_cast<int>(m_Scene->size())) {
            return false;
        }
        
        m_Scene->erase(m_Scene->begin() + m_EntityIndex);
        return true;
    }

    bool Undo() override {
        if (!m_Scene || m_EntityIndex < 0 || m_EntityIndex > static_cast<int>(m_Scene->size())) {
            return false;
        }
        
        // Restore at original position
        m_Scene->insert(m_Scene->begin() + m_EntityIndex, m_DeletedEntity);
        return true;
    }

    std::string GetName() const override {
        return "Delete Entity: " + m_DeletedEntity.name;
    }

private:
    std::vector<SceneObject>* m_Scene;
    int m_EntityIndex;
    SceneObject m_DeletedEntity;
};

/**
 * @brief Command for modifying entity color
 */
class ModifyColorCommand : public ICommand {
public:
    ModifyColorCommand(std::vector<SceneObject>* scene, 
                       int entityIndex,
                       const glm::vec4& oldColor,
                       const glm::vec4& newColor)
        : m_Scene(scene)
        , m_EntityIndex(entityIndex)
        , m_OldColor(oldColor)
        , m_NewColor(newColor)
    {
    }

    bool Execute() override {
        if (!IsValid()) return false;
        (*m_Scene)[m_EntityIndex].color = m_NewColor;
        return true;
    }

    bool Undo() override {
        if (!IsValid()) return false;
        (*m_Scene)[m_EntityIndex].color = m_OldColor;
        return true;
    }

    std::string GetName() const override {
        return "Modify Color";
    }

private:
    bool IsValid() const {
        return m_Scene && m_EntityIndex >= 0 && 
               m_EntityIndex < static_cast<int>(m_Scene->size());
    }

    std::vector<SceneObject>* m_Scene;
    int m_EntityIndex;
    glm::vec4 m_OldColor;
    glm::vec4 m_NewColor;
};

}  // namespace Vest
