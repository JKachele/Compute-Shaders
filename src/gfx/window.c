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
float Yaw   = -90.0f;
float Pitch = 0.0f;

static float Speed = 2.5f;
static bool MouseCapture = true;
static bool FirstMouse = true;


static void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
        WindowWidth = width;
        WindowHeight = height;
        glViewport(0, 0, width, height);
} 

static void mouseCallback(GLFWwindow *window, double xPos, double yPos) {
        const float Sensitivity = 0.1f;

        if (!MouseCapture) {
                return;
        }

        // Store last mouse position
        static float lastX = 0.0;
        static float lastY = 0.0;

        // Set last position to current if mouse first entered window
        // Prevents large movement jump when first entering
        if (FirstMouse) {
                lastX = xPos;
                lastY = yPos;
                FirstMouse = false;
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

static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
        Speed += (float)yoffset;
        if (Speed < 2.5f) {
                Speed = 2.5f;
        }
}

static void toggleMouse(GLFWwindow* window) {
        MouseCapture = !MouseCapture;

        if (MouseCapture) {
                glfwSetInputMode(window, GLFW_CURSOR, CURSOR_MODE);
                imgui_setMouse(false);
                FirstMouse = true;
        } else {
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                imgui_setMouse(true);
        }
}

void win_processInput(GLFWwindow *window, float deltaTime, struct camera *cam) {
        float camSpeed = Speed * deltaTime;
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
                glfwSetWindowShouldClose(window, true);
        }
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
                cam_move(cam, FRONT, camSpeed);
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
                cam_move(cam, LEFT, camSpeed);
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
                cam_move(cam, BACK, camSpeed);
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
                cam_move(cam, RIGHT, camSpeed);
        }
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
                cam_move(cam, UP, camSpeed);
        }
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
                cam_move(cam, DOWN, camSpeed);
        }
        if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS) {
                toggleMouse(window);
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

        // Hide the cusor and capture its position
        glfwSetCursorPosCallback(window, mouseCallback);
        glfwSetScrollCallback(window, scrollCallback);
        glfwSetInputMode(window, GLFW_CURSOR, CURSOR_MODE);

        return window;
}

