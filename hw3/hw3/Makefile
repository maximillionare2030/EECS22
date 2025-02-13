# Makefile: PhotoLab

CC = gcc
CFLAGS = -Wall -std=c11

# Targets for object files without DEBUG mode

# <Name of make command> : <dependencies>
# <command to build target>
FileIO.o: FileIO.c FileIO.h Constants.h
	$(CC) $(CFLAGS) -c FileIO.c -o FileIO.o


Advanced.o: Advanced.c Advanced.h Constants.h
	$(CC) $(CFLAGS) -c Advanced.c -o Advanced.o

DIPs.o: DIPs.c DIPs.h Constants.h
	$(CC) $(CFLAGS) -c DIPs.c -o DIPs.o

PhotoLab.o: PhotoLab.c FileIO.h Advanced.h Constants.h DIPs.h
	$(CC) $(CFLAGS) -c PhotoLab.c -o PhotoLab.o

Image.o: Image.c FileIO.h Image.h
	$(CC) $(CFLAGS) -c Image.c -o Image.o

Test.o: Test.c Test.h Image.h FileIO.h DIPs.h Advanced.h
	$(CC) $(CFLAGS) -c Test.c -o Test.o

libfilter.a: DIPs.o Advanced.o
	ar rc libfilter.a DIPs.o Advanced.o
	ranlib libfilter.a

PhotoLab: PhotoLab.o FileIO.o libfilter.a
	$(CC) -Wall PhotoLab.o FileIO.o -lfilter -L. -o PhotoLab -lm

all: PhotoLab

clean:
	rm -f *.o
	rm -f PhotoLab
	rm -f libfilter.a

