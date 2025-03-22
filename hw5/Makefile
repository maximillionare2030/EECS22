#######################################################################
# Homework Assignment 5, for EECS 22, Winter 2025
#
# Initial author: Max Truong
#
# Makefile: Makefile for HW 5
#
#######################################################################

CC     = gcc
DEBUG  = -DDEBUG
CFLAGS = -Wall -std=c11 -c
LFLAGS = -Wall -lm

all: MovieLab

########### generate object files ###########

#target to generate Image.o
Image.o: Image.c Image.h
	$(CC) $(CFLAGS) Image.c -o Image.o

#target to generate DIPs.o
DIPs.o: DIPs.h DIPs.c Constants.h Image.h
	$(CC) $(CFLAGS) DIPs.c -o DIPs.o

#target to generate FileIO.o
FileIO.o: FileIO.h FileIO.c Constants.h Image.h
	$(CC) $(CFLAGS) FileIO.c -o FileIO.o

#target to generate ImageList.o
ImageList.o: ImageList.h ImageList.c Image.h
	$(CC) $(CFLAGS) ImageList.c -o ImageList.o

#target to generate Movie.o
Movie.o: Movie.h Movie.c ImageList.h Image.h
	$(CC) $(CFLAGS) Movie.c -o Movie.o

#target to generate MovieLab.o
MovieLab.o: MovieLab.c FileIO.h DIPs.h Movie.h Constants.h
	$(CC) $(CFLAGS) MovieLab.c -o MovieLab.o

########### generate executable ###########

#target to generate MovieLab
MovieLab: MovieLab.o Image.o DIPs.o FileIO.o ImageList.o Movie.o
	$(CC) $(LFLAGS) MovieLab.o Image.o DIPs.o FileIO.o ImageList.o Movie.o -o MovieLab

#target to clean the directory
clean:
	rm -f *.o MovieLab
