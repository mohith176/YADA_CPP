#include "ui/UserInterface.h"
#include <iostream>
#include <limits>
#include <algorithm>
#include <sstream>
#include <fstream>

UserInterface::UserInterface() {
    foodLog = std::make_unique<FoodLog>();
    commandManager = std::make_unique<CommandManager>();
    loadData();
}

void UserInterface::run() {
    if (!userProfile) {
        std::cout << "Welcome to YADA! Please create your profile first.\n";
        createProfile();
    }

    while (true) {
        showMainMenu();
        std::string choice = getInput("Enter your choice: ");
        
        if (choice == "1") {
            handleFoodManagement();
        } else if (choice == "2") {
            handleLogManagement();
        } else if (choice == "3") {
            handleProfileManagement();
        } else if (choice == "4") {
            saveData();
        } else if (choice == "5") {
            saveData();  // Auto-save before exit
            break;
        } else {
            std::cout << "Invalid choice. Please try again.\n";
        }
    }
}

void UserInterface::showMainMenu() {
    std::cout << "\nYADA - Yet Another Diet Assistant\n";
    std::cout << "1. Food Management\n";
    std::cout << "2. Log Management\n";
    std::cout << "3. Profile Management\n";
    std::cout << "4. Save All Data\n";
    std::cout << "5. Exit\n";
}

void UserInterface::handleFoodManagement() {
    while (true) {
        std::cout << "\nFood Management\n";
        std::cout << "1. Add Basic Food\n";
        std::cout << "2. Add Composite Food\n";
        std::cout << "3. Search Food\n";
        std::cout << "4. List All Foods\n";
        std::cout << "5. Back to Main Menu\n";

        std::string choice = getInput("Enter your choice: ");
        
        if (choice == "1") {
            addBasicFood();
        } else if (choice == "2") {
            addCompositeFood();
        } else if (choice == "3") {
            searchFood();
        } else if (choice == "4") {
            listAllFoods();
        } else if (choice == "5") {
            break;
        } else {
            std::cout << "Invalid choice. Please try again.\n";
        }
    }
}

void UserInterface::handleLogManagement() {
    while (true) {
        std::cout << "\nLog Management\n";
        std::cout << "1. Add Food to Log\n";
        std::cout << "2. Remove Food from Log\n";
        std::cout << "3. View Log for Date\n";
        std::cout << "4. Undo Last Action\n";
        std::cout << "5. Back to Main Menu\n";

        std::string choice = getInput("Enter your choice: ");
        
        if (choice == "1") {
            addFoodToLog();
        } else if (choice == "2") {
            removeFoodFromLog();
        } else if (choice == "3") {
            viewLogForDate();
        } else if (choice == "4") {
            undoLastAction();
        } else if (choice == "5") {
            break;
        } else {
            std::cout << "Invalid choice. Please try again.\n";
        }
    }
}

void UserInterface::handleProfileManagement() {
    while (true) {
        std::cout << "\nProfile Management\n";
        std::cout << "1. View Profile\n";
        std::cout << "2. Update Profile\n";
        std::cout << "3. Change Calorie Calculation Method\n";
        std::cout << "4. Back to Main Menu\n";

        std::string choice = getInput("Enter choice: ");
        
        if (choice == "1") {
            viewProfile();
        } else if (choice == "2") {
            updateProfile();
        } else if (choice == "3") {
            changeCalculationMethod();
        } else if (choice == "4") {
            break;
        } else {
            std::cout << "Invalid choice. Please try again.\n";
        }
    }
}

void UserInterface::addBasicFood() {
    std::string name = getInput("Enter food name: ");
    double calories = getNumericInput("Enter calories per serving: ");
    
    auto food = std::make_shared<BasicFood>(name, calories);
    
    std::string keywords;
    std::cout << "Enter search keywords (comma-separated): ";
    std::getline(std::cin, keywords);
    
    std::stringstream ss(keywords);
    std::string keyword;
    while (std::getline(ss, keyword, ',')) {
        // Trim whitespace
        keyword.erase(0, keyword.find_first_not_of(" \t"));
        keyword.erase(keyword.find_last_not_of(" \t") + 1);
        if (!keyword.empty()) {
            food->addKeyword(keyword);
        }
    }
    
    foodDatabase.push_back(food);
    std::cout << "Food added successfully!\n";
}

