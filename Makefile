# Compiler settings
CFLAGS = -Wall -Wextra -Wno-missing-braces -std=c11
CC = gcc

# Directories
SRC_DIR = ./src
INCLUDE_DIR = ./include
BUILD_DIR = ./build

# Output executable
TARGET = $(BUILD_DIR)/tile.exe

# Source files
SRC_FILES = $(wildcard $(SRC_DIR)/*.c)

# Rule to build the project
all: $(TARGET)

# Rule to compile the executable
$(TARGET): $(SRC_FILES) | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(SRC_FILES) -I $(INCLUDE_DIR) -o $(TARGET)

# Create the build directory if it doesn't exist
$(BUILD_DIR):
	@mkdir $(BUILD_DIR)

# Clean up build artifacts
clean:
	@rm -rf $(BUILD_DIR)

# Phony targets
.PHONY: all clean
