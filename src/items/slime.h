/*************************************************
 *File----------src/items/slime.h
 *Project-------Compute-Shaders
 *Author--------Justin Kachele
 *Created-------Wednesday Nov 25, 2026 11:12:54 EST
 *License-------GNU GPL-3.0
 ************************************************/
#ifndef SLIME_H
#define SLIME_H

#include "item.h"

struct slimeData {
        int count;
        vec4s *pos;
        float speed;
        float pointSize;
        uint posBuf;

        uint tempTexture;
        float diffuseRate;
        float decayRate;

        struct shader slimeCS;
        struct shader textureCS;

        struct item quad;
};

struct item item_slimeInit(void);

#endif

