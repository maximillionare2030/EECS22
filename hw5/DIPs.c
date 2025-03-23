/*********************************************************************/
/* Homework Assignment 5, for EECS 22, Winter 2024                   */
/* DIPs.c: source file for DIP operations                            */
/*********************************************************************/

#include <stdlib.h>
#include <assert.h>
#include "DIPs.h"
#include "Constants.h"
#include "string.h"
#include "math.h"

/* Black and White */
Image *BlackNWhite(Image *image)
{
	assert(image);
	int x, y, tmp;

	for (y = 0; y < ImageHeight(image); y++) {
		for (x = 0; x < ImageWidth(image); x++) {
		tmp = (GetPixelR(image, x, y) + GetPixelG(image, x, y) + GetPixelB(image, x, y)) / 3;
			SetPixelR(image, x, y, tmp);
			SetPixelG(image, x, y, tmp);
		SetPixelB(image, x, y, tmp);
		}
	}
	return image;
}

/* Mirror image horizontal  */
Image *HMirror(Image *image) {
	assert(image);
	int x, y;
	for (y = 0; y < ImageHeight(image); y++) {
		for (x = 0; x < ImageWidth(image)/2; x++) {
			SetPixelG(image, ImageWidth(image) - 1 - x, y, GetPixelG(image, x, y));
			SetPixelB(image, ImageWidth(image) - 1 - x, y, GetPixelB(image, x, y));
			SetPixelR(image, ImageWidth(image) - 1 - x, y, GetPixelR(image, x, y));
		}
	}
	return image;
}

/* Edge detection */
Image *Edge(Image *image) {
	assert(image);
	Image *tmpImage = CreateImage(ImageWidth(image), ImageHeight(image));
	assert(tmpImage);

	int x, y, m, n;
	memcpy(tmpImage->R, image->R, ImageWidth(image) * ImageHeight(image) * sizeof(unsigned char));
	memcpy(tmpImage->G, image->G, ImageWidth(image) * ImageHeight(image) * sizeof(unsigned char));
	memcpy(tmpImage->B, image->B, ImageWidth(image) * ImageHeight(image) * sizeof(unsigned char));
	for (y = 1; y < ImageHeight(image) - 1; y++) {
		for (x = 1; x < ImageWidth(image) - 1; x++) {
			int tmpR = 0;
			int tmpG = 0;
			int tmpB = 0;
			for (m = -1; m <= 1; m++) {
				for (n = -1; n <= 1; n++) {
					tmpR += (GetPixelR(tmpImage, x, y) - GetPixelR(tmpImage, x + n, y + m));
					tmpG += (GetPixelG(tmpImage, x, y) - GetPixelG(tmpImage, x + n, y + m));
					tmpB += (GetPixelB(tmpImage, x, y) - GetPixelB(tmpImage, x + n, y + m));
				}
			}
			SetPixelR(image, x, y, (tmpR > 255) ? 255 : (tmpR < 0) ? 0 : tmpR);
			SetPixelG(image, x, y, (tmpG > 255) ? 255 : (tmpG < 0) ? 0 : tmpG);
			SetPixelB(image, x, y, (tmpB > 255) ? 255 : (tmpB < 0) ? 0 : tmpB);
		}
	}
	for (y = 0; y < ImageHeight(image); y++) {
		x = 0;
		SetPixelR(image, x, y, 0);
		SetPixelG(image, x, y, 0);
		SetPixelB(image, x, y, 0);
		x = ImageWidth(image) - 1;
		SetPixelR(image, x, y, 0);
		SetPixelG(image, x, y, 0);
		SetPixelB(image, x, y, 0);
	}
	for (x = 0; x < ImageWidth(image); x++) {
		y = 0;
		SetPixelR(image, x, y, 0);
		SetPixelG(image, x, y, 0);
		SetPixelB(image, x, y, 0);
		y = ImageHeight(image) - 1;
		SetPixelR(image, x, y, 0);
		SetPixelG(image, x, y, 0);
		SetPixelB(image, x, y, 0);
	}
	DeleteImage(tmpImage);
	tmpImage = NULL;
	return image;
}

