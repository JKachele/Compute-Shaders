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

struct imgui_data {
        float fps;
        float camX, camY, camZ;
        float yaw, pitch;
        int   numItems;
        float winWidth, winHeight;
};

void imgui_init(GLFWwindow *window);
void imgui_setMouse(bool capture);
void imgui_newFrame(void);
void imgui_render(void);
void imgui_shutdown(void);
void imgui_debugPanel(struct imgui_data *data);

#ifdef __cplusplus
}
#endif

#endif
