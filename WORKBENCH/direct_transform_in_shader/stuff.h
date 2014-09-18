
#include "../../utils/opengl.h"

#include <stddef.h>
#include <vector>
#include <stdlib.h>
#include <string>
#include <FreeImage.h>

#pragma pack(4) // making sure that our structure will be alligned in memory by a 4-byte order
// for now our each vertex is only presented by 2 vectors - it's position and it's coordinate on a polygon (or fragment, or texture, or whatever different tutorials call them)
struct TstVertex
{
    float position[4]; // position of a vertex in global coordinate system
    GLint mat_idx; // tst index in a matrix
};
#pragma pack() // returning default pack value


/* current routine will create a 4-vertex square and will specify order of traversal for vertices */
void generate_rectangle(float width, float height, std::vector<TstVertex>& vert, std::vector<unsigned short>& order);

std::string read_text_from_file(const char* file);

void link_shader_program(GLuint& gl_program);

void create_shader_program(const char* vshader_file, const char* fshader_file, GLuint& out_program);

void load_geometry_to_gpu(std::vector<TstVertex>& vertices, std::vector<unsigned short>& vert_order,
                          GLuint& vert_buffer_id, GLuint& vert_order_buffer_id);

void bind_uniforms(GLuint gl_program,
                   float angle_x, float angle_y, float angle_z,
                   float offset_x, float offset_y, float offset_z);

void bind_shaders_attributes(GLuint gl_program);

void draw_geometry(GLuint gl_program, GLuint vert_buffer_id,
                   GLuint vert_order_buffer_id, size_t vert_order_array_len,
                   float angle_x, float angle_y, float angle_z,
                   float offset_x, float offset_y, float offset_z);

void bind_texture_to_shader_program(GLuint texture_id, GLuint gl_program);

void load_texture_to_gpu(const char* texture_file_name, GLuint& texture_id);

void read_image_from_file(const char* file, std::vector<BYTE>& buffer,
                unsigned int &width, unsigned int &height, GLuint &gl_flag);

std::string get_file_extension(const std::string& file_name);
