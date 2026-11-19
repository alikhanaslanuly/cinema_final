#pragma once
#include <glad/glad.h>
#include <cstdio>

// Drains the GL error queue and prints every error to the console.
inline void glCheck(const char* where) {
    GLenum e;
    int guard = 0;
    while ((e = glGetError()) != GL_NO_ERROR && guard++ < 8) {
        const char* s = "UNKNOWN";
        switch (e) {
            case GL_INVALID_ENUM: s = "INVALID_ENUM"; break;
            case GL_INVALID_VALUE: s = "INVALID_VALUE"; break;
            case GL_INVALID_OPERATION: s = "INVALID_OPERATION"; break;
            case GL_OUT_OF_MEMORY: s = "OUT_OF_MEMORY"; break;
            case GL_INVALID_FRAMEBUFFER_OPERATION: s = "INVALID_FRAMEBUFFER_OPERATION"; break;
        }
        std::fprintf(stderr, "[GL] error %s (0x%X) at %s\n", s, e, where);
    }
}
