
#include <stdio.h>
#include <iostream>
#include <typeinfo>

#define DBG_SIMPLE_VECTORS
#include <utils/simple_vectors.h>


int main(int argc, char* argv[])
{
	/* basic tests for Vec2_dbg reference class ... */
	Vec2_dbg<int>::data_type a;
	std::cout << "type of Vec2_dbg<int>::data_type : " << typeid(a).name() << std::endl;
	Vec2_dbg<float> b(.0f, 2.0f);
	printf("Vector B:"); b.TRACE();
	Vec2_dbg<float> c(3.0f, 0.f);
	printf("Vector C:"); c.TRACE();
	(b + c).TRACE();
	(b - c).TRACE();
	((b + c).normalize()).TRACE();
	((b - c).normalize()).TRACE();
	(b+=c).TRACE();
	(b-=c).TRACE();
	((b + c)*2).TRACE();
	((b + c)/2).TRACE();
	(c*=2).TRACE();
	(c/=2).TRACE();
	printf("B dot C : %f ...\n", b.dot(c));
	//printf("B cross C: %f ...\n", b.cross(c)); //WILL THROW AND EXCEPTION !!!

	/* same stuff for Vec2 class ... */
	Vec2<int>::data_type a1;
	std::cout << "type of Vec2<int>::data_type : " << typeid(a1).name() << std::endl;
	Vec2<float> b1(.0f, 2.0f);
	printf("Vector B:"); b1.TRACE();
	Vec2<float> c1(3.0f, 0.f);
	printf("Vector C:"); c1.TRACE();



	printf("All done, press ENTER to exit ...");
	getchar();
	return 0;
}

