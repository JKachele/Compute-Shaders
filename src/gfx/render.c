/*************************************************
 *File----------src/gfx/render.c
 *Project-------Compute-Shaders
 *Author--------Justin Kachele
 *Created-------Wednesday Nov 11, 2026 14:15:06 EST
 *License-------GNU GPL-3.0
 ************************************************/

#include "render.h"
#include "shader.h"
#include "texture.h"

// Create all vertex and element objects and compile shaders
struct renderer renderInit(void) {
        struct renderer renderer;

        // float vertices[] = {
        //         //X      Y      Z       Xuv   Yuv
        //          0.5f,  0.5f,  0.0f,    1.0f, 1.0f,     // Top Right
        //          0.5f, -0.5f,  0.0f,    1.0f, 0.0f,     // Bottom Right
        //         -0.5f, -0.5f,  0.0f,    0.0f, 0.0f,     // Bottom Left
        //         -0.5f,  0.5f,  0.0f,    0.0f, 1.0f      // Top Left
        // };
        // uint indices[] = {
        //         0, 1, 3,        // First Triangle
        //         3, 2, 1         // Second Triangle
        // };

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
        renderer.VAO = VAO;

        // Allocate GPU Memory to store vertices
        uint VBO;
        glGenBuffers(1, &VBO);

        // Create Element Buffer to store vertex indices to draw triangles
        // uint EBO;
        // glGenBuffers(1, &EBO);
        // renderer.EBO = EBO;

        // Bind VAO to store vertex buffers and attrib pointers
        glBindVertexArray(VAO);

        // Bind the VBO Buffer to the GL_ARRAY_BUFFER object and copy vertex data to buffer
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        // Bind the EBO and store index data to buffer
        // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

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

        // Compile and link shader programs
        uint shaderProgram = shaderInit();
        shaderUse(shaderProgram);
        renderer.shaderProgram = shaderProgram;

        // Load textures
        struct texture textures = textureInit(shaderProgram);
        renderer.textures = textures;

        // Enable Z-Buffer for Depth Testing
        glEnable(GL_DEPTH_TEST);

        return renderer;
}

static void transform(uint shaderProgram) {
        // Model Matrix
        mat4s model = mat4_identity();
        model = glms_rotate(model, (float)glfwGetTime(), (vec3s){0.5f, 1.0f, 0.0f});
        shaderSetMat4(shaderProgram, "model", model);

        // View Matrix
        mat4s view = mat4_identity();
        view = glms_translate(view, (vec3s){0.0f, 0.0f, -3.0f});
        shaderSetMat4(shaderProgram, "view", view);

        // Projection Matrix
        mat4s projection;
        projection = glms_perspective(glm_rad(45.0f), 800.0f/600.0f, 0.1f, 100.0f);
        shaderSetMat4(shaderProgram, "projection", projection);
}


// Render vertex and element objects using shaders
void render(struct renderer renderer) {
        // Clear previous buffer
        glClearColor(0.07f, 0.3f, 0.7f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Attach shader program
        shaderUse(renderer.shaderProgram);

        // Transform vertices
        transform(renderer.shaderProgram);

        // Bind Textures
        bindTextures(renderer.textures);

        // Attach Vertex Array and Element Buffer
        glBindVertexArray(renderer.VAO);
        // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer.EBO);

        // Draw elements as triangles
        // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

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

        for (uint i = 0; i < 10; i++) {
                mat4s model = mat4_identity();
                model = glms_translate(model, cubePositions[i]);
                float angle = 20.0f * i;
                model = glms_rotate(model, (float)glfwGetTime() + glm_rad(angle),
                                (vec3s){1.0f, 0.3f, 0.5f});
                shaderSetMat4(renderer.shaderProgram, "model", model);
                glDrawArrays(GL_TRIANGLES, 0, 36);
        }
}

