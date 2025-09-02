#ifndef YADA_USER_INTERFACE_H
#define YADA_USER_INTERFACE_H

#include "food/Food.h"
#include "food/BasicFood.h"
#include "food/CompositeFood.h"
#include "log/FoodLog.h"
#include "profile/UserProfile.h"
#include "command/Command.h"
#include <memory>
#include <vector>
#include <string>

class UserInterface {
public:
    UserInterface();
    void run();

private:
    std::vector<std::shared_ptr<Food>> foodDatabase;
    std::unique_ptr<FoodLog> foodLog;
    std::unique_ptr<UserProfile> userProfile;
    std::unique_ptr<CommandManager> commandManager;

    // Menu functions
    void showMainMenu();
    void handleFoodManagement();
    void handleLogManagement();
    void handleProfileManagement();
    
    // Food management functions
    void addBasicFood();
    void addCompositeFood();
    void searchFood();
    void listAllFoods();
    
    // Log management functions
    void addFoodToLog();
    void removeFoodFromLog();
    void viewLogForDate();
    void undoLastAction();
    
    // Profile management functions
    void createProfile();
    void updateProfile();
    void viewProfile();
    void changeCalculationMethod();
    
    // Helper functions
    std::string getInput(const std::string& prompt);
    double getNumericInput(const std::string& prompt);
    std::vector<std::shared_ptr<Food>> searchFoodByKeywords(const std::string& keywords, bool matchAll = false);
    
    // File operations
    void saveData();
    void loadData();
    void saveFoodDatabase(const std::string& filename) const;
    void loadFoodDatabase(const std::string& filename);
    void saveUserProfile(const std::string& filename) const;
    void loadUserProfile(const std::string& filename);
    std::shared_ptr<Food> findFoodByName(const std::string& name) const;
};

#endif // YADA_USER_INTERFACE_H 