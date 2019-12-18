PROGRAM = 1
CC      = gcc
CFLAGS  = -g 
LDLIBS  = -lglut -lGLU -lGL -lm

$(PROGRAM): main.o image.o
	$(CC) -o $(PROGRAM) image.o main.o $(LDLIBS)
