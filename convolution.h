#ifndef CONVOLUTION_H
#define CONVOLUTION_H
/**
 * @file convolution.h
 * @brief Implement the 2D convolution
 * @author Loïc Damien and Simon Menetrey
 * @date October 7, 2015
 */
#include <stdbool.h>
#include <stdint.h>
#include "image.h"

//
// Contains a kernel
// Half_n is the half of the width rounded to the lower integer
//
typedef struct kernel_st {
    int half_n;
    float *values;
} kernel_t;

//
// Hold the convolve function parameters
// Used for pthread_create
//
typedef struct convolve_params_st {
    img_t *src;
    img_t *dst;
    kernel_t kernel;
    int start_x;
    int start_y;
    int count;
} convolve_params_t;

bool load_kernel(char *filename, kernel_t *kernel);
void free_kernel(kernel_t kernel);
void *convolve_thread(void *params);
void convolve(img_t *src, img_t *dst, kernel_t kernel, int start_x, int start_y,
              int count);
#endif
