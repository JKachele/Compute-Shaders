######################################################################
# @author      : Justin Kachele (justin@kachele.com)
# @file        : Makefile
# @created     : Wednesday Nov 11, 2026 12:34:31 EST
######################################################################

CC = gcc
CFLAGS = -std=c11 -g -MMD
CFLAGS += -Wall -Wextra -Wpedantic 
CFLAGS += -Wno-unused-parameter -Wno-unused-variable
CFLAGS += -Ilib/cglm/include -Ilib/glad/include -Ilib/glfw/include -Ilib/stb
LDFLAGS := -lm -g

BIN := bin
BUILD_DIR := build

SRC  := $(shell find src/ -type f -name '*.c')
OBJ  := $(SRC:%=$(BUILD_DIR)/%.o)
DEPS := $(SRC:%=$(BUILD_DIR)/%.d)

# libs
GLFW := lib/glfw/src/libglfw3.a
GLAD := lib/glad/src/glad.o
CGLM := lib/cglm/libcglm.a
LIBS := $(GLFW) $(GLAD) $(CGLM)

OUT := $(BIN)/out

.PHONY: build run clean

run: $(OUT)
	$(OUT) | tee $(BIN)/output.log

build: $(OUT)

$(OUT): $(OBJ) $(LIBS)
	@mkdir -p $(dir $@)
	$(CC) -o $(OUT) $(OBJ) $(LIBS) $(LDFLAGS)

$(BUILD_DIR)/%.c.o: %.c
	@mkdir -p $(dir $@)
	$(CC) -o $@ -c $< $(CFLAGS)

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

