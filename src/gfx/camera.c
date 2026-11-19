/*************************************************
 *File----------src/gfx/camera.c
 *Project-------Compute-Shaders
 *Author--------Justin Kachele
 *Created-------Monday Nov 16, 2026 11:09:37 EST
 *License-------GNU GPL-3.0
 ************************************************/

#include "camera.h"

struct camera cameraInit(void) {
        struct camera cam;
        cam.pos = (vec3s){0.0f, 0.1f, 3.0f};
        cam.front = glms_normalize((vec3s){0.0f, 0.0f, -1.0f});
        cam.up = (vec3s){0.0f, 1.0f, 0.0f};
        cam.speed = 0.05f;
        cam.pitch = -45.0f;
        cam.yaw = -90.0f;
        cam.fov = 45.0f;
        cam.winWidth = 800.0f;
        cam.winHeight = 600.0f;
        return cam;
}

void cameraSet(struct camera *cam, vec3s pos, vec3s front, vec3s up, float speed) {
        cam->pos = pos;
        cam->front = front;
        cam->up = up;
        cam->speed = speed;
}

void cameraMove(struct camera *cam, direction dir) {
        vec3s camRight = glms_normalize(glms_cross(cam->front, cam->up));
        vec3s camUp = glms_normalize(glms_cross(cam->front, camRight));
        switch (dir) {
        case FRONT:
                cam->pos = vec3_add(cam->pos, vec3_scale(cam->front, cam->speed));
                break;
        case RIGHT:
                cam->pos = vec3_add(cam->pos, vec3_scale(camRight, cam->speed));
                break;
        case BACK:
                cam->pos = vec3_sub(cam->pos, vec3_scale(cam->front, cam->speed));
                break;
        case LEFT:
                cam->pos = vec3_sub(cam->pos, vec3_scale(camRight, cam->speed));
                break;
        case UP:
                cam->pos = vec3_sub(cam->pos, vec3_scale(camUp, cam->speed));
                break;
        case DOWN:
                cam->pos = vec3_add(cam->pos, vec3_scale(camUp, cam->speed));
                break;
        }
}

void cameraUpdateAngle(struct camera *cam, float yaw, float pitch) {
        vec3s dir;
        dir.x = cos(glm_rad(yaw)) * cos(glm_rad(pitch));
        dir.y = sin(glm_rad(pitch));
        dir.z = sin(glm_rad(yaw)) * cos(glm_rad(pitch));
        cam->front = glms_normalize(dir);
}

void cameraUpdateWindow(struct camera *cam, float winWidth, float winHeight) {
        cam->winHeight = winHeight;
        cam->winWidth = winWidth;
}

mat4s cameraLookAt(struct camera cam) {
        return glms_lookat(cam.pos, vec3_add(cam.pos, cam.front), cam.up);
}

mat4s cameraProjection(struct camera cam) {
        return glms_perspective(glm_rad(cam.fov), cam.winWidth/cam.winHeight, 0.1f, 100.0f);
}

