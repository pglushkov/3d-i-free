#ifndef MY_SHADER_H
#define MY_SHADER_H

#include "opengl.h"

int shaderAttachFromFile(GLuint program, GLenum type, const char *filePath);
GLint linkCompiledProgram(GLuint& program_id);
char* shaderLoadSource(const char *filePath);
GLuint shaderCompileFromFile(GLenum type, const char *filePath);


#endif /* of MY_SHADER_H*/

