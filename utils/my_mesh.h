#ifndef MY_MESH_H
#define MY_MESH_H

#include "my_material.h"
#include "my_geometry.h"
#include "my_position_matrix.h"
#include "custom_uniform_holder.h"

struct DrawableObject
{
        virtual void draw() = 0;
        virtual void draw(const MyMaterial& mater) = 0;
};

struct EuclideanSpaceObject
{
        virtual void rotate() = 0;
        virtual void scale() = 0;
        virtual void translate() = 0;
};

class MyMesh: public DrawableObject
{
        UniformHolder custom_uni;

        MyGeometry geom;
        MyPositionMatrix<float> pos_mat;

        MyMesh( const MyMesh& mesh);
        const MyMesh& operator=(const MyMesh& mesh) { return *this; }

        void LoadGeometryToGpu();
        void BindShadersAttributes(const MyMaterial& mater);
        void BindUniforms(const MyMaterial& mater);

        GLuint vert_buffer_id;
        GLuint ord_buffer_id;

public:
        MyMesh(const MyGeometry& igeom);
        MyMesh(const std::vector<MyVertex>& vert, const std::vector<unsigned short>& ord);
        MyMesh(const float* vert_data, size_t vert_num, const unsigned short *ord_data, size_t ord_num);

        MyMesh();

        virtual ~MyMesh();
        virtual void draw();
        virtual void draw(const MyMaterial& mater);

        void TRACE_GEOM() const { geom.TRACE(); }

        MyPositionMatrix<float>& GetObjTransform() { return pos_mat; }

        void AddCustomIntUniform(const char* name, CUSTOM_UNIFORM_TYPE type, GLint* value, size_t size = 0)
        {
            custom_uni.AddIntUniform(type, name, value, size);
        }

        void AddCustomFpUniform(const char* name, CUSTOM_UNIFORM_TYPE type, GLfloat* value, size_t size = 0)
        {
            custom_uni.AddFpUniform(type, name, value, size);
        }
};

#endif

