
#include "stuff.h"

#include <math.h>
#include <stdexcept>
#include <iostream>
#include <fstream>

//float tst_matrix[16] =
//    {
//        -0.5f, 0.5f, 0.0f, 1.0f,
//        0.5f, 0.5f, 0.0f, 1.0f,
//        0.5f, -0.5f, 0.0f, 1.0f,
//        -0.5f, -0.5f, 0.0f, 1.0f,
//    };

float tst_matrix[16] =
    {
        1.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 1.0f, 0.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f,
    };

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

//        vert[0].position[0] = 0.0f;
//        vert[0].position[1] = 0.0f;
//        vert[1].position[0] = 0.0f;
//        vert[1].position[1] = 0.0f;
//        vert[2].position[0] = 0.0f;
//        vert[2].position[1] = 0.0f;
//        vert[3].position[0] = 0.0f;
//        vert[3].position[1] = 0.0f;

//        // DBG
//        for (size_t k = 0; k < 4; ++k)
//            printf("Rectangle vertex(%d) : [%f,%f] ...\n", k+1, vert[k].position[0], vert[k].position[1]);

        // fragment coordinates of generate square are to be: (x,y)
//        (0,1) --------- (1,1)
//              |       |
//              |       |
//              |       |
//        (0,0) --------- (1,0)

        vert[0].mat_idx = 0;
        vert[1].mat_idx = 1;
        vert[2].mat_idx = 2;
        vert[3].mat_idx = 3;

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

void draw_geometry(GLuint gl_program, GLuint vert_buffer_id, GLuint vert_order_buffer_id, size_t vert_order_array_len, float intensity, GLuint texture_id)
{
        // making specified shader program a current one
        glUseProgram(gl_program);

        glBindBuffer(GL_ARRAY_BUFFER, vert_buffer_id);          // binding current vertex buffer
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vert_order_buffer_id);   // binding current vertex-order buffer

        bind_shaders_attributes(gl_program); // passing attributes markup to the shader
        bind_uniforms(gl_program, intensity); // passing some custom uniform data to the shader
        bind_texture_to_shader_program(texture_id, gl_program);

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
        GLint pc_attr  = glGetAttribLocation(gl_program, "in_mat_idx");


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
        } else {
            printf("attribute %s fuckup ...\n", "in_vertex_position");
            throw std::runtime_error("ERROR in bind_shader_attributes() : coult not find requested attribute");
        }

        if(pc_attr != -1)
        {
            glVertexAttribIPointer(
                pc_attr,
                1,                                      // attribute size, 1 uints
                GL_INT,                                 // type of data in values of the attribute
                //GL_FALSE,                               // flag if we want to normalize the values (RTFM on glVertexAttribPointer)
                sizeof(TstVertex),                      // size of whole description for one vertex with all it's attribute
                (const void *)(4 * sizeof(float))       // current attribute offset from the beginning of the vertex
            );
            glEnableVertexAttribArray(pc_attr);
        } else {
            printf("attribute %s fuckup ...\n", "in_mat_idx");
            throw std::runtime_error("ERROR in bind_shader_attributes() : coult not find requested attribute");
        }

}

void bind_uniforms(GLuint gl_program, float intensity)
{
    // setting necessary uniforms

    // for now, let's assume, that our shader program only expects one uniform value into it's vertex shader.
    GLint uniform_handle = glGetUniformLocation(gl_program, "in_intensity");
    if (uniform_handle != -1) {
        glUniform1f(uniform_handle, intensity);
    } else {
        printf("uniform %s fuck-up ... \n", "in_intensity");
        throw std::runtime_error("ERROR in bind_uniforms : Could not locate a uniform!");
    }

    uniform_handle = glGetUniformLocation(gl_program, "tst_matrix");
    if (uniform_handle != -1) {
        glUniformMatrix4fv(uniform_handle, 1, true, &tst_matrix[0]);
    } else {
        printf("uniform %s fuck-up ... \n", "tst_matrix");
        throw std::runtime_error("ERROR in bind_uniforms : Could not locate a uniform!");
    }

    // uniforms can be easily passed to fragment and geometry shaders as well, it's not a strict vertex-shader privilege
    // there are 4 general sorts of attributes, that one can pass:
//        - single value (int or float)
//        - 2- or 3- or 4- sized vector (int or float)
//        - array of 1- or 2- or 3- or 4- sized vectors (int or float)
//        - 2x2- or 3x3- or 4x4- sized matrices (float only!)
//        to get some more usefull insight on this astonishing topic - RTFM on glUniform<bla-bla>("eat a dick")
}

