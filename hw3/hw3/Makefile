#Makefile: PhotoLab

all: PhotoLab PhotoLabTest

clean:
	rm -f *.o
	rm -f *.a
	rm -f PhotoLab
	rm -f PhotoLabTest

PhotoLab.o: PhotoLab.c FileIO.h Advanced.h DIPs.h Constants.h
	gcc -Wall -std=c11 -c PhotoLab.c -o PhotoLab.o

PhotoLabTest.o: PhotoLab.c FileIO.h Advanced.h DIPs.h Constants.h
	gcc -Wall -DDEBUG -std=c11 -c PhotoLab.c -o PhotoLabTest.o

FileIO.o: FileIO.c FileIO.h Constants.h
	gcc -Wall -std=c11 -c FileIO.c -o FileIO.o

Advanced.o: Advanced.c Advanced.h Constants.h
	gcc -Wall -std=c11 -c Advanced.c -o Advanced.o

DIPs.o: DIPs.c DIPs.h Constants.h
	gcc -Wall -std=c11 -c DIPs.c -o DIPs.o

PhotoLab: PhotoLab.o FileIO.o libFilter.a
	gcc PhotoLab.o FileIO.o -L. -lFilter -lm -o PhotoLab

PhotoLabTest: FileIO.o PhotoLabTest.o libFilter.a
	gcc -g PhotoLabTest.o FileIO.o -L. -lFilter -lm -o PhotoLabTest

libFilter.a: DIPs.o Advanced.o
	ar rc libFilter.a DIPs.o Advanced.o
	ranlib libFilter.a
