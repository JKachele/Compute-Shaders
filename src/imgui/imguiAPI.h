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

#define IMGUI_S8     ImGuiDataType_S8
#define IMGUI_U8     ImGuiDataType_U8
#define IMGUI_S16    ImGuiDataType_S16
#define IMGUI_U16    ImGuiDataType_U16
#define IMGUI_S32    ImGuiDataType_S32
#define IMGUI_U32    ImGuiDataType_U32
#define IMGUI_S64    ImGuiDataType_S64
#define IMGUI_U64    ImGuiDataType_U64
#define IMGUI_FLOAT  ImGuiDataType_Float
#define IMGUI_DOUBLE ImGuiDataType_Double

void imgui_init(GLFWwindow *window);
void imgui_setMouse(bool capture);
void imgui_newFrame(void);
void imgui_startRender(void);
void imgui_finishRender(void);
void imgui_shutdown(void);
void imgui_showDemoWindow(void);

// Wigdets
void imgui_seperator(void);
void imgui_seperatorText(const char *label);
void imgui_text(const char *fmt, ...);
void imgui_dragFloat(const char *label, float *v, float speed, float min, float max);

#ifdef __cplusplus
}
#endif

#endif
