# Compiler
CXX := g++

# Directories
INCLUDE_DIR := include
SOURCE_DIR := source
DEPENDENCIES_DIR := dependencies
BUILD_DIR := build
BIN_DIR := bin

# Flags
CXXFLAGS := -Wall -Wextra -I$(INCLUDE_DIR) -I$(DEPENDENCIES_DIR)

# Source and Object Files
SOURCES := $(wildcard $(SOURCE_DIR)/*.cpp)
OBJECTS := $(patsubst $(SOURCE_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SOURCES))

# Executable Name
TARGET := $(BIN_DIR)/chess

# Default Target
all: $(TARGET)

# Link Object Files to Create Executable
$(TARGET): $(OBJECTS) | $(BIN_DIR)
	$(CXX) $(OBJECTS) -o $(TARGET)

# Compile Source Files into Object Files
$(BUILD_DIR)/%.o: $(SOURCE_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Create Build and Bin Directories if They Don't Exist
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# Clean Build Files
clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)
