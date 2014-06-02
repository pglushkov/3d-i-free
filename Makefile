
.PHONY: all

all:
	echo SPECIFY EXACT TARGET TO MAKE !!!

glx:
	gcc -O -o glx_tst glx_tst.c -lGL -lGLw -lGLU

clean:
	rm -f *.o glx_tst 
