#include "food/BasicFood.h"

BasicFood::BasicFood(const std::string& name, double caloriesPerServing)
    : Food(name), caloriesPerServing(caloriesPerServing) {}

double BasicFood::getCaloriesPerServing() const {
    return caloriesPerServing;
}

std::string BasicFood::getType() const {
    return "Basic";
}

void BasicFood::setCaloriesPerServing(double calories) {
    caloriesPerServing = calories;
} 