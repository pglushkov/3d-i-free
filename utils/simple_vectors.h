#ifndef SIMPLE_VECTORS_H
#define SIMPLE_VECTORS_H


#include <math.h>


template<typename T> class Vec2
{
	T x;
	T y;

public:
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


