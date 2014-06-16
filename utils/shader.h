#ifndef MY_SHADER_H
#define MY_SHADER_H

#include "opengl.h"

void shaderAttachFromFile(GLuint program, GLenum type, const char *filePath);
char* shaderLoadSource(const char *filePath);
GLuint shaderCompileFromFile(GLenum type, const char *filePath);


#endif /* of MY_SHADER_H*/

