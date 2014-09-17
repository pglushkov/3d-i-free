#ifndef MY_POSITION_MATRIX_H
#define MY_POSITION_MATRIX_H

#include <array>
#include <cmath>
#include <stdexcept>
#include <iostream>
#include <memory.h>

#include "misc_utils.h"
#include "my_square_matrix.h"

template<typename T> MySquareMatrix<T, 4> CreateOXRotation(float angle)
{
    //                  !!!!! ATTENTION !!!!!
    //  OpenGL matrices are column-major, our Matrix implementaion is row-major. We have to consider this fact
    //  when we pass our matrix by glUniformMatrixNfv(). This row-column-majorness change results into a
    //  transposition of our matrix. To account for this fact we intentionally generate transposed versions
    //  of rotation matrices for right-handed coordinate system.

        MySquareMatrix<T, 4> r;
        float rad = my_utils::DegToRad(angle);

        r[1][1] = cos(rad);
        r[1][2] = sin(rad);
        r[2][1] = -sin(rad);
        r[2][2] = cos(rad);

        return r;
}

template<typename T> MySquareMatrix<T, 4> CreateOYRotation(float angle)
{
    //                  !!!!! ATTENTION !!!!!
    //  OpenGL matrices are column-major, our Matrix implementaion is row-major. We have to consider this fact
    //  when we pass our matrix by glUniformMatrixNfv(). This row-column-majorness change results into a
    //  transposition of our matrix. To account for this fact we intentionally generate transposed versions
    //  of rotation matrices for right-handed coordinate system.

        MySquareMatrix<T, 4> r;
        float rad = my_utils::DegToRad(angle);

        r[0][0] = cos(rad);
        r[0][2] = -sin(rad);
        r[2][0] = sin(rad);
        r[2][2] = cos(rad);

        return r;
}

template<typename T> MySquareMatrix<T, 4> CreateOZRotation(float angle)
{
    //                  !!!!! ATTENTION !!!!!
    //  OpenGL matrices are column-major, our Matrix implementaion is row-major. We have to consider this fact
    //  when we pass our matrix by glUniformMatrixNfv(). This row-column-majorness change results into a
    //  transposition of our matrix. To account for this fact we intentionally generate transposed versions
    //  of rotation matrices for right-handed coordinate system.

        MySquareMatrix<T, 4> r;
        float rad = my_utils::DegToRad(angle);

        r[0][0] = cos(rad);
        r[0][1] = sin(rad);
        r[1][0] = -sin(rad);
        r[1][1] = cos(rad);

        return r;
}

template<typename T> MySquareMatrix<T, 4> CreateAxisRotation(T x, T y, T z, float angle)
{
    //                  !!!!! ATTENTION !!!!!
    //  OpenGL matrices are column-major, our Matrix implementaion is row-major. We have to consider this fact
    //  when we pass our matrix by glUniformMatrixNfv(). This row-column-majorness change results into a
    //  transposition of our matrix. To account for this fact we intentionally generate transposed versions
    //  of rotation matrices for right-handed coordinate system.

        float rad = my_utils::DegToRad(angle);
        float COS = cos(rad);
        float SIN = sin(rad);

        MySquareMatrix<T, 4> r;

        r[0][0] = COS + (1.0f - COS)*x*x;       r[0][1] = (1.0f - COS)*x*y + SIN*z;     r[0][2] = (1.0f - COS)*x*z - SIN*y;
        r[1][0] = (1.0f - COS)*y*x - SIN*z;     r[1][1] = COS + (1.0f - COS)*y*y;       r[1][2] = (1.0f - COS)*y*z + SIN*x;
        r[2][0] = (1.0f - COS)*z*x + SIN*y;     r[2][1] = (1.0f - COS)*z*y - SIN*x;     r[2][2] = COS + (1.0f - COS)*z*z;

        return r;
}

