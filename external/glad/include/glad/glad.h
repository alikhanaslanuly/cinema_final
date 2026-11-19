/* Minimal GLAD-style OpenGL 3.3 core loader (only the functions used by cinema3d).
 * API-compatible with GLAD 1.x: include <glad/glad.h>, call gladLoadGLLoader((GLADloadproc)glfwGetProcAddress).
 * Build with GLFW_INCLUDE_NONE so that no system GL header is included next to it.
 * To switch to a generated GLAD: replace external/glad/ with the files from https://glad.dav1d.de (GL 3.3 core, gl only). */
#ifndef CINEMA_GLAD_SUBSET_H
#define CINEMA_GLAD_SUBSET_H
#if defined(__gl_h_) || defined(__GL_H__)
#error "System OpenGL header included before glad.h. Define GLFW_INCLUDE_NONE."
#endif
#include <stddef.h>
#ifdef __cplusplus
extern "C" {
#endif
#ifndef APIENTRY
 #if defined(_WIN32)
  #define APIENTRY __stdcall
 #else
  #define APIENTRY
 #endif
#endif
#ifndef APIENTRYP
 #define APIENTRYP APIENTRY *
#endif
typedef unsigned int GLenum;
typedef unsigned char GLboolean;
typedef unsigned int GLbitfield;
typedef void GLvoid;
typedef signed char GLbyte;
typedef short GLshort;
typedef int GLint;
typedef unsigned char GLubyte;
typedef unsigned short GLushort;
typedef unsigned int GLuint;
typedef int GLsizei;
typedef float GLfloat;
typedef double GLdouble;
typedef char GLchar;
typedef ptrdiff_t GLintptr;
typedef ptrdiff_t GLsizeiptr;

#define GL_FALSE 0
#define GL_TRUE 1
#define GL_NONE 0
#define GL_NO_ERROR 0
#define GL_DEPTH_BUFFER_BIT 0x00000100
#define GL_COLOR_BUFFER_BIT 0x00004000
#define GL_TRIANGLES 0x0004
#define GL_FLOAT 0x1406
#define GL_UNSIGNED_INT 0x1405
#define GL_UNSIGNED_BYTE 0x1401
#define GL_FRONT 0x0404
#define GL_BACK 0x0405
#define GL_CCW 0x0901
#define GL_CW 0x0900
#define GL_CULL_FACE 0x0B44
#define GL_DEPTH_TEST 0x0B71
#define GL_LESS 0x0201
#define GL_LEQUAL 0x0203
#define GL_VIEWPORT 0x0BA2
#define GL_UNPACK_ALIGNMENT 0x0CF5
#define GL_VENDOR 0x1F00
#define GL_RENDERER 0x1F01
#define GL_VERSION 0x1F02
#define GL_SHADING_LANGUAGE_VERSION 0x8B8C
#define GL_MAJOR_VERSION 0x821B
#define GL_MINOR_VERSION 0x821C
#define GL_INVALID_ENUM 0x0500
#define GL_INVALID_VALUE 0x0501
#define GL_INVALID_OPERATION 0x0502
#define GL_OUT_OF_MEMORY 0x0505
#define GL_INVALID_FRAMEBUFFER_OPERATION 0x0506
#define GL_TEXTURE_2D 0x0DE1
#define GL_TEXTURE0 0x84C0
#define GL_TEXTURE_MAG_FILTER 0x2800
#define GL_TEXTURE_MIN_FILTER 0x2801
#define GL_TEXTURE_WRAP_S 0x2802
#define GL_TEXTURE_WRAP_T 0x2803
#define GL_TEXTURE_BORDER_COLOR 0x1004
#define GL_NEAREST 0x2600
#define GL_LINEAR 0x2601
#define GL_LINEAR_MIPMAP_LINEAR 0x2703
#define GL_REPEAT 0x2901
#define GL_CLAMP_TO_EDGE 0x812F
#define GL_CLAMP_TO_BORDER 0x812D
#define GL_RED 0x1903
#define GL_RGB 0x1907
#define GL_RGBA 0x1908
#define GL_SRGB8 0x8C41
#define GL_SRGB8_ALPHA8 0x8C43
#define GL_RGB8 0x8051
#define GL_RGBA8 0x8058
#define GL_DEPTH_COMPONENT 0x1902
#define GL_DEPTH_COMPONENT24 0x81A6
#define GL_ARRAY_BUFFER 0x8892
#define GL_ELEMENT_ARRAY_BUFFER 0x8893
#define GL_STATIC_DRAW 0x88E4
#define GL_FRAGMENT_SHADER 0x8B30
#define GL_VERTEX_SHADER 0x8B31
#define GL_COMPILE_STATUS 0x8B81
#define GL_LINK_STATUS 0x8B82
#define GL_INFO_LOG_LENGTH 0x8B84
#define GL_FRAMEBUFFER 0x8D40
#define GL_FRAMEBUFFER_BINDING 0x8CA6
#define GL_DEPTH_ATTACHMENT 0x8D00
#define GL_FRAMEBUFFER_COMPLETE 0x8CD5
#define GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT 0x8CD6
#define GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT 0x8CD7
#define GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER 0x8CDB
#define GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER 0x8CDC
#define GL_FRAMEBUFFER_UNSUPPORTED 0x8CDD

typedef void* (*GLADloadproc)(const char* name);
/* returns 1 on success, 0 if the context is missing any function */
int gladLoadGLLoader(GLADloadproc load);

typedef void (APIENTRYP GLAD_PFN_glClear)(GLbitfield mask);
extern GLAD_PFN_glClear glad_glClear;
#define glClear glad_glClear
typedef void (APIENTRYP GLAD_PFN_glClearColor)(GLfloat r, GLfloat g, GLfloat b, GLfloat a);
extern GLAD_PFN_glClearColor glad_glClearColor;
#define glClearColor glad_glClearColor
typedef void (APIENTRYP GLAD_PFN_glEnable)(GLenum cap);
extern GLAD_PFN_glEnable glad_glEnable;
#define glEnable glad_glEnable
typedef void (APIENTRYP GLAD_PFN_glDisable)(GLenum cap);
extern GLAD_PFN_glDisable glad_glDisable;
#define glDisable glad_glDisable
typedef void (APIENTRYP GLAD_PFN_glViewport)(GLint x, GLint y, GLsizei w, GLsizei h);
extern GLAD_PFN_glViewport glad_glViewport;
#define glViewport glad_glViewport
typedef void (APIENTRYP GLAD_PFN_glCullFace)(GLenum mode);
extern GLAD_PFN_glCullFace glad_glCullFace;
#define glCullFace glad_glCullFace
typedef void (APIENTRYP GLAD_PFN_glFrontFace)(GLenum mode);
extern GLAD_PFN_glFrontFace glad_glFrontFace;
#define glFrontFace glad_glFrontFace
typedef void (APIENTRYP GLAD_PFN_glDepthFunc)(GLenum func);
extern GLAD_PFN_glDepthFunc glad_glDepthFunc;
#define glDepthFunc glad_glDepthFunc
typedef GLenum (APIENTRYP GLAD_PFN_glGetError)(void);
extern GLAD_PFN_glGetError glad_glGetError;
#define glGetError glad_glGetError
typedef const GLubyte* (APIENTRYP GLAD_PFN_glGetString)(GLenum name);
extern GLAD_PFN_glGetString glad_glGetString;
#define glGetString glad_glGetString
typedef void (APIENTRYP GLAD_PFN_glGetIntegerv)(GLenum pname, GLint* data);
extern GLAD_PFN_glGetIntegerv glad_glGetIntegerv;
#define glGetIntegerv glad_glGetIntegerv
typedef void (APIENTRYP GLAD_PFN_glPixelStorei)(GLenum pname, GLint param);
extern GLAD_PFN_glPixelStorei glad_glPixelStorei;
#define glPixelStorei glad_glPixelStorei
typedef void (APIENTRYP GLAD_PFN_glDrawArrays)(GLenum mode, GLint first, GLsizei count);
extern GLAD_PFN_glDrawArrays glad_glDrawArrays;
#define glDrawArrays glad_glDrawArrays
typedef void (APIENTRYP GLAD_PFN_glDrawElements)(GLenum mode, GLsizei count, GLenum type, const void* indices);
extern GLAD_PFN_glDrawElements glad_glDrawElements;
#define glDrawElements glad_glDrawElements
typedef void (APIENTRYP GLAD_PFN_glGenTextures)(GLsizei n, GLuint* textures);
extern GLAD_PFN_glGenTextures glad_glGenTextures;
#define glGenTextures glad_glGenTextures
typedef void (APIENTRYP GLAD_PFN_glBindTexture)(GLenum target, GLuint texture);
extern GLAD_PFN_glBindTexture glad_glBindTexture;
#define glBindTexture glad_glBindTexture
typedef void (APIENTRYP GLAD_PFN_glDeleteTextures)(GLsizei n, const GLuint* textures);
extern GLAD_PFN_glDeleteTextures glad_glDeleteTextures;
#define glDeleteTextures glad_glDeleteTextures
typedef void (APIENTRYP GLAD_PFN_glTexImage2D)(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void* pixels);
extern GLAD_PFN_glTexImage2D glad_glTexImage2D;
#define glTexImage2D glad_glTexImage2D
typedef void (APIENTRYP GLAD_PFN_glTexParameteri)(GLenum target, GLenum pname, GLint param);
extern GLAD_PFN_glTexParameteri glad_glTexParameteri;
#define glTexParameteri glad_glTexParameteri
typedef void (APIENTRYP GLAD_PFN_glTexParameterfv)(GLenum target, GLenum pname, const GLfloat* params);
extern GLAD_PFN_glTexParameterfv glad_glTexParameterfv;
#define glTexParameterfv glad_glTexParameterfv
typedef void (APIENTRYP GLAD_PFN_glDrawBuffer)(GLenum buf);
extern GLAD_PFN_glDrawBuffer glad_glDrawBuffer;
#define glDrawBuffer glad_glDrawBuffer
typedef void (APIENTRYP GLAD_PFN_glReadBuffer)(GLenum src);
extern GLAD_PFN_glReadBuffer glad_glReadBuffer;
#define glReadBuffer glad_glReadBuffer
typedef void (APIENTRYP GLAD_PFN_glActiveTexture)(GLenum texture);
extern GLAD_PFN_glActiveTexture glad_glActiveTexture;
#define glActiveTexture glad_glActiveTexture
typedef void (APIENTRYP GLAD_PFN_glGenerateMipmap)(GLenum target);
extern GLAD_PFN_glGenerateMipmap glad_glGenerateMipmap;
#define glGenerateMipmap glad_glGenerateMipmap
typedef GLuint (APIENTRYP GLAD_PFN_glCreateShader)(GLenum type);
extern GLAD_PFN_glCreateShader glad_glCreateShader;
#define glCreateShader glad_glCreateShader
typedef void (APIENTRYP GLAD_PFN_glShaderSource)(GLuint shader, GLsizei count, const GLchar* const* string, const GLint* length);
extern GLAD_PFN_glShaderSource glad_glShaderSource;
#define glShaderSource glad_glShaderSource
typedef void (APIENTRYP GLAD_PFN_glCompileShader)(GLuint shader);
extern GLAD_PFN_glCompileShader glad_glCompileShader;
#define glCompileShader glad_glCompileShader
typedef void (APIENTRYP GLAD_PFN_glGetShaderiv)(GLuint shader, GLenum pname, GLint* params);
extern GLAD_PFN_glGetShaderiv glad_glGetShaderiv;
#define glGetShaderiv glad_glGetShaderiv
typedef void (APIENTRYP GLAD_PFN_glGetShaderInfoLog)(GLuint shader, GLsizei bufSize, GLsizei* length, GLchar* infoLog);
extern GLAD_PFN_glGetShaderInfoLog glad_glGetShaderInfoLog;
#define glGetShaderInfoLog glad_glGetShaderInfoLog
typedef void (APIENTRYP GLAD_PFN_glDeleteShader)(GLuint shader);
extern GLAD_PFN_glDeleteShader glad_glDeleteShader;
#define glDeleteShader glad_glDeleteShader
typedef GLuint (APIENTRYP GLAD_PFN_glCreateProgram)(void);
extern GLAD_PFN_glCreateProgram glad_glCreateProgram;
#define glCreateProgram glad_glCreateProgram
typedef void (APIENTRYP GLAD_PFN_glAttachShader)(GLuint program, GLuint shader);
extern GLAD_PFN_glAttachShader glad_glAttachShader;
#define glAttachShader glad_glAttachShader
typedef void (APIENTRYP GLAD_PFN_glLinkProgram)(GLuint program);
extern GLAD_PFN_glLinkProgram glad_glLinkProgram;
#define glLinkProgram glad_glLinkProgram
typedef void (APIENTRYP GLAD_PFN_glGetProgramiv)(GLuint program, GLenum pname, GLint* params);
extern GLAD_PFN_glGetProgramiv glad_glGetProgramiv;
#define glGetProgramiv glad_glGetProgramiv
typedef void (APIENTRYP GLAD_PFN_glGetProgramInfoLog)(GLuint program, GLsizei bufSize, GLsizei* length, GLchar* infoLog);
extern GLAD_PFN_glGetProgramInfoLog glad_glGetProgramInfoLog;
#define glGetProgramInfoLog glad_glGetProgramInfoLog
typedef void (APIENTRYP GLAD_PFN_glDeleteProgram)(GLuint program);
extern GLAD_PFN_glDeleteProgram glad_glDeleteProgram;
#define glDeleteProgram glad_glDeleteProgram
typedef void (APIENTRYP GLAD_PFN_glUseProgram)(GLuint program);
extern GLAD_PFN_glUseProgram glad_glUseProgram;
#define glUseProgram glad_glUseProgram
typedef GLint (APIENTRYP GLAD_PFN_glGetUniformLocation)(GLuint program, const GLchar* name);
extern GLAD_PFN_glGetUniformLocation glad_glGetUniformLocation;
#define glGetUniformLocation glad_glGetUniformLocation
typedef void (APIENTRYP GLAD_PFN_glUniform1i)(GLint location, GLint v0);
extern GLAD_PFN_glUniform1i glad_glUniform1i;
#define glUniform1i glad_glUniform1i
typedef void (APIENTRYP GLAD_PFN_glUniform1f)(GLint location, GLfloat v0);
extern GLAD_PFN_glUniform1f glad_glUniform1f;
#define glUniform1f glad_glUniform1f
typedef void (APIENTRYP GLAD_PFN_glUniform2f)(GLint location, GLfloat v0, GLfloat v1);
extern GLAD_PFN_glUniform2f glad_glUniform2f;
#define glUniform2f glad_glUniform2f
typedef void (APIENTRYP GLAD_PFN_glUniform3f)(GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
extern GLAD_PFN_glUniform3f glad_glUniform3f;
#define glUniform3f glad_glUniform3f
typedef void (APIENTRYP GLAD_PFN_glUniform4f)(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
extern GLAD_PFN_glUniform4f glad_glUniform4f;
#define glUniform4f glad_glUniform4f
typedef void (APIENTRYP GLAD_PFN_glUniformMatrix4fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
extern GLAD_PFN_glUniformMatrix4fv glad_glUniformMatrix4fv;
#define glUniformMatrix4fv glad_glUniformMatrix4fv
typedef void (APIENTRYP GLAD_PFN_glGenVertexArrays)(GLsizei n, GLuint* arrays);
extern GLAD_PFN_glGenVertexArrays glad_glGenVertexArrays;
#define glGenVertexArrays glad_glGenVertexArrays
typedef void (APIENTRYP GLAD_PFN_glBindVertexArray)(GLuint array);
extern GLAD_PFN_glBindVertexArray glad_glBindVertexArray;
#define glBindVertexArray glad_glBindVertexArray
typedef void (APIENTRYP GLAD_PFN_glDeleteVertexArrays)(GLsizei n, const GLuint* arrays);
extern GLAD_PFN_glDeleteVertexArrays glad_glDeleteVertexArrays;
#define glDeleteVertexArrays glad_glDeleteVertexArrays
typedef void (APIENTRYP GLAD_PFN_glGenBuffers)(GLsizei n, GLuint* buffers);
extern GLAD_PFN_glGenBuffers glad_glGenBuffers;
#define glGenBuffers glad_glGenBuffers
typedef void (APIENTRYP GLAD_PFN_glBindBuffer)(GLenum target, GLuint buffer);
extern GLAD_PFN_glBindBuffer glad_glBindBuffer;
#define glBindBuffer glad_glBindBuffer
typedef void (APIENTRYP GLAD_PFN_glBufferData)(GLenum target, GLsizeiptr size, const void* data, GLenum usage);
extern GLAD_PFN_glBufferData glad_glBufferData;
#define glBufferData glad_glBufferData
typedef void (APIENTRYP GLAD_PFN_glDeleteBuffers)(GLsizei n, const GLuint* buffers);
extern GLAD_PFN_glDeleteBuffers glad_glDeleteBuffers;
#define glDeleteBuffers glad_glDeleteBuffers
typedef void (APIENTRYP GLAD_PFN_glVertexAttribPointer)(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer);
extern GLAD_PFN_glVertexAttribPointer glad_glVertexAttribPointer;
#define glVertexAttribPointer glad_glVertexAttribPointer
typedef void (APIENTRYP GLAD_PFN_glEnableVertexAttribArray)(GLuint index);
extern GLAD_PFN_glEnableVertexAttribArray glad_glEnableVertexAttribArray;
#define glEnableVertexAttribArray glad_glEnableVertexAttribArray
typedef void (APIENTRYP GLAD_PFN_glGenFramebuffers)(GLsizei n, GLuint* framebuffers);
extern GLAD_PFN_glGenFramebuffers glad_glGenFramebuffers;
#define glGenFramebuffers glad_glGenFramebuffers
typedef void (APIENTRYP GLAD_PFN_glBindFramebuffer)(GLenum target, GLuint framebuffer);
extern GLAD_PFN_glBindFramebuffer glad_glBindFramebuffer;
#define glBindFramebuffer glad_glBindFramebuffer
typedef void (APIENTRYP GLAD_PFN_glFramebufferTexture2D)(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
extern GLAD_PFN_glFramebufferTexture2D glad_glFramebufferTexture2D;
#define glFramebufferTexture2D glad_glFramebufferTexture2D
typedef GLenum (APIENTRYP GLAD_PFN_glCheckFramebufferStatus)(GLenum target);
extern GLAD_PFN_glCheckFramebufferStatus glad_glCheckFramebufferStatus;
#define glCheckFramebufferStatus glad_glCheckFramebufferStatus
typedef void (APIENTRYP GLAD_PFN_glDeleteFramebuffers)(GLsizei n, const GLuint* framebuffers);
extern GLAD_PFN_glDeleteFramebuffers glad_glDeleteFramebuffers;
#define glDeleteFramebuffers glad_glDeleteFramebuffers

#ifdef __cplusplus
}
#endif
#endif
