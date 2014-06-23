
.PHONY: all

I_FREE_3D_DIR = /home/peter/DEVEL/opengl/3d-i-free/

SOURCES = $(wildcard *.cpp)
SOURCES += ../utils/shader.cpp
OBJECTS = $(SOURCES:%.cpp=%.o)
TARGET = example2

COMP_FLAGS = -c -g -Wall -std=c++11
GL_LDLIBS = -lGL -lGLw -lGLU
LD_FLAGS = -g
CFLAGS = -I$(I_FREE_3D_DIR)

CC = g++-4.8
LD = g++-4.8

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(LD) $(LD_FLAGS) $^ -o $@ $(GL_LDLIBS)


clean:
	rm -f *.o $(TARGET) $(OBJECTS)

%.o: %.cpp
	$(CC) $(CFLAGS) $(COMP_FLAGS) $< -o $@


