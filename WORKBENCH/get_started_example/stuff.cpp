
#include "stuff.h"

#include <math.h>
#include <stdexcept>
#include <iostream>
#include <fstream>

void generate_rectangle(float width, float height, std::vector<TstVertex>& vert, std::vector<unsigned short>& order)
{
        order.resize(6);
        order[0] = 0;
        order[1] = 1;
        order[2] = 2;
        order[3] = 2;
        order[4] = 3;
        order[5] = 0;

        vert.resize(4);

        // making sure, that our square will fit into the screen, which is [-1:1][-1:1] in X and Y. Z will be set to 0 for now
        height = abs(height) > 2.0f ? 2.0f : height;
        width = abs(width) > 2.0f ? 2.0f : width;

        // left top x
        float ltx = 0.0f - (width / 2.0f);
        // left top y
        float lty = 0.0f + (height / 2.0f);

        // vertices of generated square are to be: (order)
//        0 --------- 1
//          |       |
//          |       |
//          |       |
//        3 --------- 2

        vert[0].position[0] = ltx;
        vert[0].position[1] = lty;
        vert[1].position[0] = ltx + width;
        vert[1].position[1] = lty;
        vert[2].position[0] = ltx + width;
        vert[2].position[1] = lty - height;
        vert[3].position[0] = ltx;
        vert[3].position[1] = lty - height;

        // fragment coordinates of generate square are to be: (x,y)
//        (0,1) --------- (1,1)
//              |       |
//              |       |
//              |       |
//        (0,0) --------- (1,0)

        vert[0].frag_coord[0] = 0.0f;
        vert[0].frag_coord[1] = 1.0f;

        vert[1].frag_coord[0] = 1.0f;
        vert[1].frag_coord[1] = 1.0f;

        vert[2].frag_coord[0] = 1.0f;
        vert[2].frag_coord[1] = 0.0f;

        vert[3].frag_coord[0] = 0.0f;
        vert[3].frag_coord[1] = 0.0f;

        for (size_t k = 0; k < 4; ++k) {
                // putting square straight on XY plane by default
                vert[k].position[2] = 0.0f;
                vert[k].position[3] = 1.0f;
        }

}


// Input '*out_program' will contain ID of compiled program with both shaders attached to it...
void create_shader_program(const char* vshader_file, const char* fshader_file, GLuint& out_program)
{
        GLuint vert_shader, frag_shader;
        GLint length, result;

        std::string vertex_shader_code(read_text_from_file(vshader_file));
        std::string fragment_shader_code(read_text_from_file(fshader_file));

        // create shader object, set the source, and compile
        vert_shader = glCreateShader(GL_VERTEX_SHADER);
        frag_shader = glCreateShader(GL_FRAGMENT_SHADER);

        out_program = glCreateProgram();

        length = vertex_shader_code.length();
        const char* tmp = vertex_shader_code.c_str();
        glShaderSource(vert_shader, 1, (const GLchar **)&tmp, &length);
        glCompileShader(vert_shader);


        length = fragment_shader_code.length();
        tmp = fragment_shader_code.c_str();
        glShaderSource(frag_shader, 1, (const GLchar **)&tmp, &length);
        glCompileShader(frag_shader);

        // make sure the compilation was successful
        glGetShaderiv(vert_shader, GL_COMPILE_STATUS, &result);
        if(result == GL_FALSE) {
                char *log;

                // get the shader info log
                glGetShaderiv(vert_shader, GL_INFO_LOG_LENGTH, &length);
                log = (char*)malloc(length);
                glGetShaderInfoLog(vert_shader, length, &result, log);

                // print an error message and the info log
                fprintf(stderr, "Error : Unable to compile vertex shader, log: %s\n", log);
                free(log);

                glDeleteShader(vert_shader);
                throw std::runtime_error("Error : could not compile vertex shader!");
        }

        glGetShaderiv(frag_shader, GL_COMPILE_STATUS, &result);
        if(result == GL_FALSE) {
                char *log;

                // get the shader info log
                glGetShaderiv(frag_shader, GL_INFO_LOG_LENGTH, &length);
                log = (char*)malloc(length);
                glGetShaderInfoLog(frag_shader, length, &result, log);

                // print an error message and the info log
                fprintf(stderr, "Error : Unable to compile fragment shader, log: %s\n", log);
                free(log);

                glDeleteShader(frag_shader);
                throw std::runtime_error("Error : could not compile fragment shader!");
        }

        // attaching shader to the program ...
        glAttachShader(out_program, vert_shader);
        glAttachShader(out_program, frag_shader);

        // delete the shader - it won't actually be destroyed until the program that it's attached to has been destroyed
        glDeleteShader(vert_shader);
        glDeleteShader(frag_shader);

}

void link_shader_program(GLuint& gl_program)
{
    GLint result;

    glLinkProgram(gl_program);
    glGetProgramiv(gl_program, GL_LINK_STATUS, &result);
    if(result == GL_FALSE) {
            GLint length;
            char *log;

            // get the program info log
            glGetProgramiv(gl_program, GL_INFO_LOG_LENGTH, &length);
            log = (char*)malloc(length);
            glGetProgramInfoLog(gl_program, length, &result, log);

            // print an error message and the info log
            fprintf(stderr, "Error in MyMaterial(): Program linking failed, log: %s\n", log);
            free(log);

            // delete the program
            glDeleteProgram(gl_program);
            gl_program = 0;
            throw std::runtime_error("Error in MyMaterial() : could not link the shader program ...");
    }

}


