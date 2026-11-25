######################################################################
# @author      : Justin Kachele (justin@kachele.com)
# @file        : Makefile
# @created     : Wednesday Nov 11, 2026 12:34:31 EST
######################################################################

CC       := gcc
CXX      := g++
FLAGS    := -O3 -MMD -Wall -Wextra -Wpedantic 
FLAGS    += -Wno-unused-parameter -Wno-unused-variable -Wno-missing-braces
FLAGS    += -Ilib/glad/include -Ilib/glfw/include -Ilib/stb
FLAGS    += -Ilib/cglm/include -DCGLM_OMIT_NS_FROM_STRUCT_API
FLAGS    += -I/usr/include/freetype2 -I/usr/include/libpng16
CFLAGS   := -std=c11 $(FLAGS)
CXXFLAGS := -std=c++17 $(FLAGS)
CXXFLAGS += -Ilib/imgui -Ilib/imgui/backends
CXXFLAGS += -DIMGUI_IMPL_OPENGL_LOADER_CUSTOM
CXXFLAGS += -include glad/glad.h
LDFLAGS  := -lm -lfreetype -g

BIN := bin
BUILD_DIR := build
BUILD_IMGUI := lib/imgui/build

SRC  := $(shell find src/ -type f -name '*.c')
OBJ  := $(SRC:%=$(BUILD_DIR)/%.o)
DEPS := $(SRC:%=$(BUILD_DIR)/%.d)

CXX_SRC := $(shell find src/ -type f -name '*.cpp')
CXX_OBJ := $(CXX_SRC:%.cpp=$(BUILD_DIR)/%.cpp.o)

IMGUI_SRC := lib/imgui/imgui.cpp \
             lib/imgui/imgui_draw.cpp \
             lib/imgui/imgui_widgets.cpp \
             lib/imgui/imgui_tables.cpp \
             lib/imgui/backends/imgui_impl_glfw.cpp \
             lib/imgui/backends/imgui_impl_opengl3.cpp
IMGUI_OBJ := $(IMGUI_SRC:%.cpp=$(BUILD_IMGUI)/%.cpp.o)

# libs
GLFW := lib/glfw/src/libglfw3.a
GLAD := lib/glad/src/glad.o
CGLM := lib/cglm/libcglm.a
LIBS := $(GLFW) $(GLAD) $(CGLM)

OUT := $(BIN)/out

ifdef WSL_DISTRO_NAME
	CFLAGS += -DWSL
	CXXFLAGS += -DWSL
endif

.PHONY: build run clean

run: $(OUT)
	$(OUT) | tee $(BIN)/output.log

build: $(OUT)

$(OUT): $(OBJ) $(CXX_OBJ) $(IMGUI_OBJ) $(LIBS)
	@mkdir -p $(dir $@)
	$(CXX) -o $(OUT) $(OBJ) $(CXX_OBJ) $(IMGUI_OBJ) $(LIBS) $(LDFLAGS)

$(BUILD_DIR)/%.c.o: %.c
	@mkdir -p $(dir $@)
	$(CC) -o $@ -c $< $(CFLAGS)

$(BUILD_DIR)/%.cpp.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) -o $@ -c $< $(CXXFLAGS)

$(BUILD_IMGUI)/%.cpp.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) -o $@ -c $< $(CXXFLAGS)

-include $(DEPS)

# Libaries
$(GLFW):
	cd lib/glfw && cmake . && make

$(GLAD):
	cd lib/glad && $(CC) -o src/glad.o -Iinclude -c src/glad.c

$(CGLM):
	cd lib/cglm && cmake . -DCGLM_STATIC=ON && make

clean:
	rm -rf $(BIN)/
	rm -rf $(BUILD_DIR)/

