 /*-- Written in C -- */

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

//#include <utils/my_material.h>
//#include <utils/my_geometry.h>
#include <utils/my_mesh.h>
#include <utils/simple_vectors.h>

#include <FreeImage.h>

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

void test_free_image()
{
        FIBITMAP* bmap = FreeImage_Load(FIF_JPEG, "../data/green_red.jpg", JPEG_DEFAULT);

        /* print something about it ... */
        std::cout << "FIBITMAP = " << bmap << ", BPP = " << FreeImage_GetBPP(bmap)
                  << ", Width = " << FreeImage_GetWidth(bmap) << ", Height = " << FreeImage_GetHeight(bmap)
                  << ", ColorType = " << FreeImage_GetColorType(bmap) << ", DataBits = "
                  << (void*)FreeImage_GetBits(bmap) << std::endl;

        unsigned int bytespp = FreeImage_GetLine(bmap); /* getting number of bytes-per-pixel */
        for (size_t h = 0; h < FreeImage_GetHeight(bmap); ++h) {
                /* getting address of first pixel in current line */
                BYTE* bits = FreeImage_GetScanLine(bmap, h);
                for (unsigned int w = 0; w < FreeImage_GetWidth(bmap); ++w) {

                        /* do smth with current pixel */
                        std::cout << " RGB (" << h + 1 << ":" << w + 1 << "): "
                                << (int)bits[0] << "," << (int)bits[1] << "," << (int)bits[2];
                        /* jump to next pixel in current line*/
                        bits += bytespp;
                }
                std::cout << std::endl;
        }

        FreeImage_Unload(bmap);
}

int main(int argc, char *argv[]) {

        test_free_image();

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

        MyMesh square(geometry_gen::generate_rectangle(1.0f, 0.5f), "vshader_attr.txt", "fshader_attr.txt");

        square.TRACE_GEOM();

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
                            }
                        }
                        default:
                            break;
                    }
                }

                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                square.draw();

                glXSwapBuffers(dpy, win);

        } /* this closes while(true) { */
} /* this is the } which closes int main(int argc, char *argv[]) { */




