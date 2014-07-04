
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
        void UseMaterial(bool use);
        const GLuint GetProgramHandle() const { return gl_program; }
        void AddTexture(const char* texture_file_name);
        ~MyMaterial();
};

