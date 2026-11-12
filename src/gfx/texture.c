/*************************************************
 *File----------src/gfx/texture.c
 *Project-------Compute-Shaders
 *Author--------Justin Kachele
 *Created-------Thursday Nov 12, 2026 12:55:12 EST
 *License-------GNU GPL-3.0
 ************************************************/

#define STB_IMAGE_IMPLEMENTATION

#include "texture.h"
#include <stb_image.h>
#include "shader.h"

const char *Container = "assets/textures/container.jpg";
const char *Face      = "assets/textures/awesomeface.png";

static uint loadTexture(const char *filename, GLenum format, bool flip) {
        // Create and bind texture handle
        uint texHandle;
        glGenTextures(1, &texHandle);
        glBindTexture(GL_TEXTURE_2D, texHandle);
        
        // Set texture wrapping/filtering options on bound texture
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                        GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Load the texture
        stbi_set_flip_vertically_on_load(flip);
        int width, height, nChannels;
        unsigned char *data = stbi_load(filename, &width, &height, &nChannels, 0);

        // Verify success and generate texture and mipmaps
        if (data) {
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,
                                format, GL_UNSIGNED_BYTE, data);
                glGenerateMipmap(GL_TEXTURE_2D);
        } else {
                fprintf(stderr, "Failed to load texture!\n");
        }

        // Free image data and return handle
        stbi_image_free(data);
        return texHandle;
}

struct texture textureInit(uint shaderProgram) {
        struct texture texture;
        texture.numTextures = 2;
        texture.textures = malloc(texture.numTextures * sizeof(uint));

        texture.textures[0] = loadTexture(Container, GL_RGB, false);
        shaderSetInt(shaderProgram, "tex0", 0);

        texture.textures[1] = loadTexture(Face, GL_RGBA, true);
        shaderSetInt(shaderProgram, "tex1", 1);

        return texture;
}

void bindTextures(struct texture textures) {
        GLenum units[] = {GL_TEXTURE0, GL_TEXTURE1, GL_TEXTURE2, GL_TEXTURE3};
        for (int i = 0; i < textures.numTextures; i++) {
                glActiveTexture(units[i]);
                glBindTexture(GL_TEXTURE_2D, textures.textures[i]);
        }
}

