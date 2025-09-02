#include "food/Food.h"
#include <algorithm>

Food::Food(const std::string& name) : name(name) {}

const std::string& Food::getName() const {
    return name;
}

const std::vector<std::string>& Food::getKeywords() const {
    return keywords;
}

void Food::addKeyword(const std::string& keyword) {
    // Check if keyword already exists
    if (std::find(keywords.begin(), keywords.end(), keyword) == keywords.end()) {
        keywords.push_back(keyword);
    }
} 