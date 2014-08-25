#ifndef MY_GEOMETRY_H
#define MY_GEOMETRY_H

#include <vector>
#include <stdlib.h>

#define VERTEX_POSITION_SZ 4
#define VERTEX_PLANE_COORD_SZ 2
#define VERTEX_NORMAL_SZ 3
#define VERTEX_DEF_COLOR_SZ 4

#define MY_VERTEX_SZ (VERTEX_POSITION_SZ + VERTEX_PLANE_COORD_SZ + VERTEX_NORMAL_SZ + VERTEX_DEF_COLOR_SZ)

#pragma pack(4)
struct MyVertex {
        float position[VERTEX_POSITION_SZ];
        float plane_coord[VERTEX_PLANE_COORD_SZ];
        float normal[VERTEX_NORMAL_SZ];
        float def_color[VERTEX_DEF_COLOR_SZ];

        void TRACE(unsigned int id = 0) const;
};
#pragma pack()

/* for now only single-precision FP implementation is considered */
class MyGeometry {
        std::vector<MyVertex> vertices;
        std::vector<unsigned short> order;

public:
        MyGeometry(const std::vector<MyVertex>& ivertices, const std::vector<unsigned short>& iorder):
                vertices(ivertices), order(iorder) {}
        MyGeometry(const float* vert_data, size_t vert_num, const unsigned short *ord_data, size_t ord_num);

        size_t GetVerticesSize() const { return vertices.size() * sizeof(MyVertex); }
        size_t GetOrderSize() const { return order.size() * sizeof(unsigned short); }
        size_t GetOrderNum() const { return order.size(); }
        size_t GetVerticesNum() const { return vertices.size(); }

        const void* VerticesData() { return &vertices[0]; }
        const void* OrderData() { return &order[0]; }

        void TRACE() const;
};

namespace geometry_gen
{
        #define DEFAULT_CUBE_SIZE 2.0f

        MyGeometry generate_rectangle(float width, float height);
        MyGeometry generate_rectangle_two_sides(float width, float height);
        MyGeometry generate_cube(float sz);

        MyGeometry generate_triangle(float edge1, float edge2, float edge3);
}

#endif