std::string read_text_from_file(const char* file)
{
        //DBG
        printf("ReadTextFromFile : file to open = (%s)\n", file);

        std::ifstream read;
        read.open(file, std::ifstream::in);
        std::vector<char> data;

        if( read.good() ) {
                read.seekg(0, std::ios::end);
                size_t file_sz = read.tellg();
                read.seekg(0);
                data.resize(file_sz + 1);
                read.read(&data[0], file_sz);
                data[file_sz] = '\0';

        } else {
                throw std::runtime_error("Could not open specified file!");
        }

        read.close();
        return std::string(&data[0]);
}

void load_geometry_to_gpu(std::vector<TstVertex>& vertices, std::vector<unsigned short>& vert_order,
                          GLuint& vert_buffer_id, GLuint& vert_order_buffer_id)
{
        size_t vsz = vertices.size() * sizeof(TstVertex);
        size_t osz = vert_order.size() * sizeof(unsigned short);;

        glGenBuffers(1, &vert_buffer_id);
        glGenBuffers(1, &vert_order_buffer_id);

        glBindBuffer(GL_ARRAY_BUFFER, vert_buffer_id);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vert_order_buffer_id);

        glBufferData(GL_ARRAY_BUFFER, vsz, (void*)&vertices[0], GL_DYNAMIC_DRAW);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, osz, (void*)&vert_order[0], GL_DYNAMIC_DRAW);
}

void draw_geometry(GLuint gl_program, GLuint vert_buffer_id, GLuint vert_order_buffer_id, size_t vert_order_array_len, float intensity)
{
        // making specified shader program a current one
        glUseProgram(gl_program);

        glBindBuffer(GL_ARRAY_BUFFER, vert_buffer_id);          // binding current vertex buffer
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vert_order_buffer_id);   // binding current vertex-order buffer

        bind_shaders_attributes(gl_program); // passing attributes markup to the shader
        bind_uniforms(gl_program, intensity); // passing some custom uniform data to the shader

        // actual drawing
        glDrawElements(GL_TRIANGLES, vert_order_array_len, GL_UNSIGNED_SHORT, (const void *)(0));
}

void bind_shaders_attributes(GLuint gl_program)
{
//           We here assume that shaders have further attributes:
//           1) position (float Vec4)
//           2) popygon coordinates (float Vec2)

//           What this routine does is passing info about vertex attributes to the vertex shader, i.e. a mapping of vertex array, that we had allready loaded to GPU.
//           Here we're supposed to know exactly how a cetain attribute is actually called in the code of a vertex shader, that we're about to use!!

        // One more time pay attention to specified names of the attributes!
        // If the won't be found in the shader progrom - everything will fuck itself up own ass with a powerdrill!!!
        GLint pos_attr = glGetAttribLocation(gl_program, "in_vertex_position");
        GLint pc_attr  = glGetAttribLocation(gl_program, "in_vertex_polygon_coord");


        if(pos_attr != -1)
        {
            glVertexAttribPointer(
                pos_attr,
                4,                   // attribute size, 4 floats
                GL_FLOAT,            // type of data in values of the attribute
                GL_FALSE,            // flag if we want to normalize the values (RTFM on glVertexAttribPointer)
                sizeof(TstVertex),   // size of whole description for one vertex with all it's attribute
                (const void *)(0)    // current attribute offset from the beginning of the vertex
            );

            glEnableVertexAttribArray(pos_attr);
        }

        if(pc_attr != -1)
        {
            glVertexAttribPointer(
                pc_attr,
                2,                                      // attribute size, 4 floats
                GL_FLOAT,                               // type of data in values of the attribute
                GL_FALSE,                               // flag if we want to normalize the values (RTFM on glVertexAttribPointer)
                sizeof(TstVertex),                      // size of whole description for one vertex with all it's attribute
                (const void *)(4 * sizeof(float))       // current attribute offset from the beginning of the vertex
            );
            glEnableVertexAttribArray(pc_attr);
        }

}

void bind_uniforms(GLuint gl_program, float intensity)
{
    // setting necessary uniforms

    // for now, let's assume, that our shader program only expects one uniform value into it's vertex shader.
    GLint uniform_handle = glGetUniformLocation(gl_program, "in_intensity");
    if (uniform_handle != -1) {
        glUniform1f(uniform_handle, intensity);
    }

    // uniforms can be easily passed to fragment and geometry shaders as well, it's not a strict vertex-shader privilege
    // there are 4 general sorts of attributes, that one can pass:
//        - single value (int or float)
//        - 2- or 3- or 4- sized vector (int or float)
//        - array of 1- or 2- or 3- or 4- sized vectors (int or float)
//        - 2x2- or 3x3- or 4x4- sized matrices (float only!)
//        to get some more usefull insight on this astonishing topic - RTFM on glUniform<bla-bla>("eat a dick")
}


