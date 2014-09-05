

#include "small_tests.h"

/* common includes */
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <ctime>
#include <cmath>

/* x-server related includes */
#include<X11/X.h>
#include<X11/Xlib.h>

#include <utils/opengl.h>

#include <utils/my_mesh.h>
#include <utils/simple_vectors.h>
#include <utils/misc_utils.h>
#include <utils/my_position_matrix.h>
#include <utils/my_square_matrix.h>

#include <FreeImage.h>


void test_pos_mat()
{
        // MySquareMatrix<float, 4> a;
        // MySquareMatrix<float, 4> a1;

        // a.TRACE();

        // std::cout << a[1][2] << std::endl;
        // //std::cout << a[4][0] << std::endl; //this line will cause an exception!

        // for (size_t k = 0; k < a.size; ++k)
                // for (size_t m = 0; m < a.size; ++m) {
                        // a[k][m] = float(k*m);
                        // a1[k][m] = 3.0f;
                // }

        // a.TRACE();
        // a1.TRACE();
        // a1 *= a;
        // a1.TRACE();

        // auto b = CreateOXRotation<float> (30.0f);
        // b.TRACE();

        // a = b;
        // a.TRACE();


        MyPositionMatrix<float> c;
        c.TRACE();
        c.Rotate_Z(30);
        c.TRACE();


        MySquareMatrix<float, 4> :: data_row v;
        v[0] = 1.01; v[1] = 1.0f; v[2] = 1.0f; v[3] = 1.0f;

        Vec4<float> v1(c * v);
        v1.TRACE();

        MyPositionMatrix<float> c1;
        c1.TRACE();
        float dbg[16];
        memcpy(dbg, &c1.get_data()[0][0], sizeof(float) * 16);
        printf("Results of memcpy from MyPositionMatrix.get_data() : ");
        for (unsigned int k = 0; k < 16; ++k)
                printf(" %f ", dbg[k]);
        printf("\n ************* \n");
        printf(" size of data_row: %u \n",sizeof(MyPositionMatrix<float>::data_row));
        printf(" size of full_data: %u \n",sizeof(MySquareMatrix<float, 4>::full_data));
}

void test_free_image()
{
        FIBITMAP* bmap = FreeImage_Load(FIF_JPEG, "../data/green_red.jpg", JPEG_DEFAULT);
        //FIBITMAP* bmap = FreeImage_Load(FIF_BMP, "../data/green_red.bmp", BMP_DEFAULT);

        /* print something about it ... */
        std::cout << "FIBITMAP = " << bmap << ", BPP = " << FreeImage_GetBPP(bmap)
                  << ", Width = " << FreeImage_GetWidth(bmap) << ", Height = " << FreeImage_GetHeight(bmap)
                  << ", ColorType = " << FreeImage_GetColorType(bmap) << ", DataBits = "
                  << (void*)FreeImage_GetBits(bmap) << std::endl;

        unsigned int bytespl = FreeImage_GetLine(bmap); /* getting number of bytes-per-line */
        unsigned int bytespp = bytespl / FreeImage_GetWidth(bmap);
        std::cout << "Bytes-Per-Line: " << bytespl << ", Bytes-Per-Pixel: " << bytespp << std::endl;
        for (size_t h = 0; h < FreeImage_GetHeight(bmap); ++h) {
                /* getting address of first pixel in current line */
                BYTE* bits = FreeImage_GetScanLine(bmap, h);
                for (unsigned int w = 0; w < FreeImage_GetWidth(bmap); ++w) {

                        /* do smth with current pixel */
                        // std::cout << " RGB (" << h + 1 << ":" << w + 1 << "): "
                               // << (int)bits[FI_RGBA_RED] << "," << (int)bits[FI_RGBA_GREEN]
                               // << "," << (int)bits[FI_RGBA_BLUE];
                        /* jump to next pixel in current line*/
                        bits += bytespp;
                }
                // std::cout << std::endl;
        }

        FreeImage_Unload(bmap);
}

void test_square_matrix()
{
        const size_t sz = 4;
        const size_t msz = sz - 1;
        const size_t msz1 = msz - 1;
        const size_t msz2 = msz1 - 1;
        MySquareMatrix<float, sz> mat;
        mat.FILL_SERIAL();

        MySquareMatrix<float, msz> mminor= mat.Minor(0,0);
        MySquareMatrix<float, msz1> mminor1 = mminor.Minor(2,2);
        MySquareMatrix<float, msz2> mminor2 = mminor1.Minor(0,0);

        mat.TRACE();
        printf("Determinnant of above matrix is: %f ...\n", Det_Minors_Recur<float, sz>::det(mat));
        mat.Transpose();
        mat.TRACE();
        mminor.TRACE();
        mminor1.TRACE();
        mminor2.TRACE();

        MySquareMatrix<float, 3> tst;
        tst[0][0] = 3.0f;
        tst[0][1] = 4.0f;
        tst[0][2] = 1.0f;
        tst[1][0] = 10.0f;
        tst[1][1] = 2.0f;
        tst[1][2] = 8.0f;
        tst[2][0] = 1.0f;
        tst[2][1] = 1.0f;
        tst[2][2] = 3.0f;

        MySquareMatrix<float, msz> inv = tst.Invert();
        inv.TRACE();
}

