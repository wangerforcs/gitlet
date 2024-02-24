CXX = g++
TARGET = gitlet
INCLUDE_DIR = include
FLAGS = -std=c++11 -g -I $(INCLUDE_DIR)
SRC_DIR = src
BIN_DIR = bin
$(shell mkdir -p $(BIN_DIR))
SRC = $(wildcard $(SRC_DIR)/*.cpp)
SRC += main.cpp
OBJ = $(patsubst %.cpp, $(BIN_DIR)/%.o, $(notdir $(SRC)))

$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $(TARGET) $(FLAGS)

vpath %.cpp $(SRC_DIR)

$(BIN_DIR)/%.o: %.cpp
	$(CXX) -c $< -o $@ $(FLAGS)

.PHONY: clean
clean:
	rm -rf $(TARGET) $(BIN_DIR)/$(OBJ)

.PHONY: del
del:
	rm -rf .gitlet