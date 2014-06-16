
.PHONY: all

TESTS = vector_tst glx_tst #(etc ...)

GL_LDLIBS = -lGL -lGLw -lGLU

I_FREE_3D_DIR = /home/peter/DEVEL/opengl/3d-i-free/

CFLAGS = -g -Wall -std=c++11

CC = g++-4.8

all:
	echo SPECIFY EXACT TARGET TO MAKE !!!

glx_tst: tst/glx_tst.cpp utils/shader.cpp
	$(CC) $(CFLAGS) -O $^ -o $@ $(GL_LDLIBS)

tst: $(TESTS)

clean:
	rm -f *.o $(TESTS)

vector_tst: tst/vectors_tst.cpp
	$(CC) $(CFLAGS) -I$(I_FREE_3D_DIR) $< -o $@


