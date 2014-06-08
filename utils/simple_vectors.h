#ifndef SIMPLE_VECTORS_H
#define SIMPLE_VECTORS_H


#include <cmath>
#include <iostream>

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
		std::cout << "TRACING VECTOR " << this <<": X=" << x << ", Y=" << y << ", LEN="
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
		*this + (-inp);
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


