#ifndef YADA_FOOD_H
#define YADA_FOOD_H

#include <string>
#include <vector>

class Food {
public:
    Food(const std::string& name);
    virtual ~Food() = default;

    // Pure virtual functions
    virtual double getCaloriesPerServing() const = 0;
    virtual std::string getType() const = 0;

    // Common functions
    const std::string& getName() const;
    const std::vector<std::string>& getKeywords() const;
    void addKeyword(const std::string& keyword);

protected:
    std::string name;
    std::vector<std::string> keywords;
};

#endif // YADA_FOOD_H 