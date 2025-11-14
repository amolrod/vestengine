#pragma once

#include <vector>
#include <memory>
#include <deque>

#include "Core/Base.h"
#include "Core/Log.h"
#include "Commands/ICommand.h"

namespace Vest {

/**
 * @brief Manages command execution, undo, and redo operations
 * 
 * The CommandManager maintains two stacks:
 * - Undo stack: commands that can be undone
 * - Redo stack: commands that can be redone
 * 
 * It also supports command merging for consecutive similar operations.
 */
class CommandManager {
public:
    explicit CommandManager(size_t maxHistorySize = 50)
        : m_MaxHistorySize(maxHistorySize)
    {
    }

    /**
     * @brief Execute a command and add it to the undo stack
     * @param command Command to execute
     * @return true if successful
     */
    bool ExecuteCommand(Scope<ICommand> command) {
        if (!command) {
            VEST_CORE_WARN("Attempted to execute null command");
            return false;
        }

        if (!command->Execute()) {
            VEST_CORE_ERROR("Command execution failed: {0}", command->GetName());
            return false;
        }

        VEST_CORE_TRACE("Executed command: {0}", command->GetName());

        // Try to merge with previous command
        if (!m_UndoStack.empty() && m_UndoStack.back()->CanMergeWith(command.get())) {
            if (m_UndoStack.back()->MergeWith(command.get())) {
                VEST_CORE_TRACE("Merged with previous command");
                return true;
            }
        }

        // Add to undo stack
        m_UndoStack.push_back(std::move(command));

        // Clear redo stack (can't redo after new command)
        m_RedoStack.clear();

        // Enforce history limit
        while (m_UndoStack.size() > m_MaxHistorySize) {
            m_UndoStack.pop_front();
        }

        return true;
    }

    /**
     * @brief Undo the last command
     * @return true if successful
     */
    bool Undo() {
        if (m_UndoStack.empty()) {
            VEST_CORE_TRACE("Nothing to undo");
            return false;
        }

        auto& command = m_UndoStack.back();
        if (!command->Undo()) {
            VEST_CORE_ERROR("Command undo failed: {0}", command->GetName());
            return false;
        }

        VEST_CORE_TRACE("Undid command: {0}", command->GetName());

        // Move to redo stack
        m_RedoStack.push_back(std::move(command));
        m_UndoStack.pop_back();

        return true;
    }

    /**
     * @brief Redo the last undone command
     * @return true if successful
     */
    bool Redo() {
        if (m_RedoStack.empty()) {
            VEST_CORE_TRACE("Nothing to redo");
            return false;
        }

        auto& command = m_RedoStack.back();
        if (!command->Execute()) {
            VEST_CORE_ERROR("Command redo failed: {0}", command->GetName());
            return false;
        }

        VEST_CORE_TRACE("Redid command: {0}", command->GetName());

        // Move back to undo stack
        m_UndoStack.push_back(std::move(command));
        m_RedoStack.pop_back();

        return true;
    }

    /**
     * @brief Check if undo is available
     */
    bool CanUndo() const { return !m_UndoStack.empty(); }

    /**
     * @brief Check if redo is available
     */
    bool CanRedo() const { return !m_RedoStack.empty(); }

    /**
     * @brief Get the name of the next command that would be undone
     */
    std::string GetUndoCommandName() const {
        return m_UndoStack.empty() ? "" : m_UndoStack.back()->GetName();
    }

    /**
     * @brief Get the name of the next command that would be redone
     */
    std::string GetRedoCommandName() const {
        return m_RedoStack.empty() ? "" : m_RedoStack.back()->GetName();
    }

    /**
     * @brief Clear all undo/redo history
     */
    void Clear() {
        m_UndoStack.clear();
        m_RedoStack.clear();
        VEST_CORE_INFO("Command history cleared");
    }

    /**
     * @brief Get the current size of the undo stack
     */
    size_t GetUndoStackSize() const { return m_UndoStack.size(); }

    /**
     * @brief Get the current size of the redo stack
     */
    size_t GetRedoStackSize() const { return m_RedoStack.size(); }

    /**
     * @brief Set maximum history size
     */
    void SetMaxHistorySize(size_t size) {
        m_MaxHistorySize = size;
        while (m_UndoStack.size() > m_MaxHistorySize) {
            m_UndoStack.pop_front();
        }
    }

private:
    std::deque<Scope<ICommand>> m_UndoStack;
    std::deque<Scope<ICommand>> m_RedoStack;
    size_t m_MaxHistorySize;
};

}  // namespace Vest
