/*************************************************
 *File----------src/compute/computeShader.c
 *Project-------Compute-Shaders
 *Author--------Justin Kachele
 *Created-------Monday Nov 23, 2026 09:10:35 EST
 *License-------GNU GPL-3.0
 ************************************************/

#include "computeShader.h"

struct shader cs_init(const char *shaderPath) {
        struct shader computeShader = shader_init();
        shader_add(&computeShader, shaderPath, GL_COMPUTE_SHADER);
        shader_link(&computeShader);
        return computeShader;
}

void cs_execute(struct shader cs, ivec3s groups, GLbitfield barriers) {
        shader_use(cs);
        glDispatchCompute(groups.x, groups.y, groups.z);
        glMemoryBarrier(barriers);
}

void cs_getBufferData(uint buffer, GLenum bufType, uint offset, uint size, void *output) {
        glBindBuffer(bufType, buffer);
        glGetBufferSubData(bufType, offset, size, output);
}

