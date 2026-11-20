/*************************************************
 *File----------src/items/particle.c
 *Project-------Compute-Shaders
 *Author--------Justin Kachele
 *Created-------Friday Nov 20, 2026 10:53:19 EST
 *License-------GNU GPL-3.0
 ************************************************/

#include "particle.h"

static const char *VERTEX_SHADER = "assets/shaders/vert.vs.glsl";
static const char *FRAGMENT_SHADER = "assets/shaders/color.fs.glsl";
static const char *COMPUTE_SHADER = "assets/shaders/particles/part.cs.glsl";

static void calcPositions(vec4s particles[], ivec3s numParticles) {
        vec4s p = {0.0f};

        float dx = 2.0f / numParticles.x;
        float dy = 2.0f / numParticles.y;
        float dz = 2.0f / numParticles.z;

        mat4s translation = mat4_identity();
        translation = glms_translate(translation, (vec3s){-1.0, -1.0, -1.0});

        int particleIndex = 0;
        for (int x = 0; x < numParticles.x; x++) {
                for (int y = 0; y < numParticles.y; y++) {
                        for (int z = 0; z < numParticles.z; z++) {
                                p.x = dx * x;
                                p.y = dy * y;
                                p.z = dz * z;
                                p.w = 1.0;
                                p = mat4_mulv(translation, p);
                                particles[particleIndex++] = p;
                        }
                }
        }
}

struct item particleInit(void) {
        struct item particle = {0};
        struct particles *particles = malloc(sizeof(struct particles));
        particle.data = particles;

        particles->count = (ivec3s){64, 64, 64};
        particles->total = particles->count.x * particles->count.y * particles->count.z;

        GLuint bufSize = particles->total * sizeof(vec4s);

        particles->positions = malloc(bufSize);
        calcPositions(particles->positions, particles->count);

        uint posBuf;
        glCreateBuffers(1, &posBuf);
        particles->posBuf = posBuf;

        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, posBuf);
        glBufferData(GL_SHADER_STORAGE_BUFFER, bufSize,
                        particles->positions[0].raw, GL_DYNAMIC_DRAW);

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

        particle.hasEBO = false;
        particle.mode = GL_POINTS;
        particle.count = particles->total;
        particle.renderFn = renderParticles;

        struct shader computeShader = shaderInit();
        shaderAdd(&computeShader, COMPUTE_SHADER, GL_COMPUTE_SHADER);
        shaderLink(&computeShader);
        particles->computeShader = computeShader;

        // Compile and link shader programs
        struct shader shader = shaderInit();
        shaderAdd(&shader, VERTEX_SHADER, GL_VERTEX_SHADER);
        shaderAdd(&shader, FRAGMENT_SHADER, GL_FRAGMENT_SHADER);
        shaderLink(&shader);
        particle.shader = shader;

        return particle;
}

static void executeComputeShader(struct particles parts) {
        shaderUse(parts.computeShader);
        glDispatchCompute(parts.count.x/8, parts.count.y/8, parts.count.z/8);
        glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
}

void renderParticles(struct item self, struct camera cam) {
        struct particles parts = *(struct particles*)self.data;
        executeComputeShader(parts);

        shaderUse(self.shader);
        shaderSetVec4(self.shader, "color", (vec4s){0.0, 0.0, 0.0, 1.0});

        glPointSize(2.0f);

        renderItem(self, cam);
}

