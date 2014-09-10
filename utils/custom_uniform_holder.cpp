
#include "custom_uniform_holder.h"

#include <stdexcept>
#include <stdio.h>

void UniformHolder:: AddIntUniform ( CUSTOM_UNIFORM_TYPE type, const char* name, GLint* data, size_t size)
{

    if( CheckExists(name) )
        throw std::runtime_error("ERROR in UniformHolder::AddIntUniform() : Uniform with such name exists! Pick another one!");

    switch (type)
    {
        case INT32_VAL:
            int_uniforms[name] = CreateIntValUniform(data[0]);
            break;
        case INT32_VECTOR:
            int_uniforms[name] = CreateIntVecUniform(data, size);
            break;
        default:
            throw std::runtime_error("ERROR in UniformHolder::AddIntUniform() : Erroneous type of uniform specified!");
    }
}

void UniformHolder:: AddFpUniform ( CUSTOM_UNIFORM_TYPE type, const char* name, GLfloat* data, size_t size)
{
    if( CheckExists(name) )
        throw std::runtime_error("ERROR in UniformHolder::AddUniform() : Uniform with such name exists! Pick another one!");

    switch (type)
    {
        case FP32_VAL:
            fp_uniforms[name] = CreateFpValUniform(data[0]);
            break;
        case FP32_VECTOR:
            fp_uniforms[name] = CreateFpVecUniform(data, size);
            break;
        case FP32_MATRIX_2x2:
            fp_uniforms[name] = CreateFpMat2x2Uniform(data);
            break;
        case FP32_MATRIX_3x3:
            fp_uniforms[name] = CreateFpMat3x3Uniform(data);
            break;
        case FP32_MATRIX_4x4:
            fp_uniforms[name] = CreateFpMat4x4Uniform(data);
            break;
        default:
            throw std::runtime_error("ERROR in UniformHolder::AddFpUniform() : Erroneous type of uniform specified!");
    }
}

bool UniformHolder:: CheckExists(const char* name)
{
    bool result;

    iuni_holder_type::iterator it1 = int_uniforms.find(name);
    fpuni_holder_type::iterator it2 = fp_uniforms.find(name);

    if (it1 == int_uniforms.end() && it2 == fp_uniforms.end())
        result = false;
    else
        result = true;

    return result;
}


// ATTENTION, YO BICHES!
// Before calling this routine, make sure that necessary shader program had been 'enabled' by glUseProgram(...)
// If not - this routine is plainly useless!
int UniformHolder:: BindAllUniforms(const GLuint gl_program)
{
    int result;

    printf("BindAllUniforms: step1\n");

    if (int_uniforms.size()) {
        iuni_holder_type::iterator i1 = int_uniforms.begin();
        while (i1++ != int_uniforms.end()){
            result = BindUniform( (*i1).second , (*i1).first.c_str() , gl_program);
            if (result) return result;
        }
    }

    printf("BindAllUniforms: step2\n");

    if (fp_uniforms.size()) {
        fpuni_holder_type::iterator i2 = fp_uniforms.begin();
        while (i2++ != fp_uniforms.end()){
            result = BindUniform( (*i2).second , (*i2).first.c_str() , gl_program);
            if (result) return result;
        }
    }

    printf("BindAllUniforms: step3\n");

    return 0;
}

// ATTENTION, YO BICHES!
// Before calling this routine, make sure that necessary shader program had been 'enabled' by glUseProgram(...)
// If not - this routine is plainly useless!
int UniformHolder:: BindSpecialUniform(const char* name, const GLuint gl_program)
{
    iuni_holder_type::iterator i1 = int_uniforms.find(name);
    if(i1 != int_uniforms.end()) {
        return BindUniform( (*i1).second , (*i1).first.c_str() , gl_program);
    }

    fpuni_holder_type::iterator i2 = fp_uniforms.find(name);
    if(i2 != fp_uniforms.end()) {
        return BindUniform( (*i2).second , (*i2).first.c_str() , gl_program);
    }

    // if we got here - then specified uniform was found not in int_uniforms, nor in fp_uniforms, returning error ...
    return -1;
}



// =================================================================
// =========== Actual Uniform Container Creation Routines ==========
// =================================================================

