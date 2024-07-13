/************************************************************************
**
** NAME:        steganography.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Dan Garcia  -  University of California at Berkeley
**              Copyright (C) Dan Garcia, 2020. All rights reserved.
**				Justin Yokota - Starter Code
**				XueJie1
**
** DATE:        2020-08-23
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "imageloader.h"

//Determines what color the cell at the given row/col should be. This should not affect Image, and should allocate space for a new Color.
Color *evaluateOnePixel(Image *image, int row, int col)
{
	//YOUR CODE HERE
	Color *one_pixel = malloc(sizeof(Color));
	uint8_t B = image->image[row][col].B;
	uint8_t least_bit = B & 1;
	// if the least bit is 1, color is white, else is black
	if(least_bit) {
		one_pixel->R = 255;
		one_pixel->G = 255;
		one_pixel->B = 255;
	} else {
		one_pixel->R = 0;
		one_pixel->G = 0;
		one_pixel->B = 0;
	}

	return one_pixel;
}

// Given an image, creates a new image extracting the LSB of the B channel.
Image *steganography(Image *image)
{
    if (image == NULL) {
        return NULL;
    }

    Image *decrypted = malloc(sizeof(Image));
    if (decrypted == NULL) {
        return NULL;
    }

    decrypted->rows = image->rows;
    decrypted->cols = image->cols;

    decrypted->image = malloc(sizeof(Color *) * decrypted->rows);
    if (decrypted->image == NULL) {
        free(decrypted);
        return NULL;
    }

    for (int i = 0; i < decrypted->rows; i++) {
        decrypted->image[i] = malloc(sizeof(Color) * decrypted->cols);
        if (decrypted->image[i] == NULL) {
            // Free previously allocated rows
            for (int j = 0; j < i; j++) {
                free(decrypted->image[j]);
            }
            free(decrypted->image);
            free(decrypted);
            return NULL;
        }
    }

    for (int row_count = 0; row_count < decrypted->rows; row_count++) {
        for (int col_count = 0; col_count < decrypted->cols; col_count++) {
            Color *pixel = evaluateOnePixel(image, row_count, col_count);
            if (pixel == NULL) {
                // Handle error: free allocated memory and return NULL
                for (int i = 0; i < decrypted->rows; i++) {
                    free(decrypted->image[i]);
                }
                free(decrypted->image);
                free(decrypted);
                return NULL;
            }
            decrypted->image[row_count][col_count] = *pixel;
            free(pixel);  // Free the memory allocated by evaluateOnePixel
        }
    }

    return decrypted;
}

/*
Loads a file of ppm P3 format from a file, and prints to stdout (e.g. with printf) a new image, 
where each pixel is black if the LSB of the B channel is 0, 
and white if the LSB of the B channel is 1.

argc stores the number of arguments.
argv stores a list of arguments. Here is the expected input:
argv[0] will store the name of the program (this happens automatically).
argv[1] should contain a filename, containing a file of ppm P3 format (not necessarily with .ppm file extension).
If the input is not correct, a malloc fails, or any other error occurs, you should exit with code -1.
Otherwise, you should return from main with code 0.
Make sure to free all memory before returning!
*/
int main(int argc, char **argv)
{
	//YOUR CODE HERE
	if (argc < 1) {
		fprintf(stderr, "Argument needed\n");
		return -1;
	}
	char *filename = argv[1];
	Image *image = readData(filename);
	free(filename);
	if (image == NULL){
		fprintf(stderr, "Failed to read from file\n");
		return -1;
	}
	
	Image *new_image = steganography(image);
	free(image);
	if (new_image == NULL) {
		fprintf(stderr, "Cannot convert image\n");
		return -1;
	}

	writeData(new_image);
	free(new_image);
	return 0;

}
