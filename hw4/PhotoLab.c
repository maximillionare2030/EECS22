#include <stdio.h>
#include <stdlib.h>
#include "Test.h"
#include "DIPs.h"
#include "FileIO.h"
#include "Image.h"
#include "Advanced.h"

/* Menu */
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
    printf("10: Add border to an image\n");
    printf("11: Pixelate an image\n");
    printf("12: Shift an image\n");
    printf("13: Crop an image\n");
    printf("14: Resize an image\n");
    printf("15: Add Watermark to an image\n");
    printf("16: Rotate an image by 90 degrees\n");
    printf("17: Test all functions\n");
    printf("18: Exit\n");
}

int main() {

    Image *image = NULL;


	#ifdef DEBUG
	AutoTest();
	#else
	int option;			/* user input option */
	char fname[SLEN];		/* input file name */

	PrintMenu();
	printf("Please make your choice: ");
	scanf("%d", &option);


	while (option != EXIT) {
		if (option == 1) {
			printf("Please input the file name to load: ");
			scanf("%75s", fname);
            image = LoadImage(fname);
		}
		/* menu item 2 - 10 requires image is loaded first */
		else if (option >= 2 && option <= 16) {
			if (image == NULL)	 {
				printf("No image to process!\n");
			}
			/* now image is loaded */
			else {
				switch(option) {
					case 2:
						printf("Please input the file name to save: ");
						scanf("%75s", fname);
						SaveImage(fname, image);
						break;
					case 3:
						BlackNWhite(image);
						printf("\"Black & White\" operation is done!\n");
						break;
					case 4:
						Negative(image);
						printf("\"Negative\" operation is done!\n");
						break;
					case 5: {
                        int target_r, target_g, target_b, threshold;
                        double factor_r, factor_g, factor_b;
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
                        ColorFilter(image, target_r, target_g, target_b, threshold, factor_r, factor_g, factor_b);
						printf("\"Color Filter\" operation is done!\n");
						break;
                    }
					case 6:
                        Edge(image);
						printf("\"Edge\" operation is done!\n");
						break;
					case 7:
                        Shuffle(image);
						printf("\"Shuffle\" operation is done!\n");
						break;
					case 8:
                        VFlip(image);
                        printf("\"VFlip\" operation is done!\n");
						break;
					case 9:
						HMirror(image);
						printf("\"HMirror\" operation is done!\n");
						break;
                    case 10: {
                        int border_width;
                        char colorOption[SLEN];
                        printf("Enter border width:");
                        scanf("%d", &border_width);
                        printf("Available border colors : black, white, red, green, blue, yellow, cyan, pink, orange\n");
                        printf("Select border color from the options: ");
                        scanf("%s",colorOption);
                        AddBorder(image, colorOption, border_width);
                        printf("\"Border\" operation is done!\n");
                        break;
                    }

					case 11: {
                        int block_size;
						printf("Enter the block size: ");
						scanf("%d", &block_size);
						Pixelate(image, block_size);
						printf("\"Pixelate\" operation is done!\n");
						break;
					}
					case 12: {
                        int shiftX, shiftY;
                        printf("Enter the shift value for X-axis: ");
                        scanf("%d", &shiftX);
                        printf("Enter the shift value for Y-axis: ");
                        scanf("%d", &shiftY);
                        Shift(image, shiftX, shiftY);
                        printf("\"Shift\" operation is done!\n");
					}
                    case 13: {
                        int X, Y, W, H;
                        printf("Enter the X offset value: ");
                        scanf("%d", &X);
                        printf("Enter the Y offset value: ");
                        scanf("%d", &Y);
                        printf("Please input the crop width: ");
                        scanf("%d", &W);
                        printf("Please input the crop height: ");
                        scanf("%d", &H);
                        Crop(image, X, Y, W, H);
                        printf("\"Crop\" operation is done!\n");
                        break;
                    }
                    case 14: {
                        int newWidth, newHeight;
                        printf("Please input the new image width: ");
                        scanf("%d", &newWidth);
                        printf("Please input the new image height: ");
                        scanf("%d", &newHeight);
                        Resize(image, newWidth, newHeight);
                        printf("\"Resize\" operation is done!\n");
                        break;
                    }
                    case 15: {
                        char watermarkImgName[SLEN] = "watermark_template";
                        Image *watermark_image = LoadImage(watermarkImgName);
                        Watermark(image, watermark_image);
                        DeleteImage(watermark_image);
                        printf("\"Watermark\" operation is done!\n");
                        break;
                    }

					default:
						printf("Invalid selection!\n");
						break;

				}
			}

		}

		else if (option == 17) {
			int ATC = AutoTest();
            if (ATC == 0) printf("AutoTest failed, error code RC.");
            else printf("AutoTest finished successfully.");
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
	#endif

	return 0;
}
