PROGRAM = 1
CC      = gcc
CFLAGS  = -g 
LDLIBS  = -lglut -lGLU -lGL -lm

$(PROGRAM): main.o
	$(CC) -o $(PROGRAM) main.o $(LDLIBS)