void UserInterface::addCompositeFood() {
    if (foodDatabase.empty()) {
        std::cout << "No basic foods available. Please add some basic foods first.\n";
        return;
    }

    std::string name = getInput("Enter composite food name: ");
    auto compositeFood = std::make_shared<CompositeFood>(name);

    while (true) {
        listAllFoods();
        std::cout << "\nEnter food number to add (0 to finish): ";
        size_t index;
        std::cin >> index;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (index == 0) break;
        if (index > foodDatabase.size()) {
            std::cout << "Invalid food number.\n";
            continue;
        }

        double servings = getNumericInput("Enter number of servings: ");
        compositeFood->addComponent(foodDatabase[index - 1], servings);
    }

    std::string keywords;
    std::cout << "Enter search keywords (comma-separated): ";
    std::getline(std::cin, keywords);
    
    std::stringstream ss(keywords);
    std::string keyword;
    while (std::getline(ss, keyword, ',')) {
        keyword.erase(0, keyword.find_first_not_of(" \t"));
        keyword.erase(keyword.find_last_not_of(" \t") + 1);
        if (!keyword.empty()) {
            compositeFood->addKeyword(keyword);
        }
    }

    foodDatabase.push_back(compositeFood);
    std::cout << "Composite food added successfully!\n";
}

void UserInterface::searchFood() {
    std::string keywords = getInput("Enter search keywords: ");
    std::cout << "Match (1) ANY or (2) ALL keywords? ";
    std::string matchChoice = getInput("");
    bool matchAll = (matchChoice == "2");
    
    auto results = searchFoodByKeywords(keywords, matchAll);
    
    if (results.empty()) {
        std::cout << "No foods found matching your search.\n";
        return;
    }

    std::cout << "\nSearch Results:\n";
    for (size_t i = 0; i < results.size(); ++i) {
        const auto& food = results[i];
        std::cout << i + 1 << ". " << food->getName() 
                 << " (" << food->getType() << ") - "
                 << food->getCaloriesPerServing() << " calories\n";
    }
}

void UserInterface::listAllFoods() {
    if (foodDatabase.empty()) {
        std::cout << "No foods in database.\n";
        return;
    }

    std::cout << "\nAll Foods:\n";
    for (size_t i = 0; i < foodDatabase.size(); ++i) {
        const auto& food = foodDatabase[i];
        std::cout << i + 1 << ". " << food->getName() 
                 << " (" << food->getType() << ") - "
                 << food->getCaloriesPerServing() << " calories\n";
    }
}

void UserInterface::addFoodToLog() {
    if (foodDatabase.empty()) {
        std::cout << "No foods available. Please add some foods first.\n";
        return;
    }

    std::string date = getInput("Enter date (YYYY-MM-DD) or press Enter for today: ");
    if (date.empty()) {
        date = FoodLog::getCurrentDate();
    }

    listAllFoods();
    std::cout << "\nEnter food number to add to log: ";
    size_t index;
    std::cin >> index;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (index < 1 || index > foodDatabase.size()) {
        std::cout << "Invalid food number.\n";
        return;
    }

    double servings = getNumericInput("Enter number of servings: ");
    
    // Show current entries for the date before adding
    auto entries = foodLog->getEntriesForDate(date);
    if (!entries.empty()) {
        std::cout << "\nCurrent entries for " << date << ":\n";
        for (size_t i = 0; i < entries.size(); ++i) {
            const auto& entry = entries[i];
            auto food = findFoodByName(entry.foodId);
            if (food) {
                std::cout << i + 1 << ". " << food->getName() 
                         << " - " << entry.servings << " serving(s)\n";
            }
        }
    }

    // Create a command that includes the date
    auto command = std::make_unique<AddFoodCommand>(*foodLog, foodDatabase[index - 1], servings, date);
    commandManager->executeCommand(std::move(command));
    std::cout << "Food added to log for " << date << " successfully!\n";

    // Show updated total calories
    auto foodLookup = [this](const std::string& name) -> std::shared_ptr<Food> {
        return findFoodByName(name);
    };
    double totalCalories = foodLog->getTotalCaloriesForDate(date, foodLookup);
    std::cout << "Total calories for " << date << ": " << totalCalories;
    if (userProfile) {
        double targetCalories = userProfile->getTargetCalories();
        std::cout << " (Target: " << targetCalories << ")\n";
    } else {
        std::cout << "\n";
    }
}

