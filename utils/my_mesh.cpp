

#include "my_mesh.h"

#include <stdio.h>

void MyMesh::LoadGeometryToGpu()
{

        size_t vsz = geom.GetVerticesSize();
        size_t osz = geom.GetOrderSize();

        glGenBuffers(1, &vert_buffer_id);
        glGenBuffers(1, &ord_buffer_id);

        glBindBuffer(GL_ARRAY_BUFFER,vert_buffer_id);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ord_buffer_id);

        glBufferData(GL_ARRAY_BUFFER, vsz, geom.VerticesData(), GL_DYNAMIC_DRAW);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, osz, geom.OrderData(), GL_DYNAMIC_DRAW);

        //printf("geometry: vert : %lu bytes; ndx: %lu bytes\n", vsz, osz);
}

void MyMesh::BindShadersAttributes()
{
        /*
           We here assume that shaders have further attributes:
           1) position (Vec4)
           2) default_color (Vec 4)
           3) normals (Vec3)
           4) plane coordinates (Vec2)
         */
        GLuint gl_program = mater.GetProgramHandle();

        GLint pos_attr = glGetAttribLocation(gl_program, "in_position");
        GLint pc_attr  = glGetAttribLocation(gl_program, "in_plane_coord");
        GLint norm_attr = glGetAttribLocation(gl_program, "in_norm");
        GLint col_attr = glGetAttribLocation(gl_program, "in_def_color");

        /* DBG */
        /*printf(" Binding attributes: pos_attr = %d, pc_attr = %d, norm_attr = %d, col_attr = %d ...\n",
                        pos_attr, pc_attr, norm_attr, col_attr);*/

        if(pos_attr != -1)
        {
            glVertexAttribPointer(
                pos_attr,
                4,                   // attribute size, 4 floats
                GL_FLOAT,
                GL_FALSE,
                sizeof(MyVertex),
                (const void *)(0)    // attribute offset from the beginning of the vertex
            );

            glEnableVertexAttribArray(pos_attr);
        }
        if(pc_attr != -1)
        {
            glVertexAttribPointer(
                pc_attr,
                2,
                GL_FLOAT,
                GL_FALSE,
                sizeof(MyVertex),
                (const void *)(4 * sizeof(float))
            );
            glEnableVertexAttribArray(pc_attr);
        }

        if(norm_attr != -1)
        {
            glVertexAttribPointer(
                norm_attr,
                3,
                GL_FLOAT,
                GL_FALSE,
                sizeof(MyVertex),
                (const void *)(6 * sizeof(float))
            );
            glEnableVertexAttribArray(norm_attr);
        }

        if(col_attr != -1)
        {
            glVertexAttribPointer(
                col_attr,
                4,
                GL_FLOAT,
                GL_FALSE,
                sizeof(MyVertex),
                (const void *)(9 * sizeof(float))
            );
            glEnableVertexAttribArray(col_attr);
        }
}

MyMesh::MyMesh(const MyGeometry& igeom, const char* vshader_file, const char* fshader_file) :
        geom(igeom), mater(vshader_file, fshader_file)
{
        LoadGeometryToGpu();
}

MyMesh::MyMesh(const std::vector<MyVertex>& vert, const std::vector<unsigned short>& ord, const char* vshader_file,
                const char* fshader_file) :
        geom(vert, ord), mater(vshader_file, fshader_file)
{
        LoadGeometryToGpu();
}

MyMesh::MyMesh(const float* vert_data, size_t vert_num, const unsigned short *ord_data, size_t ord_num,
                const char* vshader_file, const char* fshader_file) :
        geom(vert_data, vert_num, ord_data, ord_num), mater(vshader_file, fshader_file)

{
        LoadGeometryToGpu();
}

MyMesh::~MyMesh()
{
        glDeleteBuffers(1, &vert_buffer_id);
        glDeleteBuffers(1, &ord_buffer_id);
}

void MyMesh::draw()
{

        mater.UseMaterial(true);                                // setting current shader program
        glBindBuffer(GL_ARRAY_BUFFER, vert_buffer_id);          // binding current vertex buffer
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ord_buffer_id);   // binding current vertex-order buffer

        BindShadersAttributes(); // passing attributes markup to the shader

        // setting necessary uniforms
        GLint timeUniformHandle = glGetUniformLocation(mater.GetProgramHandle(), "time");
        if (timeUniformHandle != -1) {
                GLfloat time = (GLfloat)clock() / (GLfloat)CLOCKS_PER_SEC  * 10.0;
                glUniform1f(timeUniformHandle, time);
        }

        // actual drawing
        glDrawElements(GL_TRIANGLES, geom.GetOrderNum(), GL_UNSIGNED_SHORT, (const void *)(0));
}

