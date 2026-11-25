/*************************************************
 *File----------src/gfx/imgui_backend.h
 *Project-------Compute-Shaders
 *Author--------Justin Kachele
 *Created-------Wednesday Nov 23, 2026
 *License-------GNU GPL-3.0
 ************************************************/
#ifndef IMGUIAPI_H
#define IMGUIAPI_H

#include <GLFW/glfw3.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

void imgui_init(GLFWwindow *window);
void imgui_setMouse(bool capture);
void imgui_newFrame(void);
void imgui_startRender(void);
void imgui_finishRender(void);
void imgui_shutdown(void);

// Wigdets
void imgui_seperator(void);
void imgui_text(const char *fmt, ...);
void imgui_dragFloat(const char *label, float *v, float speed, float min, float max);

#ifdef __cplusplus
}
#endif

#endif
