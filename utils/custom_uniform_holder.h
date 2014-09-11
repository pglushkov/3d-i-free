#ifndef CUSTOM_UNIFORM_HOLDER_H
#define CUSTOM_UNIFORM_HOLDER_H

#include "opengl.h"

#include <string>
#include <vector>
#include <map>

enum CUSTOM_UNIFORM_TYPE
{
    // current type of uniforms are formed based on types of glUniform...() routines (see https://www.opengl.org/sdk/docs/man/html/glUniform.xhtml for reference)
    // routines of type glUniform<N><T>(...), where N = size, T = type, are not accounted for except if size = 1 (a simple vector that is)
    INT32_VAL = 0,
    FP32_VAL,
    INT32_VECTOR,
    FP32_VECTOR,
    FP32_MATRIX_2x2,
    FP32_MATRIX_3x3,
    FP32_MATRIX_4x4,
};

struct IntUniform
{
    CUSTOM_UNIFORM_TYPE type;
    std::vector<GLint> vals;
};

struct FpUniform
{
    CUSTOM_UNIFORM_TYPE type;
    std::vector<GLfloat> vals;
};

class UniformHolder
{
public:
    typedef std::map<std::string, IntUniform> iuni_holder_type;
    typedef std::map<std::string, FpUniform> fpuni_holder_type;

private:
    iuni_holder_type int_uniforms;
    fpuni_holder_type fp_uniforms;
public:

    size_t TotalCount() { return int_uniforms.size() + fp_uniforms.size(); }
    size_t IntCount() { return int_uniforms.size(); }
    size_t FpCount() { return fp_uniforms.size(); }

    void AddIntUniform ( CUSTOM_UNIFORM_TYPE type, const char* name, GLint* data, size_t size = 0);
    void AddFpUniform ( CUSTOM_UNIFORM_TYPE type, const char* name, GLfloat* data, size_t size = 0);

    bool CheckExists(const char* name);

    // ATTENTION!!!
    // Before calling on of further two routines, make sure that necessary shader program had been 'enabled' by glUseProgram(...)
    // If not - this routine is plainly useless!
    int BindAllUniforms(const GLuint gl_program);
    int BindSpecialUniform(const char* name, const GLuint gl_program);
};

// =====================================
// further are some utility routines ...

IntUniform CreateIntValUniform(GLint val);
IntUniform CreateIntVecUniform(GLint* vals, size_t count);
FpUniform CreateFpValUniform(GLfloat val);
FpUniform CreateFpVecUniform(GLfloat* vals, size_t count);
FpUniform CreateFpMat2x2Uniform(GLfloat* vals);
FpUniform CreateFpMat3x3Uniform(GLfloat* vals);
FpUniform CreateFpMat4x4Uniform(GLfloat* vals);

// ATTENTION!!!
// Before calling on of further two routines, make sure that necessary shader program had been 'enabled' by glUseProgram(...)
// If not - this routine is plainly useless!
int BindUniformInt(const IntUniform& u, const char* name, const GLuint gl_program);
int BindUniformFp(const FpUniform& u, const char* name, const GLuint gl_program);

#endif // CUSTOM_UNIFORM_HOLDER_H
