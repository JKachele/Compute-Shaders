/*************************************************
 *File----------src/main.c
 *Project-------Compute-Shaders
 *Author--------Justin Kachele
 *Created-------Wednesday Nov 11, 2026 12:38:04 EST
 *License-------GNU GPL-3.0
 ************************************************/

#include <stdio.h>
#include "util.h"
#include "gfx/window.h"
#include "gfx/render.h"

extern float WindowWidth;
extern float WindowHeight;
extern float Yaw;
extern float Pitch;

int main(int argc, char *argv[]) {
        GLFWwindow *window = win_init("Hello Window", 800, 600);

        struct renderer renderer = ren_init();

        // int x, y, z;
        // glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 0, &x);
        // glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 1, &y);
        // glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 2, &z);
        // printf("X:%d, Y:%d, Z:%d\n", x, y, z);
        // glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 0, &x);
        // glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 1, &y);
        // glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 2, &z);
        // printf("X:%d, Y:%d, Z:%d\n", x, y, z);

        float deltaTime = 0.0f; // Time between current and last frame
        float lastFrame = 0.0f; // Time of last frame

        // Main Rendering Loop
        while (!glfwWindowShouldClose(window)) {
                float currentFrame = glfwGetTime();
                deltaTime = currentFrame - lastFrame;
                lastFrame = currentFrame;

                // Update Camera
                cam_update(&renderer.camera, WindowWidth, WindowHeight, Yaw, Pitch);

                // User Inputs
                win_processInput(window, deltaTime, &renderer.camera);

                // Rendering
                render(renderer);

                // Display the buffer to the window and check/call events
                glfwSwapBuffers(window);
                glfwPollEvents();
        }

        // Exit
        glfwTerminate();
        return 0;
}

