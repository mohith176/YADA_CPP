#include "profile/UserProfile.h"
#include <cmath>

UserProfile::UserProfile(Gender gender, double height, int age)
    : gender(gender), height(height), age(age), 
      weight(0.0), activityLevel(ActivityLevel::SEDENTARY), 
      targetCalories(0.0), calculationMethod(CalorieCalculationMethod::AVERAGE_OF_BOTH) {}

Gender UserProfile::getGender() const { return gender; }
double UserProfile::getHeight() const { return height; }
int UserProfile::getAge() const { return age; }
double UserProfile::getWeight() const { return weight; }
ActivityLevel UserProfile::getActivityLevel() const { return activityLevel; }
double UserProfile::getTargetCalories() const { return targetCalories; }
CalorieCalculationMethod UserProfile::getCalculationMethod() const { return calculationMethod; }

void UserProfile::setAge(int newAge) {
    age = newAge;
    updateTargetCalories();
}

void UserProfile::setWeight(double newWeight) {
    weight = newWeight;
    updateTargetCalories();
}

void UserProfile::setActivityLevel(ActivityLevel level) {
    activityLevel = level;
    updateTargetCalories();
}

void UserProfile::setCalculationMethod(CalorieCalculationMethod method) {
    calculationMethod = method;
    updateTargetCalories();
}

double UserProfile::getActivityMultiplier() const {
    switch (activityLevel) {
        case ActivityLevel::SEDENTARY: return 1.2;
        case ActivityLevel::LIGHT: return 1.375;
        case ActivityLevel::MODERATE: return 1.55;
        case ActivityLevel::VERY_ACTIVE: return 1.725;
        case ActivityLevel::EXTRA_ACTIVE: return 1.9;
        default: return 1.2;
    }
}

double UserProfile::calculateBMR() const {
    if (gender == Gender::MALE) {
        return 88.362 + (13.397 * weight) + (4.799 * height) - (5.677 * age);
    } else {
        return 447.593 + (9.247 * weight) + (3.098 * height) - (4.330 * age);
    }
}

double UserProfile::calculateHarrisBenedictTDEE() const {
    return calculateBMR() * getActivityMultiplier();
}

double UserProfile::calculateMifflinStJeorTDEE() const {
    double bmr;
    if (gender == Gender::MALE) {
        bmr = (10 * weight) + (6.25 * height) - (5 * age) + 5;
    } else {
        bmr = (10 * weight) + (6.25 * height) - (5 * age) - 161;
    }
    return bmr * getActivityMultiplier();
}

void UserProfile::updateTargetCalories() {
    switch (calculationMethod) {
        case CalorieCalculationMethod::HARRIS_BENEDICT:
            targetCalories = calculateHarrisBenedictTDEE();
            break;
        case CalorieCalculationMethod::MIFFLIN_ST_JEOR:
            targetCalories = calculateMifflinStJeorTDEE();
            break;
        case CalorieCalculationMethod::AVERAGE_OF_BOTH:
            targetCalories = (calculateHarrisBenedictTDEE() + calculateMifflinStJeorTDEE()) / 2.0;
            break;
    }
} 