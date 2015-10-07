/**
 * @file image.c
 * @brief Implement image management
 * @author Loïc Damien and Simon Menetrey
 * @date October 7, 2015
 */
#include <stdlib.h>
#include <math.h>
#include "image.h"

//
// Allocate the memory for an image of size width*height
// This is a function from ppm.c (file provided by the professor)
//
img_t *alloc_img(int width, int height) {
    img_t *img = malloc(sizeof(img_t));
    if (img == NULL) {
        return NULL;
    }
    img->width = width;
    img->height = height;
    img->data = malloc(sizeof(pixel_t) * width * height);
    if (img->data == NULL) {
        free(img);
        return NULL;
    }
    return img;
}

//
// Free an image
// This is a function from ppm.c (file provided by the professor)
//
void free_img(img_t *img) {
    free(img->data);
    free(img);
}

//
// Get a pixel from the image
// Handle pixels outside the image by returning a black pixel
//
pixel_t get_pixel(img_t *img, int x, int y) {
    int width = img->width;
    int height = img->height;
    pixel_t value;
    if (x < 0 || x >= width || y < 0 || y >= height) {
        value.r = 0;
        value.g = 0;
        value.b = 0;
    } else {
        value = img->data[y * img->width + x];
    }
    return value;
}

//
// Set a pixel in the image
//
void set_pixel(img_t *img, int x, int y, pixel_t value) {
    img->data[y * img->width + x] = value;
}

//
// Convert a float to an uint8_t (clamp values between 0 and 255)
//
uint8_t float_to_u8(float v) {
    if (v < 0.0) {
        return 0;
    } else if (v > 255.0) {
        return 255;
    } else {
        return (uint8_t)roundf(v);
    }
}

//
// Create a pixel from float values (RGB)
//
pixel_t pixel_from_float(float r, float g, float b) {
    pixel_t value;
    value.r = float_to_u8(r);
    value.g = float_to_u8(g);
    value.b = float_to_u8(b);
    return value;
}
