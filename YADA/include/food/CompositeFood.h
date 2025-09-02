#ifndef YADA_COMPOSITE_FOOD_H
#define YADA_COMPOSITE_FOOD_H

#include "food/Food.h"
#include <map>
#include <memory>

class CompositeFood : public Food {
public:
    explicit CompositeFood(const std::string& name);

    // Implementation of pure virtual functions
    double getCaloriesPerServing() const override;
    std::string getType() const override;

    // CompositeFood specific functions
    void addComponent(const std::shared_ptr<Food>& food, double servings);
    void removeComponent(const std::string& foodName);
    const std::map<std::shared_ptr<Food>, double>& getComponents() const;

private:
    std::map<std::shared_ptr<Food>, double> components; // Food component and its servings
};

#endif // YADA_COMPOSITE_FOOD_H 