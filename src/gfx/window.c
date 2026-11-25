/*************************************************
 *File----------src/gfx/window.c
 *Project-------Compute-Shaders
 *Author--------Justin Kachele
 *Created-------Wednesday Nov 11, 2026 13:54:14 EST
 *License-------GNU GPL-3.0
 ************************************************/

#include <stdio.h>
#include <stdbool.h>
#include "window.h"
#include "../imgui/imguiAPI.h"

#ifdef WSL
#define CURSOR_MODE GLFW_CURSOR_HIDDEN
#else
#define CURSOR_MODE GLFW_CURSOR_DISABLED
#endif

extern float WindowWidth;
extern float WindowHeight;


static void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
        WindowWidth = width;
        WindowHeight = height;
        glViewport(0, 0, width, height);
} 

void win_processInput(GLFWwindow *window, float deltaTime, struct camera *cam) {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
                glfwSetWindowShouldClose(window, true);
        }
}

GLFWwindow *win_init(const char *title, int width, int height) {
        // Initialize glfw and set openGL Versions
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        // Create window
        WindowWidth = width;
        WindowHeight = height;
        GLFWwindow *window = glfwCreateWindow(width, height, title, NULL, NULL);
        if (window == NULL) {
                fprintf(stderr, "Failed to create GLFW window\n");
                glfwTerminate();
                return NULL;
        }
        glfwMakeContextCurrent(window);

        // Initialize glad
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
                fprintf(stderr, "Failed to initialize GLAD\n");
                return NULL;
        }

        // Set OpenGL's viewport (Screen size)
        glViewport(0, 0, width, height);

        // Give glfw the callback function if the window size changes
        glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

        return window;
}

