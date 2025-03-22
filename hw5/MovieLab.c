/*********************************************************************/
/* Homework Assignment 5, for EECS 22, Winter 2024                   */
/* MovieLab.c: source file for the main function                     */
/* Please use this template for your HW5.                            */
/*********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include <math.h>

#include "FileIO.h"
#include "DIPs.h"
#include "Movie.h"
#include "Constants.h"
#include "Image.h"

/* Load one movie frame from the input file */
YUVImage* LoadOneFrame(const char* fname, int n,
	unsigned int width, unsigned height);

/* Load the movie frames from the input file */
Movie *LoadMovie(const char *fname, int frameNum,
	unsigned int width, unsigned height);

/* Saves one movie frame to the output file */
void SaveOneFrame(YUVImage *image, const char *fname, FILE *file);

/* Save the movie frames to the output file */
int SaveMovie(const char *fname, Movie *movie);

/* Print the command-line arguments usage of the program */
void PrintUsage();

int main(int argc, char *argv[])
{
	int x = 0;
	char *fin = NULL;
	char *fout = NULL;

	Movie *movie = NULL;

	unsigned int frameNum = 0;
	unsigned int width = 512;
	unsigned int height = 288;

	/* the while loop to check options in the command line */
	while (x < argc) {
		/* the input file name */
		if (strcmp(argv[x], "-i") == 0) {
			if (x < argc - 1) {
				fin = argv[x + 1];
			} /*fi*/
			else {
				printf("Missing argument for the input file name!\n");
				return 5;
			} /*esle*/
			x += 2;
			continue;
		} /*fi*/

		/* the output file name */
		if (strcmp(argv[x], "-o") == 0) {
			if (x < argc - 1) {
				fout = argv[x + 1];
			} /*fi*/
			else {
				printf("Missing argument for the output file name!\n");
				return 5;
			} /*esle*/
			x += 2;
			continue;
		} /*fi*/

		/* Number of frames*/
		if (strcmp(argv[x], "-f") == 0) {
			if (x < argc - 1) {
				if (sscanf(argv[x+1], "%u", &frameNum) == 0) {
				}
			} /*fi*/
			else {
				printf("Missing argument for the number of frames to load!\n");
				return 5;
			} /*esle*/
			x += 2;
			continue;
		} /*fi*/

		/* the image dimensions option */
		if (strcmp(argv[x], "-s") == 0) {
			if (x < argc - 1) {
				if (sscanf(argv[x+1], "%ux%u", &width, &height) == 2) {
				}
			} /*fi*/
			else {
				printf("Missing argument for image dimensions!\n");
				return 5;
			} /*esle*/
			x += 2;
			continue;
		} /*fi*/

		/* the help option */
		if (strcmp(argv[x], "-h") == 0) {
			PrintUsage();
			return 0;
		} /*fi*/

		if (strcmp(argv[x], "-bw") == 0) {
			if (!movie) {
				movie = LoadMovie(fin, frameNum, width, height);
			}

			YUV2RGBMovie(movie);
			IENTRY *curr, *next;
			ILIST *frames = movie->Frames;
			curr = frames-> First;
			while(curr) {
				next = curr->Next;
				BlackNWhite(curr->Image);
				curr = next;
			}
			RGB2YUVMovie(movie);
			x++;
			printf("\nOperation Black and White is done!\n");
			continue;
		}
		if (strcmp(argv[x], "-hmirror") == 0) {
			if (!movie) {
				movie = LoadMovie(fin, frameNum, width, height);
			}

			YUV2RGBMovie(movie);
			IENTRY *curr, *next;
			ILIST *frames = movie->Frames;
			curr = frames-> First;
			while(curr) {
				next = curr->Next;
				HMirror(curr->Image);
				curr = next;
			}
			RGB2YUVMovie(movie);
			printf("\nOperation HMirror is done!\n");
			x++;
			continue;
		}

		if (strcmp(argv[x], "-edge") == 0) {
			if (!movie) {
				movie = LoadMovie(fin, frameNum, width, height);
			}

			YUV2RGBMovie(movie);
			IENTRY *curr, *next;
			ILIST *frames = movie->Frames;
			curr = frames-> First;
			while(curr) {
				next = curr->Next;
				Edge(curr->Image);
				curr = next;
			}
			RGB2YUVMovie(movie);
			printf("\nOperation Edge is done!\n");
			x++;
			continue;
		}
		if (strcmp(argv[x], "-crop") == 0) {
			unsigned int start, end, newFNum;
			if (!movie) {
				movie = LoadMovie(fin, frameNum, width, height);
			}
			if (x < argc - 1) {
				if (sscanf(argv[x + 1], "%u-%u", &start, &end) == 2) {
					newFNum = (end - start) + 1;
					CropImageList(movie->Frames, start, end);
					printf("Operation Crop is done! New number of frames is %u.\n", newFNum);
				}
			} else {
				printf("Missing argument for start and end!\n");
				return 5;
			}
			x += 2;
			continue;
		}

		/* Fast Forward */
		if (strcmp(argv[x], "-fast") == 0) {
			if (!movie) movie = LoadMovie(fin, frameNum, width, height);
			if (x + 1 >= argc) {
				printf("Missing argument for -fast!\n");
				return 5;
			}
			unsigned int factor;
			if (sscanf(argv[x + 1], "%u", &factor) != 1 || factor < 1) {
				printf("Invalid argument for -fast!\n");
				return 5;
			}
			FastImageList(movie->Frames, factor);
			frameNum /= factor; // Update frame count
			printf("Operation Fast Forward is done! New number of frames is %u", frameNum);
			x += 2; // Skip "-fast" and its argument
			continue;
		}

		if (strcmp(argv[x], "-reverse") == 0) {
			if (!movie) {
				movie = LoadMovie(fin, frameNum, width, height);
			}
			ReverseImageList(movie->Frames);
			x++;
			continue;
		}

		x++;
	} /*elihw*/

	if (fin == NULL) {
		printf("Missing argument for the input file name!\n");
		PrintUsage();
		return 5;
	}

	if (fout == NULL) {
		printf("Missing argument for the output file name!\n");
		PrintUsage();
		return 5;
	}

	// Load movie and Convert back to YUV
    if (!movie) {
        printf("Failed to load movie!\n");
        return 1;
    }

    // Save the movie
    if (SaveMovie(fout, movie)) {
        printf("Failed to save movie!\n");
        DeleteMovie(movie);
        return 1;
    }

    // Clean up
    DeleteMovie(movie);

    fin = NULL;
    fout = NULL;

    return 0;
}

