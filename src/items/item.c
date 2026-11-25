/*************************************************
 *File----------src/items/item.c
 *Project-------Compute-Shaders
 *Author--------Justin Kachele
 *Created-------Monday Nov 16, 2026 15:44:03 EST
 *License-------GNU GPL-3.0
 ************************************************/

#include "item.h"

void item_setNumCopies(struct item *self, int copies) {
        self->numCopies = copies;
        self->pos    = malloc(copies * sizeof(vec3s));
        self->scale   = malloc(copies * sizeof(float));
        self->angle  = malloc(copies * sizeof(float));
        self->axis = malloc(copies * sizeof(vec3s));
}

void item_setState(struct item *self, int index, vec3s pos, vec3s scale, float angle, vec3s axis) {
        self->pos[index] = pos;
        self->scale[index] = scale;
        self->angle[index] = angle;
        self->axis[index] = axis;
}

void item_setDefault(struct item *self) {
        item_setNumCopies(self, 1);
        item_setState(self, 0, vec3_zero(), vec3_one(), 0.0f, vec3_zero());
}

static void transform(struct item item, int index, struct camera cam) {
        // Model Matrix
        mat4s model = mat4_identity();
        model = glms_scale(model, item.scale[index]);
        model = glms_translate(model, item.pos[index]);
        float angle = item.angle[index];
        model = glms_rotate(model, glm_rad(angle), item.axis[index]);
        shader_setMat4(item.shader, "model", model);

        // Projection Matrix
        mat4s projection = cam_projection(cam);
        shader_setMat4(item.shader, "projection", projection);
}

void item_render(struct item self, struct camera cam) {
        // Attach shader program
        shader_use(self.shader);

        // Bind Textures
        tex_bind(self.textures);

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

