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
#include "imgui/imguiAPI.h"

extern float WindowWidth;
extern float WindowHeight;
extern float Yaw;
extern float Pitch;

int main(int argc, char *argv[]) {
        GLFWwindow *window = win_init("Hello Window", 800, 600);

        struct renderer renderer = render_init();

        imgui_init(window);

        float deltaTime = 0.0f; // Time between current and last frame
        float lastFrame = 0.0f; // Time of last frame

        // Main Rendering Loop
        while (!glfwWindowShouldClose(window)) {
                float currentFrame = glfwGetTime();
                deltaTime = currentFrame - lastFrame;
                lastFrame = currentFrame;

                imgui_newFrame();

                // Update Camera
                cam_update(&renderer.camera, WindowWidth, WindowHeight, Yaw, Pitch);

                // User Inputs
                win_processInput(window, deltaTime, &renderer.camera);

                // Update
                render_update(&renderer, deltaTime);

                // Debug panel
                struct imgui_data dbg = {
                        .fps = renderer.fps,
                        .camX = renderer.camera.pos.x,
                        .camY = renderer.camera.pos.y,
                        .camZ = renderer.camera.pos.z,
                        .yaw = Yaw,
                        .pitch = Pitch,
                        .numItems = renderer.numItems,
                        .winWidth = WindowWidth,
                        .winHeight = WindowHeight,
                };
                imgui_debugPanel(&dbg);

                // Rendering
                render(renderer);

                // Render ImGui on top
                imgui_render();

                // Display the buffer to the window and check/call events
                glfwSwapBuffers(window);
                glfwPollEvents();
        }

        // Exit
        imgui_shutdown();
        glfwTerminate();
        return 0;
}

