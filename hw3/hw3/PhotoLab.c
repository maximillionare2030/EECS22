/*********************************************************************/
/* PhotoLab.c: Assignment 2 for EECS 22, Winter 2024                 */
/*********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "Constants.h"
#include "FileIO.h"
#include "DIPS.h"
#include "Advanced.h"

void PrintMenu() {
	printf("\n----------------------------\n");
	printf(" 1: Load a PPM image\n");
	printf(" 2: Save an image in PPM and JPEG format\n");
	printf(" 3: Change a color image to Black & White\n");
	printf(" 4: Make a negative of an image\n");
	printf(" 5: Color filter an image\n");
	printf(" 6: Sketch the edge of an image\n");
	printf(" 7: Shuffle an image\n");
	printf(" 8: Flip an image vertically\n");
	printf(" 9: Mirror an image horizontally\n");
	printf("10: Pixelate the image\n");
	printf("11: Create a fisheye image\n");
	printf("12: Posterize an image\n");
	printf("13: Rotate and zoom an image\n");
	printf("14: Motion Blur\n");
	printf("15: Test all functions\n");
	printf("16: Exit\n");
}

void AutoTest(unsigned char R[WIDTH][HEIGHT], unsigned char G[WIDTH][HEIGHT], unsigned char B[WIDTH][HEIGHT]) {
	char fname[SLEN] = "EngPlaza";
	char sname[SLEN];

	LoadImage(fname, R, G, B);
	Negative(R, G, B);
	strcpy(sname, "negative");
	SaveImage(sname, R, G, B);
	printf("Negative tested!\n\n");

	LoadImage(fname, R, G, B);
	ColorFilter(R, G, B, 250, 250, 250, 60, 0, 0, 255);
	strcpy(sname, "colorfilter");
	SaveImage(sname, R, G, B);
	printf("Color Filter tested!\n\n");

	LoadImage(fname, R, G, B);
	Edge(R, G, B);
	strcpy(sname, "edge");
	SaveImage(sname, R, G, B);
	printf("Edge Detection tested!\n\n");

	LoadImage(fname, R, G, B);
	HMirror(R, G, B);
	strcpy(sname, "hmirror");
	SaveImage(sname, R, G, B);
	printf("HMirror tested!\n\n");

	LoadImage(fname, R, G, B);
	AddBorder (R, G, B, "orange", 32);
	strcpy(sname, "border");
	SaveImage(sname, R, G, B);
	printf("Border tested!\n\n");

	LoadImage(fname, R, G, B);
	BlackNWhite(R, G, B);
	strcpy(sname, "bw");
	SaveImage(sname, R, G, B);
	printf("Black & White tested!\n\n");

	LoadImage(fname, R, G, B);
	VFlip(R, G, B);
	strcpy(sname, "vflip");
	SaveImage(sname, R, G, B);
	printf("HFlip tested!\n\n");

	LoadImage(fname, R, G, B);
	Shuffle(R, G, B);
	strcpy(sname, "shuffle");
	SaveImage(sname, R, G, B);
	printf("Shuffle tested!\n\n");

	/*
	LoadImage(fname, R, G, B);
	FishEye(R, G, B, 0.5, 2.0);
	strcpy(sname, "fisheye");
	SaveImage(sname, R, G, B);
	printf("Fisheye tested!\n\n");

	LoadImage(fname, R, G, B);
	Posterize(R, G, B, 7, 7, 7) ;
	strcpy(sname, "posterize");
	SaveImage(sname, R, G, B);
	printf("Posterize tested!\n\n");

	LoadImage(fname, R, G, B);
	Rotate(R, G, B, 22, 0.78, 110, 220);
	strcpy(sname, "rotate");
	SaveImage(sname, R, G, B);
	printf("Rotate tested!\n\n");

	LoadImage(fname, R, G, B);
	MotionBlur(40, R, G, B) ;
	strcpy(sname, "blur");
	SaveImage(sname, R, G, B);
	printf("MotionBlur tested!\n\n");
	*/
}


