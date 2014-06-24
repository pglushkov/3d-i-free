
.PHONY: all

GL_LDLIBS = -lGL -lGLw -lGLU
I_FREE_3D_DIR = /home/peter/DEVEL/opengl/3d-i-free/
COMP_FLAGS = -c -g -Wall -std=c++11
CFLAGS = -I$(I_FREE_3D_DIR)

CC = g++-4.8
LD = g++-4.8

TARGET = example1

SOURCES = my_material.cpp example1.cpp shaders_code.cpp
OBJECTS = $(SOURCES:%.cpp=%.o)

all: $(TARGET)

clean:
	rm -f *.o $(OBJECTS) $(TARGET)

$(TARGET) : $(OBJECTS)
	$(LD) $^ -o $@ $(LD_FLAGS) $(GL_LDLIBS)

%.o : %.cpp
	$(CC) $(COMP_FLAGS) $(CFLAGS) $< -o $@

