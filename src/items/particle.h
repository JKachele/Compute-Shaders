/*************************************************
 *File----------src/items/particle.h
 *Project-------Compute-Shaders
 *Author--------Justin Kachele
 *Created-------Friday Nov 20, 2026 10:53:22 EST
 *License-------GNU GPL-3.0
 ************************************************/
#ifndef PARTICLE_H
#define PARTICLE_H

#include "item.h"

struct particles {
        ivec3s count;
        int total;
        vec4s *positions;
        uint posBuf;

        struct shader computeShader;
};

struct item particleInit(void);
void renderParticles(struct item self, struct camera cam);

#endif