int main() {
	unsigned char   R[WIDTH][HEIGHT];	/* use three 2-D arrays to store R, G, B components */
	unsigned char   G[WIDTH][HEIGHT];
	unsigned char   B[WIDTH][HEIGHT];

	int option;			/* user input option */
	char fname[SLEN];		/* input file name */
	char colorOption[SLEN];

	PrintMenu();
	printf("Please make your choice: ");
	scanf("%d", &option);

	int r24 = -1;			/* return code of LoadImage() */
	/* ColorFilter() parameters */
	int target_r, target_g, target_b, threshold;
	double factor_r, factor_g, factor_b;
	/* AddBorder() parameter */
	int border_width;

	while (option != EXIT) {
		if (option == 1) {
			printf("Please input the file name to load: ");
			scanf("%75s", fname);
			r24 = LoadImage(fname, R, G, B);
		}
		/* menu item 2 - 10 requires image is loaded first */
		else if (option >= 2 && option <= 14) {
			if (r24 != SUCCESS)	 {
				printf("No image is read.\n");
			}
			/* now image is loaded */
			else {
				switch(option) {
					case 2:
						printf("Please input the file name to save: ");
						scanf("%75s", fname);
						SaveImage(fname, R, G, B);
						break;
					case 3:
						BlackNWhite(R, G, B);
						printf("\"Black & White\" operation is done!\n");
						break;
					case 4:
						Negative(R,G,B);
						printf("\"Negative\" operation is done!\n");
						break;
					case 5:
						printf("Enter Red   component for the target color: ");
						scanf("%d", &target_r);
						printf("Enter Green component for the target color: ");
						scanf("%d", &target_g);
						printf("Enter Blue  component for the target color: ");
						scanf("%d", &target_b);
						printf("Enter threshold for the color difference: ");
						scanf("%d", &threshold);
						printf("Enter value for Red component in the target color: ");
						scanf("%lf", &factor_r);
						printf("Enter value for Green component in the target color: ");
						scanf("%lf", &factor_g);
						printf("Enter value for Blue  component in the target color: ");
						scanf("%lf", &factor_b);
						ColorFilter(R, G, B, target_r, target_g, target_b, threshold, factor_r, factor_g, factor_b);
						printf("\"Color Filter\" operation is done!\n");
						break;
					case 6:
						Edge(R, G, B);
						printf("\"Edge\" operation is done!\n");
						break;
					case 7:
						Shuffle(R, G, B);
						printf("\"Shuffle\" operation is done!\n");
						break;
					case 8:
						VFlip(R, G, B);
						printf("\"VFlip\" operation is done!\n");
						break;
					case 9:
						HMirror(R, G, B);
						printf("\"HMirror\" operation is done!\n");
						break;
					case 10: {
						int block_size;
						printf("Enter the block size: ");
						scanf("%d", &block_size);
						Pixelate(R, G, B, block_size);
						printf("\"Pixelate\" operation is done!\n");
						break;
					}
					case 11: {
						double base_factor, k, scaling_factor;
						printf("Enter a value for base factor: ");
						scanf("%lf", &base_factor);
						printf("Enter a value for k: ");
						scanf("%lf", &k);
						printf("Enter a value for scaling factor: ");
						scanf("%lf", &scaling_factor);
						FishEye(R, G, B, base_factor, k, scaling_factor);
						printf("\"Fisheye\" operation is done!\n");
						break;
					}

					case 12: {
						int rbits, gbits, bbits;
						printf("Enter number of posterization bits for R channel (1 to 8): ");
						scanf("%d", &rbits);
						printf("Enter number of posterization bits for G channel (1 to 8): ");
						scanf("%d", &gbits);
						printf("Enter number of posterization bits for B channel (1 to 8): ");
						scanf("%d", &bbits);
						Posterize(R, G, B, rbits, gbits, bbits);
						printf("\"Posterize\" operation is done!\n");
						break;
					}
					default:
						break;
				}
			}
		}
		else if (option == 15) {
			AutoTest(R, G, B);
			r24 = SUCCESS;	/* set returned code SUCCESS, since image is loaded */
		}
		else {
			printf("Invalid selection!\n");
		}

		/* Process finished, waiting for another input */
		PrintMenu();
		printf("Please make your choice: ");
		scanf("%d", &option);
	}
	printf("You exit the program.\n");
	return 0;
}
