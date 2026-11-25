/*************************************************
 *File----------src/gfx/shader.c
 *Project-------Compute-Shaders
 *Author--------Justin Kachele
 *Created-------Wednesday Nov 11, 2026 14:27:30 EST
 *License-------GNU GPL-3.0
 ************************************************/

#include "shader.h"

static uint compile(const char *path, GLenum type) {
        // Open shader file
        FILE *f = fopen(path, "rb");
        if (f == NULL) {
                fprintf(stderr, "Error loading shader at %s\n", path);
                exit(1);
        }

        // Get file length
        fseek(f, 0, SEEK_END);
        int64 len = ftell(f);
        assert(len > 0);

        // Get shader program in string variable
        fseek(f, 0, SEEK_SET);
        char *shaderSource = calloc(1, len);
        assert(shaderSource != NULL);
        assert(fread(shaderSource, 1, len, f) > 0);
        fclose(f);

        // Attach source to handle and compile
        uint shaderHandle = glCreateShader(type);
        glShaderSource(shaderHandle, 1, (const GLchar**)&shaderSource, NULL);
        glCompileShader(shaderHandle);

        // Check for compilation success
        int success;
        char infoLog[512];
        glGetShaderiv(shaderHandle, GL_COMPILE_STATUS, &success);
        if (success == 0) {
                glGetShaderInfoLog(shaderHandle, 512, NULL, infoLog);
                fprintf(stderr, "ERROR: SHADER %s COMPILATION FAILED\n%s\n", path, infoLog);
        }

        free(shaderSource);
        return shaderHandle;
}

struct shader shader_init(void) {
        struct shader shader;
        shader.numObjects = 0;
        shader.objectCap = 8;
        shader.objects = malloc(8 * sizeof(uint));
        shader.program = glCreateProgram();
        return shader;
}

void shader_add(struct shader *self, const char *path, GLenum type) {
        self->numObjects++;
        if (self->numObjects > self->objectCap) {
                self->objectCap += 8;
                self->objects = realloc(self->objects, self->objectCap * sizeof(uint));
        }
        uint shaderObj = compile(path, type);
        self->objects[self->numObjects-1] = shaderObj;
        glAttachShader(self->program, shaderObj);
}

void shader_link(struct shader *self) {
        // Link all attached shader objects
        glLinkProgram(self->program);

        // Check shader linking success
        int success;
        char infoLog[512];
        glGetProgramiv(self->program, GL_LINK_STATUS, &success);
        if (success == 0) {
                glGetProgramInfoLog(self->program, 512, NULL, infoLog);
                fprintf(stderr, "ERROR: SHADER LINKING FAILED\n%s\n", infoLog);
                return;
        }

        glValidateProgram(self->program);

        glGetProgramiv(self->program, GL_VALIDATE_STATUS, &success);
        if (success == 0) {
                glGetProgramInfoLog(self->program, 512, NULL, infoLog);
                fprintf(stderr, "Invalid shader program: '%s'\n", infoLog);
                return;
        }

        // No longer need individual shader programs
        for (int i = 0; i < self->numObjects; i++) {
                glDeleteShader(self->objects[i]);
        }

        // Activate shader program
        glUseProgram(self->program);
}

void shader_use(struct shader self) {
        glUseProgram(self.program);
}

void shader_destroy(struct shader self) {
        glDeleteProgram(self.program);
}

void shader_setBool(struct shader self, const char *name, bool value) {
        shader_use(self);
        glUniform1i(glGetUniformLocation(self.program, name), (int)value);
}

void shader_setInt(struct shader self, const char *name, int value) {
        shader_use(self);
        glUniform1i(glGetUniformLocation(self.program, name), value);
}

void shader_setFloat(struct shader self, const char *name, float value) {
        shader_use(self);
        glUniform1f(glGetUniformLocation(self.program, name), value);
}

void shader_setVec3(struct shader self, const char *name, vec3s vec) {
        shader_use(self);
        uint location = glGetUniformLocation(self.program, name);
        glUniform3f(location, vec.x, vec.y, vec.z);
}

void shader_setVec4(struct shader self, const char *name, vec4s vec) {
        shader_use(self);
        uint location = glGetUniformLocation(self.program, name);
        glUniform4f(location, vec.x, vec.y, vec.z, vec.w);
}

void shader_setMat4(struct shader self, const char *name, mat4s mat) {
        shader_use(self);
        uint location = glGetUniformLocation(self.program, name);
        glUniformMatrix4fv(location, 1, GL_FALSE, (const GLfloat*)&mat.raw);
}

