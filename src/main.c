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


int main(int argc, char *argv[]) {
        GLFWwindow *window = windowInit("Hello Window", 800, 600);

        struct renderer renderer = renderInit();

        // Main Rendering Loop
        while (!glfwWindowShouldClose(window)) {
                // User Inputs
                processInput(window);

                // Rendering
                glClearColor(0.07f, 0.3f, 0.7f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT);

                render(renderer);

                // Display the buffer to the window and check/call events
                glfwSwapBuffers(window);
                glfwPollEvents();
        }

        // Exit
        glfwTerminate();
        return 0;
}

