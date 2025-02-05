/******************************************************************************/
/* PhotoLab_v2.c: Template for assignment #3, EECS 22, Winter 2024            */
/* Please use this template to submit your hw3. (WL 24)                       */
/******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define SLEN 80					/* maximum length of file names and string*/

static const int WIDTH=512; 			/* image width */
static const int HEIGHT=288; 			/* image height */
static const int SUCCESS=0; 			/* return code for success */
static const int EXIT=12; 			/* menu item number for EXIT */
static const int MAX_PIXEL=255; 		/* max pixel value */
static const int MIN_PIXEL=0; 			/* min pixel value */
static const int SHUFF_HEIGHT_DIV=4; 	/* Height division for shuffling */
static const int SHUFF_WIDTH_DIV=4; 	/* Width division for shuffling */


void PrintMenu();

/* load image from a file */
int LoadImage(const char fname[SLEN], unsigned char R[WIDTH][HEIGHT], unsigned char G[WIDTH][HEIGHT], unsigned char B[WIDTH][HEIGHT]);

/* save a processed image */
int SaveImage(const char fname[SLEN], unsigned char R[WIDTH][HEIGHT], unsigned char G[WIDTH][HEIGHT], unsigned char B[WIDTH][HEIGHT]);

/* reverse image color */
void Negative(unsigned char R[WIDTH][HEIGHT], unsigned char G[WIDTH][HEIGHT], unsigned char B[WIDTH][HEIGHT]);

/* color filter */
void ColorFilter(unsigned char R[WIDTH][HEIGHT], unsigned char G[WIDTH][HEIGHT], unsigned char B[WIDTH][HEIGHT], int target_r, int target_g, int target_b, int threshold, int replace_r, int replace_g, int replace_b);

/* edge detection */
void Edge(unsigned char R[WIDTH][HEIGHT], unsigned char G[WIDTH][HEIGHT], unsigned char B[WIDTH][HEIGHT]);

/* mirror image horizontally */
void HMirror(unsigned char R[WIDTH][HEIGHT], unsigned char G[WIDTH][HEIGHT], unsigned char B[WIDTH][HEIGHT]);

/* change color image to black & white */
void BlackNWhite(unsigned char R[WIDTH][HEIGHT], unsigned char G[WIDTH][HEIGHT], unsigned char B[WIDTH][HEIGHT]);

/* shuffle the image */
void Shuffle(unsigned char R[WIDTH][HEIGHT], unsigned char G[WIDTH][HEIGHT], unsigned char B[WIDTH][HEIGHT]);

/* add border */
void AddBorder(unsigned char R[WIDTH][HEIGHT], unsigned char G[WIDTH][HEIGHT], unsigned char B[WIDTH][HEIGHT], char color[SLEN], int border_width);

/* flip image vertically */
void VFlip(unsigned char R[WIDTH][HEIGHT], unsigned char G[WIDTH][HEIGHT], unsigned char B[WIDTH][HEIGHT]);

/* Create a fisheye image W24 */
void FishEye(unsigned char R[WIDTH][HEIGHT], unsigned char G[WIDTH][HEIGHT], unsigned char B[WIDTH][HEIGHT], double distortion_factor, double scaling_factor);

/* posterize the image */
void Posterize(unsigned char R[WIDTH][HEIGHT], unsigned char G[WIDTH][HEIGHT], unsigned char B[WIDTH][HEIGHT], unsigned int rbits, unsigned int gbits, unsigned int bbits);

/* rotate and zoom the image */
void Rotate(unsigned char R[WIDTH][HEIGHT], unsigned char G[WIDTH][HEIGHT], unsigned char B[WIDTH][HEIGHT], double Angle, double ScaleFactor,  int CenterX, int CenterY);

/* motion blur */
void MotionBlur(int BlurAmount, unsigned char R[WIDTH][HEIGHT], unsigned char G[WIDTH][HEIGHT], unsigned char B[WIDTH][HEIGHT]);

