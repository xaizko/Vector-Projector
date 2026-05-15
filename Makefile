CXX = g++
CXXFLAGS = -std=c++23 -Wall -Wextra
LDLIBS = -lraylib

SRC = src/main.cpp
OBJ = $(SRC:src/%.cpp=bin/%.o)
DEP = $(OBJ:.o=.d)
TARGET = bin/vector_projection

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $@ $(LDLIBS)

bin/%.o: src/%.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

-include $(DEP)

clean:
	rm -rf bin
