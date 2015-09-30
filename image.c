#include <stdlib.h>
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