void bind_texture_to_shader_program(GLuint texture_id, GLuint gl_program)
{
        // setting active texturing unit - think of a 'texture_unit' as a special 'hardware' block in GPU, that provides
        // fast texture resampling, access, interpolation when accessing 'between' pixels ... GPU has a limited number of
        // texturing units, but their id's are guaranteed to start from constant GL_TEXTURE0 and then grow up to some limit,
        // see main() and one of first printf's, it actially will show a number of these units, that can be simultaneously
        // accesed while executing fragment shader code
        glActiveTexture( GL_TEXTURE0 );

        // binding out texture ID with just selected texturing unit
        glBindTexture(GL_TEXTURE_2D, texture_id);

        // and finally we have to tell our shader, which texturing unit to use, will simply pass number of the unit using a uniform
        GLint tex_id_in_shader = glGetUniformLocation(gl_program, "my_cool_texture");
        if (tex_id_in_shader != -1) {
            glUniform1i(tex_id_in_shader, GL_TEXTURE0);
        }
}


void load_texture_to_gpu(const char* texture_file_name, GLuint& texture_id)
{
        std::vector<BYTE> tex_data;

        unsigned int width, height;
        GLuint gl_flag;

        read_image_from_file(texture_file_name, tex_data, width, height, gl_flag);

        /* DBG */
        printf("In load_texture_to_gpu(): width=%u height = %u, gl_flag=%u ...\n", width, height, gl_flag);

        glGenTextures(1, &texture_id);

        glBindTexture(GL_TEXTURE_2D, texture_id);

        glTexImage2D(GL_TEXTURE_2D, 0, gl_flag, width, height, 0, gl_flag, GL_UNSIGNED_BYTE, &tex_data[0]);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

void read_image_from_file(const char* file, std::vector<BYTE>& buffer,
                unsigned int &width, unsigned int &height, GLuint &gl_flag)
{
        FIBITMAP* bmap;

        /* for now only bmp / jpeg / png files are supported ... */
        std::string file_ext = get_file_extension(file);
        if ( file_ext == "")
                throw std::runtime_error("Error! Specified texture file has no extension!");

        if ( file_ext == "jpg")
                bmap = FreeImage_Load(FIF_JPEG, file, JPEG_DEFAULT);
        else if (file_ext == "bmp")
                bmap = FreeImage_Load(FIF_BMP, file, BMP_DEFAULT);
        else if (file_ext == "png")
                bmap = FreeImage_Load(FIF_PNG, file, PNG_DEFAULT);
        else
                throw std::runtime_error("Error! Specifed texture file must be .jpg or .bmp or .png!");

        if (!bmap)
                throw std::runtime_error("Error! Could not read data from specified texture file!");

        int color_type = FreeImage_GetColorType(bmap);
        if (color_type != FIC_RGB && color_type != FIC_RGBALPHA)
                throw std::runtime_error("Error! Unsupported data format in specified texture file!");

        unsigned int bpp = FreeImage_GetBPP(bmap);
        width = FreeImage_GetWidth(bmap);
        height = FreeImage_GetHeight(bmap);
        unsigned int bytes_per_line = FreeImage_GetLine(bmap);
        unsigned int bytes_per_pixel = bytes_per_line / width;
        unsigned int bytes_to_copy = bytes_per_pixel * width * height;
        void* image_data = FreeImage_GetBits(bmap);

        if (bytes_per_pixel == 3)
                gl_flag = GL_RGB;
        else if (bytes_per_pixel == 4)
                gl_flag = GL_RGBA;
        else
                throw std::runtime_error("Error! For now only RGB-24-bit and RGBA-32-bit is supported!");

        buffer.resize(bytes_to_copy);

        /* print something about it ... */
        std::cout << "FIBITMAP = " << bmap << ", BPP = " << bpp
                  << ", Width = " << width << ", Height = " << height
                  << ", Bytes-per-pixel = " << bytes_per_pixel
                  << ", ColorType = " << color_type << ", DataBits = "
                  << image_data << std::endl;

        for (size_t h = 0; h < height; ++h) {
                /* getting address of first pixel in current line */
                BYTE* bits = FreeImage_GetScanLine(bmap, h);
                for (unsigned int w = 0; w < width; ++w) {
                        size_t buf_idx = h * bytes_per_line + w * bytes_per_pixel;
                        buffer[ buf_idx + 0] = bits[FI_RGBA_RED];
                        buffer[ buf_idx + 1] = bits[FI_RGBA_GREEN];
                        buffer[ buf_idx + 2] = bits[FI_RGBA_BLUE];
                        /* jump to next pixel in current line*/
                        bits += bytes_per_pixel;

                        /* DBG */
                        // printf("texbufidx = %lu  of %u ...\n", buf_idx, bytes_to_copy);
                }
        }

        FreeImage_Unload(bmap);
}

std::string get_file_extension(const std::string& file_name)
{
        size_t pos = file_name.find_last_of(".");
        if ( pos != std::string::npos)
                return file_name.substr(pos + 1);
        else
                return "";
}
