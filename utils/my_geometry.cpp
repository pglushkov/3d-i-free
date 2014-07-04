
#include "my_geometry.h"

#include <cmath>
#include <iostream>

void MyVertex::TRACE(unsigned int id) const
{
        std::cout << "Tracing Vertex " << id << ", size = " << sizeof(*this) << std::endl;
        std::cout << "  Position: " << this->position[0] << " " << this->position[1] << " "
                << this->position[2] << " " << this->position[3] << " ... ";
        std::cout << "  Plane coords: " << this->plane_coord[0] << " " << this->plane_coord[1] << " ... " << std::endl;
        std::cout << "  Normals: " << this->normal[0] << " " << this->normal[1] << " " << this->normal[2] << " ...";
        std::cout << "  Def colors: " << this->def_color[0] << " " << this->def_color[1] << " "
                << this->def_color[2] << " " << this->def_color[3] << " ... " << std::endl;
}

MyGeometry:: MyGeometry(const float* vert_data, size_t vert_num, const unsigned short *ord_data, size_t ord_num)
{
        vertices.resize(vert_num);
        order.resize(ord_num);

        size_t offset = 0;
        for (size_t k = 0; k < vert_num; ++k) {
                offset = k * MY_VERTEX_SZ;
                vertices[k] =
                {
                        /* position data */
                        vert_data[offset + 0], vert_data[offset + 1], vert_data[offset + 2], vert_data[offset + 3],
                        /* plane coordinates data */
                        vert_data[offset + 4], vert_data[offset + 5],
                        /* normals data */
                        vert_data[offset + 6], vert_data[offset + 7], vert_data[offset + 8],
                        /* default color data */
                        vert_data[offset + 9], vert_data[offset + 10], vert_data[offset + 11], vert_data[offset + 13],
                };
        }

        for (size_t k = 0; k < ord_num; ++k)
                order[k] = ord_data[k];
}

void MyGeometry::TRACE() const
{
        std::cout << "Tracing geometry: " << this << std::endl;
        for (unsigned int k = 0; k < vertices.size(); ++k)
                vertices[k].TRACE(k);

        printf("Vert order: ");
        for (unsigned int k = 0; k < order.size(); ++k)
                printf(" %d ", order[k]);
        printf("\n");
}

namespace geometry_gen
{

        MyGeometry generate_rectangle(float width, float height)
        {
                std::vector<MyVertex> vert(4);
                std::vector<unsigned short> ord( { 0, 1, 2, 2, 3, 0} );

                height = abs(height) > 2.0f ? 2.0f : height;
                width = abs(width) > 2.0f ? 2.0f : width;

                /* left bottom x */
                float lbx = 0.0f - (width / 2.0f);
                /* left bottom y */
                float lby = 0.0f + (height / 2.0f);

                vert[0].position[0] = lbx;
                vert[0].position[1] = lby;
                vert[1].position[0] = lbx + width;
                vert[1].position[1] = lby;
                vert[2].position[0] = lbx + width;
                vert[2].position[1] = lby - height;
                vert[3].position[0] = lbx;
                vert[3].position[1] = lby - height;

                vert[0].plane_coord[0] = 0.0f;
                vert[0].plane_coord[1] = 0.0f;

                vert[1].plane_coord[0] = 1.0f;
                vert[1].plane_coord[1] = 0.0f;

                vert[2].plane_coord[0] = 1.0f;
                vert[2].plane_coord[1] = 1.0f;

                vert[3].plane_coord[0] = 0.0f;
                vert[3].plane_coord[1] = 1.0f;

                for (size_t k = 0; k < 4; ++k) {
                        /* putting square straight on XY plane by default */
                        vert[k].position[2] = 0.0f;
                        vert[k].position[3] = 1.0f;
                        
                        /* white color by default */
                        vert[k].def_color[0] = 1.0f;
                        vert[k].def_color[1] = 1.0f;
                        vert[k].def_color[2] = 1.0f;
                        vert[k].def_color[3] = 1.0f;
                        
                        /* normal is along z-direction by default */
                        vert[k].normal[0] = 0.0f;
                        vert[k].normal[1] = 0.0f;
                        vert[k].normal[2] = 1.0f;
                }

                return MyGeometry(vert, ord);
        }


        MyGeometry generate_triangle(float edge1, float edge2, float edge3)
        {
                std::vector<MyVertex> vert(3);
                std::vector<unsigned short> ord(3);

                return MyGeometry(vert, ord);
        }

}


