/*************************************************
 *File----------src/gfx/font.c
 *Project-------Compute-Shaders
 *Author--------Justin Kachele
 *Created-------Monday Nov 23, 2026 12:15:57 EST
 *License-------GNU GPL-3.0
 ************************************************/

#include "font.h"
#include "texture.h"

static const char *JETBRAINSMONO = "assets/fonts/JetBrainsMonoNerdFont-Regular.ttf";
static const char *VERTEX_SHADER = "assets/shaders/font/font.vs.glsl";
static const char *FRAGMENT_SHADER = "assets/shaders/font/font.fs.glsl";

static struct glyph font_generateChar(FT_Face face, char c) {
        struct glyph glyph = {0};

        // Load character glyph
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
                fprintf(stderr, "ERROR:FREETYTPE: Failed to load Glyph\n");  
                return glyph;
        }

        // Generate texture
        uint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows,
                        0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);

        // Set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glyph.textureID = texture;
        glyph.size = (ivec2s){face->glyph->bitmap.width, face->glyph->bitmap.rows};
        glyph.bearing = (ivec2s){face->glyph->bitmap_left, face->glyph->bitmap_top};
        glyph.advance = face->glyph->advance.x;
        return glyph;
}

struct font font_init(void) {
        struct font font = {0};

        // Load freetype library
        FT_Library ft;
        if (FT_Init_FreeType(&ft)) {
                fprintf(stderr, "ERROR:FREETYTPE: Could not init FreeType Library\n");
                return font;
        }

        // Load font ttf
        FT_Face face;
        if (FT_New_Face(ft, JETBRAINSMONO, 0, &face)) {
                fprintf(stderr, "ERROR:FREETYTPE: Failed to load font %s\n", JETBRAINSMONO);
                return font;
        }

        FT_Set_Pixel_Sizes(face, 0, 100);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction

        // Generate glyphs for 128 ASCII characters
        for (unsigned char c = 0; c < 128; c++) {
                font.glyphs[c] = font_generateChar(face, c);
        }

        FT_Done_Face(face);
        FT_Done_FreeType(ft);

        // Create VAO and VBO to store quad data for texture
        uint VAO;
        uint VBO;
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        font.VAO = VAO;
        font.VBO = VBO;
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float)*6*4, NULL, GL_DYNAMIC_DRAW);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4*sizeof(float), 0);
        glEnableVertexAttribArray(0);
        glBindVertexArray(0);

        // Compile and link shader programs
        struct shader shader = shader_init();
        shader_add(&shader, VERTEX_SHADER, GL_VERTEX_SHADER);
        shader_add(&shader, FRAGMENT_SHADER, GL_FRAGMENT_SHADER);
        shader_link(&shader);
        font.shader = shader;

        return font;
}

void font_render(struct font self, const char *text, float x, float y, float size, vec3s color,
                float winWidth, float winHeight) {
        shader_use(self.shader);
        shader_setVec3(self.shader, "textColor", color);
        mat4s proj = glms_ortho(0.0, winWidth, 0.0, winHeight, -0.1, 0.1);
        shader_setMat4(self.shader, "projection", proj);
        glActiveTexture(GL_TEXTURE0);
        glBindVertexArray(self.VAO);

        // Font is 100px tall
        float scale = size / 100;

        for (int i = 0; text[i] != '\0'; i++) {
                uint8 c = text[i];
                struct glyph glyph = self.glyphs[c];

                // Get size and position of quad
                float xPos = x + glyph.bearing.x * scale;
                float yPos = y - (glyph.size.y - glyph.bearing.y) * scale;
                float w = glyph.size.x * scale;
                float h = glyph.size.y * scale;

                // Get quad verticies
                float verts[6][4] = {
                        {xPos,     yPos + h, 0.0f, 0.0f},
                        {xPos,     yPos,     0.0f, 1.0f},
                        {xPos + w, yPos,     1.0f, 1.0f},

                        {xPos,     yPos + h, 0.0f, 0.0f},
                        {xPos + w, yPos,     1.0f, 1.0f},
                        {xPos + w, yPos + h, 1.0f, 0.0f}
                };

                // Use glyph texture for quad
                glBindTexture(GL_TEXTURE_2D, glyph.textureID);

                // Update VBO Memory with quad verticies
                glBindBuffer(GL_ARRAY_BUFFER, self.VBO);
                glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(verts), verts); 
                glBindBuffer(GL_ARRAY_BUFFER, 0);

                // Render quad
                glDrawArrays(GL_TRIANGLES, 0, 6);

                // Advance cursor position to next glyph
                // Bitshift 6 because advance is stored as 1/64 pixels
                x += (glyph.advance >> 6) * scale;
        }
        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);
}

