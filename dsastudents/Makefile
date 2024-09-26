CXX = g++
CXXFLAGS = -g -Iinclude -Isrc
BUILD_DIR = build
ANN_SOURCES = $(wildcard src/ann/*.cpp)
USER_SOURCE = $(file)
TARGET = multilayer_perceptron

ANN_OBJECTS = $(patsubst src/ann/%.cpp,$(BUILD_DIR)/%.o,$(ANN_SOURCES))
USER_OBJECT = $(BUILD_DIR)/$(basename $(notdir $(USER_SOURCE))).o

.PHONY: all clean

$(TARGET): $(BUILD_DIR) $(ANN_OBJECTS) $(USER_OBJECT)
	$(CXX) $(CXXFLAGS) -o $@ $(ANN_OBJECTS) $(USER_OBJECT)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/%.o: src/ann/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(USER_OBJECT): $(USER_SOURCE) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(TARGET)