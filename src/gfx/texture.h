/*************************************************
 *File----------src/gfx/texture.h
 *Project-------Compute-Shaders
 *Author--------Justin Kachele
 *Created-------Thursday Nov 12, 2026 12:55:16 EST
 *License-------GNU GPL-3.0
 ************************************************/
#ifndef TEXTURE_H
#define TEXTURE_H

#include "../util.h"
#include "shader.h"

struct texture {
        int numTextures;
        uint *textures;
};

struct texture tex_init(struct shader shader);
void tex_bind(struct texture textures);

#endif

