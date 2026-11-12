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

struct renderer {
        uint shaderProgram;
        uint VAO;
        uint EBO;
};

struct renderer render(void);

#endif

