/************************************************************************
**
** NAME:        gameoflife.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Justin Yokota - Starter Code
**				YOUR NAME HERE
**
**
** DATE:        2020-08-23
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "imageloader.h"

// Determines what color the cell at the given row/col should be. This function allocates space for a new Color.
// Note that you will need to read the eight neighbors of the cell in question. The grid "wraps", so we treat the top row as adjacent to the bottom row
// and the left column as adjacent to the right column.
Color *evaluateOneCell(Image *image, int row, int col, uint32_t rule)
{
	// YOUR CODE HERE
	if (image == NULL)
	{
		fprintf(stderr, "Image is NULL\n");
		return NULL;
	}

	int dx[] = {-1, -1, -1, 0, 0, 1, 1, 1}; // the row (vertical) offsets
	int dy[] = {-1, 0, 1, -1, 1, -1, 0, 1}; // the column (horizontal) offsets
	// now each pair of (dx[i], dy[i]) corresponds to a directon, (-1, -1) top right, etc.
	// count alive of the neighbours of the current cell
	int alive_count = 0;
	for (int i = 0; i < 8; i++)
	{
		int ni = (row + dx[i] + image->rows) % image->rows;
		int nj = (col + dy[i] + image->cols) % image->cols;
		if (image->image[ni][nj].R == 255 && image->image[ni][nj].G == 255 && image->image[ni][nj].B == 255)
		{ // alive
			alive_count++;
		}
	}
	free(dx);
	free(dy);

	// analyze rule:
	uint8_t *buffer = malloc(sizeof(uint8_t) * 8); // store the half part of the 16-bit binary
	if (buffer == NULL)
	{
		fprintf(stderr, "Failed to allocate memory\n");
		return NULL;
	}

	if (image->image[row][col].R == 255 &&
		image->image[row][col].G == 255 &&
		image->image[row][col].B == 255) // current cell is alive
	{
		for (int i = 0; i < 9; i++)
		{
			rule >>= 1;
		}
		for (int i = 0; i < 8; i++)
		{
			buffer[i] = (rule & 1) ? 1 : 0;
			rule >>= 1;
		}
	}
	else // current cell is dead
	{
		for (int i = 0; i < 8; i++)
		{
			buffer[i] = (rule & 1) ? 1 : 0;
			rule >>= 1;
		}
	}

	Color *next_state = malloc(sizeof(Color));
	if (buffer[alive_count] == 1)
	{
		next_state->R = 255;
		next_state->G = 255;
		next_state->B = 255;
	}
	else
	{
		next_state->R = 0;
		next_state->G = 0;
		next_state->B = 0;
	}
	free(buffer);
	return next_state;
}

// The main body of Life; given an image and a rule, computes one iteration of the Game of Life.
// You should be able to copy most of this from steganography.c
Image *life(Image *image, uint32_t rule)
{
	// YOUR CODE HERE
	if (image == NULL)
	{
		fprintf(stderr, "Image is NULL\n");
		return NULL;
	}

	Image *next_image = malloc(sizeof(Image));
	if (next_image == NULL)
	{
		fprintf(stderr, "Failed to allocate memory\n");
		return NULL;
	}

	next_image->cols = image->cols;
	next_image->rows = image->rows;

	next_image->image = malloc(sizeof(Color *) * next_image->rows);
	if (next_image->image == NULL)
	{
		fprintf(stderr, "Failed to allocate memory\n");
		free(next_image);
		return NULL;
	}

	for (int i = 0; i < next_image->rows; i++)
	{
		next_image->image[i] = malloc(sizeof(Color) * next_image->cols);
		if (next_image->image[i] == NULL)
		{
			fprintf(stderr, "Failed to allocate memory");
			for (int j = 0; j < i; j++)
				free(next_image->image[j]);
			free(next_image->image);
			free(next_image);
			return NULL;
		}
	}

	for (int row_num = 0; row_num < next_image->rows; row_num++)
	{
		for (int col_num = 0; col_num < next_image->cols; col_num++)
		{
			Color *next_pixel = evaluateOneCell(image, row_num, col_num, rule);
			if (next_pixel == NULL)
			{
				fprintf(stderr, "Failed to allocate memory");
				for (int j = 0; j < next_image->rows; j++)
					free(next_image->image[j]);
				free(next_image->image);
				free(next_image);
				return NULL;
			}
			next_image->image[row_num][col_num] = *next_pixel;
			free(next_pixel);
		}
	}
	return next_image;
}

/*
Loads a .ppm from a file, computes the next iteration of the game of life, then prints to stdout the new image.

argc stores the number of arguments.
argv stores a list of arguments. Here is the expected input:
argv[0] will store the name of the program (this happens automatically).
argv[1] should contain a filename, containing a .ppm.
argv[2] should contain a hexadecimal number (such as 0x1808). Note that this will be a string.
You may find the function strtol useful for this conversion.
If the input is not correct, a malloc fails, or any other error occurs, you should exit with code -1.
Otherwise, you should return from main with code 0.
Make sure to free all memory before returning!

You may find it useful to copy the code from steganography.c, to start.
*/
int main(int argc, char **argv)
{
	// YOUR CODE HERE
	if (argc != 2)
	{
		printf("usage: ./%s filename rule\n", argv[0]);
		printf("filename is an ASCII PPM file (type P3) with maximum value 255.\n");
		printf("rule is a hex number beginning with 0x; Life is 0x1808.");
		return -1;
	}

	char *image_name = argv[1];
	Image *image = readData(image_name);
	if (image == NULL)
	{
		fprintf(stderr, "Failed to read from file %s\n", image_name);
		return -1;
	}
	free(image_name);

	char *endptr;
	uint32_t rule = strtol(argv[2], &endptr, 16);
	if (*endptr != 0)
	{
		fprintf(stderr, "Failed to convert char* to uint32_t\n");
		return -1;
	}
	free(endptr);
	Image *next_image = life(image, rule);
	if (next_image == NULL)
	{
		fprintf(stderr, "Failed to iterate next image\n");
		return -1;
	}
	writeData(next_image);
	free(next_image);
	return 0;
}
