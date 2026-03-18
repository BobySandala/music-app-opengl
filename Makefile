CXX = g++
CXXFLAGS = -std=c++17 -Iinclude -Isrc -Wall -MMD -MP
LDFLAGS = -lglfw -lGL -ldl -pthread -lcurl -pthread -ldl

SRC_DIR = src
BUILD_DIR = build

CPP_FILES := $(shell find $(SRC_DIR) -name '*.cpp')
C_FILES := $(shell find $(SRC_DIR) -name '*.c')

OBJ_FILES := $(CPP_FILES:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o) \
             $(C_FILES:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

TARGET = music_player

all: $(TARGET)

$(TARGET): $(OBJ_FILES)
	$(CXX) $^ -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf build $(TARGET)

run: all
	./$(TARGET)

# 👇 ADD THIS LINE
-include $(OBJ_FILES:.o=.d)