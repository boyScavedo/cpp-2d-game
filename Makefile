# ================================
# Project Configuration
# ================================

PROJECT_NAME := cpp_2d_game
CXX          := g++
CXX_STANDARD := -std=c++20

# Directory layout
SRC_DIR      := src
INC_DIR      := include
BUILD_DIR    := build

APP_DIR      := $(SRC_DIR)/Application
ENGINE_DIR   := $(SRC_DIR)/Engine
GAMEPLAY_DIR := $(SRC_DIR)/Gameplay
UTILS_DIR    := $(SRC_DIR)/Utils

# ================================
# SDL3 Configuration
# ================================
# Adjust SDL3_PATH if SDL3 is not system-installed

SDL3_PATH    := C:/SDL3/x86_64-w64-mingw32
SDL3_INC     := $(SDL3_PATH)/include
SDL3_LIB     := -L$(SDL3_PATH)/lib -lSDL3
SDL3_DLL     := $(SDL3_PATH)/bin/SDL3.dll

# ================================
# Compiler Flags
# ================================

WARNINGS     := -Wall -Wextra -Wpedantic
DEBUG_FLAGS  := -g -O0
RELEASE_FLAGS:= -O2 -DNDEBUG

INCLUDES     := -I$(INC_DIR) -I$(SDL3_INC)

CXXFLAGS     := $(CXX_STANDARD) $(WARNINGS) $(DEBUG_FLAGS) $(INCLUDES)

# ================================
# Source Files
# ================================

APP_SRC      := $(wildcard $(APP_DIR)/*.cpp)
ENGINE_SRC   := $(wildcard $(ENGINE_DIR)/*.cpp)
GAMEPLAY_SRC := $(wildcard $(GAMEPLAY_DIR)/*.cpp)
UTILS_SRC    := $(wildcard $(UTILS_DIR)/*.cpp)

SRC_FILES    := $(APP_SRC) \
                $(ENGINE_SRC) \
                $(GAMEPLAY_SRC) \
                $(UTILS_SRC)

# ================================
# Object Files
# ================================

OBJ_FILES := $(SRC_FILES:%.cpp=$(BUILD_DIR)/%.o)

# ================================
# Default Target
# ================================

all: $(BUILD_DIR) copy_assets $(BUILD_DIR)/$(PROJECT_NAME).exe

# ================================
# Build Rules
# ================================

copy_assets:
	mkdir -p $(BUILD_DIR)/assets
	cp $(SDL3_DLL) $(BUILD_DIR)/ 2>/dev/null || echo "Warning: SDL3.dll not found in $(SDL3_PATH)/bin"
	cp -r assets/* $(BUILD_DIR)/assets/

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)
	mkdir -p $(BUILD_DIR)/$(APP_DIR)
	mkdir -p $(BUILD_DIR)/$(ENGINE_DIR)
	mkdir -p $(BUILD_DIR)/$(GAMEPLAY_DIR)
	mkdir -p $(BUILD_DIR)/$(UTILS_DIR)

$(BUILD_DIR)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/$(PROJECT_NAME).exe: $(OBJ_FILES)
	$(CXX) $(OBJ_FILES) -o $@ $(SDL3_LIB)

# ================================
# Run
# ================================

run: all
	$(BUILD_DIR)/$(PROJECT_NAME).exe

# ================================
# Release Build
# ================================

release: CXXFLAGS := $(CXX_STANDARD) $(WARNINGS) $(RELEASE_FLAGS) $(INCLUDES)
release: clean all

# ================================
# Clean
# ================================

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all run clean release copy_assets
