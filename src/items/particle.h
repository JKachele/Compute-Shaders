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

struct particleData {
        ivec3s count;
        int total;
        vec4s *pos;
        vec4s *vel;
        float pointSize;
        uint posBuf;
        uint velBuf;

        struct shader computeShader;

        vec3s bhPos[2];
        float bhGrav[2];
        float bhSpeed;
        float bhPointSize;
        struct item blackHoles;
};

struct item item_particleInit(void);

#endif

