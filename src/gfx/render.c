/*************************************************
 *File----------src/gfx/render.c
 *Project-------Compute-Shaders
 *Author--------Justin Kachele
 *Created-------Wednesday Nov 11, 2026 14:15:06 EST
 *License-------GNU GPL-3.0
 ************************************************/

#include "render.h"
#include "shader.h"

struct renderer render(void) {
        float vertices[] = {
                // X      Y      Z
                -0.5f, -0.5f,  0.0f,
                 0.5f, -0.5f,  0.0f,
                 0.0f,  0.5f,  0.0f
        };

        // Create a vertex array object to store vertex attributes and buffer objects
        uint VAO;
        glGenVertexArrays(1, &VAO);

        // Allocate GPU Memory to store vertices
        uint VBO;
        glGenBuffers(1, &VBO);

        // Bind VAO to store vertex buffers and attrib pointers
        glBindVertexArray(VAO);

        // Bind the VBO Buffer to the GL_ARRAY_BUFFER object
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        // Copy vertex data to the buffer's memory
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        // Tell openGL how to interpret the vertex data and enable the attribute
        glVertexAttribPointer(
                        0,                      // Attribute index (location = 0 in vert shader)
                        3,                      // How many values in this vertex attribute
                        GL_FLOAT,               // Data type used in vertex attribute
                        GL_FALSE,               // Does the vertex need to be normalized
                        3 * sizeof(float),      // Stride (Distance between vertex attributes)
                        (void*)0);              // Offset of data in buffer
        glEnableVertexAttribArray(0);

        // Unbind VAO so other VAO calls wint modify this VAO
        glBindVertexArray(0);

        // Compile and link shader programs
        uint shaderProgram = shaderInit();

        struct renderer renderer = {.shaderProgram = shaderProgram, .VAO = VAO};
        return renderer;
}

