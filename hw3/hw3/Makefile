# Makefile: PhotoLab

CC = gcc
CFLAGS = -Wall -std=c11

# Targets for object files without DEBUG mode
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

# Targets for object files with DEBUG mode
FileIOD.o: FileIO.c FileIO.h Constants.h
	$(CC) $(CFLAGS) -DDEBUG -c FileIO.c -o FileIOD.o

AdvancedD.o: Advanced.c Advanced.h Constants.h
	$(CC) $(CFLAGS) -DDEBUG -c Advanced.c -o AdvancedD.o

DIPsD.o: DIPs.c DIPs.h Constants.h
	$(CC) $(CFLAGS) -DDEBUG -c DIPs.c -o DIPsD.o

PhotoLabD.o: PhotoLab.c FileIO.h Advanced.h Constants.h DIPs.h
	$(CC) $(CFLAGS) -DDEBUG -c PhotoLab.c -o PhotoLabD.o

# Targets for executables
PhotoLab: PhotoLab.o FileIO.o libfilter.a
	$(CC) -Wall PhotoLab.o FileIO.o -lfilter -L. -o PhotoLab -lm

PhotoLabTest: PhotoLabD.o FileIOD.o libfilterD.a
	$(CC) -Wall PhotoLabD.o FileIOD.o -lfilter -L. -o PhotoLabTest -lm

libfilter.a: DIPs.o Advanced.o
	ar rc libfilter.a DIPs.o Advanced.o
	ranlib libfilter.a

libfilterD.a: DIPsD.o AdvancedD.o
	ar rc libfilterD.a DIPsD.o AdvancedD.o
	ranlib libfilterD.a

all: PhotoLab PhotoLabTest

clean:
	rm -f *.o
	rm -f PhotoLab PhotoLabTest
	rm -f libfilter.a libfilterD.a

test: PhotoLabTest
	./PhotoLabTest
