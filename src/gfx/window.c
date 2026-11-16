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

float WindowWidth  = 800.0f;
float WindowHeight = 600.0f;
float Yaw = -90.0f;
float Pitch = 0.0f;

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
        WindowWidth = width;
        WindowHeight = height;
        glViewport(0, 0, width, height);
} 

void mouseCallback(GLFWwindow *window, double xPos, double yPos) {
        const float Sensitivity = 0.1f;

        // Store last mouse position
        static float lastX = 400;
        static float lastY = 300;
        static bool firstMouse = true;

        // Set last position to current if mouse first entered window
        // Prevents large movement jump when first entering
        if (firstMouse) {
                lastX = xPos;
                lastY = yPos;
                firstMouse = false;
        }

        // Calculate mouse offsets
        float xOffset = (xPos - lastX) * Sensitivity;
        float yOffset = (lastY - yPos) * Sensitivity; // reversed since y goes from bottom to top
        lastX = xPos;
        lastY = yPos;

        // Update Yaw/Pitch with offsets
        Yaw   += xOffset;
        Pitch += yOffset;

        // Constrain pitch to not flip over y axis
        if (Pitch > 89.0f)  { Pitch = 89.0f;  }
        if (Pitch < -89.0f) { Pitch = -89.0f; }
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
        glViewport(0, 0, 800, 600);

        // Give glfw the callback function if the window size changes
        glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

        // Hide the cusor and capture its position
        glfwSetCursorPosCallback(window, mouseCallback);
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        return window;
}

