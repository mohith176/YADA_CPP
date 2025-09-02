#ifndef YADA_FOOD_DATA_SOURCE_H
#define YADA_FOOD_DATA_SOURCE_H

#include "food/BasicFood.h"
#include <vector>
#include <memory>

class FoodDataSource {
public:
    virtual ~FoodDataSource() = default;
    virtual std::vector<std::shared_ptr<BasicFood>> getFoods() = 0;
    virtual bool addFood(const std::shared_ptr<BasicFood>& food) = 0;
    virtual void saveChanges() = 0;
};

// File-based food source
class FileFoodSource : public FoodDataSource {
public:
    explicit FileFoodSource(const std::string& filename);
    std::vector<std::shared_ptr<BasicFood>> getFoods() override;
    bool addFood(const std::shared_ptr<BasicFood>& food) override;
    void saveChanges() override;

private:
    std::string filename;
    std::vector<std::shared_ptr<BasicFood>> foods;
    void loadFromFile();
    void saveToFile();
};

// User input food source
class UserInputFoodSource : public FoodDataSource {
public:
    std::vector<std::shared_ptr<BasicFood>> getFoods() override;
    bool addFood(const std::shared_ptr<BasicFood>& food) override;
    void saveChanges() override;

private:
    std::vector<std::shared_ptr<BasicFood>> foods;
};

// Web-based food source (placeholder for future implementation)
class WebFoodSource : public FoodDataSource {
public:
    explicit WebFoodSource(const std::string& apiUrl);
    std::vector<std::shared_ptr<BasicFood>> getFoods() override;
    bool addFood(const std::shared_ptr<BasicFood>& food) override;
    void saveChanges() override;

private:
    std::string apiUrl;
    std::vector<std::shared_ptr<BasicFood>> foods;
};

#endif // YADA_FOOD_DATA_SOURCE_H 