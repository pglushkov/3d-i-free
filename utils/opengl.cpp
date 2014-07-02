

#include "opengl.h"

#include <stdio.h>

void print_opengl_error(const char* where)
{
        GLenum err;
        err = glGetError();
        if(err != GL_NO_ERROR)
            printf("GL error: %d ... (at \"%s\") \n", err, where);
}

