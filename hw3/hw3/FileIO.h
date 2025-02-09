/* Header file for FileIO.c, with function declarations of LoadImage and SaveImage */
#ifndef FILEIO_H
#define FILEIO_H

#include <stdio.h>
#include "Constants.h"

/* load image from a file */
int LoadImage(const char fname[SLEN], unsigned char R[WIDTH][HEIGHT], unsigned char G[WIDTH][HEIGHT], unsigned char B[WIDTH][HEIGHT]);

/* save a processed image */
int SaveImage(const char fname[SLEN], unsigned char R[WIDTH][HEIGHT], unsigned char G[WIDTH][HEIGHT], unsigned char B[WIDTH][HEIGHT]);

#endif
