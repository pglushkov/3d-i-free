#ifndef MY_MESH_H
#define MY_MESH_H

#include "my_material.h"
#include "my_geometry.h"

struct DrawableObject
{
        virtual void draw() = 0;
};

struct EuclideanSpaceObject
{
        virtual void rotate() = 0;
        virtual void scale() = 0;
        virtual void translate() = 0;
};

class MyMesh: public DrawableObject
{
        MyGeometry geom;
        MyMaterial mater;

        MyMesh( const MyMesh& mesh);
        const MyMesh& operator=(const MyMesh& mesh) { return *this; }

        void LoadGeometryToGpu();
        void BindShadersAttributes();

        GLuint vert_buffer_id;
        GLuint ord_buffer_id;

public:
        MyMesh(const MyGeometry& igeom, const char* vshader_file, const char* fshader_file);
        MyMesh(const std::vector<MyVertex>& vert, const std::vector<unsigned short>& ord, const char* vshader_file,
                        const char* fshader_file);
        MyMesh(const float* vert_data, size_t vert_num, const unsigned short *ord_data, size_t ord_num,
                        const char* vshader_file, const char* fshader_file);

        void AddTexture(const char* file_name) { mater.AddTexture(file_name); }

        virtual ~MyMesh();
        virtual void draw();

        void TRACE_GEOM() const { geom.TRACE(); }
};

#endif

