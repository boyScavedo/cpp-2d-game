# ================================
# Project Configuration
# ================================

PROJECT_NAME := cpp_2d_game

SRC_DIR      := src
INC_DIR      := include
BUILD_DIR    := build
ASSETS_DIR   := assets

CXX_STANDARD := -std=c++20
WARNINGS     := -Wall -Wextra -Wpedantic
DEBUG_FLAGS  := -g -O0
RELEASE_FLAGS:= -O2 -DNDEBUG

# ================================
# Platform Detection
# ================================

UNAME_S := $(shell uname -s)

# ================================
# Compiler & SDL Configuration
# ================================

ifeq ($(UNAME_S),Darwin)
    # -------- macOS (Homebrew SDL3) --------
    CXX := clang++

    SDL_CFLAGS := $(shell pkg-config --cflags sdl3)
    SDL_LIBS   := $(shell pkg-config --libs sdl3)

    PLATFORM_LIBS := $(SDL_LIBS)
    PLATFORM_INCLUDES := $(SDL_CFLAGS)

    TARGET := $(BUILD_DIR)/$(PROJECT_NAME)

else
    # -------- Windows (MinGW SDL3) --------
    CXX := g++

    SDL3_PATH ?= C:/SDL3/x86_64-w64-mingw32
    SDL3_INC  := $(SDL3_PATH)/include
    SDL3_LIBS := -L$(SDL3_PATH)/lib -lSDL3
    SDL3_DLL  := $(SDL3_PATH)/bin/SDL3.dll

    PLATFORM_LIBS := $(SDL3_LIBS)
    PLATFORM_INCLUDES := -I$(SDL3_INC)

    TARGET := $(BUILD_DIR)/$(PROJECT_NAME).exe
endif

# ================================
# Compiler Flags
# ================================

CXXFLAGS := $(CXX_STANDARD) \
            $(WARNINGS) \
            $(DEBUG_FLAGS) \
            -I$(INC_DIR) \
            $(PLATFORM_INCLUDES)

# ================================
# Source Files
# ================================

SRC_FILES := $(shell find $(SRC_DIR) -name "*.cpp")

# ================================
# Object Files
# ================================

OBJ_FILES := $(SRC_FILES:%.cpp=$(BUILD_DIR)/%.o)

# ================================
# Default Target
# ================================

all: directories copy_assets $(TARGET)

# ================================
# Build Rules
# ================================

$(TARGET): $(OBJ_FILES)
	@echo "Linking executable"
	@$(CXX) $(OBJ_FILES) -o $@ $(PLATFORM_LIBS)
	@echo "Build Successfull!"

$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	@echo "Compiling $<"
	@$(CXX) $(CXXFLAGS) -c $< -o $@

# ================================
# Directory Setup
# ================================

directories:
	@echo "Creating build directory"
	@mkdir -p $(BUILD_DIR)

# ================================
# Assets & Runtime Dependencies
# ================================

copy_assets:
	@echo "Copying assets"
	@mkdir -p $(BUILD_DIR)/$(ASSETS_DIR)
	@cp -r $(ASSETS_DIR)/* $(BUILD_DIR)/$(ASSETS_DIR)/ 2>/dev/null || echo "Warning: assets folder missing"

ifeq ($(UNAME_S),Darwin)
	@echo "macOS: no runtime SDL copy needed"
else
	@echo "Copying SDL3.dll"
	@cp $(SDL3_DLL) $(BUILD_DIR)/ 2>/dev/null || echo "Warning: SDL3.dll not found"
endif

# ================================
# Test
# ================================

test:
	@echo "No tests configured yet"

# ================================
# Run
# ================================

run: all
	$(TARGET)

# ================================
# Release Build
# ================================

release: CXXFLAGS := $(CXX_STANDARD) $(WARNINGS) $(RELEASE_FLAGS) -I$(INC_DIR) $(PLATFORM_INCLUDES)
release: clean all

# ================================
# Clean
# ================================

clean:
	@echo "Cleaning build directory"
	@rm -rf $(BUILD_DIR)
	@echo "Build directory cleaned"

.PHONY: all clean run release test copy_assets directories
