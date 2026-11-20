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
        fread(shaderSource, 1, len, f);
        assert(strlen(shaderSource) > 0);
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

struct shader shaderInit(void) {
        struct shader shader;
        shader.numObjects = 0;
        shader.objectCap = 8;
        shader.objects = malloc(8 * sizeof(uint));
        shader.program = glCreateProgram();
        return shader;
}

void shaderAdd(struct shader *self, const char *path, GLenum type) {
        self->numObjects++;
        if (self->numObjects > self->objectCap) {
                self->objectCap += 8;
                self->objects = realloc(self->objects, self->objectCap * sizeof(uint));
        }
        uint shaderObj = compile(path, type);
        self->objects[self->numObjects-1] = shaderObj;
        glAttachShader(self->program, shaderObj);
}

void shaderLink(struct shader *self) {
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

void shaderUse(struct shader self) {
        glUseProgram(self.program);
}

void shaderDestroy(struct shader self) {
        glDeleteProgram(self.program);
}

void shaderSetBool(struct shader self, const char *name, bool value) {
        glUniform1i(glGetUniformLocation(self.program, name), (int)value);
}

void shaderSetInt(struct shader self, const char *name, int value) {
        glUniform1i(glGetUniformLocation(self.program, name), value);
}

void shaderSetFloat(struct shader self, const char *name, float value) {
        glUniform1f(glGetUniformLocation(self.program, name), value);
}

void shaderSetVec3(struct shader self, const char *name, vec3s vec) {
        uint location = glGetUniformLocation(self.program, name);
        glUniform3f(location, vec.x, vec.y, vec.z);
}

void shaderSetVec4(struct shader self, const char *name, vec4s vec) {
        uint location = glGetUniformLocation(self.program, name);
        glUniform4f(location, vec.x, vec.y, vec.z, vec.w);
}

void shaderSetMat4(struct shader self, const char *name, mat4s mat) {
        uint location = glGetUniformLocation(self.program, name);
        glUniformMatrix4fv(location, 1, GL_FALSE, (const GLfloat*)&mat.raw);
}

