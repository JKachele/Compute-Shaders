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

void renderGrid(struct item item, struct camera cam) {
        shaderUse(item.shaderProgram);
        shaderSetVec3(item.shaderProgram, "cameraPos", cam.pos);

        renderItem(item, cam);
}

