#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "DIPs.h"
#include "Image.h"
#include "Constants.h"

/* Black and White */
Image *BlackNWhite(Image *image) {
    assertImage(image);
    int x, y, tmp;
    for (y = 0; y < image->H; y++)
    {
        for (x = 0; x < image->W; x++)
        {
            tmp = (GetPixelR(image, x, y) + GetPixelG(image, x, y) + GetPixelB(image, x, y)) / 3;
            SetPixelR(image, x, y, tmp);
            SetPixelG(image, x, y, tmp);
            SetPixelB(image, x, y, tmp);
        }
    }
    return image;
}

/* Reverse image color */
Image *Negative(Image *image) {
    assertImage(image);
    int x, y;
    for (y = 0; y < image->H; y++) {
        for (x =0; x < image->W; x++) {

            SetPixelR(image, x, y, MAX_PIXEL - GetPixelR(image, x, y));
            SetPixelG(image, x, y, MAX_PIXEL - GetPixelG(image, x, y));
            SetPixelB(image, x, y, MAX_PIXEL - GetPixelB(image, x, y));
        }
    }
    return image;
}

/* Color filter */
Image *ColorFilter(Image *image, int target_r, int target_g, int target_b, int threshold, int replace_r, int replace_g, int replace_b) {
    int x, y;

	replace_r = (replace_r > MAX_PIXEL) ? MAX_PIXEL : (replace_r < 0) ? 0 : replace_r;
	replace_g = (replace_g > MAX_PIXEL) ? MAX_PIXEL : (replace_g < 0) ? 0 : replace_g;
	replace_b = (replace_b > MAX_PIXEL) ? MAX_PIXEL : (replace_b < 0) ? 0 : replace_b;

    for (y = 0; y < image->H; y++) {
        for (x = 0; x < image->W; x++) {
            /* Set the pixel color to the new values */
            if (abs(GetPixelR(image, x, y)- target_r) <= threshold
                    && abs(GetPixelG(image, x, y) - target_g) <= threshold
                    && abs(GetPixelB(image, x, y) - target_b) <= threshold) {
                SetPixelR(image, x, y, replace_r);
                SetPixelG(image, x, y, replace_g);
                SetPixelB(image, x, y, replace_b);
            }
        }
    }
    return image;
}

/* Edge detection */
Image *Edge(Image *image) {
    int             x, y, m, n, a, b;
    Image *temp_image = CreateImage(image->W, image->H);
	for (y = 0; y < image->H; y++){
		for (x = 0; x < image->W; x++) {
            /* Create Copy of image*/
			 SetPixelR(temp_image, x, y, GetPixelR(image, x, y));
             SetPixelG(temp_image, x, y, GetPixelG(image, x, y));
             SetPixelB(temp_image, x, y, GetPixelB(image, x, y));
		}
	}
	int sumR = 0;   /* sum of the intensity differences with neighbors */
	int sumG = 0;
	int sumB = 0;
	for (y = 1; y < image->H - 1; y++){
		for (x = 1; x < image->W - 1; x++){
			for (n = -1; n <= 1; n++){
				for (m = -1; m <= 1; m++) {
					a = (x + m >= image->W) ? image->W - 1 : (x + m < 0) ? 0 : x + m;
					b = (y + n >= image->H) ? image->H - 1 : (y + n < 0) ? 0 : y + n;
                    sumR += (GetPixelR(temp_image, x, y) - GetPixelR(temp_image, a, b));
					sumG += (GetPixelG(temp_image, x, y) - GetPixelG(temp_image, a, b));
                    sumB += (GetPixelB(temp_image, x, y) - GetPixelB(temp_image, a, b));
				}
			}
            // Find potential maximum and minimum values
			sumR = (sumR > MAX_PIXEL) ? MAX_PIXEL: (sumR < 0) ? 0: sumR;
			sumG = (sumG > MAX_PIXEL) ? MAX_PIXEL: (sumG < 0) ? 0: sumG;
			sumB = (sumB > MAX_PIXEL) ? MAX_PIXEL: (sumB < 0) ? 0: sumB;
            SetPixelR(image, x, y, sumR);
            SetPixelG(image, x, y, sumG);
            SetPixelB(image, x, y, sumB);
			sumR = sumG = sumB = 0; // Reset sums
		}
	}
	/* set all 24 borders to 0 */
	for (y = 0; y < image->H; y++) {
		SetPixelR(image, 0, y, 0);
        SetPixelG(image, 0, y, 0);
        SetPixelB(image, 0, y, 0);
        SetPixelR(image, image->W - 1, y, 0);
        SetPixelG(image, image->W - 1, y, 0);
        SetPixelB(image, image->W - 1, y, 0);
	}
	for (x = 0; x < image->W; x++) {
        SetPixelR(image, x, 0, 0);
        SetPixelG(image, x, 0, 0);
        SetPixelB(image, x, 0, 0);
        SetPixelR(image, x, image->H - 1, 0);
        SetPixelG(image, x, image->H - 1, 0);
        SetPixelB(image, x, image->H - 1, 0);
	}

    DeleteImage(temp_image);
    return image;
}

