/*********************************************************************/
/* PhotoLab.c: Assignment 2 for EECS 22, Winter 2024                 */
/*********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/*** global definitions ***/
#define WIDTH  512		/* image width */
#define HEIGHT 288		/* image height */
#define SLEN    80		/* maximum length of file names */
#define MENU_SIZE 13  // Define the number of menu items


/*** function declarations **
 * TODO
*/

/* print a menu */
void PrintMenu(void);

/* read image from a file */
int LoadImage(const char fname[SLEN],
	      unsigned char R[WIDTH][HEIGHT],
	      unsigned char G[WIDTH][HEIGHT],
	      unsigned char B[WIDTH][HEIGHT]);

/* save a processed image */
int SaveImage(const char fname[SLEN],
	      unsigned char R[WIDTH][HEIGHT],
	      unsigned char G[WIDTH][HEIGHT],
	      unsigned char B[WIDTH][HEIGHT]);

/* change a color image to black & white */
void BlackNWhite(unsigned char R[WIDTH][HEIGHT],
		 unsigned char G[WIDTH][HEIGHT],
		 unsigned char B[WIDTH][HEIGHT]);

/* reverse image color */
void Negative(unsigned char R[WIDTH][HEIGHT],
	      unsigned char G[WIDTH][HEIGHT],
	      unsigned char B[WIDTH][HEIGHT]);

/* color filter */
void ColorFilter(unsigned char R[WIDTH][HEIGHT],
		 unsigned char G[WIDTH][HEIGHT],
                 unsigned char B[WIDTH][HEIGHT],
		 int target_r, int target_g, int target_b, int threshold,
		 int replace_r, int replace_g, int replace_b);

/* edge detection */
void Edge(unsigned char R[WIDTH][HEIGHT],
	  unsigned char G[WIDTH][HEIGHT],
          unsigned char B[WIDTH][HEIGHT]);

/* mirror image horizontally */
void HMirror(unsigned char R[WIDTH][HEIGHT],
	     unsigned char G[WIDTH][HEIGHT],
             unsigned char B[WIDTH][HEIGHT]);

/* shuffle the image */
void Shuffle(unsigned char R[WIDTH][HEIGHT],
	     unsigned char G[WIDTH][HEIGHT],
             unsigned char B[WIDTH][HEIGHT]);


/* add border */
void AddBorder(unsigned char R[WIDTH][HEIGHT],
	       unsigned char G[WIDTH][HEIGHT],
               unsigned char B[WIDTH][HEIGHT],
	       char color[SLEN], int border_width);

/* flip image vertically */
void VFlip(unsigned char R[WIDTH][HEIGHT],
	   unsigned char G[WIDTH][HEIGHT],
           unsigned char B[WIDTH][HEIGHT]);

/* test all functions */
void AutoTest(unsigned char R[WIDTH][HEIGHT],
	      unsigned char G[WIDTH][HEIGHT],
	      unsigned char B[WIDTH][HEIGHT]);

int menuPrompt(void);
void callMenuFunctions(int choice, unsigned char R[WIDTH][HEIGHT], unsigned char G[WIDTH][HEIGHT], unsigned char B[WIDTH][HEIGHT]);

int main(void)
{
    /* Two dimensional arrays to hold the current image data, */
    /* one array for each color component.                    */
    unsigned char   R[WIDTH][HEIGHT];
    unsigned char   G[WIDTH][HEIGHT];
    unsigned char   B[WIDTH][HEIGHT];


    /* Please replace the following code with proper menu with function calls for DIP operations */
    int choice = 0;
    while (choice != 12) {
        choice = menuPrompt();
        callMenuFunctions(choice, R, G, B);
    }
    /* End of replacing */

    return 0;
}


/**
 * Declaring Menu Functions and Prompts
 */


