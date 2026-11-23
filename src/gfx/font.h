/*************************************************
 *File----------src/gfx/font.h
 *Project-------Compute-Shaders
 *Author--------Justin Kachele
 *Created-------Monday Nov 23, 2026 12:15:59 EST
 *License-------GNU GPL-3.0
 ************************************************/
#ifndef FONT_H
#define FONT_H

#include "../util.h"
#include "shader.h"

struct glyph {
        uint textureID; // ID handle of glyph texture
        ivec2s size;    // Size of glyph
        ivec2s bearing; // Offsets from baseline to left/top of glyph
        uint advance;   // Offset to next glyph
};

struct font {
        struct glyph glyphs[128];

        uint VAO;
        uint VBO;
        struct shader shader;
};

struct font font_init(void);
void font_render(struct font self, const char *text, float x, float y, float scale, vec3s color,
                float winWidth, float winHeight);

#endif

