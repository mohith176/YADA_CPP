#include "food/BasicFood.h"
#include "food/CompositeFood.h"
#include "ui/UserInterface.h"
#include <iostream>
#include <memory>

int main() {
    try {
        UserInterface ui;
        ui.run();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
} 