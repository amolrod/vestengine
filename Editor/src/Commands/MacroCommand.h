#pragma once

#include <vector>
#include <string>

#include "Core/Base.h"
#include "Commands/ICommand.h"

namespace Vest {

/**
 * @brief Composite command that groups multiple commands into one
 * 
 * Useful for operations that involve multiple steps that should be
 * undone/redone as a single unit (e.g., creating an entity with specific properties).
 */
class MacroCommand : public ICommand {
public:
    explicit MacroCommand(std::string name = "Macro Command")
        : m_Name(std::move(name))
    {
    }

    /**
     * @brief Add a command to the group
     */
    void AddCommand(Scope<ICommand> command) {
        if (command) {
            m_Commands.push_back(std::move(command));
        }
    }

    /**
     * @brief Execute all commands in order
     */
    bool Execute() override {
        for (auto& command : m_Commands) {
            if (!command->Execute()) {
                // Attempt to rollback executed commands
                for (auto it = m_Commands.rbegin(); it->get() != command.get(); ++it) {
                    (*it)->Undo();
                }
                return false;
            }
        }
        return !m_Commands.empty();
    }

    /**
     * @brief Undo all commands in reverse order
     */
    bool Undo() override {
        bool success = true;
        for (auto it = m_Commands.rbegin(); it != m_Commands.rend(); ++it) {
            if (!(*it)->Undo()) {
                success = false;
            }
        }
        return success;
    }

    /**
     * @brief Get the name of the macro command
     */
    std::string GetName() const override {
        return m_Name + " (" + std::to_string(m_Commands.size()) + " operations)";
    }

    /**
     * @brief Macro commands cannot be merged
     */
    bool CanMergeWith(const ICommand* other) const override {
        return false;
    }

    bool MergeWith(const ICommand* other) override {
        return false;
    }

    /**
     * @brief Get the number of commands in the macro
     */
    size_t GetCommandCount() const { return m_Commands.size(); }

    /**
     * @brief Check if the macro is empty
     */
    bool IsEmpty() const { return m_Commands.empty(); }

private:
    std::vector<Scope<ICommand>> m_Commands;
    std::string m_Name;
};

}  // namespace Vest
