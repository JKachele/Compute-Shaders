/*************************************************
 *File----------src/items/item.c
 *Project-------Compute-Shaders
 *Author--------Justin Kachele
 *Created-------Monday Nov 16, 2026 15:44:03 EST
 *License-------GNU GPL-3.0
 ************************************************/

#include "item.h"

void itemSetNumCopies(struct item *item, int copies) {
        item->numCopies = copies;
        item->pos    = malloc(copies * sizeof(vec3s));
        item->size   = malloc(copies * sizeof(float));
        item->angle  = malloc(copies * sizeof(float));
        item->axis = malloc(copies * sizeof(vec3s));
}

void itemSetState(struct item *item, int index, vec3s pos, float scale, float angle, vec3s axis) {
        item->pos[index] = pos;
        item->size[index] = scale;
        item->angle[index] = angle;
        item->axis[index] = axis;
}

static void transform(struct item item, int index, struct camera cam) {
        // Model Matrix
        mat4s model = mat4_identity();
        model = glms_scale_uni(model, item.size[index]);
        model = glms_translate(model, item.pos[index]);
        float angle = item.angle[index];
        model = glms_rotate(model, glm_rad(angle), item.axis[index]);
        shaderSetMat4(item.shaderProgram, "model", model);

        // View Matrix
        mat4s view = cameraLookAt(cam);
        shaderSetMat4(item.shaderProgram, "view", view);

        // Projection Matrix
        mat4s projection = cameraProjection(cam);
        shaderSetMat4(item.shaderProgram, "projection", projection);
}

void renderItem(struct item self, struct camera cam) {
        // Attach shader program
        shaderUse(self.shaderProgram);

        // Bind Textures
        bindTextures(self.textures);

        // Attach Vertex Array and Element Buffer
        glBindVertexArray(self.VAO);

        for (int i = 0; i < self.numCopies; i++) {
                // Transform verticies
                transform(self, i, cam);
                // Draw box
                if (self.hasEBO) {
                        glDrawElements(self.mode, self.count, GL_UNSIGNED_INT, 0);
                } else {
                        glDrawArrays(self.mode, 0, self.count);
                }
        }
}

