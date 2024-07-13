/************************************************************************
**
** NAME:        imageloader.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Dan Garcia  -  University of California at Berkeley
**              Copyright (C) Dan Garcia, 2020. All rights reserved.
**              Justin Yokota - Starter Code
**				XueJie1
**
**
** DATE:        2020-08-15
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include "imageloader.h"

// Opens a .ppm P3 image file, and constructs an Image object.
// You may find the function fscanf useful.
// Make sure that you close the file with fclose before returning.
Image *readData(char *filename)
{
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        fprintf(stderr, "Error opening file '%s'\n", filename);
        return NULL;
    }

    Image *img = malloc(sizeof(Image));
    if (img == NULL) {
        fclose(fp);
        return NULL;
    }

    char format[3];
    if (fscanf(fp, "%2s", format) != 1 || strcmp(format, "P3") != 0) {
        fprintf(stderr, "Invalid file format\n");
        fclose(fp);
        free(img);
        return NULL;
    }

    if (fscanf(fp, "%d %d", &img->cols, &img->rows) != 2) {
        fprintf(stderr, "Error reading dimensions\n");
        fclose(fp);
        free(img);
        return NULL;
    }

    int maxval;
    if (fscanf(fp, "%d", &maxval) != 1 || maxval != 255) {
        fprintf(stderr, "Invalid max color value\n");
        fclose(fp);
        free(img);
        return NULL;
    }

    // Allocate memory for image data
    img->image = malloc(img->rows * sizeof(Color *));
    for (uint32_t i = 0; i < img->rows; i++) {
        img->image[i] = malloc(img->cols * sizeof(Color));
    }

    // Read pixel data
    for (uint32_t i = 0; i < img->rows; i++) {
        for (uint32_t j = 0; j < img->cols; j++) {
            if (fscanf(fp, "%hhu %hhu %hhu", 
                       &img->image[i][j].R, 
                       &img->image[i][j].G, 
                       &img->image[i][j].B) != 3) {
                fprintf(stderr, "Error reading pixel data\n");
                // Clean up and return NULL
            }
        }
    }

    fclose(fp);
    return img;
}
// Given an image, prints to stdout (e.g. with printf) a .ppm P3 file with the image's data.
void writeData(Image *image)
{
	// YOUR CODE HERE
	if (image == NULL)
	{
		fprintf(stderr, "Empty image object\n");
		return;
	}

	printf("P3\n");
	printf("%d %d\n255\n", image->cols, image->rows);
	for (int row_count = 0; row_count < image->rows; row_count++)
	{
		for (int col_count = 0; col_count < image->cols; col_count++)
		{
			if (col_count != image->cols - 1)
				printf("% 3hhu % 3hhu % 3hhu   ",
					   image->image[row_count][col_count].R, image->image[row_count][col_count].G, image->image[row_count][col_count].B);
			else
				printf("% 3hhu % 3hhu % 3hhu",
					   image->image[row_count][col_count].R, image->image[row_count][col_count].G, image->image[row_count][col_count].B);
		}
		printf("\n");
	}
}

// Frees an image
void freeImage(Image *image)
{
	// YOUR CODE HERE
	if (image)
	{
		if (image->image)
		{
			for (uint32_t i = 0; i < image->rows; i++)
			{
				free(image->image[i]);
			}
			free(image->image);
		}
		free(image);
	}
}