template<typename T> class MyPositionMatrix
{
        MySquareMatrix<T, 4> data;
public:
        const size_t size = 4;
        typedef std::array<T, 3> translation_vector;
        typedef std::array<T, 3> scale_vector;
        typedef typename MySquareMatrix<T, 4>::data_row data_row;

        MySquareMatrix<T,4>& get_data() { return data; }

        void Translate(const translation_vector& tr)
        {
                for (size_t k = 0; k < this->size - 1; ++k)
                        this->data[k][this->size - 1] += tr[k];
        }

        void Translate(const T x, const T y, const T z)
        {
                this->data[0][3] += x;
                this->data[1][3] += y;
                this->data[2][3] += z;
        }

        void SetPosition(const translation_vector& pos)
        {
                data[0][3] = pos[0];
                data[1][3] = pos[1];
                data[2][3] = pos[2];
        }

        void SetPosition(const T x, const T y, const T z)
        {
        	data[0][3] = x;
        	data[1][3] = y;
        	data[2][3] = z;
        }

        void Scale(const scale_vector& sc)
        {
                for (size_t k = 0; k < this->size - 1; ++k)
                        this->data[k][k] *= sc[k];

        }

        void Scale(const T scal)
        {
                for (size_t k = 0; k < this->size - 1; ++k)
                        this->data[k][k] *= scal;
        }

        MyPositionMatrix(const MySquareMatrix<T, 4>& inp)
        {
                this->_data = inp->data();
        }

        MyPositionMatrix() {}


        void TRACE() { data.TRACE(); }
        void TRACE_AXIS_GLOB()
        {
                data_row axis({0.0f, 0.0f, 1.0f, 1.0f});
                MySquareMatrix<float, 4> inv = data.Invert();
                data_row global_axis = inv * axis;
                global_axis[2] = -global_axis[2];
                std::cout << "axis.x=" << global_axis[0] << " axis.y="
                          << global_axis[1] << " axis.z=" << global_axis[2] << std::endl;

                std::cout << "glob_pos.x=" << inv[0][3] << " glob_pos.y="
                          << inv[1][3] << " glob_pos.z=" << inv[2][3] << std::endl;
        }

        void Rotate_X(float angle)
        {
                auto tmp = CreateOXRotation<T>(angle);
                data *= tmp;
        }

        void Rotate_Y(float angle)
        {
                auto tmp = CreateOYRotation<T>(angle);
                data *= tmp;
        }

        void Rotate_Z(float angle)
        {
                auto tmp = CreateOZRotation<T>(angle);
                data *= tmp;
        }

        void Rotate_Axis( T x, T y, T z, float angle)
        {
                auto tmp = CreateAxisRotation<T>(x, y, z, angle);
                data *= tmp;
        }

        void RotateGlobal_Y(float angle)
        {
//                auto inv = data.Invert();
                data_row y_axis({0.0f, 1.0f, 0.0f, 1.0f});
                MySquareMatrix<float, 4> inv = data.Invert();
                data_row global_y_axis = inv * y_axis;
//                std::cout << "glob_y.x=" << global_y_axis[0] << " glob_y.y="
//                          << global_y_axis[1] << " glob_y.z=" << global_y_axis[2] << std::endl;
                Rotate_Axis(global_y_axis, angle);
        }

        void Rotate_Axis(std::array<T,3>& axis, float angle)
        {
                Rotate_Axis(axis[0], axis[1], axis[2], angle);
        }

        void Rotate_Axis(data_row& axis, float angle)
        {
                Rotate_Axis(axis[0], axis[1], axis[2], angle);
        }

        MyPositionMatrix<T>& operator *= (MyPositionMatrix<T>& inp)
        {
                data *= inp.get_data();

                return *this;
        }

        data_row operator* (data_row& inp)
        {
                data_row res = data * inp;
                return res;
        }

        void Reset()
        {
                data.Reset();
        }

};


template<typename T> class MyProjectionMatrix
{
        MySquareMatrix<T, 4> data;
public:
        const size_t size = 4;
        typedef typename MySquareMatrix<T, 4>::data_row data_row;

        MySquareMatrix<T,4>& get_data() { return data; }


        MyProjectionMatrix(const T near, const T far, const T fov)
        {
                float rad = (fov / 360.0f) * 2.0f * M_PI;
                T scale = T(1) / tan(rad * 0.5f);
                data[0][0] = scale;
                data[1][1] = scale;
                data[2][2] = -far / (far - near);
                data[3][2] = -far * near / (far - near);
                data[2][3] = -1;
                data[3][3] = 0;
        }

    MyProjectionMatrix(const T near, const T far, const T aspect, const T fov)
	{
        T near1 = near;
        T far1 = far;

		T rad = (fov / 360.0f) * 2.0f * M_PI;
		T scale = 1.0f / tan(rad * 0.5f);
		//T height = 1.0f;
		//T width = height * aspect;
		//data[0][0] = near / (width / 2.0f) * scale;
		//data[1][1] = near / (height / 2.0f) * scale;
		/* if we presume, that width = 2.0 as in opengl space and height = width / aspect, then we can write: */
		data[0][0] = scale / aspect;
		data[1][1] = scale;
        data[2][2] = (-(far1 + near1)) / (far1 - near1);
        data[2][3] = (-2.0f*far1*near1) / (far1 - near1);
		data[3][2] = -1.0f;
		data[3][3] = 0.0f;

        /* dbg */
        //data.Reset();
	}

        void TRACE() { data.TRACE(); }

};

#endif


