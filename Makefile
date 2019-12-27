PROGRAM = lava
CC      = gcc
CFLAGS  = -g 
LDLIBS  = -lglut -lGLU -lGL -lm

$(PROGRAM): main.o image.o utility.o models.o
	$(CC) -o $(PROGRAM) image.o main.o utility.o models.o $(LDLIBS)