void callMenuFunctions(int choice, unsigned char R[WIDTH][HEIGHT], unsigned char G[WIDTH][HEIGHT], unsigned char B[WIDTH][HEIGHT]) {
    switch (choice) {
        case 1:
            {
                char file_name[100];
                printf("Please input the file name to load: ");
                scanf("%s", file_name);
                while (getchar() != '\n');  // Discard extra characters in input buffer
                LoadImage(file_name, R, G, B);
                break;
            }
        case 2:
            {
                char file_name[100];
                printf("Please input the file name to save: ");
                scanf("%s", file_name);
                while (getchar() != '\n');  // Discard extra characters in input buffer
                SaveImage(file_name, R, G, B);
                break;
            }
        case 3:
            {
                BlackNWhite(R, G, B);
                printf("\"Black and White\" operation is done!\n");
                break;
            }
        case 4: {
                Negative(R, G, B);
                printf("\"Negative\" operation is done!\n");
                break;
        }
        case 5: {
                int target_r, target_g, target_b, threshold, replace_r, replace_g, replace_b;
                printf("Enter Red component for the target color: ");
                scanf("%d", &target_r);
                printf("Enter Green component for the target color: ");
                scanf("%d", &target_g);
                printf("Enter Blue component for the target color: ");
                scanf("%d", &target_b);
                printf("Enter threshold for the color difference: ");
                scanf("%d", &threshold);
                printf("Enter value for Red component in the target color: ");
                scanf("%d", &replace_r);
                printf("Enter value for Green component in the target color: ");
                scanf("%d", &replace_g);
                printf("Enter value for Blue component in the target color: ");
                scanf("%d", &replace_b);
                ColorFilter(R, G, B, target_r, target_g, target_b, threshold, replace_r, replace_g, replace_b);
                printf("\"Color Filter\" operation is done!\n");
                break;
        }
        case 6: {
                Edge(R, G, B);
                printf("\"Edge\" operation is done!\n");
                break;
        }

        case 7: {
                Shuffle(R, G, B);
                printf("\"Shuffle\" operation is done!\n");
                break;
        }


        case 12:
            {
                // Code for option 12
                break;
            }
        default:
            printf("Invalid choice.\n");
            break;
    }
}

const char *menu_options[] = {              // Dynamically sized Array of pointers to constant characters
    "--------------------------------",
    "1: Load a PPM image",
    "2: Save an image in PPM and JPEG format",
    "3: Change a color image to Black and White",
    "4: Make a negative of an image",
    "5: Color filter an image",
    "6: Sketch the edge of an image",
    "7: Shuffle an image",
    "8: Flip an image horizontally",
    "9: Mirror an image horizontally",
    "10: Pixelate an image",
    "11: Test all functions",
    "12: Exit"
};

int menuPrompt (void) {
    int choice = 0;

    for (int i = 0; i < MENU_SIZE; i++) {
        printf("%s\n", menu_options[i]);
    }
    printf("Please make your choice: ");
    scanf("%d", &choice);
    while(getchar() != '\n');  // Discard extra characters in input buffer

    if (choice > 0 && choice <= MENU_SIZE) {
        return choice;
    }
    else {
        printf("\nPlease select a valid option\n\n");
        menuPrompt();
    }
    return 0;
}

int LoadImage(const char fname[SLEN], unsigned char R[WIDTH][HEIGHT], unsigned char G[WIDTH][HEIGHT], unsigned char B[WIDTH][HEIGHT])
{
    FILE *File;
    char Type[SLEN];
    int  Width, Height, MaxValue;
    int  x, y;
    char ftype[] = ".ppm";
    char fname_ext[SLEN + sizeof(ftype)];

    strcpy(fname_ext, fname);
    strcat(fname_ext, ftype);

    File = fopen(fname_ext, "r");
    if (!File) {
        printf("\nCannot open file \"%s\" for reading!\n", fname);
        return 1;
    }
    fscanf(File, "%79s", Type);
    if (Type[0] != 'P' || Type[1] != '6' || Type[2] != 0) {
        printf("\nUnsupported file format!\n");
        return 2;
    }
    fscanf(File, "%d", &Width);
    if (Width != WIDTH) {
        printf("\nUnsupported image width %d!\n", Width);
        return 3;
    }
    fscanf(File, "%d", &Height);
    if (Height != HEIGHT) {
        printf("\nUnsupported image height %d!\n", Height);
        return 4;
    }
    fscanf(File, "%d", &MaxValue);
    if (MaxValue != 255) {
        printf("\nUnsupported image maximum value %d!\n", MaxValue);
        return 5;
    }
    if ('\n' != fgetc(File)) {
        printf("\nCarriage return expected!\n");
        return 6;
    }
    for (y = 0; y < HEIGHT; y++) {
        for (x = 0; x < WIDTH; x++) {
            R[x][y] = fgetc(File);
            G[x][y] = fgetc(File);
            B[x][y] = fgetc(File);
        }
    }
    if (ferror(File)) {
        printf("\nFile error while reading from file!\n");
        return 7;
    }
    printf("%s was read successfully!\n", fname_ext);
    fclose(File);
    return 0;
}

