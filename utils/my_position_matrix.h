#ifndef MY_POSITION_MATRIX_H
#define MY_POSITION_MATRIX_H

#include <array>
#include <cmath>
#include <stdexcept>
#include <iostream>
#include <memory.h>

template<typename T, size_t dim> class MySquareMatrix
{
public:
        typedef std::array<T, dim> data_row;
        typedef std::array<data_row, dim> full_data;
        const size_t size = dim;

private:
        std::array<data_row, dim> _data;

public:
        MySquareMatrix()
        {
                Reset();
        }
        data_row* data() { return &_data[0]; }
        data_row& operator[] (const size_t idx)
        {
                if (idx >= dim)
                        throw std::runtime_error("In MyPositionMatrix : index exceeds matrix dimension!");
                return _data[idx];
        }


        void operator=(MySquareMatrix<T, dim>& inp)
        {
                for (size_t k = 0; k < dim; ++k)
                        for (size_t m = 0; m < dim; ++m)
                                _data[k][m] = inp[k][m];
        }

        MySquareMatrix<T, dim>& operator*=(MySquareMatrix<T,dim>& inp)
        {
                MySquareMatrix<T, dim> tmp;
                for (size_t k = 0; k < dim; ++k) {
                        for (size_t m = 0 ; m < dim; ++m) {

                                /* row-by-column multiplication */
                                tmp[k][m] = T(0);
                                for (size_t idx = 0; idx < dim; ++idx)
                                        tmp[k][m] += _data[k][idx] * inp[m][idx];
                        }
                }

                *this = tmp;
                return *this;
        }

        data_row operator*(data_row& vec)
        {
                data_row res;

                for (size_t k = 0; k < dim; ++k) {
                        res[k] = T(0);
                        for (size_t m = 0; m < dim; ++m) {
                                res[k] += _data[k][m] * vec[m];
                        }
                }

                return res;
        }

        void TRACE()
        {
                std::cout << "Tracing MySquareMatrix " << this << std::endl;
                for (size_t k = 0; k < dim; ++k) {
                        std::cout << "row " << k + 1 << ": ";
                        for (size_t m = 0; m < dim; ++m) {
                                std::cout << _data[k][m] << " ";
                        }
                        std::cout << std::endl;
                }
                std::cout << std::endl;
        }

        void Reset()
        {
                // reset matrix to unity
                for (size_t k = 0; k < dim; ++k) {
                        memset(&_data[k][0], 0, sizeof(T) * dim);
                        _data[k][k] = T(1);
                }
        }
};


template<typename T> MySquareMatrix<T, 4> CreateOXRotation(float angle)
{
        MySquareMatrix<T, 4> r;

        float rad = (angle/360.0f) * 2.0f * M_PI;

        r[1][1] = cos(rad);
        r[1][2] = -sin(rad);
        r[2][1] = sin(rad);
        r[2][2] = cos(rad);

        return r;
}

template<typename T> MySquareMatrix<T, 4> CreateOYRotation(float angle)
{
        MySquareMatrix<T, 4> r;

        float rad = (angle/360.0f) * 2.0f * M_PI;

        r[0][0] = cos(rad);
        r[0][2] = sin(rad);
        r[2][0] = -sin(rad);
        r[2][2] = cos(rad);

        return r;
}

template<typename T> MySquareMatrix<T, 4> CreateOZRotation(float angle)
{
        MySquareMatrix<T, 4> r;

        float rad = (angle/360.0f) * 2.0f * M_PI;

        r[0][0] = cos(rad);
        r[0][1] = -sin(rad);
        r[1][0] = sin(rad);
        r[1][1] = cos(rad);

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

        void SetPerspective( T per)
        {
                /* not quite sure yet how to properly implement it ... */
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
		T rad = (fov / 360.0f) * 2.0f * M_PI;
		T scale = 1.0f / tan(rad * 0.5f);
		//T height = 1.0f;
		//T width = height * aspect;
		//data[0][0] = near / (width / 2.0f) * scale;
		//data[1][1] = near / (height / 2.0f) * scale;
		/* if we presume, that width = 2.0 as in opengl space and height = width / aspect, then we can write: */
		data[0][0] = scale / aspect;
		data[1][1] = scale;
		data[2][2] = (-(far + near)) / (far - near);
		data[2][3] = (-2.0f*far*near) / (far - near);
		data[3][2] = -1.0f;
		data[3][3] = 0.0f;

	}

        void TRACE() { data.TRACE(); }

};

#endif