/* Flip image vertically */
Image *VFlip(Image *image) {
    int             x, y;
	unsigned char   r, g, b;

	for (y = 0; y < image->H/2; y++) {
		for (x = 0; x < image->W; x++) {

            // Copy r,g,b values at
			r = GetPixelR(image, x, image->H - 1 - y);
			g = GetPixelG(image, x, image->H - 1 - y);
			b = GetPixelB(image, x, image->H - 1 - y);

            SetPixelR(image, x, image->H - 1 - y, GetPixelR(image, x, y));
            SetPixelG(image, x, image->H - 1 - y, GetPixelG(image, x, y));
            SetPixelB(image, x, image->H - 1 - y, GetPixelB(image, x, y));

            SetPixelR(image, x, y, r);
            SetPixelG(image, x, y, g);
            SetPixelB(image, x, y, b);
		}
	}
    return image;
}

/* Mirror image vertically */
Image *HMirror(Image *image) {
    int x, y;

	for (y = 0; y < image->H; y++) {
		for (x = 0; x < image->W / 2; x++) {
            SetPixelR(image, image->W - 1 - x, y, GetPixelR(image, x, y));
            SetPixelG(image, image->W - 1 - x, y, GetPixelG(image, x, y));
            SetPixelB(image, image->W - 1 - x, y, GetPixelB(image, x, y));
		}
	}
    return image;
}

/* Zoom an image */
Image *AddBorder(Image *image, char *color, int border_width) {

    int x, y;
    int border_r = 255;
    int border_g = 255;
    int border_b = 255;
    if (!strcmp(color, "black")) {
        border_r = 0;
        border_g = 0;
        border_b = 0;
    } else if (!strcmp(color, "white")) {
        border_r = 255;
        border_g = 255;
        border_b = 255;
    } else if (!strcmp(color, "red")) {
        border_r = 255;
        border_g = 0;
        border_b = 0;
    } else if (!strcmp(color, "green")) {
        border_r = 0;
        border_g = 255;
        border_b = 0;
    } else if (!strcmp(color, "blue")) {
        border_r = 0;
        border_g = 0;
        border_b = 255;
    } else if (!strcmp(color, "yellow")) {
        border_r = 255;
        border_g = 255;
        border_b = 0;
    } else if (!strcmp(color, "cyan")) {
        border_r = 0;
        border_g = 255;
        border_b = 255;
    } else if (!strcmp(color, "pink")) {
        border_r = 255;
        border_g = 192;
        border_b = 203;
    } else if (!strcmp(color, "orange")) {
        border_r = 255;
        border_g = 165;
        border_b = 0;
    } else {
        printf("Unsupported color.\n");
        return image;
    }
    for (y = 0; y < image->H; y++) {
        for (x = 0; x < image->W; x++) {
            if ((y < border_width) || (y > image->H - 1 - border_width) ||
                    (x < border_width) || (x > image->W - 1 - border_width)) {
                SetPixelR(image, x, y, border_r);
                SetPixelG(image, x, y ,border_g);
                SetPixelB(image, x, y, border_b);
            }
        }
    }
    return image;
}

