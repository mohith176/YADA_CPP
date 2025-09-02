#ifndef YADA_COMMAND_H
#define YADA_COMMAND_H

#include <string>
#include <memory>
#include <stack>
#include "food/Food.h"
#include "food/BasicFood.h"
#include "log/FoodLog.h"

// Command interface
class Command {
public:
    virtual ~Command() = default;
    virtual void execute() = 0;
    virtual void undo() = 0;
    virtual std::string getDescription() const = 0;
};

// Command for adding food to log
class AddFoodCommand : public Command {
public:
    AddFoodCommand(FoodLog& log, std::shared_ptr<Food> food, double servings, const std::string& date = "");
    void execute() override;
    void undo() override;
    std::string getDescription() const override;

private:
    FoodLog& log;
    std::shared_ptr<Food> food;
    double servings;
    double originalServings;  // Store original servings for undo
    std::string date;
    size_t entryIndex;
    bool wasExistingEntry;
};

// Command for removing food from log
class RemoveFoodCommand : public Command {
public:
    RemoveFoodCommand(FoodLog& log, size_t index, std::shared_ptr<Food> originalFood, const std::string& date = "");
    void execute() override;
    void undo() override;
    std::string getDescription() const override;

private:
    FoodLog& log;
    size_t index;
    LogEntry removedEntry;
    std::shared_ptr<Food> originalFood;
    std::string date;
};

// Command manager for handling undo/redo
class CommandManager {
public:
    void executeCommand(std::unique_ptr<Command> command);
    void undo();
    bool canUndo() const;
    void clearHistory();

private:
    std::stack<std::unique_ptr<Command>> undoStack;
};

#endif // YADA_COMMAND_H 