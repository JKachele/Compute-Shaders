/*************************************************
 *File----------src/gfx/render.c
 *Project-------Compute-Shaders
 *Author--------Justin Kachele
 *Created-------Wednesday Nov 11, 2026 14:15:06 EST
 *License-------GNU GPL-3.0
 ************************************************/

#include "render.h"
#include "shader.h"
#include "texture.h"

// Create all vertex and element objects and compile shaders
struct renderer renderInit(void) {
        struct renderer renderer;

        // Create 10 boxes
        const vec3s cubePositions[] = {
                (vec3s){ 0.0f,  0.0f,  0.0f},
                (vec3s){ 2.0f,  5.0f, -15.0f},
                (vec3s){-1.5f, -2.2f, -2.5f},
                (vec3s){-3.8f, -2.0f, -12.3f},
                (vec3s){ 2.4f, -0.4f, -3.5f},
                (vec3s){-1.7f,  3.0f, -7.5f},
                (vec3s){ 1.3f, -2.0f, -2.5f},
                (vec3s){ 1.5f,  2.0f, -2.5f},
                (vec3s){ 1.5f,  0.2f, -1.5f},
                (vec3s){-1.3f,  1.0f, -1.5f}
        };
        renderer.numItems = 1;
        renderer.items = malloc(renderer.numItems * sizeof(struct item));
        renderer.items[0] = boxInit();
        itemSetNumCopies(&renderer.items[0], 10);
        for (int i = 0; i < 10; i++) {
                struct item *box = &renderer.items[0];
                box->pos[i] = cubePositions[i];
                box->size[i] = 1.0f;
                box->angle[i] = 20.0f * i;
                box->rotate[i] = (vec3s){1.0f, 0.3f, 0.5f};
        }

        // Enable Z-Buffer for Depth Testing
        glEnable(GL_DEPTH_TEST);

        // Initalize camera
        renderer.camera = cameraInit();

        return renderer;
}

// Render vertex and element objects using shaders
void render(struct renderer renderer) {
        // Clear previous buffer
        glClearColor(0.07f, 0.3f, 0.7f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        renderItem(renderer.items[0], renderer.camera);
}

