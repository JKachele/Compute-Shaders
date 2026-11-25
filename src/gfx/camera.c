/*************************************************
 *File----------src/gfx/camera.c
 *Project-------Compute-Shaders
 *Author--------Justin Kachele
 *Created-------Monday Nov 16, 2026 11:09:37 EST
 *License-------GNU GPL-3.0
 ************************************************/

#include "camera.h"

static vec4s getViewSize(float width, float height) {
        float halfWidth  = width / 2;
        float halfHeight = height / 2;
        return (vec4s){-halfWidth, halfWidth, -halfHeight, halfHeight};
}

struct camera cam_init(float width, float height) {
        struct camera cam;
        cam.winWidth   = width;
        cam.winHeight  = height;
        cam.viewWidth  = width;
        cam.viewHeight = height;
        cam.viewSize = getViewSize(width, height);
        return cam;
}

void cam_update(struct camera *cam, float viewWidth, float viewHeight,
                float winWidth, float winHeight) {
        cam->winHeight  = winHeight;
        cam->winWidth   = winWidth;
        cam->viewHeight = viewHeight;
        cam->viewWidth  = viewWidth;
        cam->viewSize   = getViewSize(viewWidth, viewHeight);
}

mat4s cam_projection(struct camera cam) {
        return glms_ortho(cam.viewSize.x, cam.viewSize.y, cam.viewSize.z, cam.viewSize.w, -1.0, 1.0);
}

