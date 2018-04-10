#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "part2_prog3.h"

int main()
{
	// argument supplies string to where the file is located.
    readPGM("../images/cake.pgm");
	
	float max_horiz_sobel = (float)input_image.matrix[1][1];
	float min_horiz_sobel = max_horiz_sobel;
	float max_vert_sobel = max_horiz_sobel;
	float min_vert_sobel = max_vert_sobel;
	float max_avg_filter = min_vert_sobel;
	float min_avg_filter = max_avg_filter;

	// Determines MAX/MIN Values sobel filter, for normalization
	unsigned int i, j, k; // loop variables
	unsigned int height = input_image.height;
	unsigned int width = input_image.width;
	unsigned char buffer[9];

	// Adds random noise either (-1, 0, or 1) to the input image pixels
	for (i = 1; i < height; i++)
	{
		for (j = 1; j < width; j++)
		{
			short randnum = rand() % 3 - 1;
			// prevents elements in the picture that are aleady 0
			// from being corrupted.
			if (input_image.matrix[i][j] == 0 && randnum == -1)
				input_image.matrix[i][j] = 0;
			// prevents elements in the picture that are aleady 255
			// from being corrupted.
			else if (input_image.matrix[i][j] == 255 && randnum == 1)
				input_image.matrix[i][j] = 255;
			else
				input_image.matrix[i][j] += randnum;
		}
	}

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
			if (dx < min_horiz_sobel) min_horiz_sobel = dx;
			if (dx > max_horiz_sobel) max_horiz_sobel = dx;
			if (dy < min_vert_sobel) min_vert_sobel = dy;
			if (dy > max_vert_sobel) max_vert_sobel = dy;
		}
	}

	int threshold = 100;
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
			input_image.horiz_sobel[i-1][j-1] = (unsigned char)255 * (buffer[4] - min_horiz_sobel) / (max_horiz_sobel - min_horiz_sobel);
			input_image.vert_sobel[i - 1][j - 1] = (unsigned char)255 * (buffer[4] - min_vert_sobel) / (max_vert_sobel - min_vert_sobel);
			if (magnitude >= threshold) input_image.sobel[i][j] = 255;
			else input_image.sobel[i][j] = 0;
		}
	}

	writePGM("../images/horiz_sobel.pgm");
	printf("Horizontal Sobel Has Been Created\n");

	writePGM("../images/vert_sobel.pgm");
	printf("Vertical Sobel Has Been Created\n");

	writePGM("../images/sobel.pgm");
	printf("Sobel Has Been Created\n");

	// Apply Averaging Filter
	int avg_threshold = 100;
	for (i = 1; i < height; i++)
	{
		for (j = 1; j < width; j++)
		{
			double sum = 0;
			// gets 3x3 pixels for filter
			// see header file for more details on this operation
			buffer[0] = input_image.matrix[i - 1][j - 1];
			buffer[1] = input_image.matrix[i - 1][j];
			buffer[2] = input_image.matrix[i - 1][j + 1];
			buffer[3] = input_image.matrix[i][j - 1];
			buffer[4] = input_image.matrix[i][j];
			buffer[5] = input_image.matrix[i][j + 1];
			buffer[6] = input_image.matrix[i + 1][j - 1];
			buffer[7] = input_image.matrix[i + 1][j];
			buffer[8] = input_image.matrix[i + 1][j + 1];

			for (k = 0; k < sizeof(buffer); k++) 
			{
				sum += buffer[k];
			}
			
			sum /= 9;
			if (sum < min_avg_filter) min_avg_filter = sum;
			if (sum > max_avg_filter) max_avg_filter = sum;
		}
	}

	for (i = 1; i < height + 1; i++)
	{
		for (j = 1; j < width + 1; j++)
		{
			double sum = 0;
			// gets 3x3 pixels for filter
			// see header file for more details on this operation
			buffer[0] = input_image.matrix[i - 1][j - 1];
			buffer[1] = input_image.matrix[i - 1][j];
			buffer[2] = input_image.matrix[i - 1][j + 1];
			buffer[3] = input_image.matrix[i][j - 1];
			buffer[4] = input_image.matrix[i][j];
			buffer[5] = input_image.matrix[i][j + 1];
			buffer[6] = input_image.matrix[i + 1][j - 1];
			buffer[7] = input_image.matrix[i + 1][j];
			buffer[8] = input_image.matrix[i + 1][j + 1];

			for (k = 0; k < sizeof(buffer); k++)
			{
				sum += buffer[k];
			}

			sum /= 9;
			input_image.average_sobel[i-1][j-1] = (unsigned char)255 * ((int)sum - min_avg_filter) / (max_avg_filter - min_avg_filter);
		}
	}

	writePGM("../images/averaged_sobel.pgm");
	printf("Averaged Sobel Has Been Created\n");
    
	return 0;
}

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
			else if (strcmp(filename, "../images/averaged_sobel.pgm") == 0)
			{
				ch = input_image.average_sobel[i][j];
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