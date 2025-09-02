#include "command/Command.h"

// AddFoodCommand implementation
AddFoodCommand::AddFoodCommand(FoodLog& log, std::shared_ptr<Food> food, double servings, const std::string& date)
    : log(log), food(food), servings(servings), originalServings(0), 
      date(date.empty() ? FoodLog::getCurrentDate() : date), 
      entryIndex(0), wasExistingEntry(false) {}

void AddFoodCommand::execute() {
    // Store the original servings before adding
    entryIndex = log.addEntry(food, servings, date);
    wasExistingEntry = true;
    originalServings = log.getServings(entryIndex, date) - servings;  // Subtract new servings to get original
}

void AddFoodCommand::undo() {
    if (wasExistingEntry) {
        // If it was an existing entry, restore the original servings
        log.updateServings(entryIndex, originalServings, date);
    } else {
        // If it was a new entry, remove it
        log.removeEntry(entryIndex, date);
    }
}

std::string AddFoodCommand::getDescription() const {
    return "Add " + std::to_string(servings) + " serving(s) of " + food->getName() + " on " + date;
}

// RemoveFoodCommand implementation
RemoveFoodCommand::RemoveFoodCommand(FoodLog& log, size_t index, std::shared_ptr<Food> originalFood, const std::string& date)
    : log(log), index(index), originalFood(originalFood), date(date.empty() ? FoodLog::getCurrentDate() : date) {}

void RemoveFoodCommand::execute() {
    auto entries = log.getEntriesForDate(date);
    if (index < entries.size()) {
        removedEntry = entries[index];
        log.removeEntry(index, date);
    }
}

void RemoveFoodCommand::undo() {
    log.addEntry(originalFood, removedEntry.servings, date);
}

std::string RemoveFoodCommand::getDescription() const {
    return "Remove " + originalFood->getName() + " on " + date;
}

// CommandManager implementation
void CommandManager::executeCommand(std::unique_ptr<Command> command) {
    command->execute();
    undoStack.push(std::move(command));
}

void CommandManager::undo() {
    if (!undoStack.empty()) {
        undoStack.top()->undo();
        undoStack.pop();
    }
}

bool CommandManager::canUndo() const {
    return !undoStack.empty();
}

void CommandManager::clearHistory() {
    while (!undoStack.empty()) {
        undoStack.pop();
    }
} 