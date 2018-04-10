#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 800
 
struct PGMImage{
	unsigned int  width, height, max_gray;
	// Holds Pixel Data for original Image
	unsigned char matrix[MAX][MAX];
	unsigned char horiz_sobel[MAX][MAX];
	unsigned char vert_sobel[MAX][MAX];
	unsigned char sobel[MAX][MAX];
} input_image;                         // struct object to access struct members

//----------------- FUNCTION PROTOTYPES--------------------------------------------

// reads in filename pointed to by argument
// updates matrix array
void readPGM(const char* filename);

// writes the PGM files pointed to by argument
void writePGM(const char* filename);

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