/* Test all functions */
void AutoTest(unsigned char R[WIDTH][HEIGHT], unsigned char G[WIDTH][HEIGHT], unsigned char B[WIDTH][HEIGHT]);

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
		else if (option >= 2 && option <= 10) {
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
					case 10:
						printf("Enter border width:");
						scanf("%d", &border_width);
						printf("Available border colors : black, white, red, green, blue, yellow, cyan, pink, orange\n");
						printf("Select border color from the options: ");
						scanf("%s",colorOption);
						AddBorder(R, G, B, colorOption, border_width);
						printf("\"Border\" operation is done!\n");
						break;
					default:
						break;
				}
			}
		}
		else if (option == 11) {
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

int LoadImage(const char fname[SLEN], unsigned char R[WIDTH][HEIGHT], unsigned char G[WIDTH][HEIGHT], unsigned char B[WIDTH][HEIGHT]) {
	FILE           *File;
	char            Type[SLEN];
	int             Width, Height, MaxValue;
	int             x, y;
	char            ftype[] = ".ppm";
	char            fname_tmp[SLEN];
	strcpy(fname_tmp, fname);
	strcat(fname_tmp, ftype);

	File = fopen(fname_tmp, "r");
	if (!File) {
		printf("Cannot open file \"%s\" for reading!\n", fname_tmp);
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
	for (y = 0; y < HEIGHT; y++)
		for (x = 0; x < WIDTH; x++) {
			R[x][y] = fgetc(File);
			G[x][y] = fgetc(File);
			B[x][y] = fgetc(File);
		}
	if (ferror(File)) {
		printf("\nFile error while reading from file!\n");
		return 7;
	}
	printf("%s was read.\n", fname_tmp);
	fclose(File);
	return 0;
}

int SaveImage(const char fname[SLEN], unsigned char R[WIDTH][HEIGHT], unsigned char G[WIDTH][HEIGHT], unsigned char B[WIDTH][HEIGHT]) {
	FILE	*File;
	int	x, y;
	char	SysCmd[SLEN * 5];

	char	ftype[] = ".ppm";
	char	fname_tmp[SLEN];  /*avoid to modify on the original pointer, 11/10/10, X.Han*/
	char	fname_tmp2[SLEN];

	strcpy(fname_tmp, fname);
	strcpy(fname_tmp2, fname);
	strcat(fname_tmp2, ftype);

	File = fopen(fname_tmp2, "w");
	if (!File) {
		printf("Cannot open file \"%s\" for writing!\n", fname);
		return 1;
	}
	fprintf(File, "P6\n");
	fprintf(File, "%d %d\n", WIDTH, HEIGHT);
	fprintf(File, "255\n");

	for (y = 0; y < HEIGHT; y++)
		for (x = 0; x < WIDTH; x++) {
			fputc(R[x][y], File);
			fputc(G[x][y], File);
			fputc(B[x][y], File);
		}

	if (ferror(File)) {
		printf("\nFile error while writing to file!\n");
		return 2;
	}
	fclose(File);
	printf("%s was saved. \n", fname_tmp2);

	/*
	 * rename file to image.ppm, convert it to ~/public_html/<fname>.jpg
	 * and make it world readable
	 */
	sprintf(SysCmd, "/users/grad2/doemer/eecs22/bin/pnmtojpeg_hw3.tcsh %s", fname_tmp2);
	if (system(SysCmd) != 0) {
		printf("\nError while converting to JPG:\nCommand \"%s\" failed!\n", SysCmd);
		return 3;
	}
	printf("%s.jpg was stored.\n", fname_tmp);
	return 0;
}

/*******************************************/
/* Function implementations should go here */
/*******************************************/

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
	printf("11: Test all functions\n");
	printf("12: Exit\n");
}

/* reverse image color */
void Negative(unsigned char R[WIDTH][HEIGHT], unsigned char G[WIDTH][HEIGHT], unsigned char B[WIDTH][HEIGHT]) {
	int x, y;
	for (y = 0; y < HEIGHT; y++) {
		for (x = 0; x < WIDTH; x++) {
			R[x][y] = MAX_PIXEL - R[x][y];
			G[x][y] = MAX_PIXEL - G[x][y];
			B[x][y] = MAX_PIXEL - B[x][y];
		}
	}
}

/* color filter */
void ColorFilter (unsigned char R[WIDTH][HEIGHT], unsigned char G[WIDTH][HEIGHT], unsigned char B[WIDTH][HEIGHT],
		int target_r, int target_g, int target_b, int threshold, int replace_r, int replace_g, int replace_b) {

	int x, y;

	replace_r = (replace_r > MAX_PIXEL) ? MAX_PIXEL : (replace_r < 0) ? 0 : replace_r;
	replace_g = (replace_g > MAX_PIXEL) ? MAX_PIXEL : (replace_g < 0) ? 0 : replace_g;
	replace_b = (replace_b > MAX_PIXEL) ? MAX_PIXEL : (replace_b < 0) ? 0 : replace_b;

	for (y = 0; y < HEIGHT; y++){
		for (x = 0; x < WIDTH; x++){
			if (abs(R[x][y] - target_r) <= threshold
					&& abs(G[x][y] - target_g) <= threshold
					&& abs(B[x][y] - target_b) <= threshold) {
				R[x][y] = replace_r;
				G[x][y] = replace_g;
				B[x][y] = replace_b;
			}
		}
	}
}

/* edge detection */
void Edge(unsigned char R[WIDTH][HEIGHT], unsigned char G[WIDTH][HEIGHT], unsigned char B[WIDTH][HEIGHT]) {
	int             x, y, m, n, a, b;
	unsigned char   tmpR[WIDTH][HEIGHT];
	unsigned char   tmpG[WIDTH][HEIGHT];
	unsigned char   tmpB[WIDTH][HEIGHT];
	for (y = 0; y < HEIGHT; y++){
		for (x = 0; x < WIDTH; x++) {
			tmpR[x][y] = R[x][y];
			tmpG[x][y] = G[x][y];
			tmpB[x][y] = B[x][y];
		}
	}
	int sumR = 0;   /* sum of the intensity differences with neighbors */
	int sumG = 0;
	int sumB = 0;
	for (y = 1; y < HEIGHT - 1; y++){
		for (x = 1; x < WIDTH - 1; x++){
			for (n = -1; n <= 1; n++){
				for (m = -1; m <= 1; m++) {
					a = (x + m >= WIDTH) ? WIDTH - 1 : (x + m < 0) ? 0 : x + m;
					b = (y + n >= HEIGHT) ? HEIGHT - 1 : (y + n < 0) ? 0 : y + n;
					sumR += (tmpR[x][y] - tmpR[a][b]);
					sumG += (tmpG[x][y] - tmpG[a][b]);
					sumB += (tmpB[x][y] - tmpB[a][b]);
				}
			}
			R[x][y] = (sumR > MAX_PIXEL) ? MAX_PIXEL: (sumR < 0) ? 0: sumR;
			G[x][y] = (sumG > MAX_PIXEL) ? MAX_PIXEL: (sumG < 0) ? 0: sumG;
			B[x][y] = (sumB > MAX_PIXEL) ? MAX_PIXEL: (sumB < 0) ? 0: sumB;
			sumR = sumG = sumB = 0;
		}
	}
	/* set all 24 borders to 0 */
	for (y = 0; y < HEIGHT; y++) {
		R[0][y] = 0;
		G[0][y] = 0;
		B[0][y] = 0;
		R[WIDTH - 1][y] = 0;
		G[WIDTH - 1][y] = 0;
		B[WIDTH - 1][y] = 0;
	}
	for (x = 0; x < WIDTH; x++) {
		R[x][0] = 0;
		G[x][0] = 0;
		B[x][0] = 0;
		R[x][HEIGHT - 1] = 0;
		G[x][HEIGHT - 1] = 0;
		B[x][HEIGHT - 1] = 0;
	}
}

/* mirror image horizontally */
void HMirror(unsigned char R[WIDTH][HEIGHT], unsigned char G[WIDTH][HEIGHT], unsigned char B[WIDTH][HEIGHT])
{
	int             x, y;

	for (y = 0; y < HEIGHT; y++) {
		for (x = 0; x < WIDTH / 2; x++) {
			R[WIDTH - 1 - x][y] = R[x][y] ;
			G[WIDTH - 1 - x][y] = G[x][y] ;
			B[WIDTH - 1 - x][y] = B[x][y] ;
		}
	}
}

/* add border to the image */
void AddBorder(unsigned char R[WIDTH][HEIGHT], unsigned char G[WIDTH][HEIGHT], unsigned char B[WIDTH][HEIGHT],
		char color[SLEN], int border_width) {
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
		return;
	}
	for (y = 0; y < HEIGHT; y++) {
		for (x = 0; x < WIDTH; x++) {
			if ((y < border_width) || (y > HEIGHT - 1 - border_width) ||
					(x < border_width) || (x > WIDTH - 1 - border_width)) {
				R[x][y] = border_r;
				G[x][y] = border_g;
				B[x][y] = border_b;
			}
		}
	}
}

