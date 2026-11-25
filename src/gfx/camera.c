/*************************************************
 *File----------src/gfx/camera.c
 *Project-------Compute-Shaders
 *Author--------Justin Kachele
 *Created-------Monday Nov 16, 2026 11:09:37 EST
 *License-------GNU GPL-3.0
 ************************************************/

#include "camera.h"

struct camera cam_init(void) {
        struct camera cam;
        cam.winWidth = 1280.0f;
        cam.winHeight = 720.0f;
        return cam;
}

void cam_update(struct camera *cam, float winWidth, float winHeight) {
        cam->winHeight = winHeight;
        cam->winWidth = winWidth;
}

mat4s cam_projection(struct camera cam) {
        float halfWidth = cam.winWidth / 2;
        float halfHeight = cam.winHeight / 2;
        return glms_ortho(-halfWidth, halfWidth, -halfHeight, halfHeight, -1.0, 1.0);
}

