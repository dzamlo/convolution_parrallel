/**
 * @file convolution.c
 * @brief Implement the 2D convolution
 * @author Loïc Damien and Simon Menetrey
 * @date October 7, 2015
 */
#include <stdlib.h>
#include <stdio.h>

#include "convolution.h"

//
// Returns specified value from the kernel
// i and j are relative to the center of the kernel
// Go from -half_n to +half_n
// Kernel is the kernel structure
//
float get_kernel_value(kernel_t kernel, int i, int j) {
    int x = i + kernel.half_n;
    int y = j + kernel.half_n;
    int width = kernel.half_n * 2 + 1;
    return kernel.values[y * width + x];
}

//
// Loads kernel from file
// It doesn't check the validity of the file content
// Filename is the path to the kernel file and Kernel, a pointer to a kernel structure which will be filled
// Returns false if it can't load the kernel
//
bool load_kernel(char *filename, kernel_t *kernel) {
    FILE *f = fopen(filename, "r");
    if (f == NULL) {
        return false;
    }
	//Get the firt number in the first line. This is the kernel width/2 round to the lower integer
    fscanf(f, "%d", &kernel->half_n);
	//Compute the total size (HxW) and allocate memory
    int size = (kernel->half_n * 2 + 1);
    size *= size;
    kernel->values = malloc(sizeof(float) * size);
    if (kernel->values == NULL) {
        fclose(f);
        return false;
    }
	
	//Get values from kernel
    for (int i = 0; i < size; i++) {
        fscanf(f, "%f", &kernel->values[i]);
    }

    fclose(f);
    return true;
}

//
// Free the kernel memory
// Kernel is the kernel structure
//
void free_kernel(kernel_t kernel) { free(kernel.values); }

//
// Apply the kernel on one pixel
// src is the original image. dst is the future image with convolution. x and y are the coord of the pixel which will be convolved.
//
void convolve_pixel(img_t *src, img_t *dst, kernel_t kernel, int x, int y) {
    pixel_t dst_value;
    float r = 0.0;
    float g = 0.0;
    float b = 0.0;
	//Do convolution to the pixel
    for (int i = -kernel.half_n; i <= kernel.half_n; i++) {
        for (int j = -kernel.half_n; j <= kernel.half_n; j++) {
			//Get the pixel value from source image (can be the pixel itself or a neighbor)
            pixel_t src_value = get_pixel(src, x + i, y + j);
			//Get the kernel value at the coord i,j
            float kernel_value = get_kernel_value(kernel, i, j);
			//Apply
            r += kernel_value * src_value.r;
            g += kernel_value * src_value.g;
            b += kernel_value * src_value.b;
        }
    }
	
	//Set the new value to the pixel
    dst_value = pixel_from_float(r, g, b);
    set_pixel(dst, x, y, dst_value);
}

//
// Convolve the kenel on a part of the image
// Count is the number of pixels to do
// src is the original image. dst is the future image with convolution. Start_x and start_y is the coord of the first pixel to convolve.
//
void convolve(img_t *src, img_t *dst, kernel_t kernel, int start_x, int start_y,
              int count) {
    int width = src->width;
    int height = src->height;
    int x = start_x;
    int pixel_done = 0;
	//Do convolution from start_x and start_y to count
    for (int y = start_y; y < height && pixel_done < count; y++) {
        for (; x < width && pixel_done < count; x++) {
            convolve_pixel(src, dst, kernel, x, y);
            pixel_done++;
        }
        x = 0;
    }
}

//
// A wrapper around the convolve function which allows it to be called with
// pthread_create
//
void *convolve_thread(void *params) {
    convolve_params_t *p = (convolve_params_t *)params;
    convolve(p->src, p->dst, p->kernel, p->start_x, p->start_y, p->count);
    return NULL;
}
