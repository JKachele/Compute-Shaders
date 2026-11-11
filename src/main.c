/*************************************************
 *File----------src/main.c
 *Project-------Compute-Shaders
 *Author--------Justin Kachele
 *Created-------Wednesday Nov 11, 2026 12:38:04 EST
 *License-------GNU GPL-3.0
 ************************************************/

#include <stdio.h>
#include "util.h"


void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
        glViewport(0, 0, width, height);
} 

void processInput(GLFWwindow *window) {
        if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
                glfwSetWindowShouldClose(window, true);
        }
}

int main(int argc, char *argv[]) {
        // Initialize glfw and set openGL Versions
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        // Create window
        GLFWwindow *window = glfwCreateWindow(800, 600, "Hello Window", NULL, NULL);
        if (window == NULL) {
                fprintf(stderr, "Failed to create GLFW window\n");
                glfwTerminate();
                return -1;
        }
        glfwMakeContextCurrent(window);

        // Initialize glad
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
                fprintf(stderr, "Failed to initialize GLAD\n");
                return -1;
        }

        // Set OpenGL's viewport (Screen size)
        glViewport(0, 0, 800, 600);

        // Give glfw the callback function if the window size changes
        glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

        // Main Rendering Loop
        while (!glfwWindowShouldClose(window)) {
                // User Inputs
                processInput(window);

                // Rendering
                glClearColor(0.07f, 0.3f, 0.7f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT);

                // Display the buffer to the window and check/call events
                glfwSwapBuffers(window);
                glfwPollEvents();
        }

        // Exit
        glfwTerminate();
        return 0;
}

