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

float WindowWidth  = 1280.0f;
float WindowHeight = 720.0f;

float ViewWidth;
float ViewHeight;

int main(int argc, char *argv[]) {
        GLFWwindow *window = win_init("Hello Window", WindowWidth, WindowHeight);
        ViewWidth = WindowWidth;
        ViewHeight = WindowHeight;

        struct renderer renderer = render_init(WindowWidth, WindowHeight);

        imgui_init(window);

        float deltaTime = 0.0f; // Time between current and last frame
        float lastFrame = 0.0f; // Time of last frame

        // Main Rendering Loop
        while (!glfwWindowShouldClose(window)) {
                float currentFrame = glfwGetTime();
                deltaTime = currentFrame - lastFrame;
                lastFrame = currentFrame;

                // imgui_newFrame();

                // Update Camera
                cam_update(&renderer.camera, ViewWidth, ViewHeight, WindowWidth, WindowHeight);

                // User Inputs
                win_processInput(window, deltaTime, &renderer.camera);

                // Update
                render_update(&renderer, deltaTime);

                // Debug panel
                struct imgui_data dbg = {
                        .fps = renderer.fps,
                        .numItems = renderer.numItems,
                        .winWidth = WindowWidth,
                        .winHeight = WindowHeight,
                };
                // imgui_debugPanel(&dbg);

                // Rendering
                render(renderer);

                // Render ImGui on top
                // imgui_render();

                // Display the buffer to the window and check/call events
                glfwSwapBuffers(window);
                glfwPollEvents();
        }

        // Exit
        imgui_shutdown();
        glfwTerminate();
        return 0;
}