int SaveImage(const char fname[SLEN], unsigned char R[WIDTH][HEIGHT], unsigned char G[WIDTH][HEIGHT], unsigned char B[WIDTH][HEIGHT])
{
    FILE *File;
    char ftype[] = ".ppm";
    char fname_ext[SLEN + sizeof(ftype)];
    char SysCmd[SLEN * 5];
    int  x, y;

    strcpy(fname_ext, fname);
    strcat(fname_ext, ftype);

    File = fopen(fname_ext, "w");
    if (!File) {
        printf("\nCannot open file \"%s\" for writing!\n", fname);
        return 1;
    }
    fprintf(File, "P6\n");
    fprintf(File, "%d %d\n", WIDTH, HEIGHT);
    fprintf(File, "255\n");

    for (y = 0; y < HEIGHT; y++) {
        for (x = 0; x < WIDTH; x++) {
            fputc(R[x][y], File);
            fputc(G[x][y], File);
            fputc(B[x][y], File);
        }
    }

    if (ferror(File)) {
        printf("\nFile error while writing to file!\n");
        return 2;
    }
    fclose(File);
    printf("%s was saved successfully. \n", fname_ext);

    /*
     * rename file to image.ppm, convert it to ~/public_html/<fname>.jpg
     * and make it world readable
     */
    sprintf(SysCmd, "~eecs22/bin/pnmtojpeg_hw2.tcsh %s", fname_ext);
    if (system(SysCmd) != 0) {
        printf("\nError while converting to JPG:\nCommand \"%s\" failed!\n", SysCmd);
        return 3;
    }
    printf("%s.jpg was stored for viewing. \n", fname);

    return 0;
}

/*DO NOT EDIT AUTOTEST*/
/*DO NOT EDIT AUTOTEST*/
/*
void AutoTest(unsigned char R[WIDTH][HEIGHT], unsigned char G[WIDTH][HEIGHT], unsigned char B[WIDTH][HEIGHT]) {
	char fname[SLEN] = "EngPlaza";
	char sname[SLEN];

	LoadImage(fname, R, G, B);
	Negative(R, G, B);
	strcpy(sname, "negative");
	SaveImage(sname, R, G, B);
	printf("Negative tested!\n\n");

	LoadImage(fname, R, G, B);
	ColorFilter(R, G, B, 130, 130, 150, 30, 0, 255, 255);

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
	AddBorder (R, G, B, "pink", 32);
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
	printf("VFlip tested!\n\n");

	LoadImage(fname, R, G, B);
	Shuffle(R, G, B);
	strcpy(sname, "shuffle");
	SaveImage(sname, R, G, B);
	printf("Shuffle tested!\n\n");

}
*/

/**************************************************************/
/* Please add your function definitions here...               */
/**************************************************************/

void BlackNWhite(unsigned char R[WIDTH][HEIGHT], unsigned char G[WIDTH][HEIGHT], unsigned char B[WIDTH][HEIGHT]) {
    int x, y;
    unsigned char average;
    for (y = 0; y < HEIGHT; y++) {
        for (x = 0; x < WIDTH; x++) {
            average = (R[x][y] + G[x][y] + B[x][y]) / 3;
            R[x][y] = average;
            G[x][y] = average;
            B[x][y] = average;
        }
    }
}

void Negative(unsigned char R[WIDTH][HEIGHT], unsigned char G[WIDTH][HEIGHT], unsigned char B[WIDTH][HEIGHT]) {
    int x, y;
    for (y = 0; y < HEIGHT; y++) {
        for (x = 0; x < WIDTH; x++) {
            R[x][y] = 255 - R[x][y];
            G[x][y] = 255 - G[x][y];
            B[x][y] = 255 - B[x][y];
        }
    }
}

