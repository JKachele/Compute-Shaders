/*************************************************
 *File----------src/gfx/render.h
 *Project-------Compute-Shaders
 *Author--------Justin Kachele
 *Created-------Wednesday Nov 11, 2026 14:15:08 EST
 *License-------GNU GPL-3.0
 ************************************************/
#ifndef RENDER_H
#define RENDER_H

#include "../util.h"
#include "camera.h"
#include "font.h"
#include "../items/item.h"
#include "../items/box.h"
#include "../items/grid.h"
#include "../items/particle.h"

struct renderer {
        struct camera camera;
        int numItems;
        struct item *items;
        struct font font;
        float fps;
};

struct renderer render_init(void);
void render_update(struct renderer *self, float dt);
void render(struct renderer self);

#endif

