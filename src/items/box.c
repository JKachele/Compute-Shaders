/*************************************************
 *File----------src/items/box.c
 *Project-------Compute-Shaders
 *Author--------Justin Kachele
 *Created-------Monday Nov 16, 2026 15:32:45 EST
 *License-------GNU GPL-3.0
 ************************************************/

#include "box.h"

static void box_update(struct item *self, float dt) {
        for (int i = 0; i < self->numCopies; i++) {
                self->angle[i] += dt * 50;
        }
}

struct item item_boxInit(void) {
        struct item box = {0};

        float vertices[] = {
                //X      Y      Z     Xuv   Yuv
                -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
                 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
                 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
                 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
                -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
                -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

                -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
                 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
                 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
                 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
                -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
                -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

                -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
                -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
                -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
                -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
                -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
                -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

                 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
                 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
                 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
                 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
                 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
                 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

                -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
                 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
                 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
                 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
                -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
                -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

                -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
                 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
                 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
                 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
                -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
                -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
        };

        // Create a vertex array object to store vertex attributes and buffer objects
        uint VAO;
        glGenVertexArrays(1, &VAO);
        box.VAO = VAO;

        // Allocate GPU Memory to store vertices
        uint VBO;
        glGenBuffers(1, &VBO);

        // Bind VAO to store vertex buffers and attrib pointers
        glBindVertexArray(VAO);

        // Bind the VBO Buffer to the GL_ARRAY_BUFFER object and copy vertex data to buffer
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        // Tell openGL how to interpret the vertex data and enable the attribute
        glVertexAttribPointer(
                        0,                      // Attribute index (location = 0 in vert shader)
                        3,                      // How many values in this vertex attribute
                        GL_FLOAT,               // Data type used in vertex attribute
                        GL_FALSE,               // Does the vertex need to be normalized
                        5 * sizeof(float),      // Stride (Distance between vertex attributes)
                        (void*)0);              // Offset of data in buffer
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(3*sizeof(float)));
        glEnableVertexAttribArray(1);

        // Unbind VAO so other VAO calls wint modify this VAO
        glBindVertexArray(0);

        box.hasEBO = false;
        box.mode = GL_TRIANGLES;
        box.count = 36;
        box.updateFn = box_update;
        box.renderFn = item_render;

        // Compile and link shader programs
        struct shader shader = shader_init();
        shader_add(&shader, SHADER_VERT, GL_VERTEX_SHADER);
        shader_add(&shader, SHADER_FRAG_TEX, GL_FRAGMENT_SHADER);
        shader_link(&shader);
        box.shader = shader;

        // Load textures
        struct texture textures = tex_init(shader);
        box.textures = textures;

        // Create 10 boxes
        const vec3s cubePositions[] = {
                (vec3s){ 0.0f,  0.0f,  0.0f},
                (vec3s){ 2.0f,  5.0f, -15.0f},
                (vec3s){-1.5f, -2.2f, -2.5f},
                (vec3s){-3.8f, -2.0f, -12.3f},
                (vec3s){ 2.4f, -0.4f, -3.5f},
                (vec3s){-1.7f,  3.0f, -7.5f},
                (vec3s){ 1.3f, -2.0f, -2.5f},
                (vec3s){ 1.5f,  2.0f, -2.5f},
                (vec3s){ 1.5f,  0.2f, -1.5f},
                (vec3s){-1.3f,  1.0f, -1.5f}
        };

        item_setNumCopies(&box, 10);
        for (int i = 0; i < 10; i++) {
                item_setState(&box, i, cubePositions[i], 1.0f, 20.0f*i, (vec3s){1.0f, 0.3f, 0.5f});
        }

        return box;
}

