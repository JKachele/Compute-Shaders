/*************************************************
 *File----------src/items/slime.c
 *Project-------Compute-Shaders
 *Author--------Justin Kachele
 *Created-------Wednesday Nov 25, 2026 11:12:51 EST
 *License-------GNU GPL-3.0
 ************************************************/

#include "slime.h"
#include "../compute/computeShader.h"
#include "GLFW/glfw3.h"

static const char *SLIME_COMP_SHDR = "assets/shaders/slime/slime.cs.glsl";
static const char *TEX_COMP_SHDR = "assets/shaders/slime/tex.cs.glsl";

uint TexWidth = 1280;
uint TexHeight = 720;

static float randf(float min, float max) {
        float r = (float)rand() / (float)RAND_MAX;
        r *= max - min;
        r += min;
        return r;
}

static void calcPositions(vec4s pos[], int numSlime, float maxDist) {
        srand(glfwGetTime());

        vec2s p = {0.0f};
        vec2s f = {0.0f};
        for (int i = 0; i < numSlime; i++) {
                float dist = sqrtf(randf(0.0, maxDist * maxDist));
                float angle = randf(0.0, PI2);
                p.x = dist * cos(angle);
                p.y = dist * sin(angle);
                pos[i] = (vec4s){p.x, p.y, angle, 0.0};
        }
}

static void blurTrails(struct slimeData *data, uint tex, float dt) {
        shader_use(data->textureCS);
        ivec3s groups = {(TexWidth+15)/16, (TexHeight+15)/16, 1};

        glBindImageTexture(0, tex, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);
        glBindImageTexture(1, data->tempTexture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
        shader_setIVec2(data->textureCS, "direction", (ivec2s){1, 0});
        shader_setFloat(data->textureCS, "deltaT", dt);
        cs_execute(data->textureCS, groups,
                        GL_SHADER_IMAGE_ACCESS_BARRIER_BIT | GL_TEXTURE_FETCH_BARRIER_BIT);

        glBindImageTexture(0, data->tempTexture, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);
        glBindImageTexture(1, tex, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
        shader_setIVec2(data->textureCS, "direction", (ivec2s){0, 1});
        cs_execute(data->textureCS, groups,
                        GL_SHADER_IMAGE_ACCESS_BARRIER_BIT | GL_TEXTURE_FETCH_BARRIER_BIT);
}

static void updateSlime(struct item *self, struct camera cam, float dt) {
        struct slimeData *data = (struct slimeData*)self->data;

        // Difuse and fade out trails
        blurTrails(data, self->textures.textures[0], dt);

        shader_use(data->slimeCS);
        shader_setFloat(data->slimeCS, "deltaT", dt);
        shader_setFloat(data->slimeCS, "time", glfwGetTime());
        shader_setFloat(data->slimeCS, "speed", data->speed);
        shader_setVec4(data->slimeCS, "winSize", cam.viewSize);
        glBindImageTexture(0, self->textures.textures[0], 0, GL_FALSE, 0,
                        GL_READ_WRITE, GL_RGBA32F);

        // Execute compute shader
        ivec3s groups = {data->count, 1, 1};
        cs_execute(data->slimeCS, groups,
                        GL_SHADER_STORAGE_BARRIER_BIT | GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

        // Update quad
        self->scale[0] = (vec3s){cam.viewWidth, cam.viewHeight, 1.0};
}

static struct texture initTexture(uint width, uint height) {
        struct texture texture;
        texture.numTextures = 1;
        texture.textures = malloc(texture.numTextures * sizeof(uint));

        uint texHandle;
        glGenTextures(1, &texHandle);
        glBindTexture(GL_TEXTURE_2D, texHandle);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);

        texture.textures[0] = texHandle;
        return texture;
}

struct item item_slimeInit(void) {
        struct item slime = {0};
        struct slimeData *data = malloc(sizeof(struct slimeData));
        slime.data = data;

        data->count = 25000;
        GLuint bufSize = data->count * sizeof(vec4s);

        data->pos = malloc(bufSize);
        calcPositions(data->pos, data->count, 100);
        data->speed = 50.0f;

        uint posBuf;
        glCreateBuffers(1, &posBuf);
        data->posBuf = posBuf;
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, posBuf);
        glBufferData(GL_SHADER_STORAGE_BUFFER, bufSize,
                        data->pos[0].raw, GL_DYNAMIC_DRAW);

        float vertices[] = {
                -0.5f, -0.5f,  0.0f, 0.0f,
                 0.5f, -0.5f,  1.0f, 0.0f,
                 0.5f,  0.5f,  1.0f, 1.0f,
                 0.5f,  0.5f,  1.0f, 1.0f,
                -0.5f,  0.5f,  0.0f, 1.0f,
                -0.5f, -0.5f,  0.0f, 0.0f
        };

        uint VAO;
        glGenVertexArrays(1, &VAO);
        slime.VAO = VAO;

        // Allocate GPU Memory to store vertices
        uint VBO;
        glGenBuffers(1, &VBO);

        // Bind VAO to store vertex buffers and attrib pointers
        glBindVertexArray(VAO);

        // Bind the VBO Buffer to the GL_ARRAY_BUFFER object and copy vertex data to buffer
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        // Tell openGL how to interpret the vertex data and enable the attribute
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), 0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), (void*)(2*sizeof(float)));
        glEnableVertexAttribArray(1);

        // Unbind VAO so other VAO calls wint modify this VAO
        glBindVertexArray(0);

        slime.hasEBO = false;
        slime.mode = GL_TRIANGLES;
        slime.count = 6;
        slime.updateFn = updateSlime;
        slime.renderFn = item_render;

        struct shader shader = shader_init();
        shader_add(&shader, SHADER_VERT, GL_VERTEX_SHADER);
        shader_add(&shader, SHADER_FRAG_TEX, GL_FRAGMENT_SHADER);
        shader_link(&shader);
        slime.shader = shader;

        // Compile and link compute shader
        data->slimeCS = cs_init(SLIME_COMP_SHDR);
        data->textureCS = cs_init(TEX_COMP_SHDR);

        slime.textures = initTexture(TexWidth, TexHeight);
        struct texture temp = initTexture(TexWidth, TexHeight);
        data->tempTexture = temp.textures[0];

        return slime;
}

