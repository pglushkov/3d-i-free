#ifndef SIMPLE_VECTORS_H
#define SIMPLE_VECTORS_H


#include <cmath>
#include <iostream>
#include <stdexcept>
#include <array>

template<typename T, size_t sz> T vector_length(std::array<T, sz> inp)
{
	T result(0);
	for (size_t k = 0; k < sz; ++k)
		result += inp[k] * inp[k];
	result = sqrt(result);
	return result;
}

template<typename T, size_t sz> std::array<T, sz> vector_addition(std::array<T, sz> inp1, std::array<T, sz> inp2)
{
	std::array<T, sz> result;
	for (size_t k = 0; k < sz; ++k)
		result[k] = inp1[k] + inp2[k];
	return result;
}

template<typename T> class Vec2_dbg
{
	T x;
	T y;
	T len;
	

public:
	typedef T data_type;
	
	/* basic interfaces ... */
	T X() const { return x; };
	T Y() const { return y; };
	T length() const { return len; }
#ifdef DBG_SIMPLE_VECTORS
	void TRACE() const
	{
		std::cout << "TRACING DBG VECTOR " << this <<": X=" << x << ", Y=" << y << ", LEN="
			<< len << std::endl;
	}
#endif

	/* constructors ... */
	Vec2_dbg(): x(0), y(0), len(0) {}
	Vec2_dbg(T ix, T iy): x(ix), y(iy)
	{
		len = static_cast<T>( sqrt( x*x + y*y) );
	}

	/* operators and operations ... */
	Vec2_dbg<T>& operator+(const Vec2_dbg<T> &inp) const
	{
		return *new Vec2_dbg<T>(this->x + inp.X(), this->y + inp.Y());
	}
	Vec2_dbg<T>& operator-() const
	{
		return *new Vec2_dbg<T>(-(this->x), -(this->y));
	}
	Vec2_dbg<T>& operator-(const Vec2_dbg<T> &inp) const
	{
		return *this + (-inp);
	}
	Vec2_dbg<T>& normalize()
	{
		x /= len;
		y /= len;
		len = 1.0f;
		return *this;
	}
	Vec2_dbg<T>& operator+=(const Vec2_dbg<T> &inp)
	{
		this->x += inp.X();
		this->y += inp.Y();
		this->len = sqrt(x*x + y*y);
		return *this;
	}
	Vec2_dbg<T>& operator-=(const Vec2_dbg<T> &inp)
	{
		return *this+=(-inp);
	}
	Vec2_dbg<T>& operator*(const T inp)
	{
		return *(new Vec2_dbg<T>(this->x * inp, this->y * inp));
	}
	Vec2_dbg<T>& operator/(const T inp)
	{
		return (*this) * (1/inp);
	}
	Vec2_dbg<T>& operator*=(const T inp)
	{
		this->x *= inp;
		this->y *= inp;
		this->len = sqrt(x*x + y*y);
		return *this;
	}
	Vec2_dbg<T>& operator/=(const T inp)
	{
		return *this*=(1/inp);
	}
	T dot(const Vec2_dbg<T> &inp)
	{
		return this->x * inp.X() + this->y * inp.Y();
	}
	T cross(const Vec2_dbg<T> &inp)
	{
		throw new std::runtime_error("ERROR: vector cross-product is undefined in two-dimensional space ...");
		return T(0);
	}
};


template<typename T> class Vec2
{
	static const size_t __size = 2;
	std::array<T, __size> coords;
	T len;
public:
	T X() const { return coords[0]; };
	T Y() const { return coords[1]; };
	T length() const { return len; };
	const T* data() const { return coords.data(); }
#ifdef DBG_SIMPLE_VECTORS
	void TRACE() const
	{
		std::cout << "TRACING VECTOR " << this <<": X=" << this->X() << ", Y=" << this->Y() << ", LEN="
			<< this->length() << std::endl;
	}
#endif
	
	/* constructors ... */
	Vec2()
	{
		coords[0] = T(0);
		coords[1] = T(0);
		len = T(0);
	}
	Vec2(T ix, T iy)	{
		coords[0] = ix;
		coords[1] = iy;
		len = vector_length<T, __size>(coords);
	}

	typedef T data_type;
};

template<typename T> class Vec3
{
	T x;
	T y;
	T z;

public:
	typedef T data_type;
};

template<typename T> class Vec4
{
	T x;
	T y;
	T z;
	T u;

public:
	typedef T data_type;
};


#endif


