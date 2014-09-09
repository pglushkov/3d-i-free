

#include "my_mesh.h"
#include "my_world.h"

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

void MyMesh::BindShadersAttributes(const MyMaterial& mater)
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

void MyMesh::BindUniforms(const MyMaterial& mater)
{
        GLuint gl_program = mater.GetProgramHandle();
	// setting necessary uniforms

	float time = MyWorld::GetTimeSinceCreation();
	const MyWorld::light_pos_vector& global_light_pos = MyWorld::GetInstance().GetGlobalLightPos(); //non static method!!!
	MyProjectionMatrix<float>& matProj = MyWorld::GetWorldProjection();
        MySquareMatrix<float, 4>& view = MyWorld::GetWorldCameraView();

	GLint timeUniformHandle = glGetUniformLocation(gl_program, "time");
	if (timeUniformHandle != -1) {
		glUniform1f(timeUniformHandle, time);
	}

	GLint rot = glGetUniformLocation(gl_program, "world_view_position");
	if (rot != -1) {
        glUniformMatrix4fv(rot, 1, true, &pos_mat.get_data()[0][0]);
	}

	GLint proj = glGetUniformLocation(gl_program, "projection");
	if (proj != -1) {
        glUniformMatrix4fv(proj, 1, true, &matProj.get_data()[0][0]);
	}

	GLint light = glGetUniformLocation(gl_program, "light_pos");
	if (light != -1) {
		glUniform3f(light, global_light_pos[0], global_light_pos[1], global_light_pos[2]);
	}

	GLint camera = glGetUniformLocation(gl_program, "camera_transform");
	if (camera != -1) {
            glUniformMatrix4fv(camera, 1, true, &view.data()[0][0]);
	}
}

MyMesh::MyMesh(const MyGeometry& igeom) : geom(igeom)
{
        LoadGeometryToGpu();
}

MyMesh::MyMesh(const std::vector<MyVertex>& vert, const std::vector<unsigned short>& ord) :
        geom(vert, ord)
{
        LoadGeometryToGpu();
}

MyMesh::MyMesh(const float* vert_data, size_t vert_num, const unsigned short *ord_data, size_t ord_num) :
        geom(vert_data, vert_num, ord_data, ord_num)

{
        LoadGeometryToGpu();
}

MyMesh::MyMesh() : geom(geometry_gen::generate_cube(DEFAULT_CUBE_SIZE))
{
        LoadGeometryToGpu();
}

MyMesh::~MyMesh()
{
        glDeleteBuffers(1, &vert_buffer_id);
        glDeleteBuffers(1, &ord_buffer_id);
}

void MyMesh::draw(const MyMaterial &mater)
{
        mater.UseMaterial(true);

        glBindBuffer(GL_ARRAY_BUFFER, vert_buffer_id);          // binding current vertex buffer
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ord_buffer_id);   // binding current vertex-order buffer

        BindShadersAttributes(mater); // passing attributes markup to the shader
        BindUniforms(mater);

        // actual drawing
        glDrawElements(GL_TRIANGLES, geom.GetOrderNum(), GL_UNSIGNED_SHORT, (const void *)(0));
}

void MyMesh::draw()
{
        MyWorld::GetDefaultMaterial().UseMaterial(true);

        glBindBuffer(GL_ARRAY_BUFFER, vert_buffer_id);          // binding current vertex buffer
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ord_buffer_id);   // binding current vertex-order buffer

        BindShadersAttributes(MyWorld::GetDefaultMaterial()); // passing attributes markup to the shader
        BindUniforms(MyWorld::GetDefaultMaterial());

        // actual drawing
        glDrawElements(GL_TRIANGLES, geom.GetOrderNum(), GL_UNSIGNED_SHORT, (const void *)(0));
}

