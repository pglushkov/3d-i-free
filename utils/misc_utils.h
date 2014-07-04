#ifndef MY_MISC_UTILS_H
#define MY_MISC_UTILS_H

#include <string>
#include <vector>

#include "opengl.h"

namespace my_utils
{
        std::string GetFileExtension(const std::string& file_name);
        std::string ReadTextFromFile(const char* file);
        void ReadImageData(const char* file, std::vector<unsigned char>& buffer,
                unsigned int &width, unsigned int &height, GLuint &gl_flag);
}

#endif

