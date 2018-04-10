// part2_prog3.h - Part 2 Sobel Filter - Average Filter

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 800
 
struct PGMImage{
	unsigned int  width, height, max_gray;
	// Holds Pixel Data for original Image
	// original matrix had to be a float to store 
	// random numbers that had decimal places and 
	// values above 255 if necessary
	float matrix[MAX][MAX];
	unsigned char horiz_sobel[MAX][MAX];
	unsigned char vert_sobel[MAX][MAX];
	unsigned char sobel[MAX][MAX];
	unsigned char edge[MAX][MAX];
	unsigned char horiz_sobel_noisy[MAX][MAX];
	unsigned char vert_sobel_noisy[MAX][MAX];
	unsigned char noisy_sobel[MAX][MAX];
	unsigned char noisy_edge[MAX][MAX];
	unsigned char average_sobel[MAX][MAX];
	unsigned char average_sobel_noisy[MAX][MAX];
} input_image;                         // struct object to access struct members

//----------------- FUNCTION PROTOTYPES--------------------------------------------

// reads in filename pointed to by argument
// updates matrix array
void readPGM(const char* filename);

// writes the PGM files pointed to by argument
void writePGM(const char* filename);

// loops over the noisy and noise-free horizontal and vertical sobel
// arrays and compares the differences betweem the two.  Determines
// as percentages the accuracy of the noisy filter from threshold values
// 0 all the way to 255
void accuracy_CSV();

/*
	3x3 f(x,y) diagram for convolution

	-------------------------------------
	| buffer[0] | buffer[1] | buffer[2] |
	-------------------------------------
	| buffer[3] | buffer[4] | buffer[5] |
	-------------------------------------
	| buffer[6] | buffer[7] | buffer[8] |
	-------------------------------------

*/
