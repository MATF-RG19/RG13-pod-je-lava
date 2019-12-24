PROGRAM = lava
CC      = gcc
CFLAGS  = -g 
LDLIBS  = -lglut -lGLU -lGL -lm

$(PROGRAM): main.o image.o logic.o models.o
	$(CC) -o $(PROGRAM) image.o main.o logic.o models.o $(LDLIBS)
