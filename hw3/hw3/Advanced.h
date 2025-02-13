/* Header file for Advanced.c */

#ifndef ADVANCED_H
#define ADVANCED_H

#include "Constants.h"

void FishEye(unsigned char R[WIDTH][HEIGHT], unsigned char G[WIDTH][HEIGHT], unsigned char B[WIDTH][HEIGHT], double base_factor, double k, double scaling_factor);

void Posterize(unsigned char R[WIDTH][HEIGHT], unsigned char G[WIDTH][HEIGHT], unsigned char B[WIDTH][HEIGHT], int rbits, int gbits, int bbits);

#endif
