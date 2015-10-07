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
// Returns false if it can't load the kernel 
//
bool load_kernel(char *filename, kernel_t *kernel) {
    FILE *f = fopen(filename, "r");
    if (f == NULL) {
        return false;
    }
    fscanf(f, "%d", &kernel->half_n);
    int size = (kernel->half_n * 2 + 1);
    size *= size;
    kernel->values = malloc(sizeof(float) * size);
    if (kernel->values == NULL) {
        fclose(f);
        return false;
    }

    for (int i = 0; i < size; i++) {
        fscanf(f, "%f", &kernel->values[i]);
    }

    fclose(f);
    return true;
}

//
// Free the kernel memory
//
void free_kernel(kernel_t kernel) { free(kernel.values); }

//
// Apply the kernel on one pixel
//
void convolve_pixel(img_t *src, img_t *dst, kernel_t kernel, int x, int y) {
    pixel_t dst_value;
    float r = 0.0;
    float g = 0.0;
    float b = 0.0;

    for (int i = -kernel.half_n; i <= kernel.half_n; i++) {
        for (int j = -kernel.half_n; j <= kernel.half_n; j++) {
            pixel_t src_value = get_pixel(src, x + i, y + j);
            float kernel_value = get_kernel_value(kernel, i, j);
            r += kernel_value * src_value.r;
            g += kernel_value * src_value.g;
            b += kernel_value * src_value.b;
        }
    }

    dst_value = pixel_from_float(r, g, b);
    set_pixel(dst, x, y, dst_value);
}

//
// Convolve the kenel on a part of the image
// Count is the number of pixels to do
//
void convolve(img_t *src, img_t *dst, kernel_t kernel, int start_x, int start_y,
              int count) {
    int width = src->width;
    int height = src->height;
    int x = start_x;
    int pixel_done = 0;
    for (int y = start_y; y < height && pixel_done < count; y++) {
        for (; x < width && pixel_done < count; x++) {
            convolve_pixel(src, dst, kernel, x, y);
            pixel_done++;
        }
        x = 0;
    }
}

//
// A wrapper around the convolve function which allows it to be called with pthread_create
//
void *convolve_thread(void *params) {
    convolve_params_t *p = (convolve_params_t *)params;
    convolve(p->src, p->dst, p->kernel, p->start_x, p->start_y, p->count);
    return NULL;
}
