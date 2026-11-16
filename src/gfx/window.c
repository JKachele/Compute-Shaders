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

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
        glViewport(0, 0, width, height);
} 

void processInput(GLFWwindow *window, float deltaTime, struct camera *cam) {
        cam->speed = 2.5f * deltaTime;
        if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
                glfwSetWindowShouldClose(window, true);
        }
        if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
                cameraMove(cam, FRONT);
        }
        if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
                cameraMove(cam, LEFT);
        }
        if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
                cameraMove(cam, BACK);
        }
        if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
                cameraMove(cam, RIGHT);
        }
}

GLFWwindow *windowInit(const char *title, int width, int height) {
        // Initialize glfw and set openGL Versions
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        // Create window
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
        glViewport(0, 0, 800, 600);

        // Give glfw the callback function if the window size changes
        glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
        return window;
}

