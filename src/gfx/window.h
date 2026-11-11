/*************************************************
 *File----------src/gfx/window.h
 *Project-------Compute-Shaders
 *Author--------Justin Kachele
 *Created-------Wednesday Nov 11, 2026 13:54:19 EST
 *License-------GNU GPL-3.0
 ************************************************/
#ifndef WINDOW_H
#define WINDOW_H

#include "../util.h"

void processInput(GLFWwindow *window);
GLFWwindow *windowInit(const char *title, int width, int height);

#endif