void UserInterface::removeFoodFromLog() {
    std::string date = getInput("Enter date (YYYY-MM-DD) or press Enter for today: ");
    if (date.empty()) {
        date = FoodLog::getCurrentDate();
    }

    auto entries = foodLog->getEntriesForDate(date);
    if (entries.empty()) {
        std::cout << "No entries found for this date.\n";
        return;
    }

    std::cout << "\nEntries for " << date << ":\n";
    for (size_t i = 0; i < entries.size(); ++i) {
        const auto& entry = entries[i];
        auto food = findFoodByName(entry.foodId);
        if (food) {
            std::cout << i + 1 << ". " << food->getName() 
                     << " - " << entry.servings << " serving(s)\n";
        }
    }

    std::cout << "\nEnter entry number to remove: ";
    size_t index;
    std::cin >> index;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (index < 1 || index > entries.size()) {
        std::cout << "Invalid entry number.\n";
        return;
    }

    // Get the original food object for the entry being removed
    auto originalFood = findFoodByName(entries[index - 1].foodId);
    if (!originalFood) {
        std::cout << "Error: Could not find the food in database.\n";
        return;
    }

    auto command = std::make_unique<RemoveFoodCommand>(*foodLog, index - 1, originalFood, date);
    commandManager->executeCommand(std::move(command));
    std::cout << "Entry removed successfully!\n";
}

void UserInterface::viewLogForDate() {
    std::string date = getInput("Enter date (YYYY-MM-DD) or press Enter for today: ");
    if (date.empty()) {
        date = FoodLog::getCurrentDate();
    }

    auto entries = foodLog->getEntriesForDate(date);
    if (entries.empty()) {
        std::cout << "No entries found for " << date << ".\n";
        return;
    }

    std::cout << "\nFood Log for " << date << ":\n";
    for (size_t i = 0; i < entries.size(); ++i) {
        const auto& entry = entries[i];
        auto food = findFoodByName(entry.foodId);
        if (food) {
            std::cout << i + 1 << ". " << food->getName() 
                     << " - " << entry.servings << " serving(s) - "
                     << (food->getCaloriesPerServing() * entry.servings) << " calories\n";
        }
    }

    auto foodLookup = [this](const std::string& name) -> std::shared_ptr<Food> {
        return findFoodByName(name);
    };

    double totalCalories = foodLog->getTotalCaloriesForDate(date, foodLookup);
    std::cout << "\nTotal calories: " << totalCalories << "\n";
    if (userProfile) {
        double targetCalories = userProfile->getTargetCalories();
        std::cout << "Target calories: " << targetCalories << "\n";
        std::cout << "Difference: " << (totalCalories - targetCalories) << "\n";
    }
}

void UserInterface::undoLastAction() {
    if (commandManager->canUndo()) {
        commandManager->undo();
        std::cout << "Last action undone.\n";
    } else {
        std::cout << "No actions to undo.\n";
    }
}

void UserInterface::createProfile() {
    std::cout << "\nCreate User Profile\n";
    
    std::cout << "Enter gender (M/F): ";
    std::string genderStr;
    std::getline(std::cin, genderStr);
    Gender gender = (genderStr == "M" || genderStr == "m") ? Gender::MALE : Gender::FEMALE;

    double height = getNumericInput("Enter height (cm): ");
    int age = static_cast<int>(getNumericInput("Enter age: "));
    
    userProfile = std::make_unique<UserProfile>(gender, height, age);
    
    double weight = getNumericInput("Enter weight (kg): ");
    userProfile->setWeight(weight);

    std::cout << "\nSelect activity level:\n";
    std::cout << "1. Sedentary\n";
    std::cout << "2. Light Exercise\n";
    std::cout << "3. Moderate Exercise\n";
    std::cout << "4. Very Active\n";
    std::cout << "5. Extra Active\n";
    
    int activityChoice = static_cast<int>(getNumericInput("Enter choice (1-5): "));
    ActivityLevel level;
    switch (activityChoice) {
        case 1: level = ActivityLevel::SEDENTARY; break;
        case 2: level = ActivityLevel::LIGHT; break;
        case 3: level = ActivityLevel::MODERATE; break;
        case 4: level = ActivityLevel::VERY_ACTIVE; break;
        case 5: level = ActivityLevel::EXTRA_ACTIVE; break;
        default: level = ActivityLevel::SEDENTARY;
    }
    userProfile->setActivityLevel(level);

    // Add calorie calculation method selection
    std::cout << "\nSelect Calorie Calculation Method:\n";
    std::cout << "1. Harris-Benedict Equation\n";
    std::cout << "2. Mifflin-St Jeor Equation\n";
    std::cout << "3. Average of Both Methods\n";
    
    int methodChoice = static_cast<int>(getNumericInput("Enter choice (1-3): "));
    CalorieCalculationMethod method;
    switch (methodChoice) {
        case 1: method = CalorieCalculationMethod::HARRIS_BENEDICT; break;
        case 2: method = CalorieCalculationMethod::MIFFLIN_ST_JEOR; break;
        case 3: method = CalorieCalculationMethod::AVERAGE_OF_BOTH; break;
        default: method = CalorieCalculationMethod::AVERAGE_OF_BOTH;
    }
    userProfile->setCalculationMethod(method);

    std::cout << "Profile created successfully!\n";
}

