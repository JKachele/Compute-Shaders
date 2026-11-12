/*************************************************
 *File----------src/gfx/shader.c
 *Project-------Compute-Shaders
 *Author--------Justin Kachele
 *Created-------Wednesday Nov 11, 2026 14:27:30 EST
 *License-------GNU GPL-3.0
 ************************************************/

#include "shader.h"

static const char *VERTEX_PATH = "assets/shaders/vert.glsl";
static const char *FRAGMENT_PATH = "assets/shaders/frag.glsl";

static uint _compile(const char *path, GLenum type) {
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
        if (!success) {
                glGetShaderInfoLog(shaderHandle, 512, NULL, infoLog);
                if (type == GL_VERTEX_SHADER)
                        printf("ERROR: SHADER VERTEX COMPILATION FAILED\n%s\n",
                                infoLog);
                if (type == GL_FRAGMENT_SHADER)
                        printf("ERROR: SHADER FRAGMENT COMPILATION FAILED\n"
                                        "%s\n", infoLog);
                else
                        printf("ERROR: SHADER COMPILATION FAILED\n%s\n",
                                infoLog);
        }

        free(shaderSource);
        return shaderHandle;
}

uint shaderInit(void) {
        // Compile vertex and fragment shaders
        GLuint vertexShader = _compile(VERTEX_PATH, GL_VERTEX_SHADER);
        GLuint fragmentShader = _compile(FRAGMENT_PATH, GL_FRAGMENT_SHADER);

        // Create share program and link vertex and fragment shaders
        GLuint shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);

        // Check shader linking success
        int success;
        char infoLog[512];
        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
        if (!success) {
                glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
                printf("ERROR: SHADER LINKING FAILED\n%s\n",
                                infoLog);
        }

        // No longer need individual shader programs
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        // Activate shader program
        glUseProgram(shaderProgram);

        return shaderProgram;
}

void shaderUse(uint shaderProgram) {
        glUseProgram(shaderProgram);
}

void shaderDestroy(uint shaderProgram) {
        glDeleteProgram(shaderProgram);
}

