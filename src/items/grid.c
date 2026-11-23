/*************************************************
 *File----------src/items/grid.c
 *Project-------Compute-Shaders
 *Author--------Justin Kachele
 *Created-------Tuesday Nov 17, 2026 07:34:55 EST
 *License-------GNU GPL-3.0
 ************************************************/

#include "grid.h"

static const char *VERTEX_SHADER = "assets/shaders/grid/grid.vs.glsl";
static const char *FRAGMENT_SHADER = "assets/shaders/grid/grid.fs.glsl";

static void renderGrid(struct item self, struct camera cam);

struct item item_gridInit(void) {
        struct item grid = {0};

        uint VAO;
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);
        grid.VAO = VAO;

        grid.hasEBO = false;
        grid.mode = GL_TRIANGLES;
        grid.count = 36;
        grid.updateFn = NULL;
        grid.renderFn = renderGrid;

        // Compile and link shader programs
        struct shader shader = shader_init();
        shader_add(&shader, VERTEX_SHADER, GL_VERTEX_SHADER);
        shader_add(&shader, FRAGMENT_SHADER, GL_FRAGMENT_SHADER);
        shader_link(&shader);
        grid.shader = shader;

        return grid;
}

static void renderGrid(struct item self, struct camera cam) {
        shader_setVec3(self.shader, "cameraPos", cam.pos);

        item_render(self, cam);
}