void UserInterface::updateProfile() {
    if (!userProfile) {
        std::cout << "No profile exists. Please create one first.\n";
        return;
    }

    std::cout << "\nUpdate Profile\n";
    std::cout << "1. Update Age\n";
    std::cout << "2. Update Weight\n";
    std::cout << "3. Update Activity Level\n";
    
    std::string choice = getInput("Enter choice: ");
    
    if (choice == "1") {
        int age = static_cast<int>(getNumericInput("Enter new age: "));
        userProfile->setAge(age);
    } else if (choice == "2") {
        double weight = getNumericInput("Enter new weight (kg): ");
        userProfile->setWeight(weight);
    } else if (choice == "3") {
        std::cout << "\nSelect new activity level:\n";
        std::cout << "1. Sedentary\n";
        std::cout << "2. Light Exercise\n";
        std::cout << "3. Moderate Exercise\n";
        std::cout << "4. Very Active\n";
        std::cout << "5. Extra Active\n";
        
        int activityChoice = static_cast<int>(getNumericInput("Enter choice (1-5): "));
        ActivityLevel level;
        switch (activityChoice) {
            case 1: level = ActivityLevel::SEDENTARY; break;
            case 2: level = ActivityLevel::LIGHT; break;
            case 3: level = ActivityLevel::MODERATE; break;
            case 4: level = ActivityLevel::VERY_ACTIVE; break;
            case 5: level = ActivityLevel::EXTRA_ACTIVE; break;
            default: level = ActivityLevel::SEDENTARY;
        }
        userProfile->setActivityLevel(level);
    }

    std::cout << "Profile updated successfully!\n";
}

void UserInterface::viewProfile() {
    if (!userProfile) {
        std::cout << "No profile exists. Please create one first.\n";
        return;
    }

    std::cout << "\nUser Profile\n";
    std::cout << "Gender: " << (userProfile->getGender() == Gender::MALE ? "Male" : "Female") << "\n";
    std::cout << "Height: " << userProfile->getHeight() << " cm\n";
    std::cout << "Age: " << userProfile->getAge() << " years\n";
    std::cout << "Weight: " << userProfile->getWeight() << " kg\n";
    std::cout << "Activity Level: ";
    switch (userProfile->getActivityLevel()) {
        case ActivityLevel::SEDENTARY: std::cout << "Sedentary"; break;
        case ActivityLevel::LIGHT: std::cout << "Light Exercise"; break;
        case ActivityLevel::MODERATE: std::cout << "Moderate Exercise"; break;
        case ActivityLevel::VERY_ACTIVE: std::cout << "Very Active"; break;
        case ActivityLevel::EXTRA_ACTIVE: std::cout << "Extra Active"; break;
    }
    std::cout << "\nCalorie Calculation Method: ";
    switch (userProfile->getCalculationMethod()) {
        case CalorieCalculationMethod::HARRIS_BENEDICT: std::cout << "Harris-Benedict"; break;
        case CalorieCalculationMethod::MIFFLIN_ST_JEOR: std::cout << "Mifflin-St Jeor"; break;
        case CalorieCalculationMethod::AVERAGE_OF_BOTH: std::cout << "Average of Both"; break;
    }
    std::cout << "\nTarget Daily Calories: " << userProfile->getTargetCalories() << "\n";
}