void ColorFilter(unsigned char R[WIDTH][HEIGHT], unsigned char G[WIDTH][HEIGHT], unsigned char B[WIDTH][HEIGHT], int target_r, int target_g, int target_b, int threshold, int replace_r, int replace_g, int replace_b) {
    int x, y;
    for (y = 0; y < HEIGHT; y++) {
        for (x = 0; x < WIDTH; x++) {
            if (
                    (R[x][y] >= target_r - threshold && R[x][y] <= target_r + threshold) &&
                    (G[x][y] >= target_g - threshold && G[x][y] <= target_g + threshold) &&
                    (B[x][y] >= target_b - threshold && B[x][y] <= target_b + threshold)
                ) {
                R[x][y] = replace_r;
                G[x][y] = replace_g;
                B[x][y] = replace_b;
            }
        }
    }
}

/**
 * TODO: Fix
 */

void Edge(unsigned char R[WIDTH][HEIGHT], unsigned char G[WIDTH][HEIGHT], unsigned char B[WIDTH][HEIGHT]) {
    int x, y;
    int A, B_, C, D, E_center, F, G_, H, I, temp_saturation;
    // Create temporary arrays to store new image
    unsigned char temp_R[WIDTH][HEIGHT], temp_G[WIDTH][HEIGHT], temp_B[WIDTH][HEIGHT];

    for (y = 1; y < HEIGHT - 1; y++) {
        for (x = 1; x < WIDTH - 1; x++) {
            // Get 3x3 neighborhood pixel values
            A = ((int)R[x-1][y-1] + (int)G[x-1][y-1] + (int)B[x-1][y-1]);
            B_ = ((int)R[x][y-1] + (int)G[x][y-1] + (int)B[x][y-1]);
            C = ((int)R[x+1][y-1] + (int)G[x+1][y-1] + (int)B[x+1][y-1]);
            D = ((int)R[x-1][y] + (int)G[x-1][y] + (int)B[x-1][y]);
            E_center = ((int)R[x][y] + (int)G[x][y] + (int)B[x][y]);
            F = ((int)R[x+1][y] + (int)G[x+1][y] + (int)B[x+1][y]);
            G_ = ((int)R[x-1][y+1] + (int)G[x-1][y+1] + (int)B[x-1][y+1]);
            H = ((int)R[x][y+1] + (int)G[x][y+1] + (int)B[x][y+1]);
            I = ((int)R[x+1][y+1] + (int)G[x+1][y+1] + (int)B[x+1][y+1]);

            // Apply edge detection formula
            temp_saturation = (-A - B_ - C - D - F - G_ - H - I + (8 * E_center));

            // Saturate to range [0, 255]
            temp_saturation = (temp_saturation < 0) ? 0 : (temp_saturation > 255) ? 255 : temp_saturation;

            // Assign result to output channels
            temp_R[x][y] = (unsigned char)temp_saturation;
            temp_G[x][y] = (unsigned char)temp_saturation;
            temp_B[x][y] = (unsigned char)temp_saturation;
        }
    }

    // Set boundary pixels to 0
    for (y = 0; y < HEIGHT; y++) {
        R[0][y] = G[0][y] = B[0][y] = 0;
        R[WIDTH-1][y] = G[WIDTH-1][y] = B[WIDTH-1][y] = 0;
    }
    for (x = 0; x < WIDTH; x++) {
        R[x][0] = G[x][0] = B[x][0] = 0;
        R[x][HEIGHT-1] = G[x][HEIGHT-1] = B[x][HEIGHT-1] = 0;
    }

    // Overwrite R,G,B with temp arrays
    for(y = 1; y < HEIGHT-1; y++) {
        for (x = 1; x < WIDTH-1; x++) {
            R[x][y] = temp_R[x][y];
            G[x][y] = temp_G[x][y];
            B[x][y] = temp_B[x][y];

        }
    }
}

/* shuffle the image */

