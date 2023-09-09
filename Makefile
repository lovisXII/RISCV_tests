SRC_DIR := src/
INCLUDE_DIR := include/
BUILD_DIR := build/
EXECUTABLE := $(BUILD_DIR)a.out

# Get a list of all .cpp files in the SRC_DIR
SRC_FILES := $(wildcard $(SRC_DIR)*.cpp)
# Add the main source file to the list of source files
SRC_FILES += generate_user_test.cpp

# Generate a list of corresponding .o files in the BUILD_DIR
OBJ_FILES := $(patsubst $(SRC_DIR)%.cpp,$(BUILD_DIR)%.o,$(SRC_FILES))

CXX := clang++
CXXFLAGS := -g -I$(INCLUDE_DIR)

NUMBER_FILES := 10
NUMBER_TESTS := 2

.PHONY: all run compile clean

all: run

run: $(EXECUTABLE)
	./$^ --batch --test-per-file $(NUMBER_TESTS) --max-test $(NUMBER_FILES) --dir $(BUILD_DIR)

$(EXECUTABLE): $(OBJ_FILES)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(BUILD_DIR)%.o: $(SRC_DIR)%.cpp
	mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

compile: $(EXECUTABLE)

clean:
	rm -rf $(EXECUTABLE) $(BUILD_DIR)
