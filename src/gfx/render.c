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
#include "../items/particle.h"
#include "../items/slime.h"

// Create all vertex and element objects and compile shaders
struct renderer render_init(float winWidth, float winHeight) {
        struct renderer renderer;

        renderer.numItems = 1;
        renderer.items = calloc(renderer.numItems, sizeof(struct item));

        renderer.items[0] = item_slimeInit();
        item_setDefault(&renderer.items[0]);

        // Initialize and generate font
        renderer.font = font_init();

        // Enable Z-Buffer for Depth Testing
        // glEnable(GL_DEPTH_TEST);

        // Enable blending
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // Initalize camera
        renderer.camera = cam_init(winWidth, winHeight);

        return renderer;
}

// Calculates moving average
static float calcFPS(float dt) {
        static float buffer[100] = {0};
        static int index = 0;
        static int count = 0;
        static float sum = 0;

        float curFPS = 1.0 / dt;

        sum -= buffer[index];
        sum += curFPS;
        buffer[index] = curFPS;
        index = (index + 1) % 100;

        if (count < 100) {
                count++;
        }

        return sum / count;
}

void render_update(struct renderer *self, float dt) {
        for (int i = 0; i < self->numItems; i++) {
                if (self->items[i].updateFn != NULL) {
                        self->items[i].updateFn(&self->items[i], self->camera, dt);
                }
        }
        self->fps = calcFPS(dt);
}

// Render vertex and element objects using shaders
void render(struct renderer self) {
        // Clear previous buffer
        // glClearColor(0.07f, 0.3f, 0.7f, 1.0f);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Render items
        for (int i = 0; i < self.numItems; i++) {
                if (self.items[i].renderFn != NULL) {
                        self.items[i].renderFn(self.items[i], self.camera);
                }
        }

        // Render FPS
        // char fps[10] = {0};
        // sprintf(fps, "%.0f FPS", self.fps);
        // font_render(self.font, fps, 10.0f, 10.0f, 15, (vec3s){1.0, 1.0, 1.0},
        //                 self.camera.winWidth, self.camera.winHeight);
}

