/*********************************************************************/
/* FileIO.h: header file for I/O module	                             */
/*********************************************************************/
#ifndef FILE_IO_H
#define FILE_IO_H

#include "Image.h"

/* Read image from a file 				                              */
/* The size of the image needs to be pre-set				          */
/* The memory spaces of the image will be allocated in this function  */
/* Return values:							                          */
/* NULL: fail to load or create an image				              */
/* image: load or create an image successfully				          */
Image *LoadImage(const char *fname);

/* Save a processed image 		        */
/* Return values:			            */
/* 0: successfully saved the image	    */
/* 1: Cannot open the file for writing	*/
/* 2: File error while writing to file	*/
int SaveImage(const char *fname, const Image *image);

/* Allocate the memory space for the image structure                  */
/* and the memory spaces for the color intensity values               */
/* Return the pointer to thei mage, or NULL in case of error          */
Image *CreateImage(unsigned int Width, unsigned int Height);

/* Release the memory spaces for the pixel color intensity values     */
/* Deallocate all the memory spaces for the image                     */
void DeleteImage(Image *image);

#endif
