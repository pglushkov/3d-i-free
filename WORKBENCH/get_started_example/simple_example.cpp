
/* common includes */
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cmath>

/* x-server related includes */
#include<X11/X.h>
#include<X11/Xlib.h>

#include "stuff.h"

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

int main(int argc, char *argv[]) {

        // ===============================================================================================================
        // ========================= this is a basic code to create x-server window and pass a handle to it's context to GL
        // ===============================================================================================================

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

        // ===============================================================================================================
        // ================= end of x-server related setup code ==========================================================
        // ===============================================================================================================



        // enabling some global GL features ...
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);

        // SOME DIAG OUTPUT, NEVERMIND...
        printf(" gl version = %s ...\n", glGetString(GL_VERSION));
        printf(" glsl version = %s ...\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
        int max_tex_num;
        glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &max_tex_num);
        printf(" max number of textures = %d ...\n", max_tex_num);

        // GENERATING GEOMETRY -  ARRAY OF VERTICES AND ORDER OF THEIR TRAVERSAL
        float width = 1.0f;
        float height = 1.0f;
        std::vector<TstVertex> vertices;
        std::vector<unsigned short> vertices_order;
        generate_rectangle(width, height, vertices, vertices_order);

        // LOADING GENERATED GEOMETRY DATA TO THE GPU
        GLuint vertices_buffer_id, vert_order_buffer_id;
        load_geometry_to_gpu(vertices, vertices_order, vertices_buffer_id, vert_order_buffer_id);

        // CREATING COMPILED SHADER OBJECTS, CREATING GL PROGRAM FROM COMPILED SHADERS, LINKING THE PROGRAM
        // Keep in mind that as an actual shader coder is compiled when this program is run - you can freely modify a shader and than just
        // this program again - no need to recompile THIS program. Ofcourse its only true as long as you dont add/modify names of some
        // uniforms and vertex-attributes in the shader code.
        GLuint gl_program;
        create_shader_program("vshader_simple.txt", "fshader_simple.txt", gl_program);
        link_shader_program(gl_program);

        // FOR NOW THIS WILL BE THE ONLY UNIFORM, BY WICH WE'WILL TUNE OUR SHADER PROGRAM
        float intensity = 0.5f;

        // "ADDING" TEXTURES TO THE SHADER PROGRAM
        GLuint texture_id;
        load_texture_to_gpu("../../data/penguin.png", texture_id);


        // !!!!! ONE MAJOR THING TO UNDESTAND ABOUT THIS EXAMPLE !!!!!
        // Current example does not involve Projection Matrix yet. As a result we render directly to NDC (Native-Device-Coordinates) - a cube with
        // length/height/width of 2 units and centered in (0,0,0), LEFT-HANDED-CORTESIAN-COORDINATES. That is, X-axis points right, Y-axis points up,
        // Z-axis points INTO the screen! We can fully 'see' this cube, i.e. in a little perverted manner, we can say that in this example our
        // fictional ortographic camera is located at point (0,0,-1) and looks in +Z direction (into the screen that is). For this reason, the less is
        // z-coordinate of vertex - the closer it is to us in z-buffer. Another complication - with artographic projection we can get to feel of distance -
        // all objects are of the same size, no matter how far they are in Z.
        // If one enables a descent projection camera - the results will be:
        // - Z-axis will be inverted, it will start to point OUT OF the screen
        // - Coordinate system will become RIGHT-HANDED-CORTESIAN (as a result of z-invertion)
        // - Object will be drawn with a perspective
        // - We will see only half of NDC-cube - cause our 'fictional' camera will now be situated at point (0,0,0) looking towards -Z direction (INTO
        //      the scree that is)


        // MAIN EXECUTION LOOP - PICKING EVENTS FROM X-SERVER SYSTEM AND REACTING TO THEM
        while(true) {
                for (int i = 0; i < XPending(dpy); i++) // Have to process all events, cause XPending is a thread-blocking call
                {
                    XNextEvent(dpy, &xev);
                    switch(xev.type)
                    {
                        case KeyPress :
                        {
                            printf("KeyPress: keycode %u state %u\n", xev.xkey.keycode, xev.xkey.state);
                            switch (xev.xkey.keycode)
                            {
                                // just some common keycodes are processed ...

                                case 9: //ESC
                                        glXMakeCurrent(dpy, None, NULL);
                                        glXDestroyContext(dpy, glc);
                                        XDestroyWindow(dpy, win);
                                        XCloseDisplay(dpy);
                                        exit(0);
                                case 111: // up arrow
                                        // increase the intensity
                                        intensity += 0.05;
                                        break;
                                case 116: //down arrow
                                        // decrease the intensity
                                        intensity -= 0.05;
                                        break;
                                case 113: //left arrow
                                        break;
                                case 114: //right arrow
                                        break;
                                case 35: // right square bracket
                                        break;
                                case 34: // left square bracket
                                        break;
                                case 36: //Enter
                                        break;
                                case 38: // button 'a'
                                        break;
                                case 25: // button 'w'
                                        break;
                                case 39: // button 's'
                                        break;
                                case 40: // button 'd'
                                        break;
                                case 26: // button 'e'
                                        break;
                                case 24: // button 'q'
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

                /* ACTUAL DRAWING */
                draw_geometry(gl_program, vertices_buffer_id, vert_order_buffer_id, vertices_order.size(), intensity, texture_id);

                // SWAPPING CURRENTLY ON-SCREEN AND READY-TO-BE-RENDERED BUFFERS
                glXSwapBuffers(dpy, win);

        } // end of main execution loop

        //CLEAN-UP
        glDeleteProgram(gl_program);


} // end of main




