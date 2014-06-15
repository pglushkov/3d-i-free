
#include <stdio.h>
#include <iostream>
#include <typeinfo>
#include <array>

#define DBG_SIMPLE_VECTORS
#include <utils/simple_vectors.h>

int main(int argc, char* argv[])
{
	 /*
	  *NOTE: further block of code creates dozens of memory leaks, it's only hear for fast "try'n'see" procedure
	  */
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
	Vec2_dbg<float>& dbg = (b + c);
	delete &dbg;
	printf("B dot C : %f ...\n", b.dot(c));
	//printf("B cross C: %f ...\n", b.cross(c)); //WILL THROW AND EXCEPTION !!!
	/*************************** END OF "BAD" CODE **************************************************/

	/* same stuff for Vec2 class ... */
	Vec2<int>::data_type a1;
	std::cout << "type of Vec2<int>::data_type : " << typeid(a1).name() << std::endl;
	Vec2<float> b1(0.0f, 2.0f);
	printf("Vector B:"); b1.TRACE();
	Vec2<float> c1(3.0f, 0.0f);
	printf("Vector C:"); c1.TRACE();
	Vec2<float> d1(c1.data());
	printf("Vector D:"); d1.TRACE();
	//std::cout << c1[3] << std::endl; // will throws runtime_error cause breaking boundaries of Vec2
	(-c1).TRACE(); /* no memory leak but an actual object is created and returned ... */
	-c1; /* no memory leak but an actual object is created and returned ... */
	(b1 + c1).TRACE(); /* and again a new object is created and returned ... */
	(d1 += c1).TRACE(); /* no new object is created, but keep in mind, that d1 is object, created in local scope */
	(d1 -= c1).TRACE();
	(d1 * 10).TRACE();
	(d1 *= 5).TRACE();
	(d1 / 3).TRACE();
	(d1 /= 3).TRACE();
	printf("B dot C : %f ...\n", b1.dot(c1));

	printf("All done, press ENTER to exit ...");
	getchar();
	return 0;
}

