# Compiler settings
CC = gcc
CFLAGS = -Wall -std=c11

# Object files for different libraries
LIB_FILTER_OBJS = DIPs.o Advanced.o
LIB_BASIC_OBJS = FileIO.o Image.o Test.o

# Libraries
LIB_FILTER = libFilter.a
LIB_BASIC = libBasicFunc.a

# Compile individual object files
Advanced.o: Advanced.c Advanced.h
	$(CC) $(CFLAGS) -c Advanced.c -o Advanced.o

DIPs.o: DIPs.c DIPs.h Image.h
	$(CC) $(CFLAGS) -c DIPs.c -o DIPs.o

Image.o: Image.c Image.h
	$(CC) $(CFLAGS) -c Image.c -o Image.o

FileIO.o: FileIO.c FileIO.h Image.h
	$(CC) $(CFLAGS) -c FileIO.c -o FileIO.o

PhotoLab.o: PhotoLab.c Advanced.h DIPs.h FileIO.h Image.h Test.h
	$(CC) $(CFLAGS) -c PhotoLab.c -o PhotoLab.o

PhotoLabTest.o: PhotoLab.c Advanced.h DIPs.h FileIO.h Image.h Test.h
	$(CC) $(CFLAGS) -DDEBUG -std=c11 -c PhotoLab.c -o PhotoLabTest.o

Test.o: Test.c Test.h
	$(CC) $(CFLAGS) -c Test.c -o Test.o

# Create the static library for filters
$(LIB_FILTER): $(LIB_FILTER_OBJS)
	ar rc $(LIB_FILTER) $(LIB_FILTER_OBJS)
	ranlib $(LIB_FILTER)

# Create the static library for basic functions
$(LIB_BASIC): $(LIB_BASIC_OBJS)
	ar rc $(LIB_BASIC) $(LIB_BASIC_OBJS)
	ranlib $(LIB_BASIC)

# Link the main executable
PhotoLab: PhotoLab.o $(LIB_FILTER) $(LIB_BASIC)
	$(CC) PhotoLab.o -L. -lBasicFunc -lFilter -lm -o PhotoLab

# Link the test executable
PhotoLabTest: PhotoLabTest.o $(LIB_FILTER) $(LIB_BASIC)
	$(CC) PhotoLabTest.o -L. -lBasicFunc -lFilter -lm -o PhotoLabTest

# Build all
all: PhotoLab PhotoLabTest

# Clean rule
clean:
	rm -f *.o *.a PhotoLab PhotoLabTest
