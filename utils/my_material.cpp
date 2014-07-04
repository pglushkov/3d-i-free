
#include "my_material.h"
#include <memory.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <FreeImage.h>

#include <utils/misc_utils.h>

int MyMaterial::_max_textures = 0;

MyMaterial::MyMaterial(const char* vshader_file, const char* fshader_file)
{
        GLuint vert_shader, frag_shader;
        GLint length, result;

        if (_max_textures == 0)
                glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &_max_textures);
        textures.resize(0);

        std::string vshader(my_utils::ReadTextFromFile(vshader_file));
        std::string fshader(my_utils::ReadTextFromFile(fshader_file));

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
        if (textures.size() > 0)
                glDeleteTextures(textures.size(), &textures[0]);
}

void MyMaterial::UseMaterial(bool use)
{
        char tex_name[20];
        if (use) {
                glUseProgram(gl_program);
                for (unsigned int k = 0; k < textures.size(); ++k) {
                        snprintf(tex_name, 20, "texture%d", k + 1);

                        glActiveTexture(GL_TEXTURE0 + k);
                        glBindTexture(GL_TEXTURE_2D, textures[k]);
                        GLint uniform = glGetUniformLocation(gl_program, tex_name);
                        if (uniform != -1) {
                                glUniform1i(uniform, textures[k]);
                        }

                        /* DBG */
                        // printf("Texture:%s , found id = %u , texture_for_id = %u ...\n", tex_name, uniform, textures[k]);
                }
        } else {
                /* glDontUseProgram :))) */;
        }
}

void MyMaterial::AddTexture(const char* texture_file_name)
{
        if ( (int)textures.size() >= _max_textures)
                throw std::runtime_error("Error! Maximum allowed number of textures reached!");

        std::vector<BYTE> tex_data;

        unsigned int width, height;
        GLuint gl_flag;

        my_utils::ReadImageData(texture_file_name, tex_data, width, height, gl_flag);

        /* DBG */
        printf("In AddTexture: width=%u height = %u, gl_flag=%u ...\n", width, height, gl_flag);

        GLuint tmp;
        glGenTextures(1, &tmp);
        glActiveTexture(GL_TEXTURE0 + textures.size());
        glBindTexture(GL_TEXTURE_2D, tmp);

        glTexImage2D(GL_TEXTURE_2D, 0, gl_flag, width, height, 0, gl_flag, GL_UNSIGNED_BYTE, &tex_data[0]);

        /* DBG */
        print_opengl_error(" after loading texture ");

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        textures.push_back(tmp);

        /* DBG */
        printf(" current list of textures: ");
        for (size_t k = 0; k < textures.size(); ++k)
                printf(" %u ", textures[k]);
        printf("\n");
}