// Helper function to swap blocks
void swap_blocks(int start_x1, int stop_x1, int start_y1, int stop_y1,
                 int start_x2, int stop_x2, int start_y2, int stop_y2,
                 unsigned char R[WIDTH][HEIGHT],
                 unsigned char G[WIDTH][HEIGHT],
                 unsigned char B[WIDTH][HEIGHT]) {
    // Calculate block dimensions
    int block_width = stop_x1 - start_x1;
    int block_height = stop_y1 - start_y1;

    // Temporary storage for RGB channels
    unsigned char temp_R[block_width][block_height];
    unsigned char temp_G[block_width][block_height];
    unsigned char temp_B[block_width][block_height];

    // Copy first block to temp storage
    for (int i = 0; i < block_width; i++) {
        for (int j = 0; j < block_height; j++) {
            temp_R[i][j] = R[start_x1 + i][start_y1 + j];
            temp_G[i][j] = G[start_x1 + i][start_y1 + j];
            temp_B[i][j] = B[start_x1 + i][start_y1 + j];

            // Swap first block with second block
            R[start_x1 + i][start_y1 + j] = R[start_x2 + i][start_y2 + j];
            G[start_x1 + i][start_y1 + j] = G[start_x2 + i][start_y2 + j];
            B[start_x1 + i][start_y1 + j] = B[start_x2 + i][start_y2 + j];
        }
    }

    // Copy temp block to the second block's location
    for (int i = 0; i < block_width; i++) {
        for (int j = 0; j < block_height; j++) {
            R[start_x2 + i][start_y2 + j] = temp_R[i][j];
            G[start_x2 + i][start_y2 + j] = temp_G[i][j];
            B[start_x2 + i][start_y2 + j] = temp_B[i][j];
        }
    }
}

