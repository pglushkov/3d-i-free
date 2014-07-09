
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

        // MyMesh square(geometry_gen::generate_rectangle(1.0f, 1.0f),
                      // "../shaders/vshader_attr.txt", "../shaders/fshader_attr.txt");
        // MyMesh square(geometry_gen::generate_rectangle(1.0f, 1.0f),
                      // "../shaders/vshader_attr.txt", "../shaders/fshader_tex.txt");
        MyMesh square(geometry_gen::generate_rectangle(1.0f, 1.0f),
                      "../shaders/vshader_attr.txt", "../shaders/fshader_lambert.txt");
        square.AddTexture("../data/penguin.bmp");
        // std::vector<unsigned char> tex = my_utils::GenerateRgbTexture(256, 256, 255, 0, 255);
        // square.AddTexture(tex, 256, 256, GL_RGB);

        //square.TRACE_GEOM();

        MyPositionMatrix<float> mat1;
        float phase = 0.0f;

        mat1.Translate(std::array<float, 3> ({0.2f, 0.2f, 0.0f}));

        while(true) {
                for (int i = 0; i < XPending(dpy); i++) 
                {
                    XNextEvent(dpy, &xev);
                    switch(xev.type)
                    {
                        case KeyPress :
                        {
                            printf("KeyPress: keycode %u state %u\n", xev.xkey.keycode, xev.xkey.state);
                            if(xev.xkey.keycode == 9 || xev.xkey.keycode == 61 )
                            {
                                glXMakeCurrent(dpy, None, NULL);
                                glXDestroyContext(dpy, glc);
                                XDestroyWindow(dpy, win);
                                XCloseDisplay(dpy);
                                exit(0);
                            }                         }
                        default:
                            break;
                    }
                }

                //glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                // just as an example, passing position matrix to shader ...
                phase += 0.1;
                float new_scale = 1 + 0.001*cos(phase/100);
                float new_pos = 0.2*cos(phase);
                mat1.Rotate_Y(1.0f); // rotate our position by 1 deg
                mat1.Scale(new_scale);
                mat1.SetPosition(std::array<float, 3> ({new_pos, new_pos, 0.0f}));
                GLint rot = glGetUniformLocation(square.GetShaderProgramHandle(), "world_view_position");
                if (rot != -1) {
                        glUniformMatrix4fv(rot, 1, false, &mat1.get_data()[0][0]);
                }

                GLint light = glGetUniformLocation(square.GetShaderProgramHandle(), "light_dir");
                if (light != -1) {
                        glUniform3f(light, -1.0f, 0.0f, -1.0f);
                }

                square.draw();

                glXSwapBuffers(dpy, win);

        } /* this closes while(true) { */
} /* this is the } which closes int main(int argc, char *argv[]) { */




