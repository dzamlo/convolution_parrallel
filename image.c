#include <stdlib.h>
#include <math.h>
#include "image.h"

// Allocate the memory for an image of size width*height
img_t *alloc_img(int width, int height) {
    img_t *img = malloc(sizeof(img_t));
    img->width = width;
    img->height = height;
    img->data = malloc(sizeof(pixel_t) * width * height);
    return img;
}

// Free an image
void free_img(img_t *img) {
    free(img->data);
    free(img);
}

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

void set_pixel(img_t *img, int x, int y, pixel_t value) {
    img->data[y * img->width + x] = value;
}

uint8_t float_to_u8(float v) {
    if (v < 0.0) {
        return 0;
    } else if (v > 255.0) {
        return 255;
    } else {
        return (uint8_t)roundf(v);
    }
}

pixel_t pixel_from_float(float r, float g, float b) {
    pixel_t value;
    value.r = float_to_u8(r);
    value.g = float_to_u8(g);
    value.b = float_to_u8(b);
    return value;
}
