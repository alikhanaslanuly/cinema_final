#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "core/Texture.h"

#include <cstdio>

#include "core/GLUtil.h"
#include "core/Paths.h"

GLuint loadTexture2D(const std::string& relPath) {
    std::string path = paths::resolve(relPath);
    stbi_set_flip_vertically_on_load(1);
    int w = 0, h = 0, n = 0;
    unsigned char* data = stbi_load(path.c_str(), &w, &h, &n, 0);
    if (!data) {
        std::fprintf(stderr, "[Texture] cannot load '%s': %s\n", path.c_str(), stbi_failure_reason());
        return 0;
    }
    GLenum fmt = n == 1 ? GL_RED : (n == 3 ? GL_RGB : GL_RGBA);
    GLuint tex = 0;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, static_cast<GLint>(fmt), w, h, 0, fmt, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    stbi_image_free(data);
    glCheck(path.c_str());
    return tex;
}
