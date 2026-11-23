/*************************************************
 *File----------src/gfx/render.c
 *Project-------Compute-Shaders
 *Author--------Justin Kachele
 *Created-------Wednesday Nov 11, 2026 14:15:06 EST
 *License-------GNU GPL-3.0
 ************************************************/

#include "render.h"
#include "font.h"
#include "shader.h"
#include "texture.h"

// Create all vertex and element objects and compile shaders
struct renderer render_init(void) {
        struct renderer renderer;

        // Create 10 boxes
        // const vec3s cubePositions[] = {
        //         (vec3s){ 0.0f,  0.0f,  0.0f},
        //         (vec3s){ 2.0f,  5.0f, -15.0f},
        //         (vec3s){-1.5f, -2.2f, -2.5f},
        //         (vec3s){-3.8f, -2.0f, -12.3f},
        //         (vec3s){ 2.4f, -0.4f, -3.5f},
        //         (vec3s){-1.7f,  3.0f, -7.5f},
        //         (vec3s){ 1.3f, -2.0f, -2.5f},
        //         (vec3s){ 1.5f,  2.0f, -2.5f},
        //         (vec3s){ 1.5f,  0.2f, -1.5f},
        //         (vec3s){-1.3f,  1.0f, -1.5f}
        // };
        renderer.numItems = 2;
        renderer.items = malloc(renderer.numItems * sizeof(struct item));

        renderer.items[0] = item_gridInit();
        item_setDefault(&renderer.items[0]);

        renderer.items[1] = item_particleInit();
        item_setDefault(&renderer.items[1]);

        // renderer.items[2] = item_boxInit();
        // item_setNumCopies(&renderer.items[2], 10);
        // for (int i = 0; i < 10; i++) {
        //         struct item *box = &renderer.items[2];
        //         item_setState(box, i, cubePositions[i], 1.0f, 20.0f*i, (vec3s){1.0f, 0.3f, 0.5f});
        // }

        // Initialize and generate font
        renderer.font = font_init();

        // Enable Z-Buffer for Depth Testing
        glEnable(GL_DEPTH_TEST);

        // Enable blending
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // Initalize camera
        renderer.camera = cam_init();

        return renderer;
}

void render_update(struct renderer *self, float dt) {
        for (int i = 0; i < self->numItems; i++) {
                if (self->items[i].updateFn != NULL) {
                        self->items[i].updateFn(&self->items[i], dt);
                }
        }
        self->fps = 1.0f / dt;
}

static void render_fps(struct renderer self) {
        char fps[10] = {0};
        sprintf(fps, "%.0f FPS", self.fps);
        font_render(self.font, fps, 25.0f, 25.0f, 0.5f, (vec3s){1.0, 1.0, 1.0},
                        self.camera.winWidth, self.camera.winHeight);
}

// Render vertex and element objects using shaders
void render(struct renderer self) {
        // Clear previous buffer
        glClearColor(0.07f, 0.3f, 0.7f, 1.0f);
        // glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        for (int i = 0; i < self.numItems; i++) {
                if (self.items[i].renderFn != NULL) {
                        self.items[i].renderFn(self.items[i], self.camera);
                }
        }

        render_fps(self);
}

