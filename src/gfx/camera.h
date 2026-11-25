/*************************************************
 *File----------src/gfx/camera.h
 *Project-------Compute-Shaders
 *Author--------Justin Kachele
 *Created-------Monday Nov 16, 2026 11:09:40 EST
 *License-------GNU GPL-3.0
 ************************************************/
#ifndef CAMERA_H
#define CAMERA_H

#include "../util.h"

struct camera {
        float winWidth;
        float winHeight;
        float viewWidth;
        float viewHeight;
        vec4s viewSize;
};


struct camera cam_init(float width, float height);
void cam_update(struct camera *cam, float viewWidth, float viewHeight,
                float winWidth, float winHeight);
mat4s cam_lookAt(struct camera cam);
mat4s cam_projection(struct camera cam);

#endif

