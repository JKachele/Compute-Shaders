/*************************************************
 *File----------src/items/particle.c
 *Project-------Compute-Shaders
 *Author--------Justin Kachele
 *Created-------Friday Nov 20, 2026 10:53:19 EST
 *License-------GNU GPL-3.0
 ************************************************/

#include "particle.h"
#include "../compute/computeShader.h"

static const char *VERTEX_SHADER = "assets/shaders/vert.vs.glsl";
static const char *FRAGMENT_SHADER = "assets/shaders/color.fs.glsl";
static const char *COMPUTE_SHADER = "assets/shaders/particles/part.cs.glsl";

static void renderParticles(struct item self, struct camera cam);

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

struct item item_particleInit(void) {
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

        // Set item rendering properties
        particle.hasEBO = false;
        particle.mode = GL_POINTS;
        particle.count = particles->total;
        particle.renderFn = renderParticles;

        // Compile and link compute shader
        particles->computeShader = cs_init(COMPUTE_SHADER);

        // Compile and link shader programs
        struct shader shader = shader_init();
        shader_add(&shader, VERTEX_SHADER, GL_VERTEX_SHADER);
        shader_add(&shader, FRAGMENT_SHADER, GL_FRAGMENT_SHADER);
        shader_link(&shader);
        particle.shader = shader;

        return particle;
}

static void renderParticles(struct item self, struct camera cam) {
        struct particles parts = *(struct particles*)self.data;
        ivec3s groups = {parts.count.x/8, parts.count.y/8, parts.count.z/8};
        cs_execute(parts.computeShader, groups, GL_SHADER_STORAGE_BARRIER_BIT);

        shader_use(self.shader);
        shader_setVec4(self.shader, "color", (vec4s){0.0, 0.0, 0.0, 1.0});

        glPointSize(2.0f);

        item_render(self, cam);
}

