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

uint shaderInit(void);
void shaderUse(uint shaderProgram);
void shaderDestroy(uint shaderProgram);

void shaderSetBool(uint shaderProgram, const char *name, bool value);
void shaderSetInt(uint shaderProgram, const char *name, int value);
void shaderSetFloat(uint shaderProgram, const char *name, float value);

#endif

