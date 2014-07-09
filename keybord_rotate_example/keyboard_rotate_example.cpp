
/* common includes */
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <ctime>
#include <cmath>

/* x-server related includes */
#include<X11/X.h>
#include<X11/Xlib.h>

#include <utils/opengl.h>

#include <utils/my_mesh.h>
#include <utils/misc_utils.h>
#include <utils/my_position_matrix.h>

#include "small_tests.h"

#define ROTATION_INCREMENT 3.0f

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

int main(int argc, char *argv[]) {

        // test_free_image();
        // test_pos_mat();
        // return 0;

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
        glEnable(GL_BLEND);

        printf(" gl version = %s ...\n", glGetString(GL_VERSION));
        printf(" glsl version = %s ...\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
        int max_tex_num;
        glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &max_tex_num);
        printf(" max number of textures = %d ...\n", max_tex_num);

        // MyMesh mesh(geometry_gen::generate_rectangle(1.0f, 1.0f),
                      // "../shaders/vshader_attr.txt", "../shaders/fshader_attr.txt");
        // MyMesh mesh(geometry_gen::generate_rectangle(1.0f, 1.0f),
                      // "../shaders/vshader_attr.txt", "../shaders/fshader_tex.txt");
        // MyMesh mesh(geometry_gen::generate_rectangle_two_sides(1.0f, 1.0f),
                      // "../shaders/vshader_attr.txt", "../shaders/fshader_lambert.txt");
        MyMesh mesh(geometry_gen::generate_cube(1.0f),
                      "../shaders/vshader_attr.txt", "../shaders/fshader_lambert.txt");
        mesh.AddTexture("../data/penguin.bmp");
        // std::vector<unsigned char> tex = my_utils::GenerateRgbTexture(256, 256, 255, 0, 255);
        // mesh.AddTexture(tex, 256, 256, GL_RGB);

        //mesh.TRACE_GEOM();

        MyPositionMatrix<float> mat1;

        //mat1.Translate(std::array<float, 3> ({0.2f, 0.2f, 0.0f}));

        while(true) {
                for (int i = 0; i < XPending(dpy); i++) 
                {
                    XNextEvent(dpy, &xev);
                    switch(xev.type)
                    {
                        case KeyPress :
                        {
                            printf("KeyPress: keycode %u state %u\n", xev.xkey.keycode, xev.xkey.state);
                            switch (xev.xkey.keycode) 
                            {
                                case 9: //ESC
                                        glXMakeCurrent(dpy, None, NULL);
                                        glXDestroyContext(dpy, glc);
                                        XDestroyWindow(dpy, win);
                                        XCloseDisplay(dpy);
                                        exit(0);
                                case 111: // up arrow
                                        mat1.Rotate_X(-ROTATION_INCREMENT);
                                        break;
                                case 116: //down arrow
                                        mat1.Rotate_X(ROTATION_INCREMENT);
                                        break;
                                case 113: //left arrow
                                        mat1.Rotate_Y(-ROTATION_INCREMENT);
                                        break;
                                case 114: //right arrow
                                        mat1.Rotate_Y(ROTATION_INCREMENT);
                                        break;
                                case 36: //Enter
                                        mat1.Reset();
                                        break;
                                default:
                                        break;
                            }
                        }
                        default:
                            break;
                    }
                }

                //glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                GLint rot = glGetUniformLocation(mesh.GetShaderProgramHandle(), "world_view_position");
                if (rot != -1) {
                        glUniformMatrix4fv(rot, 1, false, &mat1.get_data()[0][0]);
                }

                GLint light = glGetUniformLocation(mesh.GetShaderProgramHandle(), "light_dir");
                if (light != -1) {
                        glUniform3f(light, .0f, .0f, -1.0f);
                }

                mesh.draw();

                glXSwapBuffers(dpy, win);

        } /* this closes while(true) { */
} /* this is the } which closes int main(int argc, char *argv[]) { */