void PrintUsage()
{
	printf("\nUsage: MovieLab -i <file> -o <file> -f <framenum> [options]\n"
	       "Options:\n"
	       "-s <WidthxHeight>     To set the resolution of the input stream (WidthxHeight)\n"
	       "-bw                   Activate the black and white filter on every movie frame\n"
	       "-hmirror              Activate horizontal mirror on every movie frame\n"
	       "-edge                 Activate the edge filter on every movie frame\n"
	       "-crop <start-end>     Crop the movie frames from <start> to <end>\n"
	       "-fast <factor>        Fast forward the movie by <factor>\n"
	       "-reverse              Reverse the frame order of the input movie\n"
	       "-watermark <file>     Add a watermark from <file> to every movie frame\n"
	       "-spotlight            Spotlight the fade in and fade out\n"
	       "-rotate               Rotate and zoom in/out the input movie\n"
	       "-h                    Display this usage information\n"
	      );
}

/* Load one movie frame from the input file */
YUVImage* LoadOneFrame(const char* fname, int n,
	unsigned int width, unsigned height)
{
	FILE *file;
	unsigned int x, y;
	unsigned char c;
	YUVImage* YUVimage;

	/* Check errors */
	assert(fname);
	assert(n >= 0);

	YUVimage = CreateYUVImage(width, height);
	if (YUVimage == NULL) {
		return NULL;
	}

	/* Open the input file */
	file = fopen(fname, "r");
	if (file == NULL) {
		DeleteYUVImage(YUVimage);
		return NULL;
	}

	/* Find the desired frame */
	fseek(file, 1.5 * n * width * height, SEEK_SET);

	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++) {
			c = fgetc(file);
			SetPixelY(YUVimage, x, y, c);
		} /*rof*/
	}

	for (y = 0; y < height; y += 2) {
		for (x = 0; x < width; x += 2) {
			c = fgetc(file);
			SetPixelU(YUVimage, x, y, c);
			SetPixelU(YUVimage, x + 1, y, c);
			SetPixelU(YUVimage, x, y + 1, c);
			SetPixelU(YUVimage, x + 1, y + 1, c);
		}
	}

	for (y = 0; y < height; y += 2) {
		for (x = 0; x < width; x += 2) {
			c = fgetc(file);
			SetPixelV(YUVimage, x, y, c);
			SetPixelV(YUVimage, x + 1, y, c);
			SetPixelV(YUVimage, x, y + 1, c);
			SetPixelV(YUVimage, x + 1, y + 1, c);
		}
	}

	/* Check errors */
	assert(ferror(file) == 0);

	/* Close the input file and return */
	fclose(file);
	file = NULL;
	return YUVimage;
}

/* Load the movie frames from the input file */
Movie *LoadMovie(const char *fname, int frameNum,
	unsigned int width, unsigned height)
{
	Movie *movie = CreateMovie();

	if (!movie) {
		return NULL;
	}

	for (int i = 0; i < frameNum; i++) {
		YUVImage *frame = LoadOneFrame(fname, i, width, height);
		if(!frame) {
			DeleteMovie(movie);
			return NULL;
		}
		AppendYUVImage(movie->Frames,frame);
	}
	printf("The movie file %s has been read successfully!\n", fname);
	return movie;
}

/* Save the movie frames to the output file */
int SaveMovie(const char *fname, Movie *movie)
{
	int count;
	FILE *file;
	IENTRY *curr;

	/* Open the output file */
	file = fopen(fname, "w");
	if (file == NULL) {
		return 1;
	}

	count = 0;
	curr = movie->Frames->First;
	while (curr != NULL) {

		SaveOneFrame(curr->Image, fname, file);
		curr = curr->Next;
		count++;
	}

	fclose(file);
	file = NULL;

	printf("The movie file %s has been written successfully!\n", fname);
	printf("%d frames are written to the file %s in total.\n", count, fname);
	return 0;
}

/* Saves one movie frame to the output file */
void SaveOneFrame(YUVImage *image, const char *fname, FILE *file)
{
	int x, y;
	for (y = 0; y < image->H; y++) {
		for (x = 0; x < image->W; x++) {
			fputc(GetPixelY(image, x, y), file);
		}
	}

	for (y = 0; y < image->H; y += 2) {
		for (x = 0; x < image->W; x += 2) {
			fputc(GetPixelU(image, x, y), file);
		}
	}

	for (y = 0; y < image->H; y += 2) {
		for (x = 0; x < image->W; x += 2) {
			fputc(GetPixelV(image, x, y), file);
		}
	}
}

/* EOF */
