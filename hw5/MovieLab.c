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

int main(int argc, char *argv[]) {
    int x = 0;
    char *fin = NULL;
    char *fout = NULL;

    Movie *movie = NULL;

    unsigned int frameNum = 0;
    unsigned int width = 512;
    unsigned int height = 288;

    /* Parse command-line arguments */
    while (x < argc) {
        /* the input file name */
        if (strcmp(argv[x], "-i") == 0) {
            if (x < argc - 1) {
                fin = argv[x + 1];
            } else {
                printf("Missing argument for the input file name!\n");
                return 5;
            }
            x += 2;
            continue;
        }

        /* the output file name */
        if (strcmp(argv[x], "-o") == 0) {
            if (x < argc - 1) {
                fout = argv[x + 1];
            } else {
                printf("Missing argument for the output file name!\n");
                return 5;
            }
            x += 2;
            continue;
        }

        /* Number of frames */
        if (strcmp(argv[x], "-f") == 0) {
            if (x < argc - 1) {
                if (sscanf(argv[x + 1], "%u", &frameNum) != 1) {
                    printf("Invalid argument for the number of frames!\n");
                    return 5;
                }
            } else {
                printf("Missing argument for the number of frames to load!\n");
                return 5;
            }
            x += 2;
            continue;
        }

        /* the image dimensions option */
        if (strcmp(argv[x], "-s") == 0) {
            if (x < argc - 1) {
                if (sscanf(argv[x + 1], "%ux%u", &width, &height) != 2) {
                    printf("Invalid argument for image dimensions!\n");
                    return 5;
                }
            } else {
                printf("Missing argument for image dimensions!\n");
                return 5;
            }
            x += 2;
            continue;
        }

        /* the help option */
        if (strcmp(argv[x], "-h") == 0) {
            PrintUsage();
            return 0;
        }

        /* Black and White */
        if (strcmp(argv[x], "-bw") == 0) {
            if (!movie) {
                movie = LoadMovie(fin, frameNum, width, height);
            }
            YUV2RGBMovie(movie);
            IENTRY *curr = movie->Frames->First;
            while (curr) {
                BlackNWhite((Image *)curr->Image);
                curr = curr->Next;
            }
            RGB2YUVMovie(movie);
            printf("\nOperation Black and White is done!\n");
            x++;
            continue;
        }

        /* Horizontal Mirror */
        if (strcmp(argv[x], "-hmirror") == 0) {
            if (!movie) {
                movie = LoadMovie(fin, frameNum, width, height);
            }
            YUV2RGBMovie(movie);
            IENTRY *curr = movie->Frames->First;
            while (curr) {
                HMirror((Image *)curr->Image);
                curr = curr->Next;
            }
            RGB2YUVMovie(movie);
            printf("\nOperation HMirror is done!\n");
            x++;
            continue;
        }

        /* Edge Detection */
        if (strcmp(argv[x], "-edge") == 0) {
            if (!movie) {
                movie = LoadMovie(fin, frameNum, width, height);
            }
            YUV2RGBMovie(movie);
            IENTRY *curr = movie->Frames->First;
            while (curr) {
                Edge((Image *)curr->Image);
                curr = curr->Next;
            }
            RGB2YUVMovie(movie);
            printf("\nOperation Edge is done!\n");
            x++;
            continue;
        }

        /* Crop */
        if (strcmp(argv[x], "-crop") == 0) {
            if (!movie) {
                movie = LoadMovie(fin, frameNum, width, height);
            }
            if (x + 1 >= argc) {
                printf("Missing argument for -crop!\n");
                return 5;
            }
            unsigned int start, end;
            if (sscanf(argv[x + 1], "%u-%u", &start, &end) != 2) {
                printf("Invalid argument for -crop!\n");
                return 5;
            }
            CropImageList(movie->Frames, start, end);
            frameNum = end - start + 1; // Update frame count
            printf("Operation Crop is done! New number of frames is %u.\n", frameNum);
            x += 2;
            continue;
        }

        /* Fast Forward */
        if (strcmp(argv[x], "-fast") == 0) {
            if (!movie) {
                movie = LoadMovie(fin, frameNum, width, height);
            }
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
            printf("Operation Fast Forward is done! New number of frames is %u.\n", frameNum);
            x += 2;
            continue;
        }

        /* Reverse */
        if (strcmp(argv[x], "-reverse") == 0) {
            if (!movie) {
                movie = LoadMovie(fin, frameNum, width, height);
            }
            ReverseImageList(movie->Frames);
            printf("Operation Reverse is done!\n");
            x++;
            continue;
        }

        /* Watermark */
        if (strcmp(argv[x], "-watermark") == 0) {
            char watermark_file[SLEN];
            if (x + 1 >= argc) {
                printf("Missing argument for Watermark!\n");
                return 5;
            }
            if (sscanf(argv[x + 1], "%s", watermark_file) != 1) {
                printf("Invalid argument for Watermark!\n");
                return 5;
            }

            // Load the watermark image
            Image *watermark_image = LoadImage(watermark_file);
            if (!watermark_image) {
                printf("Failed to load watermark image: %s\n", watermark_file);
                return 5;
            }

            // Load the movie if not already loaded
            if (!movie) {
                movie = LoadMovie(fin, frameNum, width, height);
                if (!movie) {
                    printf("Failed to load movie!\n");
                    DeleteImage(watermark_image); // Free the watermark image
                    return 5;
                }
                YUV2RGBMovie(movie); // Convert to RGB for watermarking
            }

            // Seed the random number generator
            srand(time(NULL));

            // Apply watermark to each frame
            IENTRY *curr = movie->Frames->First;
            int frameCounter = 0;
            unsigned int currentTopLeftX = 0;
            unsigned int currentTopLeftY = 0;

            while (curr) {
                if (frameCounter % 15 == 0) {
                    // Update watermark position every 15 frames (WILL BE UPDATED WHEN 0 % 15 == 0)
                    currentTopLeftX = rand() % (width - watermark_image->W + 1);
                    currentTopLeftY = rand() % (height - watermark_image->H + 1);
                }

                // Apply watermark to the current frame
                Image *watermarkedFrame = Watermark((Image *)curr->Image, watermark_image, currentTopLeftX, currentTopLeftY);
                if (!watermarkedFrame) {
                    printf("Failed to apply watermark to frame %d\n", frameCounter);
                    DeleteImage(watermark_image);
                    DeleteMovie(movie);
                    return 5;
                }

                // Replace the frame with the watermarked frame
                DeleteImage((Image *)curr->Image);
                curr->Image = (void *)watermarkedFrame;

                curr = curr->Next;
                frameCounter++;
            }

            // Convert back to YUV
            RGB2YUVMovie(movie);

            // Free the watermark image
            DeleteImage(watermark_image);

            printf("Operation Watermark is done!\n");
            x += 2; // Skip "-watermark" and its argument
            continue;
        }

		if (strcmp(argv[x], "-spotlight") == 0) {
			if (!movie) {
				movie = LoadMovie(fin, frameNum, width, height);
				YUV2RGBMovie(movie);
			}

			// Error check: Spotlight not allowed for 40-frame movies
			if (frameNum == 40) { // <-- Use frameNum, NOT movie->NumFrames
				fprintf(stderr, "Error: Spotlight not supported for 40-frame movies.\n");
				x++;
				continue;
			}

			// Calculate max radius (distance from center to corner)
			unsigned int radius = (unsigned int)sqrt(pow(width / 2, 2) + pow(height / 2, 2));
			unsigned int step_size = radius / 19; // 20 steps (0 to max_radius)

			// Save original frame count before adding copies
			unsigned int originalFrameNum = frameNum;

			// Copy first frame 20 times (prepend for fade-in)
			IENTRY *firstFrame = movie->Frames->First;
			for (int i = 0; i < 20; i++) {
				Image *copy = CopyRGBImage((Image *)firstFrame->Image);
				PrependRGBImage(movie->Frames, copy);
			}

			// Copy last frame 20 times (append for fade-out)
			IENTRY *lastFrame = movie->Frames->Last;
			for (int i = 0; i < 20; i++) {
				Image *copy = CopyRGBImage((Image *)lastFrame->Image);
				AppendRGBImage(movie->Frames, copy);
			}

			// Update total frame count
			frameNum += 40; // 20 (fade-in) + 20 (fade-out)

			// Process fade-in (first 20 copied frames)
			IENTRY *curr = movie->Frames->First;
			for (int i = 0; i < 20; i++) {
				Spotlight((Image *)curr->Image, width / 2, height / 2, i * step_size);
				curr = curr->Next;
			}

			// Skip original frames (no spotlight applied)
			for (int i = 0; i < originalFrameNum; i++) { // <-- Use originalFrameNum, NOT movie->NumFrames
				curr = curr->Next;
			}

			// Process fade-out (last 20 copied frames)
			for (int i = 19; i >= 0; i--) {
				Spotlight((Image *)curr->Image, width / 2, height / 2, i * step_size);
				curr = curr->Next;
			}

			RGB2YUVMovie(movie);
			printf("Operation Spotlight is done! Total frames: %u\n", frameNum);
			x++;
			continue;
		}

		if (strcmp(argv[x], "-rotate") == 0) {
			char *direction = "clockwise"; // Default direction
			if (x + 1 < argc && (strcmp(argv[x + 1], "clockwise") == 0 || strcmp(argv[x + 1], "ccw") == 0)) {
				direction = argv[x + 1];
				x++; // Skip direction argument
			}

			if (!movie) {
				movie = LoadMovie(fin, frameNum, width, height);
				YUV2RGBMovie(movie);
			}

			// Error check for 40 frames
			if (frameNum == 40) {
				fprintf(stderr, "Error: Rotate not supported for 40-frame movies.\n");
				x++;
				continue;
			}

			// Copy first frame 20 times (prepend)
			IENTRY *firstFrame = movie->Frames->First;
			for (int i = 0; i < 20; i++) {
				Image *copy = CopyRGBImage((Image *)firstFrame->Image);
				PrependRGBImage(movie->Frames, copy);
			}

			// Copy last frame 20 times (append)
			IENTRY *lastFrame = movie->Frames->Last;
			for (int i = 0; i < 20; i++) {
				Image *copy = CopyRGBImage((Image *)lastFrame->Image);
				AppendRGBImage(movie->Frames, copy);
			}

			// Update total frame count
			frameNum += 40;

			// Process first 20 frames (fade-in)
			IENTRY *curr = movie->Frames->First;
			for (int i = 0; i < 20; i++) {
				double angle = (strcmp(direction, "clockwise") == 0) ? (20.0 - i) : -(20.0 - i);
				double scale = 0.2 + 0.04 * i; // 20% to 96%
				Image *rotated = Rotate((Image *)curr->Image, angle, scale);
				DeleteImage((Image *)curr->Image);
				curr->Image = rotated;
				curr = curr->Next;
			}

			// Skip original frames
			for (int i = 0; i < frameNum - 40; i++) {
				curr = curr->Next;
			}

			// Process last 20 frames (fade-out)
			for (int i = 0; i < 20; i++) {
				double angle = (strcmp(direction, "clockwise") == 0) ? (1.0 + i) : -(1.0 + i);
				double scale = 0.96 - 0.04 * i; // 96% to 20%
				Image *rotated = Rotate((Image *)curr->Image, angle, scale);
				DeleteImage((Image *)curr->Image);
				curr->Image = rotated;
				curr = curr->Next;
			}

			RGB2YUVMovie(movie);
			printf("Operation Rotate (%s) is done! Total frames: %u\n", direction, frameNum);
			x++;
			continue;
		}

        x++;
    }

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