void UserInterface::changeCalculationMethod() {
    if (!userProfile) {
        std::cout << "No profile exists. Please create one first.\n";
        return;
    }

    std::cout << "\nSelect Calorie Calculation Method:\n";
    std::cout << "1. Harris-Benedict Equation\n";
    std::cout << "2. Mifflin-St Jeor Equation\n";
    std::cout << "3. Average of Both Methods\n";
    
    std::string choice = getInput("Enter choice (1-3): ");
    
    CalorieCalculationMethod method;
    if (choice == "1") {
        method = CalorieCalculationMethod::HARRIS_BENEDICT;
    } else if (choice == "2") {
        method = CalorieCalculationMethod::MIFFLIN_ST_JEOR;
    } else if (choice == "3") {
        method = CalorieCalculationMethod::AVERAGE_OF_BOTH;
    } else {
        std::cout << "Invalid choice. Keeping current method.\n";
        return;
    }

    userProfile->setCalculationMethod(method);
    std::cout << "Calculation method updated successfully!\n";
}

std::string UserInterface::getInput(const std::string& prompt) {
    std::cout << prompt;
    std::string input;
    std::getline(std::cin, input);
    return input;
}

double UserInterface::getNumericInput(const std::string& prompt) {
    while (true) {
        std::cout << prompt;
        std::string input;
        std::getline(std::cin, input);
        try {
            return std::stod(input);
        } catch (...) {
            std::cout << "Invalid input. Please enter a number.\n";
        }
    }
}

std::vector<std::shared_ptr<Food>> UserInterface::searchFoodByKeywords(const std::string& keywords, bool matchAll) {
    std::vector<std::shared_ptr<Food>> results;
    std::stringstream ss(keywords);
    std::string keyword;
    std::vector<std::string> searchTerms;
    
    while (std::getline(ss, keyword, ' ')) {
        if (!keyword.empty()) {
            // Convert to lowercase for case-insensitive search
            std::transform(keyword.begin(), keyword.end(), keyword.begin(), ::tolower);
            searchTerms.push_back(keyword);
        }
    }

    for (const auto& food : foodDatabase) {
        const auto& foodKeywords = food->getKeywords();
        bool isMatch = matchAll;  // For matchAll=true, start true and AND results
                                 // For matchAll=false, start false and OR results
        
        for (const auto& term : searchTerms) {
            bool termFound = false;
            for (auto foodKeyword : foodKeywords) {
                // Convert to lowercase for comparison
                std::transform(foodKeyword.begin(), foodKeyword.end(), foodKeyword.begin(), ::tolower);
                if (foodKeyword.find(term) != std::string::npos) {
                    termFound = true;
                    break;
                }
            }
            
            if (matchAll) {
                isMatch = isMatch && termFound;  // ALL keywords must match
                if (!isMatch) break;  // Early exit if any keyword doesn't match
            } else {
                isMatch = isMatch || termFound;  // ANY keyword can match
                if (isMatch) break;  // Early exit if any keyword matches
            }
        }
        
        if (isMatch) {
            results.push_back(food);
        }
    }

    return results;
}

void UserInterface::saveData() {
    try {
        saveFoodDatabase("food_database.txt");
        saveUserProfile("user_profile.txt");
        foodLog->saveToFile("food_log.txt");
        std::cout << "Data saved successfully!\n";
    } catch (const std::exception& e) {
        std::cerr << "Error saving data: " << e.what() << "\n";
    }
}

void UserInterface::loadData() {
    try {
        loadFoodDatabase("food_database.txt");
        loadUserProfile("user_profile.txt");
        foodLog->loadFromFile("food_log.txt");
        std::cout << "Data loaded successfully!\n";
    } catch (const std::exception& e) {
        std::cout << "No previous data found. Starting fresh.\n";
    }
}

