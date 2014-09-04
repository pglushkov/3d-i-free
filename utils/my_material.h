#ifndef MY_MATERIAL_H
#define MY_MATERIAL_H

#include <string>
#include <stdexcept>
#include <vector>

#include <utils/opengl.h>

class MyMaterial
{
        static int _max_textures;

        GLuint gl_program;
        std::vector<GLuint> textures;

        MyMaterial(const MyMaterial& inp) {};
        const MyMaterial& operator=(const MyMaterial& inp) { return *this; }
public:
        MyMaterial(const char* vshader_file, const char* fshader_file);
        MyMaterial();
        void UseMaterial(bool use) const;
        const GLuint GetProgramHandle() const { return gl_program; }
        void AddTexture(const char* texture_file_name);
        void AddTexture(std::vector<unsigned char>& data, unsigned int width, unsigned int height, GLuint gl_flag);
        ~MyMaterial();
};


#endif //of MY_MATERIAL_H

