# YADA (Yet Another Diet Assistant)

## Overview
YADA is a command-line diet management system implemented in C++ that helps users track their food consumption and manage caloric intake. The system provides comprehensive food tracking, calorie calculation, and progress monitoring features.

## Features

### 1. Food Database Management
- **Basic Foods**
  - Store name, keywords, and calories per serving
  - Easy addition of new basic foods
  - Extensible design for future nutritional information
  - Text-based storage for easy maintenance

- **Composite Foods**
  - Create from combinations of basic and/or composite foods
  - Specify serving sizes for each component
  - Automatic calorie calculation based on components
  - Maintain relationships between composite foods and components
  - Support for nested composite foods

- **Search Functionality**
  - Search foods by keywords
  - Display results with calorie information
  - Support for multiple search terms
  - Case-insensitive search

### 2. Daily Food Log
- **Log Management**
  - Track daily food consumption
  - Automatic combining of same food entries
  - View logs for any specific date
  - Calculate total calories consumed per day
  - Compare with daily target calories

- **Entry Management**
  - Add foods with specified servings
  - Remove food entries
  - Modify serving sizes
  - Support for unlimited undo operations
  - Date-specific operations

### 3. User Profile
- **Profile Information**
  - Gender (fixed after initial setting)
  - Height (fixed after initial setting)
  - Age (changeable daily)
  - Weight (changeable daily)
  - Activity level (changeable daily)

- **Calorie Calculation**
  - Multiple calculation methods:
    1. Harris-Benedict Equation
    2. Mifflin-St Jeor Equation
    3. Average of Both Methods
  - Switch between calculation methods anytime
  - Daily target calorie calculation
  - Progress tracking against targets
  - Activity level adjustments

## Technical Details

### Data Storage
- **Food Database**: `food_database.txt`
  - Stores basic and composite foods
  - Text-based format for easy editing
  - Maintains food relationships
  - Format: `type|name|calories|keywords_count|keyword1|keyword2|...|components_count|component1|servings1|...`

- **Daily Logs**: `food_log.txt`
  - Date-organized entries
  - Stores food references and servings
  - Optimized to reduce duplicates
  - Format: `date|food_name|servings|timestamp`

- **User Profile**: `user_profile.txt`
  - Stores user information
  - Maintains calculation preferences
  - Preserves changeable attributes
  - Format: `gender|height|age|weight|activity_level|calculation_method`

### Design Patterns
- **Command Pattern**: Implements unlimited undo functionality
- **Composite Pattern**: Manages food hierarchies
- **Strategy Pattern**: Handles different calorie calculation methods

## Getting Started

### Prerequisites
- C++ compiler with C++17 support (e.g., GCC 7+, Visual Studio 2017+)
- CMake (version 3.10 or higher) [Optional - for CMake build]
- Terminal/Command Prompt

### Building the Project

#### Using CMake (Recommended)
```bash
# Create build directory
mkdir build
cd build

# Generate build files
cmake ..

# Build the project
# For Visual Studio:
cmake --build . --config Release

# For MinGW:
mingw32-make
```

#### Manual Compilation
```bash
# For Windows (MinGW):
g++ -std=c++17 -o yada.exe ^
    src/main.cpp ^
    src/food/Food.cpp ^
    src/food/BasicFood.cpp ^
    src/food/CompositeFood.cpp ^
    src/log/FoodLog.cpp ^
    src/profile/UserProfile.cpp ^
    src/command/Command.cpp ^
    src/ui/UserInterface.cpp ^
    -I include

# For Unix-like systems:
g++ -std=c++17 -o yada \
    src/main.cpp \
    src/food/Food.cpp \
    src/food/BasicFood.cpp \
    src/food/CompositeFood.cpp \
    src/log/FoodLog.cpp \
    src/profile/UserProfile.cpp \
    src/command/Command.cpp \
    src/ui/UserInterface.cpp \
    -I include
```

### Running the Program
```bash
# If built with CMake:
./build/bin/yada

# If compiled manually:
./yada    # Unix-like systems
yada.exe  # Windows
```

## Usage Guide

### First Time Setup
1. Create your profile:
   - Enter gender (M/F)
   - Enter height (cm)
   - Enter age
   - Enter weight (kg)
   - Select activity level:
     1. Sedentary
     2. Light Exercise
     3. Moderate Exercise
     4. Very Active
     5. Extra Active
   - Choose calorie calculation method:
     1. Harris-Benedict
     2. Mifflin-St Jeor
     3. Average of Both

### Managing Foods
1. Select "Food Management" from main menu
2. Options:
   - Add basic food:
     - Enter name
     - Enter calories per serving
     - Enter search keywords (comma-separated)
   - Create composite food:
     - Enter name
     - Select component foods
     - Specify servings for each component
     - Add search keywords
   - Search foods by keywords
   - View all foods
   - Save database manually

### Tracking Food
1. Select "Log Management" from main menu
2. Options:
   - Add food to daily log:
     - Select date (YYYY-MM-DD format)
     - Choose food from list or search
     - Specify servings
   - Remove food entries
   - View log for any date
   - Undo recent changes
   - Save log manually

### Managing Profile
1. Select "Profile Management" from main menu
2. Options:
   - View current profile
   - Update changeable information:
     - Age
     - Weight
     - Activity level
   - Change calculation method
   - View calorie targets

### Data Persistence
- All data is automatically saved on exit
- Manual save option available in main menu (Option 4)
- Data loaded automatically at startup
- Text files can be manually edited if needed

## Implementation Notes

### Optimizations
- Combines same food entries in logs
- Uses food references to reduce duplication
- Efficient undo system with minimal memory usage
- Smart pointer usage for memory management

### Limitations
- Command-line interface only
- Single user system
- Basic nutritional tracking (calories only)
- Undo history not preserved between sessions

### Future Enhancements
- Additional nutritional information tracking
- Web-based food data integration
- Enhanced search capabilities
- Multi-user support

## Error Handling
- Validates all user inputs
- Handles invalid dates gracefully
- Provides clear error messages
- Prevents duplicate food entries
- Validates numerical inputs
- Graceful handling of file operations

## File Formats
All data files are stored in human-readable text format:
- `food_database.txt`: Stores food definitions and relationships
- `food_log.txt`: Stores daily consumption records
- `user_profile.txt`: Stores user information and preferences

## Troubleshooting
1. **Compilation Issues**:
   - Ensure C++17 compiler is installed
   - Check include paths are correct
   - Verify all source files are present

2. **Runtime Issues**:
   - Check file permissions for data files
   - Verify date format (YYYY-MM-DD)
   - Ensure numerical inputs are valid

3. **Data Issues**:
   - Backup data files before manual editing
   - Follow correct file formats
   - Use text editor that preserves line endings


