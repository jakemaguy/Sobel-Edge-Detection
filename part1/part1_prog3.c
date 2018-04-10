// part1_prog3.c - Part 1 Sobel Filter

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "part1_prog3.h"

int main()
{
	// argument supplies string to where the file is located.
    readPGM("../images/mri.pgm");
	
	// MIN/MAX variable Definitons
	float max_horiz_sobel = (float)input_image.matrix[1][1];
	float min_horiz_sobel = max_horiz_sobel;
	float max_vert_sobel = max_horiz_sobel;
	float min_vert_sobel = max_vert_sobel;
	float max_mag = min_vert_sobel;
	float min_mag = max_mag;
	
	// --------------------- PART 1 - Sobel Filter ------------------------------------
	
	// Determines MAX/MIN Values sobel filter, for normalization
	int i, j; // loop variables
	unsigned int height = input_image.height;
	unsigned int width = input_image.width;
	unsigned char buffer[9];

	// Loops through entire image and determines min/max values 
	// so the normalization function can convert values (0-255) unsigned char
	for (i = 1; i < height; i++)
	{
		for (j = 1; j < width; j++)
		{
			// gets 3x3 pixels for filter
			// see header file for more details on this operation
			buffer[0] = input_image.matrix[i-1][j-1];
			buffer[1] = input_image.matrix[i-1][j];
			buffer[2] = input_image.matrix[i-1][j+1];
			buffer[3] = input_image.matrix[i][j-1];
			buffer[4] = input_image.matrix[i][j];
			buffer[5] = input_image.matrix[i][j+1];
			buffer[6] = input_image.matrix[i+1][j-1];
			buffer[7] = input_image.matrix[i + 1][j];
			buffer[8] = input_image.matrix[i + 1][j + 1];

			// Sobel horizontal and vertical response
			double dx = ((buffer[0] * -1) + (buffer[3] * -2) + (buffer[6] * -1) + (buffer[5] * 2) + buffer[2] +  buffer[8]);
			double dy = ((buffer[0] * -1) + (buffer[1] * -2) + (buffer[2] * -1) + buffer[6] + (buffer[7] * 2) + buffer[8]);
			double magnitude = sqrt((dx * dx) + (dy * dy));
			
			// Determines Actual Min/Max values for normalization
			if (dx < min_horiz_sobel) min_horiz_sobel = dx;
			if (dx > max_horiz_sobel) max_horiz_sobel = dx;
			if (dy < min_vert_sobel) min_vert_sobel = dy;
			if (dy > max_vert_sobel) max_vert_sobel = dy;
			if (magnitude < min_mag) min_mag = magnitude;
			if (magnitude > max_mag) max_mag = magnitude;
		}
	}
	
	// Edit this variable to change threshold on binary image
	int threshold = 100;
	
	// Loops through entire image and calculates the vertical/horizontal
	// Sobel filter approximations, then calculates the combined
	// vertical and horizontal components to create sobel edge
	for (i = 1; i < height+1; i++)
	{
		for (j = 1; j < width+1; j++)
		{
			// gets 3x3 pixels for filter
			buffer[0] = input_image.matrix[i - 1][j - 1];
			buffer[1] = input_image.matrix[i - 1][j];
			buffer[2] = input_image.matrix[i - 1][j + 1];
			buffer[3] = input_image.matrix[i][j - 1];
			buffer[4] = input_image.matrix[i][j];
			buffer[5] = input_image.matrix[i][j + 1];
			buffer[6] = input_image.matrix[i + 1][j - 1];
			buffer[7] = input_image.matrix[i + 1][j];
			buffer[8] = input_image.matrix[i + 1][j + 1];

			double dx = ((buffer[0] * -1) + (buffer[3] * -2) + (buffer[6] * -1) + (buffer[5] * 2) + buffer[2] + buffer[8]);
			double dy = ((buffer[0] * -1) + (buffer[1] * -2) + (buffer[2] * -1) + buffer[6] + (buffer[7] * 2) + buffer[8]);
			double magnitude = sqrt((dx * dx) + (dy * dy));
			unsigned char horiz = (unsigned char)dx;
			unsigned char vert = (unsigned char)dy;
			input_image.horiz_sobel[i-1][j-1] = (unsigned char)255 * (dx - min_horiz_sobel) / (max_horiz_sobel - min_horiz_sobel);
			input_image.vert_sobel[i - 1][j - 1] = (unsigned char)255 * (dy - min_vert_sobel) / (max_vert_sobel - min_vert_sobel);
			// Basis for Binary Image
			if (magnitude >= threshold) input_image.sobel[i][j] = 255;
			else input_image.sobel[i][j] = 0;
		}
	}

	// --------------------- PART 2 - Write Noise-Free Sobel Images ------------------------------------
	
	writePGM("../images/horiz_sobel.pgm");
	printf("Horizontal Sobel Has Been Created\n");

	writePGM("../images/vert_sobel.pgm");
	printf("Vertical Sobel Has Been Created\n");

	writePGM("../images/sobel.pgm");
	printf("Sobel Has Been Created\n");
	
    return 0;
}

