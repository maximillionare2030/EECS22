#include <stdio.h>
#include "assert.h"
#include "Advanced.h"
#include "math.h"

Image *Crop(Image *image, int x, int y, int W, int H) {
    assertImage(image);

    if (x < 0 || y < 0 || x + W > image->W || y + H > image->H) {
        printf("Error: Crop dimensions out of bounds!\n");
        return NULL;
    }

    Image *cropImage = CreateImage(W, H);
    for (int i = y; i < y + H; i++) {
        for (int j = x; j < x + W; j++) {
            SetPixelR(cropImage, j - x, i - y, GetPixelR(image, j, i));
            SetPixelG(cropImage, j - x, i - y, GetPixelG(image, j, i));
            SetPixelB(cropImage, j - x, i - y, GetPixelB(image, j, i));
        }
    }

    // Copy changes to original image
    DeleteImage(image);
    image = CreateImage(W, H);
    for (int i = 0; i < H; i++) {
        for (int j = 0; j < W; j++) {
            SetPixelR(image, j, i, GetPixelR(cropImage, j, i));
            SetPixelG(image, j, i, GetPixelG(cropImage, j, i));
            SetPixelB(image, j, i, GetPixelB(cropImage, j, i));
        }
    }

    // Free cropImage
    DeleteImage(cropImage);

    return image;
}


/* Resize */
Image *Resize(Image *image, int newWidth, int newHeight) {
    assertImage(image);
    assert(newWidth > 0 && newHeight > 0 );

    // Scaling Factors
    double ScaleX = (double)image->W / newWidth;
    double ScaleY = (double)image->H / newHeight;

    // Create new image
    Image *resizeImage = CreateImage(newWidth, newHeight);

    // Iterate through each pixel in the new image
    for (unsigned int y = 0; y < newHeight; y++) {
        for (unsigned int x = 0; x < newWidth; x++) {
            // Calculate new scaled pixels
            int origX = (int)(x * ScaleX);
            int origY = (int)(y * ScaleY);

            SetPixelR(resizeImage, x, y, GetPixelR(image, origX, origY));
            SetPixelG(resizeImage, x, y, GetPixelG(image, origX, origY));
            SetPixelB(resizeImage, x, y, GetPixelB(image, origX, origY));
        }
    }


    // Copy changes to original image
    DeleteImage(image);
    image = CreateImage(newWidth, newHeight);
    for (int y = 0; y < newHeight; y++) {
        for (int x = 0; x < newWidth; x++) {
            SetPixelR(image, x, y, GetPixelR(resizeImage, x, y));
            SetPixelG(image, x, y, GetPixelG(resizeImage, x, y));
            SetPixelB(image, x, y, GetPixelB(resizeImage, x, y));
        }
    }

    //  Free resizeImage

    DeleteImage(resizeImage);

    return image;
}

/* Watermark */
Image *Watermark(Image *image, const Image *watermark_image) {
    assertImage(image);

    Image *newImage = CreateImage(image->W, image->H);

    // Copy original image + watermark to new image
    for (int j = 0; j < image->H; j++) {
        for (int i = 0; i < image->W; i++) {
            int x = i % watermark_image->W;
            int y = j % watermark_image->H;

            // Apply Watermark transformation if pixel is black
            if ((GetPixelR(watermark_image, x, y) == 0) && (GetPixelG(watermark_image, x ,y ) == 0) && GetPixelB(watermark_image, x ,y) == 0) {
                // Alter pixel RGB values, default to 255 if over
                SetPixelR(newImage, i, j, fmin(GetPixelR(image, i, j) * 1.45, 255));
                SetPixelG(newImage, i, j, fmin(GetPixelG(image, i, j) * 1.45, 255));
                SetPixelB(newImage, i, j, fmin(GetPixelB(image, i, j) * 1.45, 255));
            } else {
                // Copy original pixel values
                SetPixelR(newImage, i, j, GetPixelR(image, i, j));
                SetPixelG(newImage, i, j, GetPixelG(image, i, j));
                SetPixelB(newImage, i, j, GetPixelB(image, i, j));
            }
        }
    }

    // Copy changes to original image
    DeleteImage(image);
    image = CreateImage(newImage->W, newImage->H);
    for (int y = 0; y < newImage->H; y++) {
        for (int x = 0; x < newImage->W; x++) {
            SetPixelR(image, x, y, GetPixelR(newImage, x, y));
            SetPixelG(image, x, y, GetPixelG(newImage, x, y));
            SetPixelB(image, x, y, GetPixelB(newImage, x, y));
        }
    }

    // Free newImage
    DeleteImage(newImage);

    return image;
}

/* Rotate by 90 */
Image *RotateBy90(Image *image, int rotateDirection){
    int oldWidth = image->W;
    int oldHeight = image->H;
    int newWidth = oldHeight;
    int newHeight = oldWidth;

    Image *newImage = CreateImage(newWidth, newHeight);

    for(int i = 0; i < oldWidth; i++){
        for(int j = 0; j < oldHeight; j++){
            int newI = rotateDirection != 0 ? j : newWidth - 1 - j;
            int newJ = rotateDirection != 0 ? newHeight - 1 - i : i;

            SetPixelR(newImage, newI, newJ, GetPixelR(image, i, j));
            SetPixelG(newImage, newI, newJ, GetPixelG(image, i, j));
            SetPixelB(newImage, newI, newJ, GetPixelB(image, i, j));
        }
    }

        // Copy changes to original image
        DeleteImage(image);
        image = CreateImage(newImage->W, newImage->H);
        for (int y = 0; y < newImage->H; y++) {
            for (int x = 0; x < newImage->W; x++) {
                SetPixelR(image, x, y, GetPixelR(newImage, x, y));
                SetPixelG(image, x, y, GetPixelG(newImage, x, y));
                SetPixelB(image, x, y, GetPixelB(newImage, x, y));
            }
        }

        // Free newImage
        DeleteImage(newImage);

        return image;
}
