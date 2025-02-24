#include <stdio.h>
#include <stdlib.h>
#include "Test.h"
#include "DIPs.h"
#include "FileIO.h"
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
		else if (option >= 2 && option <= 14) {
			if (image == NULL)	 {
				printf("No image is read.\n");
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
                    /*
					case 12: {
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
					case 13: {

						printf("Enter the angle of rotation: ");
						scanf("%lf", &Angle);
						printf("Enter the scale of zooming: ");
						scanf("%lf", &ScaleFactor);
						printf("Enter the X-axis coordinate of the center of rotation: ");
						scanf("%d", &CenterX);
						printf("Enter the Y-axis coordinate of the center of rotation: ");
						scanf("%d", &CenterY);
						Rotate(R, G, B, Angle, ScaleFactor, CenterX, CenterY);
						printf("\"Rotate\" operation is done!\n");
						break;
					}
					case 14: {
						printf("Please input blur amount: ");
						scanf("%d", &BlurAmount);
						MotionBlur(BlurAmount, R, G, B);
						printf("\"motion blur\" operation is done!\n");
						break;
					}
                        */

					default:
						printf("Invalid selection!\n");
						break;

				}
			}

		}

		else if (option == 17) {
			AutoTest();
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
