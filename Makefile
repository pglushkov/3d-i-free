
.PHONY: all

TESTS = vector_tst glx_tst #(etc ...)

GL_LDLIBS = -lGL -lGLw -lGLU

I_FREE_3D_DIR = /home/peter/DEVEL/3d-i-free/

all:
	echo SPECIFY EXACT TARGET TO MAKE !!!

glx_tst: tst/glx_tst.cpp
	g++ -O $< -o $@ $(GL_LDLIBS)

tst: $(TESTS)

clean:
	rm -f *.o $(TESTS)

vector_tst: tst/vectors_tst.cpp
	g++ -I$(I_FREE_3D_DIR) $< -o $@


