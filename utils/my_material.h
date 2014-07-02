
#include <string>
#include <stdexcept>
#include <utils/opengl.h>

class MyMaterial
{
        GLuint gl_program;

        MyMaterial(const MyMaterial& inp) {};
        const MyMaterial& operator=(const MyMaterial& inp) { return *this; }
public:
        MyMaterial(const char* vshader_file, const char* fshader_file);
        void UseMaterial(bool use);
        const GLuint GetProgramHandle() const { return gl_program; }
        ~MyMaterial();
};