/* change color image to black & white */
void BlackNWhite(unsigned char R[WIDTH][HEIGHT], unsigned char G[WIDTH][HEIGHT], unsigned char B[WIDTH][HEIGHT])
{
	int             x, y, tmp;

	for (y = 0; y < HEIGHT; y++)
	{
		for (x = 0; x < WIDTH; x++)
		{
			tmp = (R[x][y] + G[x][y] + B[x][y]) / 3;
			R[x][y] = G[x][y] = B[x][y] = tmp;
		}
	}
}

/* flip image vertically */
void VFlip(unsigned char R[WIDTH][HEIGHT], unsigned char G[WIDTH][HEIGHT], unsigned char B[WIDTH][HEIGHT])
{
	int             x, y;
	unsigned char   r, g, b;

	for (y = 0; y < HEIGHT/2; y++) {
		for (x = 0; x < WIDTH; x++) {
			r = R[x][HEIGHT - 1 - y];
			g = G[x][HEIGHT - 1 - y];
			b = B[x][HEIGHT - 1 - y];

			R[x][HEIGHT - 1 - y] = R[x][y];
			G[x][HEIGHT - 1 - y] = G[x][y];
			B[x][HEIGHT - 1 - y] = B[x][y];

			R[x][y] = r;
			G[x][y] = g;
			B[x][y] = b;
		}
	}
}

