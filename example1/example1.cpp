 /*-- Written in C -- */

/* common includes */
#include <stdio.h>
#include <stdlib.h>
#include <stdexcept>
#include <iostream>
#include <string>

/* x-server related includes */
#include <X11/X.h>
#include <X11/Xlib.h>

#include <utils/opengl.h>
#include "shaders_code.h"
#include "my_material.h"

Display                 *dpy;
Window                  root;
GLint                   att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
XVisualInfo             *vi;
Colormap                cmap;
XSetWindowAttributes    swa;
Window                  win;
GLXContext              glc;
XWindowAttributes       gwa;
XEvent                  xev;

void DrawTriangle() {
        glClearColor(1.0, 1.0, 1.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(-1., 1., -1., 1., 1., 20.);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        gluLookAt(0., 0., 10., 0., 0., 0., 0., 1., 0.);

        glBegin(GL_TRIANGLES);
        glColor3f(0., 0., 0.); glVertex3f(-.75, -.75, 0.);
        glColor3f(0., 0., 0.); glVertex3f( .75, -.75, 0.);
        glColor3f(0., 0., 0.); glVertex3f( .75,  .75, 0.);
        glColor3f(0., 0., 0.); glVertex3f( -.75,  .75, 0.);
        glColor3f(0., 0., 0.); glVertex3f( -.75,  -.75, 0.);
        glColor3f(0., 0., 0.); glVertex3f( .75,  .75, 0.);
        glEnd();
}

void DrawSquare(GLuint shader_program)
{
        float vertices[8] = {
                -0.5f, 0.5f,
                0.5f, 0.5f,
                0.5f, -0.5f,
                -0.5f, -0.5f,
        };

        GLuint vao;
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        GLuint vbo;
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        /*
         * GLint posAttrib = glGetAttribLocation(shader_program, "position");
         * glEnableVertexAttribArray(posAttrib);
         * glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
         */

        glClearColor(1.0, 1.0, 1.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);
        glDrawArrays(GL_TRIANGLES, 0, 3);
}

int main(int argc, char *argv[]) {

        dpy = XOpenDisplay(NULL);

        if(dpy == NULL) {
                printf("\n\tcannot connect to X server\n\n");
                exit(0);
        }

        root = DefaultRootWindow(dpy);

        vi = glXChooseVisual(dpy, 0, att);

	if(vi == NULL) {
		printf("\n\tno appropriate visual found\n\n");
		exit(0);
	} else {
		printf("\n\tvisual %p selected\n", (void *)vi->visualid); /* %p creates hexadecimal output like in glxinfo */
	}


	cmap = XCreateColormap(dpy, root, vi->visual, AllocNone);

	swa.colormap = cmap;
	swa.event_mask = ExposureMask | KeyPressMask;

	win = XCreateWindow(dpy, root, 0, 0, 600, 600, 0, vi->depth, InputOutput, vi->visual, CWColormap | CWEventMask, &swa);

	XMapWindow(dpy, win);
	XStoreName(dpy, win, "VERY SIMPLE APPLICATION (PRESS ESC TO EXIT!) ");

	glc = glXCreateContext(dpy, vi, NULL, GL_TRUE);
	glXMakeCurrent(dpy, win, glc);

	glEnable(GL_DEPTH_TEST);

        printf(" gl version = %s ...\n", glGetString(GL_VERSION));
        printf(" glsl version = %s ...\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

        MyMaterial* mat;
        try {
                mat = new MyMaterial(shaders_code::simple_v_shader, shaders_code::simple_f_shader);
        } catch (std::runtime_error e) {
                std::cout << "Caught an exception while creating MyMaterial instance, message: " << e.what()
                        << std::endl;
        }

        bool use_flag = true;
        mat->UseMaterial(use_flag);

	while(1) {
		XNextEvent(dpy, &xev);
		if(xev.type == Expose) {
			XGetWindowAttributes(dpy, win, &gwa);
			glViewport(0, 0, gwa.width, gwa.height);
                        DrawTriangle();
                        // DrawSquare(g_program);
			glXSwapBuffers(dpy, win);
		} else if(xev.type == KeyPress) {
			/* DBG ... */
			printf("KeyPress: keycode %u state %u\n", xev.xkey.keycode, xev.xkey.state);

                        mat->UseMaterial(use_flag);
                        use_flag = !use_flag;

			if (xev.xkey.keycode == 9 ) {
				glXMakeCurrent(dpy, None, NULL);
				glXDestroyContext(dpy, glc);
				XDestroyWindow(dpy, win);
				XCloseDisplay(dpy);
                                delete mat;
				exit(0);
			}
		}
	} /* this closes while(1) { */
} /* this is the } which closes int main(int argc, char *argv[]) { */
