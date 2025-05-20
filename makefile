CXX = g++
CXXFLAGS = -std=c++23 -g
CPPFLAGS = -I$(INC_DIR)

SRC_DIR = src
INC_DIR = inc
BUILD_DIR = build
BIN_DIR = $(BUILD_DIR)/bin
OBJ_DIR = $(BUILD_DIR)/obj

SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))

TARGET = $(BIN_DIR)/program

all: $(TARGET)

$(TARGET): $(OBJS) | $(BIN_DIR)
	@echo "Linking $@..."
	@$(CXX) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	@echo "Compiling $<..."
	@$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c -o $@ $<

$(BIN_DIR) $(OBJ_DIR):
	@mkdir -p $@

clean:
	@echo "Cleaning..."
	@rm -rf $(BUILD_DIR)

.PHONY: all clean