/*************************************************
 *File----------src/gfx/shader.h
 *Project-------Compute-Shaders
 *Author--------Justin Kachele
 *Created-------Wednesday Nov 11, 2026 14:27:34 EST
 *License-------GNU GPL-3.0
 ************************************************/
#ifndef SHADER_H
#define SHADER_H

#include "../util.h"

#define SHADER_VERT     "assets/shaders/vert.vs.glsl"
#define SHADER_FRAG_TEX "assets/shaders/tex.fs.glsl"
#define SHADER_FRAG_COL "assets/shaders/color.fs.glsl"

struct shader {
        int numObjects;
        int objectCap;
        uint *objects;
        uint program;
};

struct shader shader_init(void);
void shader_add(struct shader *self, const char *path, GLenum type);
void shader_link(struct shader *self);
void shader_use(struct shader self);
void shader_destroy(struct shader self);

void shader_setBool(struct shader self, const char *name, bool value);
void shader_setInt(struct shader self, const char *name, int value);
void shader_setFloat(struct shader self, const char *name, float value);
void shader_setVec3(struct shader self, const char *name, vec3s vec);
void shader_setVec4(struct shader self, const char *name, vec4s vec);
void shader_setMat4(struct shader self, const char *name, mat4s mat);

#endif

