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

struct shader {
        int numObjects;
        int objectCap;
        uint *objects;
        uint program;
};

struct shader shaderInit(void);
void shaderAdd(struct shader *self, const char *path, GLenum type);
void shaderLink(struct shader *self);
void shaderUse(struct shader self);
void shaderDestroy(struct shader self);

void shaderSetBool(struct shader self, const char *name, bool value);
void shaderSetInt(struct shader self, const char *name, int value);
void shaderSetFloat(struct shader self, const char *name, float value);
void shaderSetVec3(struct shader self, const char *name, vec3s vec);
void shaderSetVec4(struct shader self, const char *name, vec4s vec);
void shaderSetMat4(struct shader self, const char *name, mat4s mat);

#endif

