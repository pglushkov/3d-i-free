
#include <string>
#include <stdexcept>
#include <utils/opengl.h>

class MyMaterial
{
        GLuint gl_program;

        MyMaterial(const MyMaterial& inp) {};
        const MyMaterial& operator=(const MyMaterial& inp) { return *this; }
public:
        MyMaterial(const std::string& vshader, const std::string& fshader);
        void UseMaterial(bool use);
        ~MyMaterial();
};