/* Shuffle the image */
void Shuffle(unsigned char R[WIDTH][HEIGHT], unsigned char G[WIDTH][HEIGHT], unsigned char B[WIDTH][HEIGHT])
{
	int block_width = WIDTH/SHUFF_WIDTH_DIV;
	int block_height = HEIGHT/SHUFF_HEIGHT_DIV;
	int i, j, x, y;
	unsigned char tempR[block_width][block_height];
	unsigned char tempG[block_width][block_height];
	unsigned char tempB[block_width][block_height];

	for (i = 0; i < SHUFF_HEIGHT_DIV/2; i++){ // only need to iterate throught half of the blocks
		for (j = 0; j < SHUFF_WIDTH_DIV; j++){
			for (x = 0; x < block_width; x++){
				for (y = 0; y < block_height; y++){
					tempR[x][y] = R[x + (j * block_width)][y + (i * block_height)];
					tempG[x][y] = G[x + (j * block_width)][y + (i * block_height)];
					tempB[x][y] = B[x + (j * block_width)][y + (i * block_height)];
					R[x + (j * block_width)][y + (i * block_height)] = R[x + ((SHUFF_WIDTH_DIV - 1 -j) * block_width)][y + ((SHUFF_HEIGHT_DIV - 1 -i) * block_height)];
					G[x + (j * block_width)][y + (i * block_height)] = G[x + ((SHUFF_WIDTH_DIV - 1 -j) * block_width)][y + ((SHUFF_HEIGHT_DIV - 1 -i) * block_height)];
					B[x + (j * block_width)][y + (i * block_height)] = B[x + ((SHUFF_WIDTH_DIV - 1 -j) * block_width)][y + ((SHUFF_HEIGHT_DIV - 1 -i) * block_height)];
					R[x + ((SHUFF_WIDTH_DIV - 1 -j) * block_width)][y + ((SHUFF_HEIGHT_DIV - 1 -i) * block_height)] = tempR[x][y];
					G[x + ((SHUFF_WIDTH_DIV - 1 -j) * block_width)][y + ((SHUFF_HEIGHT_DIV - 1 -i) * block_height)] = tempG[x][y];
					B[x + ((SHUFF_WIDTH_DIV - 1 -j) * block_width)][y + ((SHUFF_HEIGHT_DIV - 1 -i) * block_height)] = tempB[x][y];
				}
			}
		}
	}

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
