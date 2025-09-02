#ifndef YADA_USER_PROFILE_H
#define YADA_USER_PROFILE_H

#include <string>

enum class Gender {
    MALE,
    FEMALE
};

enum class ActivityLevel {
    SEDENTARY,
    LIGHT,
    MODERATE,
    VERY_ACTIVE,
    EXTRA_ACTIVE
};

enum class CalorieCalculationMethod {
    HARRIS_BENEDICT,
    MIFFLIN_ST_JEOR,
    AVERAGE_OF_BOTH
};

class UserProfile {
public:
    UserProfile(Gender gender, double height, int age);

    // Getters
    Gender getGender() const;
    double getHeight() const;
    int getAge() const;
    double getWeight() const;
    ActivityLevel getActivityLevel() const;
    double getTargetCalories() const;
    CalorieCalculationMethod getCalculationMethod() const;

    // Setters for changeable attributes
    void setAge(int age);
    void setWeight(double weight);
    void setActivityLevel(ActivityLevel level);
    void setCalculationMethod(CalorieCalculationMethod method);

    // Calorie calculation methods
    double calculateBMR() const;
    double calculateHarrisBenedictTDEE() const;
    double calculateMifflinStJeorTDEE() const;

private:
    const Gender gender;
    const double height;  // in cm
    int age;
    double weight;        // in kg
    ActivityLevel activityLevel;
    double targetCalories;
    CalorieCalculationMethod calculationMethod;

    double getActivityMultiplier() const;
    void updateTargetCalories();
};

#endif // YADA_USER_PROFILE_H 