void UserInterface::saveFoodDatabase(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file) {
        throw std::runtime_error("Could not open file for writing: " + filename);
    }

    for (const auto& food : foodDatabase) {
        file << food->getType() << "|" << food->getName() << "|" << food->getCaloriesPerServing();
        
        // Save keywords
        const auto& keywords = food->getKeywords();
        file << "|" << keywords.size();
        for (const auto& keyword : keywords) {
            file << "|" << keyword;
        }

        // Save components for composite foods
        if (food->getType() == "Composite") {
            auto compositeFood = std::dynamic_pointer_cast<CompositeFood>(food);
            const auto& components = compositeFood->getComponents();
            file << "|" << components.size();
            for (const auto& comp : components) {
                const auto& component = comp.first;
                const auto& servings = comp.second;
                file << "|" << component->getName() << "|" << servings;
            }
        }
        file << "\n";
    }
}

void UserInterface::loadFoodDatabase(const std::string& filename) {
    std::ifstream file(filename);
    if (!file) {
        throw std::runtime_error("Could not open file for reading: " + filename);
    }

    foodDatabase.clear();
    std::string line;
    std::map<std::string, std::vector<std::pair<std::string, double>>> pendingComposites;

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string type, name;
        double calories;
        size_t keywordCount;

        std::getline(ss, type, '|');
        std::getline(ss, name, '|');
        ss >> calories;
        ss.ignore();
        ss >> keywordCount;
        ss.ignore();

        std::shared_ptr<Food> food;
        if (type == "Basic") {
            food = std::make_shared<BasicFood>(name, calories);
        } else {
            food = std::make_shared<CompositeFood>(name);
        }

        // Load keywords
        for (size_t i = 0; i < keywordCount; ++i) {
            std::string keyword;
            std::getline(ss, keyword, '|');
            food->addKeyword(keyword);
        }

        // Handle composite food components
        if (type == "Composite") {
            size_t componentCount;
            ss >> componentCount;
            ss.ignore();

            std::vector<std::pair<std::string, double>> components;
            for (size_t i = 0; i < componentCount; ++i) {
                std::string componentName;
                double servings;
                std::getline(ss, componentName, '|');
                ss >> servings;
                ss.ignore();
                components.emplace_back(componentName, servings);
            }
            pendingComposites[name] = components;
        }

        foodDatabase.push_back(food);
    }

    // Resolve composite food components
    for (const auto& pending : pendingComposites) {
        const auto& compositeName = pending.first;
        const auto& components = pending.second;
        auto compositeFood = std::dynamic_pointer_cast<CompositeFood>(findFoodByName(compositeName));
        if (compositeFood) {
            for (const auto& comp : components) {
                const auto& componentName = comp.first;
                const auto& servings = comp.second;
                auto componentFood = findFoodByName(componentName);
                if (componentFood) {
                    compositeFood->addComponent(componentFood, servings);
                }
            }
        }
    }
}

void UserInterface::saveUserProfile(const std::string& filename) const {
    if (!userProfile) return;

    std::ofstream file(filename);
    if (!file) {
        throw std::runtime_error("Could not open file for writing: " + filename);
    }

    file << (userProfile->getGender() == Gender::MALE ? "M" : "F") << "|"
         << userProfile->getHeight() << "|"
         << userProfile->getAge() << "|"
         << userProfile->getWeight() << "|"
         << static_cast<int>(userProfile->getActivityLevel()) << "|"
         << static_cast<int>(userProfile->getCalculationMethod()) << "\n";
}

void UserInterface::loadUserProfile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file) {
        throw std::runtime_error("Could not open file for reading: " + filename);
    }

    std::string line;
    if (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string genderStr;
        double height, weight;
        int age, activityLevel, methodLevel;

        std::getline(ss, genderStr, '|');
        ss >> height;
        ss.ignore();
        ss >> age;
        ss.ignore();
        ss >> weight;
        ss.ignore();
        ss >> activityLevel;
        ss.ignore();
        ss >> methodLevel;

        Gender gender = (genderStr == "M") ? Gender::MALE : Gender::FEMALE;
        userProfile = std::make_unique<UserProfile>(gender, height, age);
        userProfile->setWeight(weight);
        userProfile->setActivityLevel(static_cast<ActivityLevel>(activityLevel));
        userProfile->setCalculationMethod(static_cast<CalorieCalculationMethod>(methodLevel));
    }
}

std::shared_ptr<Food> UserInterface::findFoodByName(const std::string& name) const {
    auto it = std::find_if(foodDatabase.begin(), foodDatabase.end(),
        [&name](const auto& food) { return food->getName() == name; });
    return (it != foodDatabase.end()) ? *it : nullptr;
} 