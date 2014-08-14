
.PHONY: all

I_FREE_3D_DIR = /home/peter/DEVEL/opengl/3d-i-free/

SOURCES = $(wildcard *.cpp)
SOURCES += ../utils/my_material.cpp
SOURCES += ../utils/my_geometry.cpp
SOURCES += ../utils/my_mesh.cpp
SOURCES += ../utils/opengl.cpp
SOURCES += ../utils/misc_utils.cpp
SOURCES += ../utils/my_world.cpp
OBJECTS = $(SOURCES:%.cpp=%.o)
TARGET = camera_move

COMP_FLAGS = -c -g -Wall -std=c++11
GL_LDLIBS = -lGL -lGLw
LD_FLAGS = -g
CFLAGS = -I$(I_FREE_3D_DIR)
LD_LIBS = $(GL_LDLIBS) -lfreeimage

CC = g++-4.8
LD = g++-4.8

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(LD) $(LD_FLAGS) $^ -o $@ $(LD_LIBS)


clean:
	rm -f *.o $(TARGET) $(OBJECTS)

%.o: %.cpp
	$(CC) $(CFLAGS) $(COMP_FLAGS) $< -o $@


