
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

                /* left top x */
                float ltx = 0.0f - (width / 2.0f);
                /* left top y */
                float lty = 0.0f + (height / 2.0f);

                vert[0].position[0] = ltx;
                vert[0].position[1] = lty;
                vert[1].position[0] = ltx + width;
                vert[1].position[1] = lty;
                vert[2].position[0] = ltx + width;
                vert[2].position[1] = lty - height;
                vert[3].position[0] = ltx;
                vert[3].position[1] = lty - height;

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
                        
                        /* cyan color by default */
                        vert[k].def_color[0] = 0.0f;
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


        MyGeometry generate_rectangle_two_sides(float width, float height)
        {
                std::vector<MyVertex> vert(8);
                std::vector<unsigned short> ord( { 0, 3, 2, 2, 1, 0,     //front side
                                                   4, 5, 6, 6, 7, 4,} ); //back side

                height = abs(height) > 2.0f ? 2.0f : height;
                width = abs(width) > 2.0f ? 2.0f : width;

                /* left top x */
                float ltx = 0.0f - (width / 2.0f);
                /* left top y */
                float lty = 0.0f + (height / 2.0f);

                /* front side */
                vert[0].position[0] = ltx;
                vert[0].position[1] = lty;
                vert[1].position[0] = ltx + width;
                vert[1].position[1] = lty;
                vert[2].position[0] = ltx + width;
                vert[2].position[1] = lty - height;
                vert[3].position[0] = ltx;
                vert[3].position[1] = lty - height;
                /* back side */
                vert[4].position[0] = ltx;
                vert[4].position[1] = lty;
                vert[5].position[0] = ltx + width;
                vert[5].position[1] = lty;
                vert[6].position[0] = ltx + width;
                vert[6].position[1] = lty - height;
                vert[7].position[0] = ltx;
                vert[7].position[1] = lty - height;

                /* "texture" coordinates */
                vert[0].plane_coord[0] = 0.0f;
                vert[0].plane_coord[1] = 1.0f;
                vert[1].plane_coord[0] = 1.0f;
                vert[1].plane_coord[1] = 1.0f;
                vert[2].plane_coord[0] = 1.0f;
                vert[2].plane_coord[1] = 0.0f;
                vert[3].plane_coord[0] = 0.0f;
                vert[3].plane_coord[1] = 0.0f;

                vert[4].plane_coord[0] = 1.0f;
                vert[4].plane_coord[1] = 1.0f;
                vert[5].plane_coord[0] = 0.0f;
                vert[5].plane_coord[1] = 1.0f;
                vert[6].plane_coord[0] = 0.0f;
                vert[6].plane_coord[1] = 0.0f;
                vert[7].plane_coord[0] = 1.0f;
                vert[7].plane_coord[1] = 0.0f;

                for (size_t k = 0; k < 4; ++k) {
                        /* putting front side of square straight on XY plane by default */
                        /* putting back side of square slightly after XY plane by default */
                        vert[k].position[2] = 0.0f;
                        vert[k + 4].position[2] = -0.001f;
                        /* fourth w-param, setting to 1 for both planes */
                        vert[k].position[3] = 1.0f;
                        vert[k + 4].position[3] = 1.0f;
                        
                        /* blue color for front side by default */
                        vert[k].def_color[0] = 0.0f;
                        vert[k].def_color[1] = 0.0f;
                        vert[k].def_color[2] = 1.0f;
                        vert[k].def_color[3] = 1.0f;
                        /* red color for back side by default */
                        vert[k + 4].def_color[0] = 1.0f;
                        vert[k + 4].def_color[1] = 0.0f;
                        vert[k + 4].def_color[2] = 0.0f;
                        vert[k + 4].def_color[3] = 1.0f;
                        
                        /* normal is along z-direction by default for front side */
                        vert[k].normal[0] = 0.0f;
                        vert[k].normal[1] = 0.0f;
                        vert[k].normal[2] = 1.0f;
                        /* normal is against z-direction by default for back side */
                        vert[k + 4].normal[0] = 0.0f;
                        vert[k + 4].normal[1] = 0.0f;
                        vert[k + 4].normal[2] = -1.0f;
                }

                return MyGeometry(vert, ord);
        }

        MyGeometry generate_cube(float sz)
        {
                const unsigned int CUBE_SIDES = 6;
                std::vector<MyVertex> vert(4 * CUBE_SIDES);
                std::vector<unsigned short> ord( { 0, 1, 2, 2, 3, 0,
                                                   4, 5, 6, 6, 7, 4,
                                                   8, 9, 10, 10, 11, 8,
                                                   12, 13, 14, 14, 15, 12,
                                                   16, 17, 18, 18, 19, 16,
                                                   20, 21, 22, 22, 23, 20,} );

                sz = abs(sz) > 2.0f ? 2.0f : sz;

                /* left-front-top x */
                float ltfx = 0.0f - (sz / 2.0f);
                /* left-front-top y */
                float ltfy = 0.0f + (sz / 2.0f);
                /* left-front-top z */
                float ltfz = 0.0f + (sz / 2.0f);

                /* close by Z*/
                vert[0].position[0] = ltfx; vert[0].position[1] = ltfy; vert[0].position[2] = ltfz;
                vert[13].position[0] = ltfx; vert[13].position[1] = ltfy; vert[13].position[2] = ltfz;
                vert[16].position[0] = ltfx; vert[16].position[1] = ltfy; vert[16].position[2] = ltfz;
                /**********************/
                vert[1].position[0] = ltfx + sz; vert[1].position[1] = ltfy; vert[1].position[2] = ltfz;
                vert[4].position[0] = ltfx + sz; vert[4].position[1] = ltfy; vert[4].position[2] = ltfz;
                vert[17].position[0] = ltfx + sz; vert[17].position[1] = ltfy; vert[17].position[2] = ltfz;
                /**********************/
                vert[2].position[0] = ltfx + sz; vert[2].position[1] = ltfy - sz; vert[2].position[2] = ltfz;
                vert[7].position[0] = ltfx + sz; vert[7].position[1] = ltfy - sz; vert[7].position[2] = ltfz;
                vert[21].position[0] = ltfx + sz; vert[21].position[1] = ltfy - sz; vert[21].position[2] = ltfz;
                /**********************/
                vert[3].position[0] = ltfx; vert[3].position[1] = ltfy - sz; vert[3].position[2] = ltfz;
                vert[14].position[0] = ltfx; vert[14].position[1] = ltfy - sz; vert[14].position[2] = ltfz;
                vert[20].position[0] = ltfx; vert[20].position[1] = ltfy - sz; vert[20].position[2] = ltfz;

                /* far by Z */
                vert[5].position[0] = ltfx + sz; vert[5].position[1] = ltfy; vert[5].position[2] = ltfz - sz;
                vert[8].position[0] = ltfx + sz; vert[8].position[1] = ltfy; vert[8].position[2] = ltfz - sz;
                vert[18].position[0] = ltfx + sz; vert[18].position[1] = ltfy; vert[18].position[2] = ltfz - sz;
                /**********************/
                vert[6].position[0] = ltfx + sz; vert[6].position[1] = ltfy - sz; vert[6].position[2] = ltfz - sz;
                vert[11].position[0] = ltfx + sz; vert[11].position[1] = ltfy - sz; vert[11].position[2] = ltfz - sz;
                vert[22].position[0] = ltfx + sz; vert[22].position[1] = ltfy - sz; vert[22].position[2] = ltfz - sz;
                /**********************/
                vert[9].position[0] = ltfx; vert[9].position[1] = ltfy; vert[9].position[2] = ltfz - sz;
                vert[12].position[0] = ltfx; vert[12].position[1] = ltfy; vert[12].position[2] = ltfz - sz;
                vert[19].position[0] = ltfx; vert[19].position[1] = ltfy; vert[19].position[2] = ltfz - sz;
                /**********************/
                vert[10].position[0] = ltfx; vert[10].position[1] = ltfy - sz; vert[10].position[2] = ltfz - sz;
                vert[15].position[0] = ltfx; vert[15].position[1] = ltfy - sz; vert[15].position[2] = ltfz - sz;
                vert[23].position[0] = ltfx; vert[23].position[1] = ltfy - sz; vert[23].position[2] = ltfz - sz;

                /* 1st side */
                vert[0].plane_coord[0] = 0.0f; vert[0].plane_coord[1] = 1.0f;
                vert[0].def_color[0] = 1.0f; vert[0].def_color[1] = 0.0f; vert[0].def_color[2] = 0.0f; vert[0].def_color[3] = 1.0f;
                vert[0].normal[0] = 0.0f; vert[0].normal[1] = 0.0f; vert[0].normal[2] = 1.0f;
                vert[1].plane_coord[0] = 1.0f; vert[1].plane_coord[1] = 1.0f;
                vert[1].def_color[0] = 1.0f; vert[1].def_color[1] = 0.0f; vert[1].def_color[2] = 0.0f; vert[1].def_color[3] = 1.0f;
                vert[1].normal[0] = 0.0f; vert[1].normal[1] = 0.0f; vert[1].normal[2] = 1.0f;
                vert[2].plane_coord[0] = 1.0f; vert[2].plane_coord[1] = 0.0f;
                vert[2].def_color[0] = 1.0f; vert[2].def_color[1] = 0.0f; vert[2].def_color[2] = 0.0f; vert[2].def_color[3] = 1.0f;
                vert[2].normal[0] = 0.0f; vert[2].normal[1] = 0.0f; vert[2].normal[2] = 1.0f;
                vert[3].plane_coord[0] = 0.0f; vert[3].plane_coord[1] = 0.0f;
                vert[3].def_color[0] = 1.0f; vert[3].def_color[1] = 0.0f; vert[3].def_color[2] = 0.0f; vert[3].def_color[3] = 1.0f;
                vert[3].normal[0] = 0.0f; vert[3].normal[1] = 0.0f; vert[3].normal[2] = 1.0f;

                /* 2nd side */
                vert[4].plane_coord[0] = 0.0f; vert[4].plane_coord[1] = 1.0f;
                vert[4].def_color[0] = 1.0f; vert[4].def_color[1] = 1.0f; vert[4].def_color[2] = 0.0f; vert[4].def_color[3] = 1.0f;
                vert[4].normal[0] = 1.0f; vert[4].normal[1] = 0.0f; vert[4].normal[2] = 0.0f;
                vert[5].plane_coord[0] = 1.0f; vert[5].plane_coord[1] = 1.0f;
                vert[5].def_color[0] = 1.0f; vert[5].def_color[1] = 1.0f; vert[5].def_color[2] = 0.0f; vert[5].def_color[3] = 1.0f;
                vert[5].normal[0] = 1.0f; vert[5].normal[1] = 0.0f; vert[5].normal[2] = 0.0f;
                vert[6].plane_coord[0] = 1.0f; vert[6].plane_coord[1] = 0.0f;
                vert[6].def_color[0] = 1.0f; vert[6].def_color[1] = 1.0f; vert[6].def_color[2] = 0.0f; vert[6].def_color[3] = 1.0f;
                vert[6].normal[0] = 1.0f; vert[6].normal[1] = 0.0f; vert[6].normal[2] = 0.0f;
                vert[7].plane_coord[0] = 0.0f; vert[7].plane_coord[1] = 0.0f;
                vert[7].def_color[0] = 1.0f; vert[7].def_color[1] = 1.0f; vert[7].def_color[2] = 0.0f; vert[7].def_color[3] = 1.0f;
                vert[7].normal[0] = 1.0f; vert[7].normal[1] = 0.0f; vert[7].normal[2] = 0.0f;

                /* 3rd side */
                vert[8].plane_coord[0] = 0.0f; vert[8].plane_coord[1] = 1.0f;
                vert[8].def_color[0] = 0.0f; vert[8].def_color[1] = 1.0f; vert[8].def_color[2] = 0.0f; vert[8].def_color[3] = 1.0f;
                vert[8].normal[0] = 0.0f; vert[8].normal[1] = 0.0f; vert[8].normal[2] = -1.0f;
                vert[9].plane_coord[0] = 1.0f; vert[9].plane_coord[1] = 1.0f;
                vert[9].def_color[0] = 0.0f; vert[9].def_color[1] = 1.0f; vert[9].def_color[2] = 0.0f; vert[9].def_color[3] = 1.0f;
                vert[9].normal[0] = 0.0f; vert[9].normal[1] = 0.0f; vert[9].normal[2] = -1.0f;
                vert[10].plane_coord[0] = 1.0f; vert[10].plane_coord[1] = 0.0f;
                vert[10].def_color[0] = 0.0f; vert[10].def_color[1] = 1.0f; vert[10].def_color[2] = 0.0f; vert[10].def_color[3] = 1.0f;
                vert[10].normal[0] = 0.0f; vert[10].normal[1] = 0.0f; vert[10].normal[2] = -1.0f;
                vert[11].plane_coord[0] = 0.0f; vert[11].plane_coord[1] = 0.0f;
                vert[11].def_color[0] = 0.0f; vert[11].def_color[1] = 1.0f; vert[11].def_color[2] = 0.0f; vert[11].def_color[3] = 1.0f;
                vert[11].normal[0] = 0.0f; vert[11].normal[1] = 0.0f; vert[11].normal[2] = -1.0f;

                /* 4th side */
                vert[12].plane_coord[0] = 0.0f; vert[12].plane_coord[1] = 1.0f;
                vert[12].def_color[0] = 0.0f; vert[12].def_color[1] = 1.0f; vert[12].def_color[2] = 1.0f; vert[12].def_color[3] = 1.0f;
                vert[12].normal[0] = -1.0f; vert[12].normal[1] = 0.0f; vert[12].normal[2] = 0.0f;
                vert[13].plane_coord[0] = 1.0f; vert[13].plane_coord[1] = 1.0f;
                vert[13].def_color[0] = 0.0f; vert[13].def_color[1] = 1.0f; vert[13].def_color[2] = 1.0f; vert[13].def_color[3] = 1.0f;
                vert[13].normal[0] = -1.0f; vert[13].normal[1] = 0.0f; vert[13].normal[2] = 0.0f;
                vert[14].plane_coord[0] = 1.0f; vert[14].plane_coord[1] = 0.0f;
                vert[14].def_color[0] = 0.0f; vert[14].def_color[1] = 1.0f; vert[14].def_color[2] = 1.0f; vert[14].def_color[3] = 1.0f;
                vert[14].normal[0] = -1.0f; vert[14].normal[1] = 0.0f; vert[14].normal[2] = 0.0f;
                vert[15].plane_coord[0] = 0.0f; vert[15].plane_coord[1] = 0.0f;
                vert[15].def_color[0] = 0.0f; vert[15].def_color[1] = 1.0f; vert[15].def_color[2] = 1.0f; vert[15].def_color[3] = 1.0f;
                vert[15].normal[0] = -1.0f; vert[15].normal[1] = 0.0f; vert[15].normal[2] = 0.0f;

                /* 5th side */
                vert[16].plane_coord[0] = 0.0f; vert[16].plane_coord[1] = 0.0f;
                vert[16].def_color[0] = 0.0f; vert[16].def_color[1] = 0.0f; vert[16].def_color[2] = 1.0f; vert[16].def_color[3] = 1.0f;
                vert[16].normal[0] = 0.0f; vert[16].normal[1] = 1.0f; vert[16].normal[2] = 0.0f;
                vert[17].plane_coord[0] = 1.0f; vert[17].plane_coord[1] = 0.0f;
                vert[17].def_color[0] = 0.0f; vert[17].def_color[1] = 0.0f; vert[17].def_color[2] = 1.0f; vert[17].def_color[3] = 1.0f;
                vert[17].normal[0] = 0.0f; vert[17].normal[1] = 1.0f; vert[17].normal[2] = 0.0f;
                vert[18].plane_coord[0] = 1.0f; vert[18].plane_coord[1] = 1.0f;
                vert[18].def_color[0] = 0.0f; vert[18].def_color[1] = 0.0f; vert[18].def_color[2] = 1.0f; vert[18].def_color[3] = 1.0f;
                vert[18].normal[0] = 0.0f; vert[18].normal[1] = 1.0f; vert[18].normal[2] = 0.0f;
                vert[19].plane_coord[0] = 0.0f; vert[19].plane_coord[1] = 1.0f;
                vert[19].def_color[0] = 0.0f; vert[19].def_color[1] = 0.0f; vert[19].def_color[2] = 1.0f; vert[19].def_color[3] = 1.0f;
                vert[19].normal[0] = 0.0f; vert[19].normal[1] = 1.0f; vert[19].normal[2] = 0.0f;

                /* 6th side */
                vert[20].plane_coord[0] = 1.0f; vert[20].plane_coord[1] = 1.0f;
                vert[20].def_color[0] = 1.0f; vert[20].def_color[1] = 0.0f; vert[20].def_color[2] = 1.0f; vert[20].def_color[3] = 1.0f;
                vert[20].normal[0] = 0.0f; vert[20].normal[1] = -1.0f; vert[20].normal[2] = 0.0f;
                vert[21].plane_coord[0] = 0.0f; vert[21].plane_coord[1] = 1.0f;
                vert[21].def_color[0] = 1.0f; vert[21].def_color[1] = 0.0f; vert[21].def_color[2] = 1.0f; vert[21].def_color[3] = 1.0f;
                vert[21].normal[0] = 0.0f; vert[21].normal[1] = -1.0f; vert[21].normal[2] = 0.0f;
                vert[22].plane_coord[0] = 0.0f; vert[22].plane_coord[1] = 0.0f;
                vert[22].def_color[0] = 1.0f; vert[22].def_color[1] = 0.0f; vert[22].def_color[2] = 1.0f; vert[22].def_color[3] = 1.0f;
                vert[22].normal[0] = 0.0f; vert[22].normal[1] = -1.0f; vert[22].normal[2] = 0.0f;
                vert[23].plane_coord[0] = 1.0f; vert[23].plane_coord[1] = 0.0f;
                vert[23].def_color[0] = 1.0f; vert[23].def_color[1] = 0.0f; vert[23].def_color[2] = 1.0f; vert[23].def_color[3] = 1.0f;
                vert[23].normal[0] = 0.0f; vert[23].normal[1] = -1.0f; vert[23].normal[2] = 0.0f;

                for (size_t k = 0; k < 4 * CUBE_SIDES; ++k)
                        vert[k].position[3] = 1.0f;

                return MyGeometry(vert, ord);
        }

        MyGeometry generate_triangle(float edge1, float edge2, float edge3)
        {
                std::vector<MyVertex> vert(3);
                std::vector<unsigned short> ord(3);

                return MyGeometry(vert, ord);
        }

}


