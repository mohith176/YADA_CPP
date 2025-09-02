#include "food/CompositeFood.h"
#include <algorithm>

CompositeFood::CompositeFood(const std::string& name)
    : Food(name) {}

double CompositeFood::getCaloriesPerServing() const {
    double totalCalories = 0.0;
    for (const auto& pair : components) {
        const auto& component = pair.first;
        const auto& servings = pair.second;
        totalCalories += component->getCaloriesPerServing() * servings;
    }
    return totalCalories;
}

std::string CompositeFood::getType() const {
    return "Composite";
}

void CompositeFood::addComponent(const std::shared_ptr<Food>& food, double servings) {
    components[food] = servings;
}

void CompositeFood::removeComponent(const std::string& foodName) {
    auto it = std::find_if(components.begin(), components.end(),
        [&foodName](const auto& pair) {
            return pair.first->getName() == foodName;
        });
    
    if (it != components.end()) {
        components.erase(it);
    }
}

const std::map<std::shared_ptr<Food>, double>& CompositeFood::getComponents() const {
    return components;
} 