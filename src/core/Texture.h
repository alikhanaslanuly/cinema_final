#pragma once
#include <glad/glad.h>
#include <string>

// Loads an image with stb_image into a GL_TEXTURE_2D (mipmaps, repeat). Returns 0 and prints
// the reason to the console on failure. Path is relative to the repo root (e.g. "assets/x.png").
GLuint loadTexture2D(const std::string& relPath);
