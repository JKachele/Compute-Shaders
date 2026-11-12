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
                 0.5f,  0.5f,  0.0f,    // Top Right
                 0.5f, -0.5f,  0.0f,    // Bottom Right
                -0.5f, -0.5f,  0.0f,    // Bottom Left
                -0.5f,  0.5f,  0.0f     // Top Left
        };
        uint indices[] = {
                0, 1, 3,        // First Triangle
                1, 2, 3         // Second Triangle
        };

        // Create a vertex array object to store vertex attributes and buffer objects
        uint VAO;
        glGenVertexArrays(1, &VAO);

        // Allocate GPU Memory to store vertices
        uint VBO;
        glGenBuffers(1, &VBO);

        // Create Element Buffer to store vertex indices to draw triangles
        uint EBO;
        glGenBuffers(1, &EBO);

        // Bind VAO to store vertex buffers and attrib pointers
        glBindVertexArray(VAO);

        // Bind the VBO Buffer to the GL_ARRAY_BUFFER object and copy vertex data to buffer
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        // Bind the EBO and store index data to buffer
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

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

        struct renderer renderer = {.shaderProgram = shaderProgram, .VAO = VAO, .EBO = EBO};
        return renderer;
}