IntUniform CreateIntValUniform(GLint val)
{
    IntUniform res;
    res.type = INT32_VAL;
    res.vals.resize(1);
    res.vals[0] = val;

    return res;
}

IntUniform CreateIntVecUniform(GLint* vals, size_t count)
{
    IntUniform res;
    res.type = INT32_VECTOR;
    res.vals.resize(count);
    res.vals.assign(vals, vals + count);

    return res;
}

FpUniform CreateFpValUniform(GLfloat val)
{
    FpUniform res;
    res.type = FP32_VAL;
    res.vals.resize(1);
    res.vals[0] = val;

    return res;
}

FpUniform CreateFpVecUniform(GLfloat* vals, size_t count)
{
    FpUniform res;
    res.type = FP32_VECTOR;
    res.vals.resize(count);
    res.vals.assign(vals, vals + count);

    return res;
}

FpUniform CreateFpMat2x2Uniform(GLfloat* vals)
{
    FpUniform res;
    res.type = FP32_MATRIX_2x2;
    res.vals.resize(4);
    res.vals.assign(vals, vals + 4);

    return res;
}

FpUniform CreateFpMat3x3Uniform(GLfloat* vals)
{
    FpUniform res;
    res.type = FP32_MATRIX_3x3;
    res.vals.resize(9);
    res.vals.assign(vals, vals + 9);

    return res;
}

FpUniform CreateFpMat4x4Uniform(GLfloat* vals)
{
    FpUniform res;
    res.type = FP32_MATRIX_4x4;
    res.vals.resize(16);
    res.vals.assign(vals, vals + 16);

    return res;
}

// =================================================================
// =========== Actual Uniform-to-Shader Binding Routines ===========
// =================================================================

// ATTENTION, YO BICHES!
// Before calling this routine, make sure that necessary shader program had been 'enabled' by glUseProgram(...)
// If not - this routine is plainly useless!
int BindUniform(const IntUniform& u, const char* name, const GLuint gl_program)
{
    GLint UniformHandle = glGetUniformLocation(gl_program, name );
    if (UniformHandle != -1) {
        if (u.type == INT32_VAL) {
            glUniform1i(UniformHandle, u.vals[0]);

        } else if (u.type == INT32_VECTOR) {
            glUniform1iv(UniformHandle, u.vals.size(), &u.vals[0]);

        } else {
            // something is seriously screwed ...
            throw std::runtime_error("ERROR in BindUniform( integer one) : Trying to bind uniform with erroneous type");
        }

        // check that actual uniform passing to GPU went fine
        if ( signal_of_opengl_error() )
            return 2;

    } else {
        // could not find specified uniform in specified program
        return 1;
    }

    return 0;
}

// ATTENTION, YO BICHES!
// Before calling this routine, make sure that necessary shader program had been 'enabled' by glUseProgram(...)
// If not - this routine is plainly useless!
int BindUniform(const FpUniform& u, const char* name, const GLuint gl_program)
{
    GLint UniformHandle = glGetUniformLocation(gl_program, name );
    if (UniformHandle != -1) {

        if (u.type == FP32_VAL) {
            glUniform1f(UniformHandle, u.vals[0]);

        } else if (u.type == FP32_VECTOR) {
            glUniform1fv(UniformHandle, u.vals.size(), &u.vals[0]);

        } else if (u.type == FP32_MATRIX_2x2) {
            glUniformMatrix2fv(UniformHandle, 1, false, &u.vals[0]);

        } else if (u.type == FP32_MATRIX_3x3) {
            glUniformMatrix3fv(UniformHandle, 1, false, &u.vals[0]);

        } else if (u.type == FP32_MATRIX_4x4) {
            glUniformMatrix4fv(UniformHandle, 1, false, &u.vals[0]);

        } else {
            // something is seriously screwed ...
            throw std::runtime_error("ERROR in BindUniform( integer one) : Trying to bind uniform with erroneous type");
        }

        // check that actual uniform passing to GPU went fine
        if ( signal_of_opengl_error() )
            return 2;

    } else {
        // could not find specified uniform in specified program
        return 1;
    }

    return 0;
}


