#include <stdint.h>

// Store a pixel with 8-bit per component
typedef struct pixel_st {
	uint8_t r, g, b;   // Red, green, blue components of the pixel
} pixel_t;

// Store a 24-bit per pixel image: dimension and pixel data
typedef struct img_st {
	int width;
	int height;
	pixel_t *data;  // Pixel data
} img_t;

img_t *alloc_img(int width, int height);
void free_img(img_t *img);
pixel_t get_pixel(img_t img, int x, int y);
void set_pixel(img_t img, int x, int y, pixel_t value);