/* Shuffle an image */
Image *Shuffle(Image *image) {
    int block_width = image->W / SHUFF_WIDTH_DIV;
    int block_height = image->H / SHUFF_HEIGHT_DIV;
    int i, j, x, y;

    Image *temp_image = CreateImage(image->W, image->H);

    for (i = 0; i < SHUFF_HEIGHT_DIV / 2; i++) { // only need to iterate through half of the blocks
        for (j = 0; j < SHUFF_WIDTH_DIV; j++) {
            for (x = 0; x < block_width; x++) {
                for (y = 0; y < block_height; y++) {
                    // Store the pixel values from the current block
                    SetPixelR(temp_image, x, y, GetPixelR(image, x + (j * block_width), y + (i * block_height)));
                    SetPixelG(temp_image, x, y, GetPixelG(image, x + (j * block_width), y + (i * block_height)));
                    SetPixelB(temp_image, x, y, GetPixelB(image, x + (j * block_width), y + (i * block_height)));

                    // Move the swapped block into the current block
                    SetPixelR(image, x + (j * block_width), y + (i * block_height),
                        GetPixelR(image, x + ((SHUFF_WIDTH_DIV - 1 - j) * block_width),
                                  y + ((SHUFF_HEIGHT_DIV - 1 - i) * block_height)));
                    SetPixelG(image, x + (j * block_width), y + (i * block_height),
                        GetPixelG(image, x + ((SHUFF_WIDTH_DIV - 1 - j) * block_width),
                                  y + ((SHUFF_HEIGHT_DIV - 1 - i) * block_height)));
                    SetPixelB(image, x + (j * block_width), y + (i * block_height),
                        GetPixelB(image, x + ((SHUFF_WIDTH_DIV - 1 - j) * block_width),
                                  y + ((SHUFF_HEIGHT_DIV - 1 - i) * block_height)));

                    // Move stored block to the swapped position
                    SetPixelR(image, x + ((SHUFF_WIDTH_DIV - 1 - j) * block_width),
                              y + ((SHUFF_HEIGHT_DIV - 1 - i) * block_height), GetPixelR(temp_image, x, y));
                    SetPixelG(image, x + ((SHUFF_WIDTH_DIV - 1 - j) * block_width),
                              y + ((SHUFF_HEIGHT_DIV - 1 - i) * block_height), GetPixelG(temp_image, x, y));
                    SetPixelB(image, x + ((SHUFF_WIDTH_DIV - 1 - j) * block_width),
                              y + ((SHUFF_HEIGHT_DIV - 1 - i) * block_height), GetPixelB(temp_image, x, y));
                }
            }
        }
    }

    DeleteImage(temp_image);
    return image;
}


/* Pixelate an image */
Image *Pixelate(Image *image, int block_size) {
    for (int x = 0; x < image->W; x += block_size) {
        for (int y = 0; y < image->H; y += block_size) {
            int sum_R = 0, sum_G = 0, sum_B = 0;
            int actual_block_w = ((x + block_size) > image->W) ? (image->W - x) : block_size;
            int actual_block_h = ((y + block_size) > image->H) ? (image->H - y) : block_size;
            int pixel_count = actual_block_w * actual_block_h;

            // Calculate sum of pixel values
            for (int j = 0; j < actual_block_w; j++) {
                for (int k = 0; k < actual_block_h; k++) {
                    sum_R += GetPixelR(image, x + j, y + k);
                    sum_G += GetPixelG(image, x + j, y + k);
                    sum_B += GetPixelB(image, x + j, y + k);
                }
            }

            // Compute the average color
            int avg_R = sum_R / pixel_count;
            int avg_G = sum_G / pixel_count;
            int avg_B = sum_B / pixel_count;

            // Apply the averaged color
            for (int j = 0; j < actual_block_w; j++) {
                for (int k = 0; k < actual_block_h; k++) {
                    SetPixelR(image, x + j, y + k, avg_R);
                    SetPixelG(image, x + j, y + k, avg_G);
                    SetPixelB(image, x + j, y + k, avg_B);
                }
            }
        }
    }
    return image;
}

/* Shift an image with wrap-around */
Image *Shift(Image *image, int shiftX, int shiftY) {
    int x, y;
    Image *temp_image = CreateImage(image->W, image->H);

    // Compute shifted positions and store in temp_image
    for (y = 0; y < image->H; y++) {
        for (x = 0; x < image->W; x++) {
            int newX = (x + shiftX + image->W) % image->W;
            int newY = (y + shiftY + image->H) % image->H;
            SetPixelR(temp_image, newX, newY, GetPixelR(image, x, y));
            SetPixelG(temp_image, newX, newY, GetPixelG(image, x, y));
            SetPixelB(temp_image, newX, newY, GetPixelB(image, x, y));
        }
    }

    // Copy shifted image back to original
    for (y = 0; y < image->H; y++) {
        for (x = 0; x < image->W; x++) {
            SetPixelR(image, x, y, GetPixelR(temp_image, x, y));
            SetPixelG(image, x, y, GetPixelG(temp_image, x, y));
            SetPixelB(image, x, y, GetPixelB(temp_image, x, y));
        }
    }

    DeleteImage(temp_image);
    return image;
}
