#ifndef YADA_FOOD_LOG_H
#define YADA_FOOD_LOG_H

#include "food/Food.h"
#include <memory>
#include <map>
#include <vector>
#include <string>
#include <ctime>
#include <functional>

struct LogEntry {
    std::string foodId;  // Store food name as reference
    double servings;
    std::time_t timestamp;
};

class FoodLog {
public:
    FoodLog();

    // Log operations
    size_t addEntry(const std::shared_ptr<Food>& food, double servings, const std::string& date = "");
    void removeEntry(size_t index, const std::string& date = "");
    void clearEntriesForDate(const std::string& date);
    void updateServings(size_t index, double newServings, const std::string& date = "");
    double getServings(size_t index, const std::string& date = "") const;
    
    // Query operations
    std::vector<LogEntry> getEntriesForDate(const std::string& date) const;
    double getTotalCaloriesForDate(const std::string& date, 
        const std::function<std::shared_ptr<Food>(const std::string&)>& foodLookup) const;
    
    // File operations
    void saveToFile(const std::string& filename) const;
    void loadFromFile(const std::string& filename);

    // Date operations
    static std::string getCurrentDate();
    static bool isValidDate(const std::string& date);

private:
    std::map<std::string, std::vector<LogEntry>> dailyLogs;  // date -> entries
    
    // Helper functions
    static std::string formatDate(const std::time_t& time);
    static std::time_t parseDate(const std::string& date);
    size_t findExistingEntry(const std::string& foodId, const std::string& date);
};

#endif // YADA_FOOD_LOG_H 