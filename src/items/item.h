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
        uint shaderProgram;
        uint VAO;
        uint EBO;
        struct texture textures;

        int numItems;
        vec3s *pos;
        float *size;
        float *angle;
        vec3s *rotate;
};


void itemSetNumCopies(struct item *item, int copies);
void renderItem(struct item item, struct camera cam);

#endif

