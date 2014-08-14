#ifndef MY_SQUARE_MATRIX_H
#define MY_SQUARE_MATRIX_H

template<typename T, size_t dim> class MySquareMatrix;
template<typename T, size_t dim> struct Det_Minors_Recur;

template<typename T, size_t dim> struct Det_Minors_Recur
{
        static T det ( MySquareMatrix<T, dim>& inp)
        {
                T result(0);
                char sign = 1;
                for (unsigned int c = 0; c < dim; ++c) {
                        MySquareMatrix<T, dim - 1> minor = inp.Minor(0, c);
//                        printf("Calculating minor, size = %lu  sign = %f  a = %f  ...\n", dim - 1, T(sign), inp[0][c]);
//                        minor.TRACE();
                        result += T(sign) * inp[0][c] * Det_Minors_Recur<T, dim - 1>::det( minor);
                        sign ^= 0xFE;
                }
                return result;
        }
};

template<typename T> struct Det_Minors_Recur <T, 1>
{
        static T det ( MySquareMatrix<T, 1>& inp)
        {
                return inp[0][0];
        }
};

template<typename T, size_t dim> class MySquareMatrix
{
public:
        typedef std::array<T, dim> data_row;
        typedef std::array<data_row, dim> full_data;
        const size_t size = dim;

private:
        full_data _data;

public:
        MySquareMatrix()
        {
        	if (dim < 1) {
        		throw std::runtime_error("ERROR in MySquareMatrix() : Size of square matrix can not be less then 1 !!!");
		}

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

        void FILL_SERIAL()
        {
                unsigned int num = 0;
                for (unsigned int r = 0; r < size; ++r) {
                        for (unsigned int c = 0; c < size; ++c ) {
                                _data[r][c] = num++;
                        }
                }
        }

        void Zero()
        {
        	memset(&data[0][0], 0, sizeof(T) * dim * dim);
        }

        MySquareMatrix< T, dim - 1> Minor(unsigned int row, unsigned int col)
        {
        	if ( row >= size || col >= size)
        		throw std::runtime_error("In MySquareMatrix.Minor() : invalid row / col input params!");
//        	Squareuconst unsigned int sz = dim - 1;
		MySquareMatrix<T, dim - 1> res;

		unsigned int rr = 0;
		unsigned int cc = 0;
		for ( unsigned int r = 0; r < size; ++r) {
			if ( r == row) continue;
			for (unsigned int c = 0; c < size; ++c) {
				if (c == col) continue;
				res[rr][cc++] = _data[r][c];
			}
			cc = 0;
			rr++;
		}
		return res;
        }

        T Cofactor(unsigned int row, unsigned int col)
        {
                MySquareMatrix<T, dim - 1> minor = Minor(row, col);
                return Det_Minors_Recur<T, dim - 1>::det(minor);
        }

        MySquareMatrix<T, dim> Invert()
        {
                MySquareMatrix<T, dim> result;
                T det = Det_Minors_Recur<T, dim>::det(*this);

                if (det == 0.0f) {
                        // here some more tricky check is supposed to be made ...
                        throw std::runtime_error("In MySquareMatrix.Invert(): Determinant is zero, this matrix sucks!");
                }

                for (unsigned int r = 0; r < dim; ++r) {
                        for (unsigned int c = 0; c < dim; ++c) {
                                result[r][c] = this->Cofactor(r,c);
                        }
                }
                result.Transpose();
                for (unsigned int r = 0; r < dim; ++r) {
                        for (unsigned int c = 0; c < dim; ++c) {
                                result[r][c] /= det;
                        }
                }
                return result;
        }

        MySquareMatrix<T, dim>& Transpose()
        {
                T buf;
                for (size_t d = 0; d < dim; ++d) {
                        for (size_t idx = d; idx < dim; ++idx) {
                                buf = _data[d][idx];
                                _data[d][idx] = _data[idx][d];
                                _data[idx][d] = buf;
                        }
                }
                return *this;
        }

//        T Det_Minor()
//        {
//                sdfsdf
//                return Det_Minors_Recur<size> (*this);
//        }

};

#endif // MY_SQUARE_MATRIX_H
