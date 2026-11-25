/*************************************************
 *File----------src/items/particle.c
 *Project-------Compute-Shaders
 *Author--------Justin Kachele
 *Created-------Friday Nov 20, 2026 10:53:19 EST
 *License-------GNU GPL-3.0
 ************************************************/

#include "particle.h"
#include "../compute/computeShader.h"
#include "cglm/struct/mat4.h"

static const char *COMPUTE_SHADER = "assets/shaders/particles/part.cs.glsl";

static float randf(float min, float max) {
        float r = (float)rand() / (float)RAND_MAX;
        r *= max - min;
        r += min;
        return r;
}

static void calcPositions(vec4s pos[], vec4s vel[], ivec3s numParticles) {
        srand(glfwGetTime());

        vec4s p = {0.0f};
        int particleIndex = 0;
        for (int x = 0; x < numParticles.x; x++) {
                for (int y = 0; y < numParticles.y; y++) {
                        for (int z = 0; z < numParticles.z; z++) {
                                p.x = randf(-1.0, 1.0);
                                p.y = randf(-1.0, 1.0);
                                p.z = randf(-1.0, 1.0);
                                p.w = 1.0;
                                pos[particleIndex] = p;
                                vel[particleIndex] = vec4_zero();
                                particleIndex++;
                        }
                }
        }
}

static struct item BlackHolesInit(void) {
        struct item bh = {0};

        float vertices[] = {0, 0, 0};

        uint VAO;
        glGenVertexArrays(1, &VAO);
        bh.VAO = VAO;

        // Allocate GPU Memory to store vertices
        uint VBO;
        glGenBuffers(1, &VBO);

        // Bind VAO to store vertex buffers and attrib pointers
        glBindVertexArray(VAO);

        // Bind the VBO Buffer to the GL_ARRAY_BUFFER object and copy vertex data to buffer
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        // Tell openGL how to interpret the vertex data and enable the attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(0);

        // Unbind VAO so other VAO calls wint modify this VAO
        glBindVertexArray(0);

        bh.hasEBO = false;
        bh.mode = GL_POINTS;
        bh.count = 1;
        bh.renderFn = item_render;

        return bh;
}

static void updateBlackHoles(struct particleData *data, float dt) {
        float angle = data->bhSpeed * dt;
        mat4s rotate = glms_rotate(mat4_identity(), glm_rad(angle), (vec3s){0, 0, 1});
        data->bhPos[0] = mat4_mulv3(rotate, data->bhPos[0], 1);
        data->bhPos[1] = mat4_mulv3(rotate, data->bhPos[1], 1);
        shader_setVec3(data->computeShader, "blackHolePos1", data->bhPos[0]);
        shader_setVec3(data->computeShader, "blackHolePos2", data->bhPos[1]);
        shader_setFloat(data->computeShader, "blackHoleGrav1", data->bhGrav[0]);
        shader_setFloat(data->computeShader, "blackHoleGrav2", data->bhGrav[1]);
        shader_setFloat(data->computeShader, "deltaT", dt);

        struct item bh = data->blackHoles;
        bh.pos[0] = data->bhPos[0];
        bh.pos[1] = data->bhPos[1];
}

static void updateParticles(struct item *self, float dt) {
        struct particleData *parts = (struct particleData*)self->data;
        updateBlackHoles(parts, dt);

        // Execute compute shader
        ivec3s groups = {parts->count.x/8, parts->count.y/8, parts->count.z/8};
        cs_execute(parts->computeShader, groups, GL_SHADER_STORAGE_BARRIER_BIT);
}

static void renderParticles(struct item self, struct camera cam) {
        struct particleData parts = *(struct particleData*)self.data;

        // Render particles
        shader_use(self.shader);
        shader_setVec4(self.shader, "color", (vec4s){0.0, 0.0, 0.0, 1.0});
        glPointSize(parts.pointSize);
        item_render(self, cam);

        // Render black holes
        shader_setVec4(self.shader, "color", (vec4s){1.0, 0.0, 0.0, 1.0});
        glPointSize(parts.bhPointSize);
        item_render(parts.blackHoles, cam);
}

struct item item_particleInit(void) {
        struct item particle = {0};
        struct particleData *partData = malloc(sizeof(struct particleData));
        particle.data = partData;

        int numP = 64;
        partData->count = (ivec3s){numP, numP, numP};
        partData->total = partData->count.x * partData->count.y * partData->count.z;

        GLuint bufSize = partData->total * sizeof(vec4s);

        partData->pos = malloc(bufSize);
        partData->vel = malloc(bufSize);
        calcPositions(partData->pos, partData->vel, partData->count);

        uint posBuf;
        glCreateBuffers(1, &posBuf);
        partData->posBuf = posBuf;
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, posBuf);
        glBufferData(GL_SHADER_STORAGE_BUFFER, bufSize,
                        partData->pos[0].raw, GL_DYNAMIC_DRAW);

        uint velBuf;
        glCreateBuffers(1, &velBuf);
        partData->velBuf = velBuf;
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, velBuf);
        glBufferData(GL_SHADER_STORAGE_BUFFER, bufSize,
                        partData->vel[0].raw, GL_DYNAMIC_DRAW);

        // Create a vertex array object to store vertex attributes and buffer objects
        uint VAO;
        glGenVertexArrays(1, &VAO);
        particle.VAO = VAO;

        // Bind VAO to store vertex buffers and attrib pointers
        glBindVertexArray(VAO);

        // Bind the VBO Buffer to the GL_ARRAY_BUFFER object and copy vertex data to buffer
        glBindBuffer(GL_ARRAY_BUFFER, posBuf);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(0);

        // Unbind VAO so other VAO calls wint modify this VAO
        glBindVertexArray(0);

        // Set item rendering properties
        particle.hasEBO = false;
        particle.mode = GL_POINTS;
        particle.count = partData->total;
        particle.updateFn = updateParticles;
        particle.renderFn = renderParticles;
        partData->pointSize = 2.0f;

        // Compile and link compute shader
        partData->computeShader = cs_init(COMPUTE_SHADER);

        // Compile and link shader programs
        struct shader shader = shader_init();
        shader_add(&shader, SHADER_VERT, GL_VERTEX_SHADER);
        shader_add(&shader, SHADER_FRAG_COL, GL_FRAGMENT_SHADER);
        shader_link(&shader);
        particle.shader = shader;

        // Create black holes
        partData->bhPos[0] = (vec3s){ 3, 0, 0};
        partData->bhPos[1] = (vec3s){-3, 0, 0};
        partData->bhGrav[0] = 10.0f;
        partData->bhGrav[1] = 10.0f;
        partData->bhSpeed = 35.0f;
        partData->bhPointSize = 10.0f;
        partData->blackHoles = BlackHolesInit();
        partData->blackHoles.shader = particle.shader;
        item_setDefault(&partData->blackHoles);
        item_setNumCopies(&partData->blackHoles, 2);
        item_setState(&partData->blackHoles, 0, partData->bhPos[0], 1.0f, 0, vec3_zero());
        item_setState(&partData->blackHoles, 1, partData->bhPos[1], 1.0f, 0, vec3_zero());

        return particle;
}

