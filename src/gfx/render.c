/*************************************************
 *File----------src/gfx/render.c
 *Project-------Compute-Shaders
 *Author--------Justin Kachele
 *Created-------Wednesday Nov 11, 2026 14:15:06 EST
 *License-------GNU GPL-3.0
 ************************************************/

#include "render.h"

void render(void) {
        float vertices[] = {
                // X      Y      Z
                -0.5f, -0.5f,  0.0f,
                 0.5f, -0.5f,  0.0f,
                 0.0f,  0.5f,  0.0f
        };

        // Allocate GPU Memory to store vertices
        uint32 VBO;
        glGenBuffers(1, &VBO);

        // Bind the VBO Buffer to the GL_ARRAY_BUFFER object
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        // Copy vertex data to the buffer's memory
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
}

