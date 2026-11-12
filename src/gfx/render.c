/*************************************************
 *File----------src/gfx/render.c
 *Project-------Compute-Shaders
 *Author--------Justin Kachele
 *Created-------Wednesday Nov 11, 2026 14:15:06 EST
 *License-------GNU GPL-3.0
 ************************************************/

#include "render.h"
#include "shader.h"

// Create all vertex and element objects and compile shaders
struct renderer renderInit(void) {
        float vertices[] = {
                //X      Y      Z        R     G     B
                 0.5f,  0.5f,  0.0f,    1.0f, 1.0f, 1.0f,       // Top Right
                 0.5f, -0.5f,  0.0f,    0.0f, 0.0f, 1.0f,       // Bottom Right
                -0.5f, -0.5f,  0.0f,    0.0f, 1.0f, 0.0f,       // Bottom Left
                -0.5f,  0.5f,  0.0f,    1.0f, 0.0f, 0.0f        // Top Left
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
                        6 * sizeof(float),      // Stride (Distance between vertex attributes)
                        (void*)0);              // Offset of data in buffer
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(3*sizeof(float)));
        glEnableVertexAttribArray(1);

        // Unbind VAO so other VAO calls wint modify this VAO
        glBindVertexArray(0);

        // Compile and link shader programs
        uint shaderProgram = shaderInit();

        shaderUse(shaderProgram);

        struct renderer renderer = {.shaderProgram = shaderProgram, .VAO = VAO, .EBO = EBO};
        return renderer;
}

static void transform(uint shaderProgram) {
        // Model Matrix
        mat4s model = mat4_identity();
        model = glms_rotate(model, glm_rad(-55.0f), (vec3s){1.0f, 0.0f, 0.0f});
        model = glms_rotate(model, (float)glfwGetTime(), (vec3s){0.0f, 0.0f, 1.0f});

        // View Matrix
        mat4s view = mat4_identity();
        view = glms_translate(view, (vec3s){0.0f, 0.0f, -3.0f});

        // Projection Matrix
        mat4s projection;
        projection = glms_perspective(glm_rad(45.0f), 800.0f/600.0f, 0.1f, 100.0f);

        uint modelLoc = glGetUniformLocation(shaderProgram, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, (const GLfloat*)&model.raw);

        uint viewLoc = glGetUniformLocation(shaderProgram, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, (const GLfloat*)&view.raw);

        uint projectionLoc = glGetUniformLocation(shaderProgram, "projection");
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, (const GLfloat*)&projection.raw);
}


// Render vertex and element objects using shaders
void render(struct renderer renderer) {
        // Attach shader program
        shaderUse(renderer.shaderProgram);

        // Transform vertices
        transform(renderer.shaderProgram);

        // Attach Vertex Array and Element Buffer
        glBindVertexArray(renderer.VAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer.EBO);

        // Draw elements as triangles
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

