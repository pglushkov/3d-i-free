
/* common includes */
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
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

#define WIN_WIDTH 640
#define WIN_HEIGHT 480
#define ASPECT_RATIO (float)WIN_WIDTH / (float)WIN_HEIGHT

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

        win = XCreateWindow(dpy, root, 0, 0, WIN_WIDTH, WIN_HEIGHT, 0, vi->depth, InputOutput, vi->visual, CWColormap | CWEventMask, &swa);

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

        /* GENERATING GEOMETRY */
        MyGeometry geom(geometry_gen::generate_cube(1.0f));
//        MyGeometry geom(geometry_gen::generate_rectangle(1.0f, 1.0f));
//        MyGeometry geom(geometry_gen::generate_rectangle_two_sides(1.0f, 1.0f));

        /* SELECTING SHADERS TO DRAW */
        /* VERTEX SHADER */
        //std::string vshader("../shaders/vshader_simple.txt");
        std::string vshader("../shaders/vshader_full.txt");
        std::string vshader1("../shaders/vshader_y_spin.txt");
        std::string vshader2("../shaders/vshader_simple");
        /* FRAGMENT SHADER */
        std::string fshader("../shaders/fshader_lambert.txt");
        std::string fshader1("../shaders/fshader_tex.txt");
        std::string fshader2("../shaders/fshader_simple.txt");

        /* GENERATING A DRAWABLE OBJECT (MESH) */
        MyMesh mesh(geom, vshader.c_str(), fshader.c_str());
        MyMesh mesh2(geom, vshader1.c_str(), fshader1.c_str());
        //mesh.TRACE_GEOM();

        /* TEXTURING */
        mesh2.AddTexture("../data/penguin.bmp");
//        std::vector<unsigned char> tex = my_utils::GenerateRgbTexture(256, 256, 255, 0, 255);
//        mesh.AddTexture(tex, 256, 256, GL_RGB);

        /* INITIALIZING OBJECT POSITION */
        std::array<float, 3> default_pos({ 0.0f, 0.0f, -6.0f});
        std::array<float, 3> default_pos2({ 2.0f, 2.5f, -10.0f});

	mesh.GetObjTransform().SetPosition(default_pos);
	mesh2.GetObjTransform().SetPosition(default_pos2);

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
                                        mesh.GetObjTransform().Rotate_X(ROTATION_INCREMENT);
                                        break;
                                case 116: //down arrow
                                        mesh.GetObjTransform().Rotate_X(-ROTATION_INCREMENT);
                                        //matWorldPos.Translate(std::array<float, 3> ({0.0f, 0.0f, 0.1f}));
                                        break;
                                case 113: //left arrow
                                        mesh.GetObjTransform().Rotate_Y(ROTATION_INCREMENT);
                                        break;
                                case 114: //right arrow
                                        mesh.GetObjTransform().Rotate_Y(-ROTATION_INCREMENT);
                                        break;
                                case 35: // right square bracket
                                        mesh.GetObjTransform().Rotate_Z(ROTATION_INCREMENT);
                                        break;
                                case 34: // left square bracket
                                        mesh.GetObjTransform().Rotate_Z(-ROTATION_INCREMENT);
                                        break;
                                case 36: //Enter
                                        mesh.GetObjTransform().Reset();
                                        mesh.GetObjTransform().SetPosition(default_pos);
                                        break;
                                default:
                                        break;
                            }
                        }
                        default:
                            break;
                    }
                }

                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                mesh.draw();
                mesh2.draw();

                glXSwapBuffers(dpy, win);

        } /* this closes while(true) { */
} /* this is the } which closes int main(int argc, char *argv[]) { */