void Shuffle(unsigned char R[WIDTH][HEIGHT],
	         unsigned char G[WIDTH][HEIGHT],
             unsigned char B[WIDTH][HEIGHT]
             ) {
                int x_separations[5] = {0, WIDTH / 4, WIDTH / 2, WIDTH * 3 / 4, WIDTH};
                int y_separations[5] = {0, HEIGHT / 4, HEIGHT / 2, HEIGHT * 3 / 4, HEIGHT};
                /**
                 * Shuffle Image swapping the following blocks:
                 * [0][0] to [WIDTH / 4][HEIGHT / 4] with [WIDTH * 3/4][HEIGHT * 3/4] to [WIDTH][HEIGHT]
                 * [WIDTH / 4][0] to [WIDTH / 2][HEIGHT / 4] with [WIDTH / 2][HEIGHT * 3/4] to [WIDTH * 3/4][HEIGHT]
                 * [WIDTH / 2][0] to [WIDTH * 3/4][HEIGHT / 4] with [WIDTH / 4][HEIGHT * 3/4] to [WIDTH / 2][HEIGHT]
                 * [WIDTH * 3/4][0] to [WIDTH][HEIGHT / 4] with [0][HEIGHT * 3/4] to [WIDTH / 4][HEIGHT]
                 *
                 * [0][HEIGHT / 4] to [WIDTH / 4][HEIGHT / 2] with [WIDTH * 3/4][HEIGHT / 2] to [WIDTH][HEIGHT * 3/4]
                 * [WIDTH / 4][HEIGHT / 4] to [WIDTH / 2][HEIGHT / 2] with [WIDTH / 2][HEIGHT / 2] to [WIDTH * 3/4][HEIGHT * 3/4]
                 * [WIDTH / 2][HEIGHT / 4] to [WIDTH * 3/4][HEIGHT / 2] with [WIDTH / 4][HEIGHT / 2] to [WIDTH / 2][HEIGHT * 3/4]
                 * [WIDTH * 3/4][HEIGHT / 4] to [WIDTH][HEIGHT / 2] with [0][HEIGHT / 2] to [WIDTH / 4][HEIGHT * 3/4]
                 *
                 * [0][HEIGHT / 2] to [WIDTH / 4][HEIGHT * 3/4] with [WIDTH * 3/4][HEIGHT / 4] to [WIDTH][HEIGHT / 2]
                 * [WIDTH / 4][HEIGHT / 2] to [WIDTH / 2][HEIGHT * 3/4] with [WIDTH / 2][HEIGHT / 4] to [WIDTH * 3/4][HEIGHT / 2]
                 * [WIDTH / 2][HEIGHT / 2] to [WIDTH * 3/4][HEIGHT * 3/4] with [WIDTH / 4][HEIGHT / 4] to [WIDTH / 2][HEIGHT / 2]
                 * [WIDTH * 3/4][HEIGHT / 2] to [WIDTH][HEIGHT * 3/4] with [0][HEIGHT / 4] to [WIDTH / 4][HEIGHT / 2]
                 *
                 * [0][HEIGHT * 3/4] to [WIDTH / 4][HEIGHT] with [WIDTH * 3/4][0] to [WIDTH][HEIGHT / 4]
                 * [WIDTH / 4][HEIGHT * 3/4] to [WIDTH / 2][HEIGHT] with [WIDTH / 2][0] to [WIDTH * 3/4][HEIGHT / 4]
                 * [WIDTH / 2][HEIGHT * 3/4] to [WIDTH * 3/4][HEIGHT] with [WIDTH / 4][0] to [WIDTH / 2][HEIGHT / 4]
                 * [WIDTH * 3/4][HEIGHT * 3/4] to [WIDTH][HEIGHT] with [0][0] to [WIDTH / 4][HEIGHT / 4]
                 */
                    swap_blocks(x_separations[0], x_separations[1], y_separations[0], y_separations[1],
                                x_separations[3], x_separations[4], y_separations[3], y_separations[4], R, G, B);

                    swap_blocks(x_separations[1], x_separations[2], y_separations[0], y_separations[1],
                                x_separations[2], x_separations[3], y_separations[3], y_separations[4], R, G, B);

                    swap_blocks(x_separations[2], x_separations[3], y_separations[0], y_separations[1],
                                x_separations[1], x_separations[2], y_separations[3], y_separations[4], R, G, B);

                    swap_blocks(x_separations[3], x_separations[4], y_separations[0], y_separations[1],
                                x_separations[0], x_separations[1], y_separations[3], y_separations[4], R, G, B);

                    swap_blocks(x_separations[0], x_separations[1], y_separations[1], y_separations[2],
                                x_separations[3], x_separations[4], y_separations[2], y_separations[3], R, G, B);

                    swap_blocks(x_separations[1], x_separations[2], y_separations[1], y_separations[2],
                                x_separations[2], x_separations[3], y_separations[2], y_separations[3], R, G, B);

                    swap_blocks(x_separations[2], x_separations[3], y_separations[1], y_separations[2],
                                x_separations[1], x_separations[2], y_separations[2], y_separations[3], R, G, B);

                    swap_blocks(x_separations[3], x_separations[4], y_separations[1], y_separations[2],
                                x_separations[0], x_separations[1], y_separations[2], y_separations[3], R, G, B);

                    swap_blocks(x_separations[0], x_separations[1], y_separations[2], y_separations[3],
                                x_separations[3], x_separations[4], y_separations[1], y_separations[2], R, G, B);

                    swap_blocks(x_separations[1], x_separations[2], y_separations[2], y_separations[3],
                                x_separations[2], x_separations[3], y_separations[1], y_separations[2], R, G, B);

                    swap_blocks(x_separations[2], x_separations[3], y_separations[2], y_separations[3],
                                x_separations[1], x_separations[2], y_separations[1], y_separations[2], R, G, B);

                    swap_blocks(x_separations[3], x_separations[4], y_separations[2], y_separations[3],
                                x_separations[0], x_separations[1], y_separations[1], y_separations[2], R, G, B);

                    swap_blocks(x_separations[0], x_separations[1], y_separations[3], y_separations[4],
                                x_separations[3], x_separations[4], y_separations[0], y_separations[1], R, G, B);

                    swap_blocks(x_separations[1], x_separations[2], y_separations[3], y_separations[4],
                                x_separations[2], x_separations[3], y_separations[0], y_separations[1], R, G, B);

                    swap_blocks(x_separations[2], x_separations[3], y_separations[3], y_separations[4],
                                x_separations[1], x_separations[2], y_separations[0], y_separations[1], R, G, B);

                    swap_blocks(x_separations[3], x_separations[4], y_separations[3], y_separations[4],
                                x_separations[0], x_separations[1], y_separations[0], y_separations[1], R, G, B);


             }





/* EOF */
