#include "core/ShadowMap.h"

#include <cstdio>

#include "core/GLUtil.h"

ShadowMap::~ShadowMap() { destroy(); }

void ShadowMap::destroy() {
    if (m_fbo) glDeleteFramebuffers(1, &m_fbo);
    if (m_tex) glDeleteTextures(1, &m_tex);
    m_fbo = m_tex = 0;
    m_ok = false;
}

bool ShadowMap::init(int resolution) { return resize(resolution); }

bool ShadowMap::resize(int resolution) {
    destroy();
    m_res = resolution;

    glGenTextures(1, &m_tex);
    glBindTexture(GL_TEXTURE_2D, m_tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, m_res, m_res, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);  // PCF is done manually in the shader
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    const GLfloat border[4] = {1.0f, 1.0f, 1.0f, 1.0f};  // outside the light frustum = "far" = lit
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border);

    glGenFramebuffers(1, &m_fbo);
    GLint prev = 0;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &prev);
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_tex, 0);
    glDrawBuffer(GL_NONE);  // depth only: no colour attachment
    glReadBuffer(GL_NONE);

    GLenum st = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    glBindFramebuffer(GL_FRAMEBUFFER, static_cast<GLuint>(prev));
    if (st != GL_FRAMEBUFFER_COMPLETE) {
        const char* why = "unknown";
        switch (st) {
            case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT: why = "INCOMPLETE_ATTACHMENT"; break;
            case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT: why = "INCOMPLETE_MISSING_ATTACHMENT"; break;
            case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER: why = "INCOMPLETE_DRAW_BUFFER"; break;
            case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER: why = "INCOMPLETE_READ_BUFFER"; break;
            case GL_FRAMEBUFFER_UNSUPPORTED: why = "UNSUPPORTED"; break;
        }
        std::fprintf(stderr, "[ShadowMap] FBO %dx%d is NOT complete: %s (0x%X). Shadows disabled.\n", m_res, m_res, why, st);
        destroy();
        return false;
    }
    glCheck("ShadowMap::resize");
    m_ok = true;
    std::printf("[ShadowMap] %dx%d depth map ready\n", m_res, m_res);
    return true;
}

void ShadowMap::toggleQuality() {
    int next = (m_res == kHigh) ? kLow : kHigh;
    resize(next);
}

void ShadowMap::begin() {
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &m_prevFbo);
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
    glViewport(0, 0, m_res, m_res);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glClear(GL_DEPTH_BUFFER_BIT);
    glEnable(GL_CULL_FACE);
    glCullFace(cullFrontFaces ? GL_FRONT : GL_BACK);
}

void ShadowMap::end(int fbWidth, int fbHeight) {
    glCullFace(GL_BACK);
    glBindFramebuffer(GL_FRAMEBUFFER, static_cast<GLuint>(m_prevFbo));
    glViewport(0, 0, fbWidth, fbHeight);
}

void ShadowMap::bindTexture(int unit) const {
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, m_tex);
    glActiveTexture(GL_TEXTURE0);
}