// READPGM()
// input arg: const char array that points to name of file to be read
void readPGM(const char* filename)
{
	int ch;
	char version[3];
	FILE *input_file;
	input_file = fopen(filename, "rb");
	// Error Checking
	if (input_file == NULL) {
		printf("Error opening file\n");
		exit(1);
	}
	fgets(version, sizeof(version), input_file);
	if (strcmp(version, "P5")) {								// program is compatable with P5 (compressed B/W Image)
		fprintf(stderr, "Wrong file type!\n");
		exit(1);
	}

	while (getc(input_file) == '#')								// Skips the comment line
	{
		while (getc(input_file) != '\n');						// Skips to end of the comment line
	}

	// Reads in the cols and rows data for the PGM file
	fseek(input_file, -1, SEEK_CUR);							//  Backs up one character
	fscanf(input_file, "%d", &input_image.width);
	fscanf(input_file, "%d", &input_image.height);			
	fscanf(input_file, "%d", &input_image.max_gray);
	fgetc(input_file);

	printf("\n width  = %d", input_image.width);
	printf("\n height = %d", input_image.height);
	printf("\n maxVal = %d", input_image.max_gray);
	printf("\n");

	unsigned int i, j;
	
	for (i = 0; i < input_image.height+2; i++)
	{
		for (j = 0; j < input_image.width+2; j++)
		{
			// ADDS 0 padding on the top and bottom edge of picture
			if (i == 0 || i == input_image.height+1)
				input_image.matrix[i][j] = 0;
			// ADDS 0 padding on the left and right edges of picture
			if (j == 0 || j == input_image.width+1)
				input_image.matrix[i][j] = 0;
			else{
				ch = fgetc(input_file);
				input_image.matrix[i][j] = ch;
			}
		}
	}
	fclose(input_file);
}


// WRITEPGM()
// input arg: const char array that points to name of file to be written
void writePGM(const char* filename) {
	char ch;
	FILE *out_file;
	out_file = fopen(filename, "wb");
	// Error checking
	if (out_file == NULL) {
		perror("cannot open file to write");
		exit(1);
	}

	//  Prints out the PGM magic number and other PGM corresponding information to the written pgmfile
	fprintf(out_file, "P5 ");
	fprintf(out_file, "%d %d ", input_image.width, input_image.height);
	fprintf(out_file, "%d\n", input_image.max_gray);
	
	unsigned int i, j;
	//	prints out full picture
	for (i = 0; i < input_image.height; i++)
	{
		for (j = 0; j < input_image.width; j++)
		{
			// Does String Comparisons to deterine which unsigned char array to print from
			// Definatley could have been done a more elegant/effcient way, sorry 
			if (strcmp(filename, "../images/vert_sobel.pgm") == 0)
			{
				ch = input_image.vert_sobel[i][j];
				fputc(ch, out_file);
			}

			else if (strcmp(filename, "../images/sobel.pgm") == 0)
			{
				ch = input_image.sobel[i][j];
				fputc(ch, out_file);
			}
			else
			{
				ch = input_image.horiz_sobel[i][j];
				fputc(ch, out_file);
			}
		}
	}
	fclose(out_file);
}