Image *Watermark(Image *image, const Image *watermark_image, unsigned int topLeftX, unsigned int topLeftY) {
    assert(image);
    assert(watermark_image);

    // Create a new image to store the result
    Image *newImage = CreateImage(image->W, image->H);
    if (!newImage) {
        return NULL;
    }

    // Copy the original image to the new image
    for (unsigned int y = 0; y < image->H; y++) {
        for (unsigned int x = 0; x < image->W; x++) {
            SetPixelR(newImage, x, y, GetPixelR(image, x, y));
            SetPixelG(newImage, x, y, GetPixelG(image, x, y));
            SetPixelB(newImage, x, y, GetPixelB(image, x, y));
        }
    }

    // Overlay the watermark
    for (unsigned int wy = 0; wy < watermark_image->H; wy++) {
        for (unsigned int wx = 0; wx < watermark_image->W; wx++) {
            unsigned int imageX = topLeftX + wx;
            unsigned int imageY = topLeftY + wy;

            // Check if the watermark pixel is within the frame
            if (imageX < image->W && imageY < image->H) {
                // Apply watermark transformation if the watermark pixel is black
                if (GetPixelR(watermark_image, wx, wy) == 0 &&
                    GetPixelG(watermark_image, wx, wy) == 0 &&
                    GetPixelB(watermark_image, wx, wy) == 0) {
                    // Alter pixel RGB values, default to 255 if over
                    SetPixelR(newImage, imageX, imageY, fmin(GetPixelR(image, imageX, imageY) * 1.45, 255));
                    SetPixelG(newImage, imageX, imageY, fmin(GetPixelG(image, imageX, imageY) * 1.45, 255));
                    SetPixelB(newImage, imageX, imageY, fmin(GetPixelB(image, imageX, imageY) * 1.45, 255));
                }
            }
        }
    }

    return newImage;
}
/* Spotlight */
Image *Spotlight(Image *image, int centerX, int centerY, unsigned int radius)
{
    // Calculate the squared radius to avoid using sqrt in the loop
    unsigned int radius_squared = radius * radius;

    // Iterate over all pixels in the image
    for (int x = 0; x < image->W; x++) {
        for (int y = 0; y < image->H; y++) {
            // Calculate the squared distance from the center
            int dx = centerX - x;
            int dy = centerY - y;
            unsigned int distance_squared = dx * dx + dy * dy;

            // If the pixel is outside the circle, set it to black
            if (distance_squared > radius_squared) {
                SetPixelR(image, x, y, 0);
                SetPixelG(image, x, y, 0);
                SetPixelB(image, x, y, 0);
            }
        }
    }

    return image;
}

Image *Rotate(Image *image, double Angle, double ScaleFactor) {
    // Create a new image of the same size as the original
    Image *newImg = CreateImage(image->W, image->H);
    if (!newImg) return NULL;

    double radians = Angle * (3.1415) / 180.0;
    double cos_theta = cos(radians);
    double sin_theta = sin(radians);

    int cx = image->W / 2;  // Center X
    int cy = image->H / 2;  // Center Y

    for (int y = 0; y < newImg->H; y++) {
        for (int x = 0; x < newImg->W; x++) {
            // Translate to center of destination image
            double x1 = (double)x - cx;
            double y1 = (double)y - cy;

            // Apply inverse rotation and scaling
            double x_rot = (x1 * cos_theta + y1 * sin_theta) / ScaleFactor;
            double y_rot = (-x1 * sin_theta + y1 * cos_theta) / ScaleFactor;

            // Translate back to source coordinates
            int src_x = (int)round(x_rot + cx);
            int src_y = (int)round(y_rot + cy);

            // Check bounds and set pixel
            if (src_x >= 0 && src_x < image->W && src_y >= 0 && src_y < image->H) {
                SetPixelR(newImg, x, y, GetPixelR(image, src_x, src_y));
                SetPixelG(newImg, x, y, GetPixelG(image, src_x, src_y));
                SetPixelB(newImg, x, y, GetPixelB(image, src_x, src_y));
            } else {
                SetPixelR(newImg, x, y, 0);
                SetPixelG(newImg, x, y, 0);
                SetPixelB(newImg, x, y, 0);
            }
        }
    }

    return newImg;
}
