#include "food/FoodDataSource.h"
#include <fstream>
#include <sstream>

FileFoodSource::FileFoodSource(const std::string& filename) : filename(filename) {
    loadFromFile();
}

std::vector<std::shared_ptr<BasicFood>> FileFoodSource::getFoods() {
    return foods;
}

bool FileFoodSource::addFood(const std::shared_ptr<BasicFood>& food) {
    // Check for duplicates
    for (const auto& existingFood : foods) {
        if (existingFood->getName() == food->getName()) {
            return false;
        }
    }
    foods.push_back(food);
    return true;
}

void FileFoodSource::saveChanges() {
    saveToFile();
}

void FileFoodSource::loadFromFile() {
    std::ifstream file(filename);
    if (!file) {
        return; // New file will be created on save
    }

    foods.clear();
    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string name;
        double calories;
        size_t keywordCount;

        std::getline(ss, name, '|');
        ss >> calories;
        ss.ignore();
        ss >> keywordCount;
        ss.ignore();

        auto food = std::make_shared<BasicFood>(name, calories);

        // Load keywords
        for (size_t i = 0; i < keywordCount; ++i) {
            std::string keyword;
            std::getline(ss, keyword, '|');
            food->addKeyword(keyword);
        }

        foods.push_back(food);
    }
}

void FileFoodSource::saveToFile() {
    std::ofstream file(filename);
    if (!file) {
        throw std::runtime_error("Could not open file for writing: " + filename);
    }

    for (const auto& food : foods) {
        const auto& keywords = food->getKeywords();
        file << food->getName() << "|"
             << food->getCaloriesPerServing() << "|"
             << keywords.size();
        
        for (const auto& keyword : keywords) {
            file << "|" << keyword;
        }
        file << "\n";
    }
}

// UserInputFoodSource implementation
std::vector<std::shared_ptr<BasicFood>> UserInputFoodSource::getFoods() {
    return foods;
}

bool UserInputFoodSource::addFood(const std::shared_ptr<BasicFood>& food) {
    foods.push_back(food);
    return true;
}

void UserInputFoodSource::saveChanges() {
    // Nothing to save for user input source
}

// WebFoodSource implementation (placeholder)
WebFoodSource::WebFoodSource(const std::string& apiUrl) : apiUrl(apiUrl) {}

std::vector<std::shared_ptr<BasicFood>> WebFoodSource::getFoods() {
    // TODO: Implement API call to get foods
    return foods;
}

bool WebFoodSource::addFood(const std::shared_ptr<BasicFood>& food) {
    // TODO: Implement API call to add food
    foods.push_back(food);
    return true;
}

void WebFoodSource::saveChanges() {
    // TODO: Implement API call to save changes
} 