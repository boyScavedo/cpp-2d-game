PROJECT_NAME := cpp_2d_game
BUILD_DIR    := build
SRC_DIR      := src
INC_DIR      := include
ASSETS_DIR   := assets

CXX := clang++

CXXFLAGS := -std=c++17 \
            -Wall -Wextra -Wpedantic \
            -g -O0 \
            -I$(INC_DIR) \
            -I/usr/local/include

SDL3_LIB := -L/usr/local/lib -lSDL3

SRCS := \
    $(SRC_DIR)/Application/main.cpp \
    $(SRC_DIR)/Engine/WindowManager.cpp

OBJS := $(SRCS:src/%.cpp=build/%.o)

TARGET := $(BUILD_DIR)/$(PROJECT_NAME)

all: copy_assets $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $@ $(SDL3_LIB) -Wl,-rpath,/usr/local/lib

build/%.o:
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c src/$*.cpp -o $@

copy_assets:
	mkdir -p $(BUILD_DIR)/$(ASSETS_DIR)
	cp -r $(ASSETS_DIR)/* $(BUILD_DIR)/$(ASSETS_DIR)/ 2>/dev/null || echo "Warning: assets folder missing"

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean copy_assets
