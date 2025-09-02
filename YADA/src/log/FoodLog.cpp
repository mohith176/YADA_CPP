#include "log/FoodLog.h"
#include <fstream>
#include <sstream>
#include <iomanip>
#include <stdexcept>
#include <regex>

FoodLog::FoodLog() {}

size_t FoodLog::addEntry(const std::shared_ptr<Food>& food, double servings, const std::string& date) {
    std::string targetDate = date.empty() ? getCurrentDate() : date;
    if (!isValidDate(targetDate)) {
        throw std::runtime_error("Invalid date format. Use YYYY-MM-DD");
    }
    
    // Check if this food already exists in today's log
    size_t existingIndex = findExistingEntry(food->getName(), targetDate);
    
    if (existingIndex != static_cast<size_t>(-1)) {
        // Food exists, update servings
        auto& entries = dailyLogs[targetDate];
        entries[existingIndex].servings += servings;
        entries[existingIndex].timestamp = std::time(nullptr);  // Update timestamp
        return existingIndex;
    } else {
        // Add new entry
        LogEntry entry{food->getName(), servings, std::time(nullptr)};
        auto& entries = dailyLogs[targetDate];
        size_t index = entries.size();
        entries.push_back(entry);
        return index;
    }
}

void FoodLog::updateServings(size_t index, double newServings, const std::string& date) {
    std::string targetDate = date.empty() ? getCurrentDate() : date;
    if (!isValidDate(targetDate)) {
        throw std::runtime_error("Invalid date format. Use YYYY-MM-DD");
    }

    auto& entries = dailyLogs[targetDate];
    if (index < entries.size()) {
        if (newServings <= 0) {
            // If servings reduced to 0 or less, remove the entry
            entries.erase(entries.begin() + index);
        } else {
            entries[index].servings = newServings;
            entries[index].timestamp = std::time(nullptr);
        }
    }
}

double FoodLog::getServings(size_t index, const std::string& date) const {
    std::string targetDate = date.empty() ? getCurrentDate() : date;
    if (!isValidDate(targetDate)) {
        throw std::runtime_error("Invalid date format. Use YYYY-MM-DD");
    }

    auto it = dailyLogs.find(targetDate);
    if (it != dailyLogs.end() && index < it->second.size()) {
        return it->second[index].servings;
    }
    return 0.0;
}

size_t FoodLog::findExistingEntry(const std::string& foodId, const std::string& date) {
    auto& entries = dailyLogs[date];
    for (size_t i = 0; i < entries.size(); ++i) {
        if (entries[i].foodId == foodId) {
            return i;
        }
    }
    return static_cast<size_t>(-1);  // Not found
}

void FoodLog::removeEntry(size_t index, const std::string& date) {
    std::string targetDate = date.empty() ? getCurrentDate() : date;
    if (!isValidDate(targetDate)) {
        throw std::runtime_error("Invalid date format. Use YYYY-MM-DD");
    }
    
    auto& entries = dailyLogs[targetDate];
    if (index < entries.size()) {
        entries.erase(entries.begin() + index);
    }
}

void FoodLog::clearEntriesForDate(const std::string& date) {
    if (!isValidDate(date)) {
        throw std::runtime_error("Invalid date format. Use YYYY-MM-DD");
    }
    dailyLogs[date].clear();
}

std::vector<LogEntry> FoodLog::getEntriesForDate(const std::string& date) const {
    if (!isValidDate(date)) {
        throw std::runtime_error("Invalid date format. Use YYYY-MM-DD");
    }
    auto it = dailyLogs.find(date);
    if (it != dailyLogs.end()) {
        return it->second;
    }
    return std::vector<LogEntry>();
}

double FoodLog::getTotalCaloriesForDate(const std::string& date, 
    const std::function<std::shared_ptr<Food>(const std::string&)>& foodLookup) const {
    if (!isValidDate(date)) {
        throw std::runtime_error("Invalid date format. Use YYYY-MM-DD");
    }
    double total = 0.0;
    auto entries = getEntriesForDate(date);
    for (const auto& entry : entries) {
        auto food = foodLookup(entry.foodId);
        if (food) {
            total += food->getCaloriesPerServing() * entry.servings;
        }
    }
    return total;
}

bool FoodLog::isValidDate(const std::string& date) {
    std::regex datePattern("^\\d{4}-\\d{2}-\\d{2}$");
    if (!std::regex_match(date, datePattern)) {
        return false;
    }
    
    std::tm tm = {};
    std::istringstream ss(date);
    ss >> std::get_time(&tm, "%Y-%m-%d");
    return !ss.fail();
}

void FoodLog::saveToFile(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file) {
        throw std::runtime_error("Could not open file for writing: " + filename);
    }

    for (const auto& pair : dailyLogs) {
        const auto& date = pair.first;
        const auto& entries = pair.second;
        for (const auto& entry : entries) {
            file << date << "|"
                 << entry.foodId << "|"
                 << entry.servings << "|"
                 << entry.timestamp << "\n";
        }
    }
}

void FoodLog::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file) {
        throw std::runtime_error("Could not open file for reading: " + filename);
    }

    dailyLogs.clear();
    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string date, foodId;
        double servings;
        std::time_t timestamp;

        std::getline(ss, date, '|');
        std::getline(ss, foodId, '|');
        ss >> servings;
        ss.ignore();
        ss >> timestamp;

        LogEntry entry{foodId, servings, timestamp};
        dailyLogs[date].push_back(entry);
    }
}

std::string FoodLog::getCurrentDate() {
    return formatDate(std::time(nullptr));
}

std::string FoodLog::formatDate(const std::time_t& time) {
    std::tm* tm = std::localtime(&time);
    std::ostringstream oss;
    oss << std::put_time(tm, "%Y-%m-%d");
    return oss.str();
}

std::time_t FoodLog::parseDate(const std::string& date) {
    std::tm tm = {};
    std::istringstream ss(date);
    ss >> std::get_time(&tm, "%Y-%m-%d");
    return std::mktime(&tm);
} 