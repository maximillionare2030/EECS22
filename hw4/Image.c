#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "Image.h"
#include "Constants.h"

/* Get the R intensity of pixel (x, y) in image */
unsigned char GetPixelR(const Image *image, unsigned int x, unsigned int y) {
    assertImage(image);

    return image->R[x + y * image->W];
}

/* Get the G intensity of pixel (x, y) in image */
unsigned char GetPixelG(const Image *image, unsigned int x, unsigned int y) {
    assertImage(image);
    return image->G[x + y * image->W];
}

/* Get the B intensity of pixel (x, y) in image */
unsigned char GetPixelB(const Image *image, unsigned int x, unsigned int y) {
    assertImage(image);
    return image->B[x + y * image->W];
}

/* Set the R intensity of pixel (x, y) in image to r */
void SetPixelR(Image *image, unsigned int x, unsigned int y, unsigned char r) {
    assertImage(image);
    image->R[x + y * image->W] = r;
}

/* Set the G intensity of pixel (x, y) in image to g */
void SetPixelG(Image *image, unsigned int x, unsigned int y, unsigned char g) {
    assertImage(image);
    image->G[x + y * image->W] = g;
}

/* Set the B intensity of pixel (x, y) in image to b */
void SetPixelB(Image *image, unsigned int x, unsigned int y, unsigned char b) {
    assertImage(image);
    image->B[x + y * image->W] = b;
}

/* Return the image's width in pixels */
unsigned int ImageWidth(const Image *image) {
    assertImage(image);
    return image->W;
}

/* Return the image's height in pixels */
unsigned int ImageHeight(const Image *image) {
    assertImage(image);
    return image->H;
}

/* Allocate dynamic memory for the image structure and its R/G/B values */
/* Return the pointer to the image, or NULL in case of error */
Image *CreateImage(unsigned int Width, unsigned int Height) {

    // Allocating memory for the image structure and its R/G/B values
    Image *image = malloc(sizeof(Image));
    image ->W = Width;
    image ->H = Height;
    image ->R = malloc(Width * Height * sizeof(unsigned char));
    image ->G = malloc(Width * Height * sizeof(unsigned char));
    image ->B = malloc(Width * Height * sizeof(unsigned char));


    //assert(image->R != NULL && image->G != NULL && image->B != NULL);
    return image;
}

/* Free the memory for the R/G/B values and Image structure */
void DeleteImage(Image *image) {
    assertImage(image);
    free(image->R);
    free(image->G);
    free(image->B);
    free(image);
}

void assertImage(const Image *image) {
    assert(image != NULL);
}
