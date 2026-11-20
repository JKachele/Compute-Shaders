/*************************************************
 *File----------src/items/item.h
 *Project-------Compute-Shaders
 *Author--------Justin Kachele
 *Created-------Monday Nov 16, 2026 15:26:59 EST
 *License-------GNU GPL-3.0
 ************************************************/
#ifndef ITEM_H
#define ITEM_H

#include "../util.h"
#include "../gfx/shader.h"
#include "../gfx/camera.h"
#include "../gfx/texture.h"


struct item {
        uint VAO;
        uint EBO;
        bool hasEBO;
        GLenum mode;    // GL_POINTS, GL_LINES, GL_TRIANGLES
        uint count;     // Num of verticies / indices

        struct shader shader;
        struct texture textures;

        int numCopies;
        vec3s *pos;
        float *size;
        float *angle;
        vec3s *axis;

        void *data;     // Any item type specific data

        void (*renderFn)(struct item, struct camera);
};


void itemSetNumCopies(struct item *self, int copies);
void itemSetState(struct item *self, int index, vec3s pos, float scale, float angle, vec3s axis);
void itemSetDefault(struct item *self);
void renderItem(struct item self, struct camera cam);

#endif

