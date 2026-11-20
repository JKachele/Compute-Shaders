/*************************************************
 *File----------src/items/grid.c
 *Project-------Compute-Shaders
 *Author--------Justin Kachele
 *Created-------Tuesday Nov 17, 2026 07:34:55 EST
 *License-------GNU GPL-3.0
 ************************************************/

#include "grid.h"

static const char *VERTEX_SHADER = "assets/shaders/vertGrid.glsl";
static const char *FRAGMENT_SHADER = "assets/shaders/fragGrid.glsl";

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
        uint shaderProgram = shaderInit(VERTEX_SHADER, FRAGMENT_SHADER);
        shaderUse(shaderProgram);
        grid.shaderProgram = shaderProgram;

        return grid;
}

void renderGrid(struct item self, struct camera cam) {
        shaderUse(self.shaderProgram);
        shaderSetVec3(self.shaderProgram, "cameraPos", cam.pos);

        renderItem(self, cam);
}

