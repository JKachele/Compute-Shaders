/*************************************************
 *File----------src/items/item.c
 *Project-------Compute-Shaders
 *Author--------Justin Kachele
 *Created-------Monday Nov 16, 2026 15:44:03 EST
 *License-------GNU GPL-3.0
 ************************************************/

#include "item.h"

void itemSetNumCopies(struct item *item, int copies) {
        item->numItems = copies;
        item->pos    = malloc(copies * sizeof(vec3s));
        item->size   = malloc(copies * sizeof(float));
        item->angle  = malloc(copies * sizeof(float));
        item->rotate = malloc(copies * sizeof(vec3s));
}

static void transform(struct item item, int index, struct camera cam) {
        // Model Matrix
        mat4s model = mat4_identity();
        model = glms_translate(model, item.pos[index]);
        float angle = item.angle[index];
        model = glms_rotate(model, glm_rad(angle), item.rotate[index]);
        shaderSetMat4(item.shaderProgram, "model", model);

        // View Matrix
        mat4s view = camLookAt(cam);
        shaderSetMat4(item.shaderProgram, "view", view);

        // Projection Matrix
        mat4s projection;
        projection = glms_perspective(glm_rad(45.0f), 800.0f/600.0f, 0.1f, 100.0f);
        shaderSetMat4(item.shaderProgram, "projection", projection);
}

void renderItem(struct item item, struct camera cam) {
        // Attach shader program
        shaderUse(item.shaderProgram);

        // Bind Textures
        bindTextures(item.textures);

        // Attach Vertex Array and Element Buffer
        glBindVertexArray(item.VAO);

        for (int i = 0; i < item.numItems; i++) {
                // Transform verticies
                transform(item, i, cam);
                // Draw box
                glDrawArrays(GL_TRIANGLES, 0, 36);
        }
}

