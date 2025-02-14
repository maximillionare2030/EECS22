/* Function declarations for Advanced.h */

#include <stdio.h>
#include <math.h>
#include "Advanced.h"
#include "Constants.h"




void FishEye(unsigned char R[WIDTH][HEIGHT], unsigned char G[WIDTH][HEIGHT], unsigned char B[WIDTH][HEIGHT], double base_factor, double k, double scaling_factor) {
    /**
     * @brief Applies a fisheye effect to the image.
     *
     * @param R Red channel of the image.
     * @param G Green channel of the image.
     * @param B Blue channel of the image.
     * @param base_factor  Base distortion factor
     * @param k: Rate of distortion increase with radius
     * @param scaling_factor: Scaling factor for controlling the overall image size
     */

   unsigned char R_out[WIDTH][HEIGHT], G_out[WIDTH][HEIGHT], B_out[WIDTH][HEIGHT];

    int center_x = WIDTH / 2;
    int center_y = HEIGHT / 2;

    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            double dx = (x - center_x) / (double)center_x;
            double dy = (y - center_y) / (double)center_y;
            double radius = sqrt(dx * dx + dy * dy);

            double distortion = (1.0 + k * radius * radius);
            double theta = atan2(dy, dx);
            double new_radius = (radius * base_factor) / (distortion * scaling_factor);
            new_radius = fmin(new_radius, 1.0);

            int x_src = floor(center_x + (new_radius * cos(theta) * center_x));
            int y_src = floor(center_y + (new_radius * sin(theta) * center_y));

            if (x_src >= 0 && x_src < WIDTH && y_src >= 0 && y_src < HEIGHT) {
                R_out[x][y] = R[x_src][y_src];
                G_out[x][y] = G[x_src][y_src];
                B_out[x][y] = B[x_src][y_src];
            } else {
                R_out[x][y] = 0;
                G_out[x][y] = 0;
                B_out[x][y] = 0;
            }
        }
    }

    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            R[x][y] = R_out[x][y];
            G[x][y] = G_out[x][y];
            B[x][y] = B_out[x][y];
        }
    }
}

void Posterize(unsigned char R[WIDTH][HEIGHT], unsigned char G[WIDTH][HEIGHT], unsigned char B[WIDTH][HEIGHT], int rbits, int gbits, int bbits) {
    /**
     * @brief Posterizes the image by reducing the number of bits for each color channel.
     *
     * @param R Red channel of the image.
     * @param G Green channel of the image.
     * @param B Blue channel of the image.
     * @param rbits Number of bits for red channel.
     * @param gbits Number of bits for green channel.
     * @param bbits Number of bits for blue channel.
     */
    for (int y = 0; y < HEIGHT; y++){
        for (int x = 0; x < WIDTH; x++){
            R[x][y] = (R[x][y] & (~((1 << rbits) - 1))) | ((1 << (rbits -1)) -1);
            G[x][y] = (G[x][y] & (~((1 << gbits) - 1))) | ((1 << (gbits -1)) -1);
            B[x][y] = (B[x][y] & (~((1 << bbits) - 1))) | ((1 << (bbits -1)) -1);
        }
    }
}

void Rotate(unsigned char R[WIDTH][HEIGHT], unsigned char G[WIDTH][HEIGHT], unsigned char B[WIDTH][HEIGHT], double Angle, double ScaleFactor, int CenterX, int CenterY) {
    double theta = -Angle * 2 * PI / 360.0;
    unsigned char R_temp[WIDTH][HEIGHT], G_temp[WIDTH][HEIGHT], B_temp[WIDTH][HEIGHT];



    // Copy original image to temp arrays
    for (int y = 0; y < HEIGHT; y++){
        for (int x = 0; x < WIDTH; x++){
            R_temp[x][y] = R[x][y];
            G_temp[x][y] = G[x][y];
            B_temp[x][y] = B[x][y];
        }
    }
    // Create new image
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            int new_x, new_y;
            new_x = (int)(((cos(theta) / ScaleFactor) * (x - CenterX)) - ((sin(theta) / ScaleFactor) * (y - CenterY)) + CenterX);
            new_y = (int)(((sin(theta) / ScaleFactor) * (x - CenterX)) + ((cos(theta) / ScaleFactor) * (y - CenterY)) + CenterY);

            if (new_x >= 0 && new_x < WIDTH && new_y >=  0 && new_y < HEIGHT) {
                R[x][y] = R_temp[new_x][new_y];
                G[x][y] = G_temp[new_x][new_y];
                B[x][y] = B_temp[new_x][new_y];
            }

            else {
                // Set to black
                R[x][y] = 0;
                G[x][y] = 0;
                B[x][y] = 0;
            }
        }
    }
}

void MotionBlur(int BlurAmount,unsigned char R[WIDTH][HEIGHT], unsigned char G[WIDTH][HEIGHT], unsigned char B[WIDTH][HEIGHT]) {
    unsigned char R_temp[WIDTH][HEIGHT], G_temp[WIDTH][HEIGHT], B_temp[WIDTH][HEIGHT];

    // Copy original image to temp arrays
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            R_temp[x][y] = R[x][y];
            G_temp[x][y] = G[x][y];
            B_temp[x][y] = B[x][y];
        }
    }

    // Apply horizontal motion blur
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            double sumR = R_temp[x][y] * 0.5; // 50% weight for the original pixel
            double sumG = G_temp[x][y] * 0.5;
            double sumB = B_temp[x][y] * 0.5;
            double totalWeight = 0.5; // Start with the original pixel's weight

            int availablePixels = (x + BlurAmount <= WIDTH) ? BlurAmount : (WIDTH - 1 - x); // Adjust for edge cases

            for (int i = 1; i <= availablePixels; i++) {
                double weight = 0.5 / availablePixels;  // Adjust remaining 50% weight based on available pixels
                sumR += R_temp[x + i][y] * weight;
                sumG += G_temp[x + i][y] * weight;
                sumB += B_temp[x + i][y] * weight;
                totalWeight += weight;
            }

            // Assign the weighted values TODO: Fix?? idk
            R[x][y] = (unsigned char)(sumR / totalWeight);
            G[x][y] = (unsigned char)(sumG / totalWeight);
            B[x][y] = (unsigned char)(sumB / totalWeight);
        }
    }
}

