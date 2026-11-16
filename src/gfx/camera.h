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
        vec3s pos;
        vec3s front;
        vec3s up;
        float speed;
        float pitch;
        float yaw;
};


struct camera cameraInit(void);
void cameraSet(struct camera *cam, vec3s pos, vec3s front, vec3s up, float speed);
void cameraMove(struct camera *cam, direction dir);
void cameraUpdateAngle(struct camera *cam, float yaw, float pitch);
mat4s camLookAt(struct camera cam);

#endif

