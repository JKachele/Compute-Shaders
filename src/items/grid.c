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

struct item gridInit(void) {
        struct item grid = {0};

        uint VAO;
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);
        grid.VAO = VAO;

        grid.hasEBO = false;
        grid.mode = GL_TRIANGLES;
        grid.count = 36;
        grid.renderFn = renderGrid;

        // Compile and link shader programs
        struct shader shader = shaderInit();
        shaderAdd(&shader, VERTEX_SHADER, GL_VERTEX_SHADER);
        shaderAdd(&shader, FRAGMENT_SHADER, GL_FRAGMENT_SHADER);
        shaderLink(&shader);
        grid.shader = shader;

        return grid;
}

void renderGrid(struct item self, struct camera cam) {
        shaderUse(self.shader);
        shaderSetVec3(self.shader, "cameraPos", cam.pos);

        renderItem(self, cam);
}

