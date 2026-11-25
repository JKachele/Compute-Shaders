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
#include "items/slime.h"
#include "imgui/imguiAPI.h"

float WindowWidth  = 1280.0f;
float WindowHeight = 720.0f;

float ViewWidth;
float ViewHeight;

void imguiWidgets(struct renderer *renderer) {
                imgui_startRender();

                imgui_text("FPS: %.1f", renderer->fps);
                imgui_seperator();
                struct slimeData *slime = renderer->items[0].data;
                imgui_dragFloat("Speed", &slime->speed, 1, 0, 1000);
                imgui_dragFloat("Diffuse", &slime->diffuseRate, 0.01, 0, 20);
                imgui_dragFloat("Decay", &slime->decayRate, 0.001, 0, 0.5);

                imgui_finishRender();
}

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

                imgui_newFrame();

                // Update Camera
                cam_update(&renderer.camera, ViewWidth, ViewHeight, WindowWidth, WindowHeight);

                // User Inputs
                win_processInput(window, deltaTime, &renderer.camera);

                // Update
                render_update(&renderer, deltaTime);

                // Rendering
                render(renderer);

                // Render ImGui on top
                imguiWidgets(&renderer);

                // Display the buffer to the window and check/call events
                glfwSwapBuffers(window);
                glfwPollEvents();
        }

        // Exit
        imgui_shutdown();
        glfwTerminate();
        return 0;
}

