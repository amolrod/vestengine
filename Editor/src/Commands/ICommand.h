#pragma once

#include <string>
#include <memory>

#include "Core/Base.h"

namespace Vest {

/**
 * @brief Base interface for all undoable commands
 * 
 * Commands encapsulate an action that can be executed and undone.
 * This pattern allows for a flexible and extensible undo/redo system.
 */
class ICommand {
public:
    virtual ~ICommand() = default;

    /**
     * @brief Execute the command
     * @return true if successful, false otherwise
     */
    virtual bool Execute() = 0;

    /**
     * @brief Undo the command, reverting its effects
     * @return true if successful, false otherwise
     */
    virtual bool Undo() = 0;

    /**
     * @brief Get a human-readable name for this command
     * @return Command name (e.g., "Move Entity", "Delete Entity")
     */
    virtual std::string GetName() const = 0;

    /**
     * @brief Check if this command can be merged with another
     * 
     * Allows consecutive similar commands to be combined
     * (e.g., multiple small translations into one)
     */
    virtual bool CanMergeWith(const ICommand* other) const { return false; }

    /**
     * @brief Merge this command with another
     * @param other Command to merge with
     * @return true if merge was successful
     */
    virtual bool MergeWith(const ICommand* other) { return false; }
};

}  // namespace Vest
