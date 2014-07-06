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

#include <utils/my_mesh.h>
#include <utils/simple_vectors.h>
#include <utils/misc_utils.h>
#include <utils/my_position_matrix.h>

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

void test_pos_mat()
{
        // MySquareMatrix<float, 4> a;
        // MySquareMatrix<float, 4> a1;

        // a.TRACE();

        // std::cout << a[1][2] << std::endl;
        // //std::cout << a[4][0] << std::endl; //this line will cause an exception!

        // for (size_t k = 0; k < a.size; ++k)
                // for (size_t m = 0; m < a.size; ++m) {
                        // a[k][m] = float(k*m);
                        // a1[k][m] = 3.0f;
                // }

        // a.TRACE();
        // a1.TRACE();
        // a1 *= a;
        // a1.TRACE();

        // auto b = CreateOXRotation<float> (30.0f);
        // b.TRACE();

        // a = b;
        // a.TRACE();


        MyPositionMatrix<float> c;
        c.TRACE();
        c.Rotate_Z(30);
        c.TRACE();


        MySquareMatrix<float, 4> :: data_row v;
        v[0] = 1.01; v[1] = 1.0f; v[2] = 1.0f; v[3] = 1.0f;

        Vec4<float> v1(c * v);
        v1.TRACE();

        MyPositionMatrix<float> c1;
        c1.TRACE();
        float dbg[16];
        memcpy(dbg, &c1.get_data()[0][0], sizeof(float) * 16);
        printf("Results of memcpy from MyPositionMatrix.get_data() : ");
        for (unsigned int k = 0; k < 16; ++k)
                printf(" %f ", dbg[k]);
        printf("\n ************* \n");
        printf(" size of data_row: %lu \n",sizeof(MyPositionMatrix<float>::data_row));
        printf(" size of full_data: %lu \n",sizeof(MySquareMatrix<float, 4>::full_data));
}

void test_free_image()
{
        FIBITMAP* bmap = FreeImage_Load(FIF_JPEG, "../data/green_red.jpg", JPEG_DEFAULT);
        //FIBITMAP* bmap = FreeImage_Load(FIF_BMP, "../data/green_red.bmp", BMP_DEFAULT);

        /* print something about it ... */
        std::cout << "FIBITMAP = " << bmap << ", BPP = " << FreeImage_GetBPP(bmap)
                  << ", Width = " << FreeImage_GetWidth(bmap) << ", Height = " << FreeImage_GetHeight(bmap)
                  << ", ColorType = " << FreeImage_GetColorType(bmap) << ", DataBits = "
                  << (void*)FreeImage_GetBits(bmap) << std::endl;

        unsigned int bytespl = FreeImage_GetLine(bmap); /* getting number of bytes-per-line */
        unsigned int bytespp = bytespl / FreeImage_GetWidth(bmap);
        std::cout << "Bytes-Per-Line: " << bytespl << ", Bytes-Per-Pixel: " << bytespp << std::endl;
        for (size_t h = 0; h < FreeImage_GetHeight(bmap); ++h) {
                /* getting address of first pixel in current line */
                BYTE* bits = FreeImage_GetScanLine(bmap, h);
                for (unsigned int w = 0; w < FreeImage_GetWidth(bmap); ++w) {

                        /* do smth with current pixel */
                        // std::cout << " RGB (" << h + 1 << ":" << w + 1 << "): "
                               // << (int)bits[FI_RGBA_RED] << "," << (int)bits[FI_RGBA_GREEN]
                               // << "," << (int)bits[FI_RGBA_BLUE];
                        /* jump to next pixel in current line*/
                        bits += bytespp;
                }
                // std::cout << std::endl;
        }

        FreeImage_Unload(bmap);
}

int main(int argc, char *argv[]) {

        //test_free_image();
        //test_pos_mat();
        //return 0;

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

        MyMesh square(geometry_gen::generate_rectangle(1.0f, 1.0f),
                      "../shaders/vshader_attr.txt", "../shaders/fshader_attr.txt");
        //square.AddTexture("../data/green.bmp");
        std::vector<unsigned char> tex = my_utils::GenerateRgbTexture(256, 256, 0, 0, 255);
        square.AddTexture(tex, 256, 256, GL_RGB);

        square.TRACE_GEOM();

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
                            }
                        }
                        default:
                            break;
                    }
                }

                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                // just as an example, passing position matrix to shader ...
                phase += 0.1;
                float new_scale = 1 + 0.001*cos(phase/100);
                float new_pos = 0.2*cos(phase);
                mat1.Rotate_Z(1.0f); // rotate our position by 1 deg
                mat1.Scale(new_scale);
                //mat1.Translate(std::array<float, 3> ({new_pos, new_pos, 0.0f}));
                mat1.SetPosition(std::array<float, 3> ({new_pos, new_pos, 0.0f}));
                printf("phase = %f , new scale = %f , new_pos = %f ...\n", phase, new_scale, new_pos);
                GLint rot = glGetUniformLocation(square.GetShaderProgramHandle(), "rotation");
                if (rot != -1) {
                        glUniformMatrix4fv(rot, 1, false, &mat1.get_data()[0][0]);
                }

                square.draw();

                glXSwapBuffers(dpy, win);

        } /* this closes while(true) { */
} /* this is the } which closes int main(int argc, char *argv[]) { */




