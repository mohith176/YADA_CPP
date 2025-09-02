#ifndef YADA_BASIC_FOOD_H
#define YADA_BASIC_FOOD_H

#include "food/Food.h"

class BasicFood : public Food {
public:
    BasicFood(const std::string& name, double caloriesPerServing);

    // Implementation of pure virtual functions
    double getCaloriesPerServing() const override;
    std::string getType() const override;

    // BasicFood specific functions
    void setCaloriesPerServing(double calories);

private:
    double caloriesPerServing;
};

#endif // YADA_BASIC_FOOD_H 