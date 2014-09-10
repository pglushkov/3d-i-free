
#ifndef MY_OPENGL_INCLUDES
#define MY_OPENGL_INCLUDES

#define GL_GLEXT_PROTOTYPES

// #include <GL/glut.h>

/* opengl related includes */
#include <GL/gl.h>
#include <GL/glext.h>
#include <GL/glx.h>

void print_opengl_error(const char* where);
GLenum get_opengl_last_error();
bool signal_of_opengl_error();

#endif /* of MY_OPENGL_INCLUDES */

