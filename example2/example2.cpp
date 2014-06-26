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
#include <utils/shader.h>

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

GLfloat square_vertices[] =
{
        /*x    y      z       color                     plane-coordinate     */
        -0.5f, 0.5f, 0.0f,    0.0f, 1.0f, 0.0f,         0.0f, 0.0f,
        -0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,         0.0f, 1.0f,
        0.5f, -0.5f, 0.0f,    1.0f, 0.0f, 0.0f,         1.0f, 1.0f,
        0.5f, 0.5f, 0.0f,     1.0f, 0.0f, 0.0f,         1.0f, 0.0f,
        -0.5f, 0.5f, 0.0f,    0.0f, 1.0f, 0.0f,         0.0f, 0.0f,
};

uint16_t vert_indices[] =
{
        0, 1, 2,
        2, 0, 3,
};

float getTimePassed()
{
        static float last_time;

        float time = (GLfloat)clock() / (GLfloat)CLOCKS_PER_SEC;

        return time - last_time;
}

void printGlError(const char* where)
{
        GLenum err;
        err = glGetError();
        printf("Current GL error: %d ... (at \"%s\") \n", err, where);
}

void DrawTriangle() {
        glClearColor(1.0, 1.0, 1.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(-1., 1., -1., 1., 1., 20.);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        //gluLookAt(0., 0., 10., 0., 0., 0., 0., 1., 0.);

        glBegin(GL_TRIANGLE_STRIP);
        glColor3f(0., 0., 0.); glVertex3f(-.75, -.75, 0.);
        glColor3f(0., 0., 0.); glVertex3f( .75, -.75, 0.);
        glColor3f(0., 0., 0.); glVertex3f( .75,  .75, 0.);
        glColor3f(1., 1., 0.); glVertex3f(-.75,  .75, 0.);
        glEnd();
}

void bindAttributesWithShaders(GLuint gl_program)
{
        /* specifying where actial attributes are located in buffered data, so that shaders could use it */

        /* basic positions in 'absolute' space */
        GLint positionAttr = glGetAttribLocation(gl_program, "in_position");
        glVertexAttribPointer(positionAttr, 3, GL_FLOAT, GL_FALSE, sizeof(float)*8 /*stride!*/, 0 /*offset*/);
        glEnableVertexAttribArray(positionAttr);

        /* default colors of vertices */
        GLint colorAttr = glGetAttribLocation(gl_program, "in_color");
        glVertexAttribPointer(colorAttr, 3, GL_FLOAT, GL_FALSE, sizeof(float)*8, (const void *)(3*sizeof(float)));
        glEnableVertexAttribArray(colorAttr);

        /* 'plane' coordenates of vertices */
        GLint pcoordAttr = glGetAttribLocation(gl_program, "in_plane_coord");
        glVertexAttribPointer(pcoordAttr, 3, GL_FLOAT, GL_FALSE, sizeof(float)*8, (const void *)(6*sizeof(float)));
        glEnableVertexAttribArray(pcoordAttr);

        /* passing time increment as a uniform ... */
        GLfloat time = getTimePassed();
        printf("DBG : time_passed = %f ...\n", time);
        GLint timeUniformHandle = glGetUniformLocation(gl_program, "time");
        glUniform1f(timeUniformHandle, time);
}

void prepareArrayForSquareDraw(GLuint &vao, GLuint& vbo)
{
        /* initializing vertex-array-object and making it 'current' */
        glGenVertexArrays(1, &vao);
        /* DBG */
        printf("Created vertex-array-object : %u ...\n", vao);
        glBindVertexArray(vao);

        /* buffering actual data into gl-buffer-object*/
        glGenBuffers(1, &vbo);
        printf("Created vertex-buffer-object : %u ...\n", vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(square_vertices), square_vertices, GL_STATIC_DRAW);

        glBindVertexArray(0);
}

void drawFigure(GLuint vao, GLuint vbo, unsigned int num_vertices)
{


        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, num_vertices);
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

        GLuint squareVao, squareVbo;
        prepareArrayForSquareDraw(squareVao, squareVbo);
        /* DBG */
        printf("Resulting vao=%u, vbo = %u ...\n", squareVao, squareVbo);


        GLint result;
        GLuint g_program = glCreateProgram();

        shaderAttachFromFile(g_program, GL_VERTEX_SHADER, "vshader_attr.txt");
        shaderAttachFromFile(g_program, GL_FRAGMENT_SHADER, "fshader_attr.txt");

        glLinkProgram(g_program);
        glGetProgramiv(g_program, GL_LINK_STATUS, &result);
        if(result == GL_FALSE) {
                GLint length;
                char *log;

                /* get the program info log */
                glGetProgramiv(g_program, GL_INFO_LOG_LENGTH, &length);
                log = (char*)malloc(length);
                glGetProgramInfoLog(g_program, length, &result, log);

                /* print an error message and the info log */
                fprintf(stderr, "sceneInit(): Program linking failed: %s\n", log);
                free(log);

                /* delete the program */
                glDeleteProgram(g_program);
                g_program = 0;
        }

        bindAttributesWithShaders(g_program);

        while(true) {
                XNextEvent(dpy, &xev);
                if(xev.type == Expose) {
                        XGetWindowAttributes(dpy, win, &gwa);

                        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                } else if(xev.type == KeyPress) {
                        /* DBG ... */
                        printf("KeyPress: keycode %u state %u\n", xev.xkey.keycode, xev.xkey.state);

                        if (xev.xkey.keycode == 9 ) {
                                glXMakeCurrent(dpy, None, NULL);
                                glXDestroyContext(dpy, glc);
                                XDestroyWindow(dpy, win);
                                XCloseDisplay(dpy);
                                exit(0);
                        }
                }

                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                glUseProgram(g_program);
                bindAttributesWithShaders(g_program);
                drawFigure(squareVao, squareVbo, 5);

                //DrawTriangle();
                glXSwapBuffers(dpy, win);

        } /* this closes while(1) { */
} /* this is the } which closes int main(int argc, char *argv[]) { */




