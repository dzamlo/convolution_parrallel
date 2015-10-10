/**
 * @file image.h
 * @brief Implement image management
 * @author Loïc Damien and Simon Menetrey
 * @date October 7, 2015
 */
#ifndef IMAGE_H
#define IMAGE_H
#include <stdint.h>

//
// Store a pixel with 8-bit per component
// This is a structure from ppm.c (file provided by the professor)
//
typedef struct pixel_st {
    uint8_t r, g, b; // Red, green, blue components of the pixel
} pixel_t;

//
// Store a 24-bit per pixel image: dimension and pixel data
// This is a structure from ppm.c (file provided by the professor)
//
typedef struct img_st {
    int width;
    int height;
    pixel_t *data; // Pixel data
} img_t;

img_t *alloc_img(int width, int height);
void free_img(img_t *img);
pixel_t get_pixel(img_t *img, int x, int y);
void set_pixel(img_t *img, int x, int y, pixel_t value);

pixel_t pixel_from_float(float r, float g, float b);

#endif
