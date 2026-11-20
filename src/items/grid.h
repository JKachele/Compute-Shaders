/*************************************************
 *File----------src/items/grid.h
 *Project-------Compute-Shaders
 *Author--------Justin Kachele
 *Created-------Tuesday Nov 17, 2026 07:34:58 EST
 *License-------GNU GPL-3.0
 ************************************************/
#ifndef GRID_H
#define GRID_H

#include "item.h"

struct item gridInit(void);
void renderGrid(struct item self, struct camera cam);

#endif

