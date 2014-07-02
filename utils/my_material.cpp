
#include "my_material.h"
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>

static std::string ReadTextFromFile(const char* file)
{
        std::ifstream read;
        read.open(file, std::ifstream::in);
        std::vector<char> data;

        if( read.good() ) {
                read.seekg(0, std::ios::end);
                size_t file_sz = read.tellg();
                read.seekg(0);
                data.resize(file_sz + 1);
                read.read(&data[0], file_sz);
                data[file_sz] = '\0';

        } else {
                throw std::runtime_error("Could not open specified file!");
        }

        read.close();
        return std::string(&data[0]);
}

MyMaterial::MyMaterial(const char* vshader_file, const char* fshader_file)
{
        GLuint vert_shader, frag_shader;
        GLint length, result;

        std::string vshader(ReadTextFromFile(vshader_file));
        std::string fshader(ReadTextFromFile(fshader_file));

        /* create shader object, set the source, and compile */
        vert_shader = glCreateShader(GL_VERTEX_SHADER);
        frag_shader = glCreateShader(GL_FRAGMENT_SHADER);

        gl_program = glCreateProgram();

        length = vshader.length();
        const char* tmp = vshader.c_str();
        glShaderSource(vert_shader, 1, (const GLchar **)&tmp, &length);
        glCompileShader(vert_shader);
        length = fshader.length();
        tmp = fshader.c_str();
        glShaderSource(frag_shader, 1, (const GLchar **)&tmp, &length);
        glCompileShader(frag_shader);

        /* make sure the compilation was successful */
        glGetShaderiv(vert_shader, GL_COMPILE_STATUS, &result);
        if(result == GL_FALSE) {
                char *log;

                /* get the shader info log */
                glGetShaderiv(vert_shader, GL_INFO_LOG_LENGTH, &length);
                log = (char*)malloc(length);
                glGetShaderInfoLog(vert_shader, length, &result, log);

                /* print an error message and the info log */
                fprintf(stderr, "Error in MyMaterial(): Unable to compile vertex shader, log: %s\n", log);
                free(log);

                glDeleteShader(vert_shader);
                throw std::runtime_error("Error in MyMaterial() : could not compile vertex shader!");
        }

        glGetShaderiv(frag_shader, GL_COMPILE_STATUS, &result);
        if(result == GL_FALSE) {
                char *log;

                /* get the shader info log */
                glGetShaderiv(frag_shader, GL_INFO_LOG_LENGTH, &length);
                log = (char*)malloc(length);
                glGetShaderInfoLog(frag_shader, length, &result, log);

                /* print an error message and the info log */
                fprintf(stderr, "Error in MyMaterial(): Unable to compile fragment shader, log: %s\n", log);
                free(log);

                glDeleteShader(frag_shader);
                throw std::runtime_error("Error in MyMaterial() : could not compile fragment shader!");
        }

        /* attaching shader to the program ...*/
        glAttachShader(gl_program, vert_shader);
        glAttachShader(gl_program, frag_shader);

        /* delete the shader - it won't actually be
         * destroyed until the program that it's attached
         * to has been destroyed */
        glDeleteShader(vert_shader);
        glDeleteShader(frag_shader);

        glLinkProgram(gl_program);
        glGetProgramiv(gl_program, GL_LINK_STATUS, &result);
        if(result == GL_FALSE) {
                GLint length;
                char *log;

                /* get the program info log */
                glGetProgramiv(gl_program, GL_INFO_LOG_LENGTH, &length);
                log = (char*)malloc(length);
                glGetProgramInfoLog(gl_program, length, &result, log);

                /* print an error message and the info log */
                fprintf(stderr, "Error in MyMaterial(): Program linking failed, log: %s\n", log);
                free(log);

                /* delete the program */
                glDeleteProgram(gl_program);
                gl_program = 0;
                throw std::runtime_error("Error in MyMaterial() : could not link the shader program ...");
        }
}

MyMaterial::~MyMaterial()
{
        glDeleteProgram(gl_program);
}

void MyMaterial::UseMaterial(bool use)
{

        if (use) {
                glUseProgram(gl_program);
                print_opengl_error(" use mater 1");
        } else {
                /* glDontUseProgram :))) */;
        }
}

