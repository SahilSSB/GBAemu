CXX      := clang++
CXXFLAGS := -std=c++17 -Wall -Wextra -g -O0 -Iinclude $(shell pkg-config --cflags sdl3)
LDFLAGS  := $(shell pkg-config --libs sdl3)
BUILD    := build
TARGET   := gba

SRCS := $(wildcard src/*.cpp)
OBJS := $(patsubst src/%.cpp,$(BUILD)/%.o,$(SRCS))
DEPS := $(OBJS:.o=.d)

.PHONY: all run clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) $(LDFLAGS) -o $@

$(BUILD)/%.o: src/%.cpp | $(BUILD)
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

$(BUILD):
	mkdir -p $(BUILD)

run: $(TARGET)
	./$(TARGET) src/arm.gba

clean:
	rm -rf $(BUILD) $(TARGET)

-include $(DEPS)
