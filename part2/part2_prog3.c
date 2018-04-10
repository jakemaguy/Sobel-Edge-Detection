// part2_prog3.c - Part 2 Sobel Filter - Average Filter

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "part2_prog3.h"

int main()
{
	// argument supplies string to where the file is located.
    readPGM("../images/cake.pgm");
	
	// MIN/MAX variable Definitons
	float max_horiz_sobel = input_image.matrix[1][1];
	float min_horiz_sobel = max_horiz_sobel;
	float max_vert_sobel = max_horiz_sobel;
	float min_vert_sobel = max_vert_sobel;
	float max_avg_filter = min_vert_sobel;
	float min_avg_filter = max_avg_filter;
	float min_edge_image = min_avg_filter;
	float max_edge_image = min_edge_image;

	// Determines MAX/MIN Values sobel filter, for normalization
	unsigned int i, j, k; // loop variables
	unsigned int height = input_image.height;
	unsigned int width = input_image.width;
	float buffer[9];
	
	// --------------------- PART 1 - Sobel Filter ------------------------------------

	for (i = 1; i < height; i++)
	{
		for (j = 1; j < width; j++)
		{
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

			// Sobel horizontal and vertical response
			double dx = ((buffer[0] * -1) + (buffer[3] * -2) + (buffer[6] * -1) + (buffer[5] * 2) + buffer[2] + buffer[8]);
			double dy = ((buffer[0] * -1) + (buffer[1] * -2) + (buffer[2] * -1) + buffer[6] + (buffer[7] * 2) + buffer[8]);
			double magnitude = sqrt((dx * dx) + (dy * dy));
			
			// Determines Actual Min/Max values for normalization
			if (dx < min_horiz_sobel) min_horiz_sobel = dx;
			if (dx > max_horiz_sobel) max_horiz_sobel = dx;
			if (dy < min_vert_sobel) min_vert_sobel = dy;
			if (dy > max_vert_sobel) max_vert_sobel = dy;
			if (magnitude > max_edge_image) max_edge_image = (int)magnitude;
			if (magnitude < min_edge_image) min_edge_image = (int)magnitude;
		}
	}
	
	// Edit This line to change noise free threshold
	int noisefree_threshold = 100;
	
	// Loops through entire image and calculates the vertical/horizontal
	// Sobel filter approximations, then calculates the combined
	// vertical and horizontal components to create sobel edge
	for (i = 1; i < height + 1; i++)
	{
		for (j = 1; j < width + 1; j++)
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
			input_image.horiz_sobel[i - 1][j - 1] = (unsigned char)(255 * ((int)horiz - min_horiz_sobel) / (max_horiz_sobel - min_horiz_sobel));
			input_image.vert_sobel[i - 1][j - 1] = (unsigned char)(255 * ((int)vert - min_vert_sobel) / (max_vert_sobel - min_vert_sobel));
			
			// Edge image is based on magnitude of horizontal and vertical Sobel Filters
			input_image.edge[i - 1][j - 1] = (unsigned char)(255 * ((int)magnitude - min_edge_image) / (max_edge_image - min_edge_image));
			
			// Basis for Binary Image
			if (magnitude >= noisefree_threshold) input_image.sobel[i][j] = 255;
			else input_image.sobel[i][j] = 0;
		}
	}
	
	// --------------------- PART 2 - Write Noise-Free Sobel Images ------------------------------------

	writePGM("../images/horiz_sobel_part2.pgm");
	printf("Horizontal Sobel Has Been Created\n");

	writePGM("../images/vert_sobel_part2.pgm");
	printf("Vertical Sobel Has Been Created\n");

	writePGM("../images/edge_image.pgm");
	printf("Edge Image Has Been Created\n");

	writePGM("../images/sobel_part2.pgm");
	printf("Noise Free Binary Sobel Has Been Created\n");

	// --------------------- PART 3 - Apply Noise-Free Averaging Filter ------------------------------------
	
	// Loops through original image array and performs average convoltution
	// on the 3x3 neighborhood mask.
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

			for (k = 0; k < 9; k++)
			{
				sum += buffer[k];
			}

			sum /= 9;
			
			// Determines actual min/max for normalization
			if (sum < min_avg_filter) min_avg_filter = sum;
			if (sum > max_avg_filter) max_avg_filter = sum;
		}
	}

	// Loops through entire image and calculates the normalized
	// averaged filtered response based of min/max values
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
	
			// loops through all values of 3x3 neigborhood mask
			// adds up sums of all buffer elements
			for (k = 0; k < 9; k++)
			{
				sum += buffer[k];
			}

			sum /= 9;
			input_image.average_sobel[i - 1][j - 1] = (unsigned char)(255 * ((int)sum - min_avg_filter) / (max_avg_filter - min_avg_filter));
		}
	}
	
	// --------------------- PART 4 - Write Noise Free Average Image ------------------------------------

	writePGM("../images/averaged_sobel_part2.pgm");
	printf("Averaged Sobel Has Been Created\n");
	
	// --------------------- PART 5 - Apply Random Noise to Original Image ------------------------------------
	
	// Edit this line to desired noise factor amount
	short noise_factor = 20;
	
	
	// Adds random noise to the input image pixels
	for (i = 1; i < height; i++)
	{
		for (j = 1; j < width; j++)
		{
			// generates number between -1 and 1
			float randnum = (2 * (float)rand() / (float)RAND_MAX) - 1;
			float current = input_image.matrix[i][j];
			// uses FABS to take absolute of floating point expression
			// part of math.h library
			float noise_level = fabs(current + (randnum*noise_factor));
			input_image.matrix[i][j] = noise_level;
		}
	}
	
	// --------------------- PART 6 - Noisy Sobel Filter ------------------------------------

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
			if (magnitude > max_edge_image) max_edge_image = (int)magnitude;
			if (magnitude < min_edge_image) min_edge_image = (int)magnitude;
		}
	}

	// Edit This Value to adjust noisy threshold
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
			input_image.horiz_sobel_noisy[i-1][j-1] = (unsigned char)(255 * ((int)horiz - min_horiz_sobel) / (max_horiz_sobel - min_horiz_sobel));
			input_image.vert_sobel_noisy[i - 1][j - 1] = (unsigned char)(255 * ((int)vert - min_vert_sobel) / (max_vert_sobel - min_vert_sobel));
			
			// Edge image is based on magnitude of horizontal and vertical Sobel Filters
			input_image.noisy_edge[i - 1][j - 1] = (unsigned char)(255 * ((int)magnitude - min_edge_image) / (max_edge_image - min_edge_image));
			
			// Basis of binary image
			if (magnitude >= threshold) input_image.noisy_sobel[i][j] = 255;
			else input_image.noisy_sobel[i][j] = 0;
		}
	}
	
	// --------------------- PART 7 - Write Noisy Sobel Images ------------------------------------

	writePGM("../images/horiz_sobel_noisy.pgm");
	printf("Noisy Horizontal Sobel Has Been Created\n");

	writePGM("../images/vert_sobel_noisy.pgm");
	printf("Noisy Vertical Sobel Has Been Created\n");

	writePGM("../images/noisy_edge_image.pgm");
	printf("Noisy Edge Image Has Been Created\n");

	writePGM("../images/sobel_noisy.pgm");
	printf("Noisy Sobel Has Been Created\n");

	// --------------------- PART 8 - Noisy Average Filter ------------------------------------
	
	// Loops through noisy image array and performs average convoltution
	// on the 3x3 neighborhood mask.
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

			for (k = 0; k < 9; k++) 
			{
				sum += buffer[k];
			}
			
			sum /= 9;
			if (sum < min_avg_filter) min_avg_filter = sum;
			if (sum > max_avg_filter) max_avg_filter = sum;
		}
	}
	
	// Loops through entire image and calculates the normalized
	// averaged filtered response based of min/max values
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

			for (k = 0; k < 9; k++)
			{
				sum += buffer[k];
			}

			sum /= 9;
			input_image.average_sobel_noisy[i-1][j-1] = (unsigned char)(255 * ((int)sum - min_avg_filter) / (max_avg_filter - min_avg_filter));
		}
	}
	
	// --------------------- PART 9 - Write Noisy Average Image ------------------------------------

	writePGM("../images/averaged_sobel_noisy_part2.pgm");
	printf("Averaged Noisy Sobel Has Been Created\n");
    
	// --------------------- PART 10 - Generate Accuracy Report ------------------------------------
	
	printf("Now Generating Accuracy CSV file in root folder\n");
	accuracy_CSV();

	

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
			// Does String Comparisons to deterine which unsigned char array to print from
			// Definatley could have been done a more elegant/effcient way, sorry 
			if (strcmp(filename, "../images/vert_sobel_part2.pgm") == 0)
			{
				ch = input_image.vert_sobel[i][j];
				fputc(ch, out_file);
			}

			else if (strcmp(filename, "../images/sobel_part2.pgm") == 0)
			{
				ch = input_image.sobel[i][j];
				fputc(ch, out_file);
			}
			else if (strcmp(filename, "../images/averaged_sobel_part2.pgm") == 0)
			{
				ch = input_image.average_sobel[i][j];
				fputc(ch, out_file);
			}

			else if (strcmp(filename, "../images/horiz_sobel_noisy.pgm") == 0)
			{
				ch = input_image.horiz_sobel_noisy[i][j];
				fputc(ch, out_file);
			}
			else if (strcmp(filename, "../images/vert_sobel_noisy.pgm") == 0)
			{
				ch = input_image.vert_sobel_noisy[i][j];
				fputc(ch, out_file);
			}
			else if (strcmp(filename, "../images/sobel_noisy.pgm") == 0)
			{
				ch = input_image.noisy_sobel[i][j];
				fputc(ch, out_file);
			}
			else if (strcmp(filename, "../images/averaged_sobel_noisy_part2.pgm") == 0)
			{
				ch = input_image.average_sobel_noisy[i][j];
				fputc(ch, out_file);
			}
			else if (strcmp(filename, "../images/edge_image.pgm") == 0)
			{
				ch = input_image.edge[i][j];
				fputc(ch, out_file);
			}
			else if (strcmp(filename, "../images/noisy_edge_image.pgm") == 0)
			{
				ch = input_image.noisy_edge[i][j];
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

// loops over the noisy and noise-free horizontal and vertical sobel
// arrays and compares the differences betweem the two.  Determines
// as percentages the accuracy of the noisy filter from threshold values
// 0 all the way to 255
void accuracy_CSV()
{
	int height = input_image.height;
	int width = input_image.width;
	int i, j;
	int threshold = 0;
	FILE *fp;
	
	fp = fopen("../accuracy.csv", "w+");
	if (fp == NULL)
	{
		printf("Please close the excel file and try again\n");
		exit(0);
	}

	fprintf(fp, "Threshold Value,Accuracy,Incorrect Pixels\n");

	while (threshold < 256)
	{
		int total_pixels = 0;
		int total_matches = 0;
		int total_missed = 0;
		float accuracy = 0;
		float missed = 0;
		for (i = 0; i < height; i++)
		{
			for (j = 0; j < width; j++)
			{
				unsigned char dx = input_image.horiz_sobel[i][j];
				unsigned char dy = input_image.vert_sobel[i][j];
				double magnitude = sqrt((dx * dx) + (dy * dy));

				unsigned char dx_noisy = input_image.horiz_sobel_noisy[i][j];
				unsigned char dy_noisy = input_image.vert_sobel_noisy[i][j];
				double magnitude_noisy = sqrt((dx_noisy * dx_noisy) + (dy_noisy * dy_noisy));

				// Case 1 = Noise-free sobel does not match noisy sobel pixels
				// Total number of pixels goes up as well as the the total missed pixels
				if (magnitude >= threshold && magnitude_noisy < threshold)
				{
					total_pixels++;
					total_missed++;
				}
				
				// Case 2 = Noise free and noisy sobel pixels pass threshold in the same spot
				if (magnitude >= threshold && magnitude_noisy >= threshold)
				{
					total_pixels++;
					total_matches++;
				}
				
				// Case 3 = Noisy sobel pixel appears but the noise free sobel does not have a matching
				// pixel.  In this case, the noisy sobel pixel is an incorrect pixel
				if (magnitude_noisy >= threshold && magnitude < threshold)
				{
					total_missed++;
				}
			}
		}
		accuracy = ((float)total_matches /(float)total_pixels) * 100.0;
		missed = ((float)total_missed / (float)(total_pixels + total_missed)) * 100.0;
		fprintf(fp, "%d,%f,%f\n", threshold, accuracy, missed);
		// inrements threshold and returns to beginning of while loop
		threshold++;
	}

	fclose(fp);
}
