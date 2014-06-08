
#include <stdio.h>

#define DBG_SIMPLE_VECTORS
#include <utils/simple_vectors.h>


int main(int argc, char* argv[])
{
	Vec2_dbg<int>::data_type a;
	Vec2_dbg<float> b(.0f, 2.0f);
	printf("Vector B:"); b.TRACE();
	Vec2_dbg<float> c(3.0f, 0.f);
	printf("Vector C:"); c.TRACE();
	(b + c).TRACE();
	(b - c).TRACE();
	((b + c).normalize()).TRACE();
	((b - c).normalize()).TRACE();
	printf("All done, press ENTER to exit ...");
	getchar();
	return 0;
}

