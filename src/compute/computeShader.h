/*************************************************
 *File----------src/compute/computeShader.h
 *Project-------Compute-Shaders
 *Author--------Justin Kachele
 *Created-------Monday Nov 23, 2026 09:10:32 EST
 *License-------GNU GPL-3.0
 ************************************************/
#ifndef COMPUTESHADER_H
#define COMPUTESHADER_H

#include "../util.h"
#include "../gfx/shader.h"

struct shader cs_init(const char *shaderPath);
void cs_execute(struct shader cs, ivec3s groups, GLbitfield barriers);
void cs_getBufferData(uint buffer, GLenum bufType, uint offset, uint size, void *output);

#endif

