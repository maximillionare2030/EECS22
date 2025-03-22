/*********************************************************************/
/* Homework Assignment 5, for EECS 22, Winter 2024                   */
/* Movie.c: source file for basic movie manipulations                */
/*********************************************************************/

#include <stdlib.h>
#include <assert.h>
#include "Movie.h"
#include "ImageList.h"
#include "Image.h"

/* Allocate the memory space for the movie and the memory space */
/* for the frame list. Return the pointer to the movie. */
Movie *CreateMovie(void)
{
	ILIST *list = CreateImageList();
	Movie *movie = (Movie *)malloc(sizeof(Movie));
	assert(movie);
	movie -> Frames = list;
	assert(movie->Frames);
	return movie;

	/* to be implemented */
}

/* Release the memory space for the frame list. */
/* Release the memory space for the movie. */
void DeleteMovie(Movie *movie)
{
    assert(movie);

    if (movie->Frames) {  // Check if Frames exists before deletion
        DeleteImageList(movie->Frames);
        movie->Frames = NULL;
    }

    free(movie);
}

unsigned char clip(int x) {
    if (x < 0) return 0;
    if (x > 255) return 255;
    return (unsigned char)x;
}



void YUV2RGBMovie(Movie *movie) {
    assert(movie);
    assert(movie->Frames);

    ILIST *YUVFrames = movie->Frames;
    IENTRY *current = YUVFrames->First;

    while (current != NULL) {
        YUVImage *YuvIMAGE = (YUVImage *)current->Image;

        // Create a new RGB image
        Image *RGBImage = CreateImage(YuvIMAGE->W, YuvIMAGE->H);
        assert(RGBImage);

        // Loop through each pixel and convert from YUV to RGB
        for (unsigned int y = 0; y < YuvIMAGE->H; y++) {
            for (unsigned int x = 0; x < YuvIMAGE->W; x++) {
                int c = GetPixelY(YuvIMAGE, x, y) - 16;
                int d = GetPixelU(YuvIMAGE, x, y) - 128;
                int e = GetPixelV(YuvIMAGE, x, y) - 128;

                // Perform YUV to RGB conversion for each pixel
                int r = clip((298 * c + 409 * e + 128) >> 8);
                int g = clip((298 * c - 100 * d - 208 * e + 128) >> 8);
                int b = clip((298 * c + 516 * d + 128) >> 8);

                // Store the RGB values in the RGB image
                SetPixelR(RGBImage, x, y, r);
                SetPixelG(RGBImage, x, y, g);
                SetPixelB(RGBImage, x, y, b);
            }
        }

        // Update current entry's image to the new RGB image
        current->Image = (void *)RGBImage;

        // Free the YUV image
        DeleteYUVImage(YuvIMAGE);

        current = current->Next;
    }
}

/* Convert RGB 2 YUV*/
void RGB2YUVMovie(Movie *movie) {
    assert(movie);
    assert(movie->Frames);

    ILIST *RGBFrames = movie->Frames;
    IENTRY *current = RGBFrames->First;

    while (current != NULL) {
        Image *RGBIMAGE = (Image *)current->Image;

        // Create a new YUV image
        YUVImage *YuvIMAGE = CreateYUVImage(RGBIMAGE->W, RGBIMAGE->H);
        assert(YuvIMAGE);

        // Loop through each pixel and convert from RGB to YUV
        for (unsigned int y = 0; y < RGBIMAGE->H; y++) {
            for (unsigned int x = 0; x < RGBIMAGE->W; x++) {
                unsigned char r = GetPixelR(RGBIMAGE, x, y);
                unsigned char g = GetPixelG(RGBIMAGE, x, y);
                unsigned char b = GetPixelB(RGBIMAGE, x, y);

                // Perform RGB to YUV conversion for each pixel
                int Y = clip((66 * r + 129 * g + 25 * b + 128) >> 8) + 16;
                int U = clip((-38 * r - 74 * g + 112 * b + 128) >> 8) + 128;
                int V = clip((112 * r - 94 * g - 18 * b + 128) >> 8) + 128;

                // Store the Y, U, V values in the YUV image
                SetPixelY(YuvIMAGE, x, y, Y);
                SetPixelU(YuvIMAGE, x, y, U);
                SetPixelV(YuvIMAGE, x, y, V);
            }
        }

        // Update current entry's image to the new YUV image
        current->Image = (void *)YuvIMAGE;

        // Free the RGB image
        DeleteImage(RGBIMAGE);

        current = current->Next;
    }
}

/* EOF */
