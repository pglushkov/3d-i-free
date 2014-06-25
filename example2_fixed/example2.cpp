 /*-- Written in C -- */

/* common includes */
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>

/* x-server related includes */
#include<X11/X.h>
#include<X11/Xlib.h>

#include "./../utils/opengl.h"
#include "./../utils/shader.h"

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


std::vector<float> square_vertices = 
//   x      y     z         tx    ty
{ 
   -0.5f,  0.5f, 0.0f,     0.0f, 0.0f,
    0.5f,  0.5f, 0.0f,     1.0f, 0.0f,
    0.5f, -0.5f, 0.0f,     1.0f, 1.0f,
   -0.5f, -0.5f, 0.0f,     0.0f, 1.0f
};
std::vector<uint16_t> square_indicies = {1,0,3, 3,2,1}; 


void printGlError(const char* where)
{
        GLenum err;
        err = glGetError();
        if(err != GL_NO_ERROR)
            printf("GL error: %d ... (at \"%s\") \n", err, where);
}


void loadGeometryToGPU(GLuint &vId, GLuint &iId )
{
    auto v_size = square_vertices.size() * sizeof(float);
    auto i_size = square_indicies.size() * sizeof(uint16_t);
    glGenBuffers(1, &vId);
    glGenBuffers(1, &iId);
    glBindBuffer(GL_ARRAY_BUFFER,vId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iId);
    glBufferData(GL_ARRAY_BUFFER,v_size,&square_vertices[0], GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,i_size,&square_indicies[0], GL_STATIC_DRAW);
    printf("geometry: vert : %lu bytes; ndx: %lu bytes\n", v_size, i_size);
}



GLuint createMaterial(const char * vsh, const char * fsh)
{
    GLuint g_program = glCreateProgram();
    shaderAttachFromFile(g_program, GL_VERTEX_SHADER, vsh);
    shaderAttachFromFile(g_program, GL_FRAGMENT_SHADER, fsh);
    glLinkProgram(g_program);
    GLint result;
    glGetProgramiv(g_program, GL_LINK_STATUS, &result);
    if(result == GL_FALSE) 
    {
        GLint length;
        char *log;
        glGetProgramiv(g_program, GL_INFO_LOG_LENGTH, &length);
        log = (char*)malloc(length);
        glGetProgramInfoLog(g_program, length, &result, log);
        fprintf(stderr, "sceneInit(): Program linking failed: %s\n", log);
        free(log);
        glDeleteProgram(g_program);
        g_program = 0;
    }
    return g_program;
}

void bindAttributes(GLuint gl_program)
{
        GLint pos_attr = glGetAttribLocation(gl_program, "in_position");
        GLint tc_attr  = glGetAttribLocation(gl_program, "tcoordinate");

        printGlError("after getting in_position location");
        if(pos_attr != -1)
        {    
            glVertexAttribPointer(
                pos_attr,                     
                3,                   // размер аттрибута - 3 флоата, позиция    
                GL_FLOAT,                 
                GL_FALSE,                 
                sizeof(float) * 5,   // размер вертекса (у нас сейчас - 5 флоатов)    
                (const void *)(0)    // смешение в вертексе до интересующего нас значения          
            );
            glEnableVertexAttribArray(pos_attr);
        }
        if(tc_attr != -1)
        {
            glVertexAttribPointer(
                tc_attr,  
                2,                       
                GL_FLOAT,                 
                GL_FALSE,                 
                sizeof(float) * 5,       
                (const void *)(3*sizeof(float)) 
            );
            glEnableVertexAttribArray(tc_attr);
        }   
        printGlError("after glVertexAttribPointer");
}


void draw(GLuint vertexID, GLuint indexID, GLuint materialID)
{
    glUseProgram(materialID);                        // текущая программа
    glBindBuffer(GL_ARRAY_BUFFER,vertexID);          // текущий буфер вершин
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,indexID);   // текущие индексы
      
    bindAttributes(materialID); // размечаем аттрибуты
   
    // заполняем униформы :

    GLint timeUniformHandle = glGetUniformLocation(materialID, "time");
    GLfloat time = (GLfloat)clock() / (GLfloat)CLOCKS_PER_SEC  * 10.0;
    glUniform1f(timeUniformHandle, time);
    
    // рисуем

    glDrawElements(GL_TRIANGLES, square_indicies.size(),GL_UNSIGNED_SHORT,(const void *)(0)); 
}


int main() 
{

    dpy = XOpenDisplay(NULL);
    if(!dpy) 
    {
        printf("\n\tcannot connect to X server\n\n");
        exit(0);
    }
    root = DefaultRootWindow(dpy);
    vi = glXChooseVisual(dpy, 0, att);
    if(vi == NULL) 
    {
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
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    XGetWindowAttributes(dpy, win, &gwa);
    glViewport(0, 0, gwa.width, gwa.height);
    glEnable(GL_DEPTH_TEST);
 
    printf(" gl version = %s ...\n", glGetString(GL_VERSION));
    printf(" glsl version = %s ...\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

    // загрузили геометрию и материал
    
    GLuint vertexID;
    GLuint indexID;
    loadGeometryToGPU(vertexID,indexID);
    auto materialID = createMaterial("vshader_attr.txt","fshader_attr.txt");

    // основной цикл

    while(true) 
    {
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
          
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // стираем
        draw(vertexID,indexID,materialID);                  // рисуем
        glXSwapBuffers(dpy, win);                           // показываем
    } 
}




