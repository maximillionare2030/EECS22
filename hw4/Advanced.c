#include <stdio.h>
#include "Advanced.h"

Image *Crop(Image *image, int x, int y, int W, int H) {
    assertImage(image);

    if (x < 0 || y < 0 || x + W > image->W || y + H > image->H) {
        printf("Error: Crop dimensions out of bounds!\n");
        return NULL;
    }

    Image *cropped_image = CreateImage(W, H);
    for (int i = y; i < y + H; i++) {
        for (int j = x; j < x + W; j++) {
            SetPixelR(cropped_image, j - x, i - y, GetPixelR(image, j, i));
            SetPixelG(cropped_image, j - x, i - y, GetPixelG(image, j, i));
            SetPixelB(cropped_image, j - x, i - y, GetPixelB(image, j, i));
        }
    }

    // Copy changes to original image
    DeleteImage(image);
    image = CreateImage(W, H);
    for (int i = 0; i < H; i++) {
        for (int j = 0; j < W; j++) {
            SetPixelR(image, j, i, GetPixelR(cropped_image, j, i));
            SetPixelG(image, j, i, GetPixelG(cropped_image, j, i));
            SetPixelB(image, j, i, GetPixelB(cropped_image, j, i));
        }
    }


    return cropped_image;
}


/* Resize */
Image *Resize(Image *image, int newWidth, int newHeight) {}

/* Watermark */
Image *Watermark(Image *image, const Image *watermark_image) {}

/* Rotate by 90 */
Image *RotateBy90(Image *image, int rotateDirection) {}

