

#include "opengl.h"

#include <stdio.h>

void print_opengl_error(const char* where)
{
        GLenum err;
        err = glGetError();
        if(err != GL_NO_ERROR)
            printf("GL error: %d ... (at \"%s\") \n", err, where);
}

GLenum get_opengl_last_error()
{
        return glGetError();
}

bool signal_of_opengl_error()
{
    GLenum err;
    err = glGetError();
    if(err != GL_NO_ERROR)
        return true;
    else
        return false;
}
