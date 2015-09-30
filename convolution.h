#include <stdint.h>
#include "image.h"


typedef struct kernel_st {
    int half_n;
    float *values;
} kernel_t;

void convolve(img_t src, img_t dst, kernel_t kernel, int start_x, int start_y, int count);  

